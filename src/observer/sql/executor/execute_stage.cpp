/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by Longda on 2021/4/13.
//

#include <string>
#include <sstream>
#include <string.h>

#include "execute_stage.h"

#include "common/io/io.h"
#include "common/log/log.h"
#include "common/seda/timer_stage.h"
#include "common/lang/string.h"
#include "session/session.h"
#include "event/storage_event.h"
#include "event/sql_event.h"
#include "event/session_event.h"
#include "event/execution_plan_event.h"
#include "sql/executor/execution_node.h"
#include "sql/executor/tuple.h"
#include "storage/common/table.h"
#include "storage/default/default_handler.h"
#include "storage/common/condition_filter.h"
#include "storage/trx/trx.h"
/*
 * @author: huahui
 * @what for: 必做题，查询元数据校验
 * begin -------------------------------------------------------------------------------------------
 */
#include "sql/parser/parse_defs.h"
/* end ---------------------------------------------------------------------------------------------*/

using namespace common;

RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node);

//! Constructor
ExecuteStage::ExecuteStage(const char *tag) : Stage(tag) {}

//! Destructor
ExecuteStage::~ExecuteStage() {}

//! Parse properties, instantiate a stage object
Stage *ExecuteStage::make_stage(const std::string &tag) {
  ExecuteStage *stage = new (std::nothrow) ExecuteStage(tag.c_str());
  if (stage == nullptr) {
    LOG_ERROR("new ExecuteStage failed");
    return nullptr;
  }
  stage->set_properties();
  return stage;
}

//! Set properties for this object set in stage specific properties
bool ExecuteStage::set_properties() {
  //  std::string stageNameStr(stageName);
  //  std::map<std::string, std::string> section = theGlobalProperties()->get(
  //    stageNameStr);
  //
  //  std::map<std::string, std::string>::iterator it;
  //
  //  std::string key;

  return true;
}

//! Initialize stage params and validate outputs
bool ExecuteStage::initialize() {
  LOG_TRACE("Enter");

  std::list<Stage *>::iterator stgp = next_stage_list_.begin();
  default_storage_stage_ = *(stgp++);
  mem_storage_stage_ = *(stgp++);

  LOG_TRACE("Exit");
  return true;
}

//! Cleanup after disconnection
void ExecuteStage::cleanup() {
  LOG_TRACE("Enter");

  LOG_TRACE("Exit");
}

void ExecuteStage::handle_event(StageEvent *event) {
  LOG_TRACE("Enter\n");

  handle_request(event);

  LOG_TRACE("Exit\n");
  return;
}

void ExecuteStage::callback_event(StageEvent *event, CallbackContext *context) {
  LOG_TRACE("Enter\n");

  // here finish read all data from disk or network, but do nothing here.
  ExecutionPlanEvent *exe_event = static_cast<ExecutionPlanEvent *>(event);
  SQLStageEvent *sql_event = exe_event->sql_event();
  sql_event->done_immediate();

  LOG_TRACE("Exit\n");
  return;
}

void ExecuteStage::handle_request(common::StageEvent *event) {
  ExecutionPlanEvent *exe_event = static_cast<ExecutionPlanEvent *>(event);
  SessionEvent *session_event = exe_event->sql_event()->session_event();
  Query *sql = exe_event->sqls();
  const char *current_db = session_event->get_client()->session->get_current_db().c_str();

  CompletionCallback *cb = new (std::nothrow) CompletionCallback(this, nullptr);
  if (cb == nullptr) {
    LOG_ERROR("Failed to new callback for ExecutionPlanEvent");
    exe_event->done_immediate();
    return;
  }
  exe_event->push_callback(cb);

  switch (sql->flag) {
    case SCF_SELECT: { // select
      RC rc = do_select(current_db, sql, exe_event->sql_event()->session_event());
      if( rc != RC::SUCCESS){
          session_event->set_response("FAILURE\n");
      }
      exe_event->done_immediate();
    }
    break;
    /*
     * @author: huahui
     * @what for: 必做题，查询元数据校验
     * begin -------------------------------------------------------------------------------------------
     */
    case SCF_INSERT: {
      RC rc = check_insert_stat(sql->sstr.insertion, session_event);
      if(rc != RC::SUCCESS){
        event->done_immediate();
        return;
      }

      StorageEvent *storage_event = new (std::nothrow) StorageEvent(exe_event);
      if (storage_event == nullptr) {
        LOG_ERROR("Failed to new StorageEvent");
        event->done_immediate();
        return;
      }

      default_storage_stage_->handle_event(storage_event);
    }
    break;
    /*end ----------------------------------------------------------------------------------------------*/

    /*
     * @author: huahui
     * @what for: 必做题，update。添加对date字段的校验
     * begin -------------------------------------------------------------------------------------------
     */
    case SCF_UPDATE: {
      const Value *values = (const Value *)(&(sql->sstr.update.value));
      RC rc = check_date_from_values(1, values);
      if(rc != RC::SUCCESS){
        char err[207];
        sprintf(err, "FAILURE\n");
        session_event->set_response(err);
        event->done_immediate();
        return;
      }

      StorageEvent *storage_event = new (std::nothrow) StorageEvent(exe_event);
      if (storage_event == nullptr) {
        LOG_ERROR("Failed to new StorageEvent");
        event->done_immediate();
        return;
      }

      default_storage_stage_->handle_event(storage_event);
    }
    break;
    /*end ----------------------------------------------------------------------------------------------*/
    case SCF_DELETE:
    case SCF_CREATE_TABLE:
    case SCF_SHOW_TABLES:
    case SCF_DESC_TABLE:
    case SCF_DROP_TABLE:
    case SCF_CREATE_INDEX:
    case SCF_DROP_INDEX: 
    case SCF_LOAD_DATA: {
      StorageEvent *storage_event = new (std::nothrow) StorageEvent(exe_event);
      if (storage_event == nullptr) {
        LOG_ERROR("Failed to new StorageEvent");
        event->done_immediate();
        return;
      }

      default_storage_stage_->handle_event(storage_event);
    }
    break;
    case SCF_SYNC: {
      RC rc = DefaultHandler::get_default().sync();
      session_event->set_response(strrc(rc));
      exe_event->done_immediate();
    }
    break;
    case SCF_BEGIN: {
      session_event->get_client()->session->set_trx_multi_operation_mode(true);
      session_event->set_response(strrc(RC::SUCCESS));
      exe_event->done_immediate();
    }
    break;
    case SCF_COMMIT: {
      Trx *trx = session_event->get_client()->session->current_trx();
      RC rc = trx->commit();
      session_event->get_client()->session->set_trx_multi_operation_mode(false);
      session_event->set_response(strrc(rc));
      exe_event->done_immediate();
    }
    break;
    case SCF_ROLLBACK: {
      Trx *trx = session_event->get_client()->session->current_trx();
      RC rc = trx->rollback();
      session_event->get_client()->session->set_trx_multi_operation_mode(false);
      session_event->set_response(strrc(rc));
      exe_event->done_immediate();
    }
    break;
    case SCF_HELP: {
      const char *response = "show tables;\n"
          "desc `table name`;\n"
          "create table `table name` (`column name` `column type`, ...);\n"
          "create index `index name` on `table` (`column`);\n"
          "insert into `table` values(`value1`,`value2`);\n"
          "update `table` set column=value [where `column`=`value`];\n"
          "delete from `table` [where `column`=`value`];\n"
          "select [ * | `columns` ] from `table`;\n";
      session_event->set_response(response);
      exe_event->done_immediate();
    }
    break;
    case SCF_EXIT: {
      // do nothing
      const char *response = "Unsupported\n";
      session_event->set_response(response);
      exe_event->done_immediate();
    }
    break;
    default: {
      exe_event->done_immediate();
      LOG_ERROR("Unsupported command=%d\n", sql->flag);
    }
  }
}

void end_trx_if_need(Session *session, Trx *trx, bool all_right) {
  if (!session->is_trx_multi_operation_mode()) {
    if (all_right) {
      trx->commit();
    } else {
      trx->rollback();
    }
  }
}

 /* @author: zihao
  * @what for: 必做题，元数据校验（单表，多表） 
  * 检查表是否存在，如果是单表，检查每个属性是否存在于这个表中；检查属性对应的表是否在查询表中出现,比如select t2.c from t 是错误的
  * 如果是多表，检查每个属性是否有表和属性名两个部分，以及属性是否在对应的表中；查询属性对应的表是否在查询的表中；条件中的所有属性也要合法
  * 对于condition部分也要检查
  * 比如t.col中，若t中没有col属性，则会返回错误
	* -----------------------------------------------------------------------------------------------------------------
	*/
RC ExecuteStage::select_check (const char *db,const Selects &selects){
  RC rc = RC::SUCCESS;
  // 单表校验
  if(selects.relation_num == 1){
    const char *table_name = selects.relations[0];
    Table * table = DefaultHandler::get_default().find_table(db, table_name);
    if (table== nullptr){
        return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    for (size_t j = 0; j < selects.attr_num ; ++j) {
        if(selects.attributes[j].relation_name != nullptr && strcmp(selects.attributes[j].relation_name, selects.relations[0])!=0) {
          return RC::SQL_SYNTAX;
        }
        if (0 == strcmp("*", selects.attributes[j].attribute_name)){
            continue;
        }
        const FieldMeta * fieldMeta = table->table_meta().field(selects.attributes[j].attribute_name);
        if (fieldMeta == nullptr){
            return RC::SCHEMA_FIELD_NOT_EXIST;
        }
    }
    for (size_t j = 0; j < selects.condition_num ; ++j) {
        if (selects.conditions[j].left_is_attr){
            if(selects.conditions[j].left_attr.relation_name != nullptr && strcmp(selects.conditions[j].left_attr.relation_name, selects.relations[0])!=0) {
                return RC::SQL_SYNTAX;
            }
            const FieldMeta * fieldMeta = table->table_meta().field(selects.conditions[j].left_attr.attribute_name);
            if (fieldMeta == nullptr){
                return RC::SCHEMA_FIELD_NOT_EXIST;
            }
        }
        if (selects.conditions[j].right_is_attr){
            if(selects.conditions[j].right_attr.relation_name != nullptr && strcmp(selects.conditions[j].right_attr.relation_name, selects.relations[0])!=0) {
                return RC::SQL_SYNTAX;
            }
            const FieldMeta * fieldMeta = table->table_meta().field(selects.conditions[j].right_attr.attribute_name);
            if (fieldMeta == nullptr){
                return RC::SCHEMA_FIELD_NOT_EXIST;
            }
        }
    }
    return RC::SUCCESS;
  }
  // 多表校验
  for(size_t i = 0; i<selects.relation_num; i++) {
    const char *table_name = selects.relations[i];
    Table * table = DefaultHandler::get_default().find_table(db, table_name);
    if (table== nullptr){
        return RC::SCHEMA_TABLE_NOT_EXIST;
    }
  }
  for(size_t j=0; j<selects.attr_num; j++) {
    RC rc2 = check_attr_for_multitable(db, selects, selects.attributes[j]);
    if(rc2 != RC::SUCCESS) {
      return rc2;
    }
  }
  for(size_t j=0; j<selects.condition_num; j++) {
    if (selects.conditions[j].left_is_attr){
      if(strcmp("*", selects.conditions[j].left_attr.attribute_name) == 0) {
        return RC::SQL_SYNTAX;
      }
      RC rc2 = check_attr_for_multitable(db, selects, selects.conditions[j].left_attr);
      if(rc2 != RC::SUCCESS) {
        return rc2;
      }
    }
    if (selects.conditions[j].right_is_attr){
      if(strcmp("*", selects.conditions[j].right_attr.attribute_name) == 0) {
        return RC::SQL_SYNTAX;
      }
      RC rc2 = check_attr_for_multitable(db, selects, selects.conditions[j].right_attr);
      if(rc2 != RC::SUCCESS) {
        return rc2;
      }
    }
  }
  return RC::SUCCESS;
}
/* --------------------------------------------------------------------------------------------------------------------------------*/

// 这里没有对输入的某些信息做合法性校验，比如查询的列名、where条件中的列名等，没有做必要的合法性校验
// 需要补充上这一部分. 校验部分也可以放在resolve，不过跟execution放一起也没有关系
RC ExecuteStage::do_select(const char *db, Query *sql, SessionEvent *session_event) {

  RC rc = RC::SUCCESS;
  Session *session = session_event->get_client()->session;
  Trx *trx = session->current_trx();
  const Selects &selects = sql->sstr.selection;
  rc = select_check(db,selects);
  if ( rc != RC::SUCCESS){
      LOG_ERROR("select error,rc=%d:%s",rc, strrc(rc));
      return rc;
  }
  // 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
  std::vector<SelectExeNode *> select_nodes;
  for (size_t i = 0; i < selects.relation_num; i++) {
    const char *table_name = selects.relations[i];
    SelectExeNode *select_node = new SelectExeNode;
    rc = create_selection_executor(trx, selects, db, table_name, *select_node);
    if (rc != RC::SUCCESS) {
      delete select_node;
      for (SelectExeNode *& tmp_node: select_nodes) {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      return rc;
    }
    select_nodes.push_back(select_node);
  }

  if (select_nodes.empty()) {
    LOG_ERROR("No table given");
    end_trx_if_need(session, trx, false);
    return RC::SQL_SYNTAX;
  }


  std::vector<TupleSet> tuple_sets;
  for (SelectExeNode *&node: select_nodes) {
    TupleSet tuple_set;
    rc = node->execute(tuple_set);
    if (rc != RC::SUCCESS) {
      for (SelectExeNode *& tmp_node: select_nodes) {
        delete tmp_node;
      }
      end_trx_if_need(session, trx, false);
      return rc;
    } else {
      tuple_sets.push_back(std::move(tuple_set));
    }
  }

  /* @author: huahui 
	 * @what for: 必做题，聚合查询和多表join
	 * -----------------------------------------------------------------------------------------------------------------
	 */
  TupleSet res_tupleset;
  std::stringstream ss;
  if (tuple_sets.size() > 1) {
    // 本次查询了多张表，需要做join操作, 结果放在res_tuple_set中
  } else {
    // 当前只查询一张表，直接返回结果即可
    res_tupleset = std::move(tuple_sets.front());
    // tuple_sets.front().print(ss);
  }

  bool hagg = false;
  std::vector<const RelAttr *> relattrs;
  rc = have_agg_from_selections(selects, hagg, relattrs);
  if(rc != RC::SUCCESS) {
    LOG_ERROR("In aggregated query without GROUP BY, expression of SELECT list contains must not contain nonaggregated colum\n");
    return rc;
  }
  
  if(hagg) {
    // 聚合查询，先做合法性校验，比如AVG(birthday)是肯定不对的
    rc = check_agg(db, selects, relattrs);
    if(rc != RC::SUCCESS) {
      return rc;
    }
    TupleSet agg_res;
    agg_select_from_tupleset(trx, db, selects, res_tupleset, relattrs, agg_res);
    res_tupleset = std::move(agg_res);
  }
  res_tupleset.print(ss);
  /* ------------------------------------------------------------------------------------------------------------
	 */

  for (SelectExeNode *& tmp_node: select_nodes) {
    delete tmp_node;
  }
  session_event->set_response(ss.str());
  end_trx_if_need(session, trx, true);
  return rc;
}

/*
 * @author: huahui
 * @what for: 必做题，查询元数据校验
 * begin -------------------------------------------------------------------------------------------
 */
// 检查以下错误
// select c1 from t1, t2
// select t100.c1 from t1,t2  t100不在t1,t2中
// select t1.c100 from t1,t2  c100不在t1中
RC ExecuteStage::check_attr_for_multitable(const char *db, const Selects &selects, const RelAttr &relattr) {
  if (0 == strcmp("*", relattr.attribute_name)){
      return RC::SUCCESS;
  }
  const char *table_name = relattr.relation_name;
  if(table_name == nullptr) { // 多表查询，但是查询属性却没有表名
    return RC::SQL_SYNTAX;
  }
  bool exist_in_select = false; // select t3.c1, t2.c1 from t1,t2  t3在{t1, t2}中不存在,exist_in_select==false
  for(size_t k=0; k<selects.relation_num; k++) {
    if(strcmp(table_name, selects.relations[k]) == 0) {
      exist_in_select = true;
    }
  }
  if(!exist_in_select) {
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  Table * table = DefaultHandler::get_default().find_table(db, table_name);
  const FieldMeta * fieldMeta = table->table_meta().field(relattr.attribute_name);
  if (fieldMeta == nullptr){ // 表名.属性 表名对应的表没有对应属性
    return RC::SCHEMA_FIELD_NOT_EXIST;
  }
  return RC::SUCCESS;
}

// 从长度为value_num的values中，判断有没有date属性
// 若有，则判断date是否合法
RC ExecuteStage::check_date_from_values(int value_num, const Value *values) {
  int days[13]={0,31,28,31,30,31,30,31,31,30,31,30,31};
  for(int i=0; i<value_num; i++) {
    const Value &value = values[i];
    if(value.type != DATES) continue;
    unsigned char *scratch = (unsigned char *)(value.data);
    int year, month, day;
    year = ((int)(scratch[0]))*256 + (int)(scratch[1]);
    month = (int)(scratch[2]);
    day = (int)(scratch[3]);
    if((year == 2038 && month > 2) || (year > 2038)) {
      LOG_ERROR("The date should not exceed February 2038");
      return RC::CONSTRAINT_CHECK; // ?这里要返回什么RC
    }

    bool leap;
    if((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)){
      leap = true;
    }else{
      leap = false;
    }
    bool match = true;
    if(month == 2){
      if(leap){
        if(day > 29){
          match = false;
        }
      }else{
        if(day > 28){
          match = false;
        }
      }
    }else{
      if(day > days[month]){
        match = false;
      }
    }
    if(!match){
      LOG_ERROR("The date is invalid");
      return RC::CONSTRAINT_CHECK; // ?这里要返回什么RC
    }
  }

  return RC::SUCCESS;
}
RC ExecuteStage::check_insert_stat(const Inserts &inserts, SessionEvent *session_event){
  // 校验insert语句中的date字段是否符合要求，即满足日期小于2038年2月，以及满足闰年平年的要求
  const Value *values = (const Value *)(inserts.values);
  RC rc = check_date_from_values(inserts.value_num, values);
  if(rc == RC::SUCCESS) return rc;
  char err[207];
  sprintf(err, "FAILURE\n");
  session_event->set_response(err);
  return RC::CONSTRAINT_CHECK; // ?这里要返回什么RC
}
/*end ----------------------------------------------------------------------------------------------*/

/* @author: huahui 
 * @what for: 必做题，聚合查询 
 * relattrs中的属性肯定都存在，因为在调用这个函数之前进行了元数据校验
 * -----------------------------------------------------------------------------------------------------------------
 */
RC ExecuteStage::check_agg(const char *db, const Selects &selects, std::vector<const RelAttr *> &relattrs) {
  for(size_t i = 0; i < relattrs.size(); i++) {
    const char *table_name = (relattrs[i]->relation_name == nullptr ? selects.relations[0] : relattrs[i]->relation_name);
    Table * table = DefaultHandler::get_default().find_table(db, table_name);
    if(!table) {
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    const FieldMeta * fieldMeta = table->table_meta().field(relattrs[i]->attribute_name);
    if(!fieldMeta) {
      return RC::SCHEMA_FIELD_MISSING;
    }
    // AVG(*)  AVG(birthday)是错的
    if((relattrs[i]->attribute_name=="*" || !fieldMeta->addable()) && relattrs[i]->agg_type==AggType::AGGAVG) {
      return RC::SQL_SYNTAX;
    }
    // MAX(*) MIN(*) AVG(*)是错误的
    if((relattrs[i]->agg_type==AggType::AGGMAX || relattrs[i]->agg_type==AggType::AGGMIN || relattrs[i]->agg_type==AggType::AGGAVG) 
        && relattrs[i]->attribute_name=="*") {
      return RC::SQL_SYNTAX;
    }
  }
  return RC::SUCCESS;
}

// 不支持group by
// 检查selections中是否有聚合属性，如果有聚合属性，就不能存在其他查询属性.
// 比如: select col, max(col2) from t; 是错误的
// hagg: true, 表示有聚合属性，同时没有别的其他非聚合属性
// relattrs: 如果hagg==true, 则relattrs中保存聚合属性
RC ExecuteStage::have_agg_from_selections(const Selects &selection, bool &hagg, std::vector<const RelAttr *> &relattrs) {
  bool has_agg = false;
  bool has_attr = false;
  for(size_t i = 0; i < selection.attr_num; i++) {
    const RelAttr &relattr = selection.attributes[i];
    if(relattr.agg_type == AggType::NOTAGG) {
      has_attr = true;
    }else {
      has_agg = true;
      relattrs.push_back(&(selection.attributes[i]));
    }
    if(has_agg && has_attr) {
      return RC::SQL_SYNTAX;
    }
  }
  hagg = has_agg;
  return RC::SUCCESS;
}
// 不支持group by
// 已经确保肯定是有至少一个聚合属性的，而且保证合法性校验
// 支持多个聚合属性的查询，支持多表。
RC ExecuteStage::agg_select_from_tupleset(Trx *trx, const char *db, const Selects &selects, TupleSet &tuple_set, std::vector<const RelAttr *> &relattrs, TupleSet &agg_res) {
  // 1. 先创建AggExeNode
  TupleSchema schema;
  for(size_t i = 0; i < relattrs.size(); i++) {
    const char *table_name = relattrs[i]->relation_name==nullptr?selects.relations[0]:relattrs[i]->relation_name;
    Table * table = DefaultHandler::get_default().find_table(db, table_name);
    if(!table) {
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    if(strcmp(relattrs[i]->attribute_name, "*") != 0){
      const FieldMeta * fieldMeta = table->table_meta().field(relattrs[i]->attribute_name);
      if(!fieldMeta) {
        return RC::SCHEMA_FIELD_MISSING;
      }
      AttrType attrtype = fieldMeta->type();
      schema.add(attrtype, table_name, relattrs[i]->attribute_name, relattrs[i]->relation_name!=nullptr, relattrs[i]->agg_type);
    } else {
      schema.add(AttrType::UNDEFINED, table_name, relattrs[i]->attribute_name, relattrs[i]->relation_name!=nullptr, relattrs[i]->agg_type);
    }
  }
  AggExeNode *agg_node = new AggExeNode;
  agg_node->init(trx, std::move(schema), std::move(tuple_set));
  // 2. 执行AggExeNode的execute函数得到结果
  agg_node->execute(agg_res);
} 
/* ------------------------------------------------------------------------------------------------------------
 */

bool match_table(const Selects &selects, const char *table_name_in_condition, const char *table_name_to_match) {
  if (table_name_in_condition != nullptr) {
    return 0 == strcmp(table_name_in_condition, table_name_to_match);
  }

  return selects.relation_num == 1;
}

/* @author: huahui  @what for: 聚合查询 多表查询  --------------------------------------------------------------*/
static RC schema_add_field(Table *table, const char *field_name, TupleSchema &schema, bool have_table_name) {
  const FieldMeta *field_meta = table->table_meta().field(field_name);
  if (nullptr == field_meta) {
    LOG_WARN("No such field. %s.%s", table->name(), field_name);
    return RC::SCHEMA_FIELD_MISSING;
  }

  schema.add_if_not_exists(field_meta->type(), table->name(), field_meta->name(), have_table_name);
  return RC::SUCCESS;
}
/* -------------------------------------------------------------------------------------------------------------*/

// 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node) {
  // 列出跟这张表关联的Attr
  TupleSchema schema;
  Table * table = DefaultHandler::get_default().find_table(db, table_name);
  if (nullptr == table) {
    LOG_WARN("No such table [%s] in db [%s]", table_name, db);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  for (int i = selects.attr_num - 1; i >= 0; i--) {
    const RelAttr &attr = selects.attributes[i];
    if (nullptr == attr.relation_name || 0 == strcmp(table_name, attr.relation_name)) {
      if (0 == strcmp("*", attr.attribute_name)) {
        /* @author: huahui  @what for: 聚合查询 多表查询  -----------------------------------------------*/
        // 列出这张表所有字段
        TupleSchema::from_table(table, schema, (attr.relation_name!=nullptr));
        /* ----------------------------------------------------------------------------------------------*/
        break; // 没有校验，给出* 之后，再写字段的错误
      } else {
        /* @author: huahui  @what for: 聚合查询 多表查询  ---------------------------------------------------*/
        // 列出这张表相关字段
        RC rc = schema_add_field(table, attr.attribute_name, schema, (attr.relation_name!=nullptr));
        /* ---------------------------------------------------------------------------------------------------*/
        if (rc != RC::SUCCESS) {
          return rc;
        }
      }
    }
  }

  // 找出仅与此表相关的过滤条件, 或者都是值的过滤条件
  std::vector<DefaultConditionFilter *> condition_filters;
  for (size_t i = 0; i < selects.condition_num; i++) {
    const Condition &condition = selects.conditions[i];
    if ((condition.left_is_attr == 0 && condition.right_is_attr == 0) || // 两边都是值
        (condition.left_is_attr == 1 && condition.right_is_attr == 0 && match_table(selects, condition.left_attr.relation_name, table_name)) ||  // 左边是属性右边是值
        (condition.left_is_attr == 0 && condition.right_is_attr == 1 && match_table(selects, condition.right_attr.relation_name, table_name)) ||  // 左边是值，右边是属性名
        (condition.left_is_attr == 1 && condition.right_is_attr == 1 &&
            match_table(selects, condition.left_attr.relation_name, table_name) && match_table(selects, condition.right_attr.relation_name, table_name)) // 左右都是属性名，并且表名都符合
        ) {
      DefaultConditionFilter *condition_filter = new DefaultConditionFilter();
      RC rc = condition_filter->init(*table, condition);
      if (rc != RC::SUCCESS) {
        delete condition_filter;
        for (DefaultConditionFilter * &filter : condition_filters) {
          delete filter;
        }
        return rc;
      }
      condition_filters.push_back(condition_filter);
    }
  }

  return select_node.init(trx, table, std::move(schema), std::move(condition_filters));
}
