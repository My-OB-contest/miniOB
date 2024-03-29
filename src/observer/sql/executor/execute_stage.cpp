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

RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node,Query *sql) ;

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
      TupleSet tmp_tupleset;
      RC rc = do_select(current_db, sql, exe_event->sql_event()->session_event(),tmp_tupleset,0);
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
      // 在check_insert_stat中不用检验插入的数据是否合法,不用检查null相关,因为在后面的make_record()中会检查
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
     * begin ------------------------------------------------------------------------------------------------------
     */
    case SCF_UPDATE: {
      /*const Value *values = (const Value *)(&(sql->sstr.update.value));
      RC rc = check_date_from_values(1, values);
      //校验where中不合规的date
      for( size_t conditionnum = 0 ; conditionnum < sql->sstr.update.condition_num ; ++conditionnum ){
          if (!sql->sstr.update.conditions->left_is_attr && sql->sstr.update.conditions->left_value.type == DATES){
              rc = check_date_from_values(1,(const Value *)&(sql->sstr.update.conditions->left_value));
          }
          if (!sql->sstr.update.conditions->right_is_attr && sql->sstr.update.conditions->right_value.type == DATES){
              rc =check_date_from_values(1,(const Value *)&(sql->sstr.update.conditions->right_value));
          }
      }*/
      convert_condexps_to_conds(sql->sstr.update.condition_num, sql->sstr.update.condition_exps, sql->sstr.update.conditions); /* @what for: expression */
      RC rc = update_check(current_db, sql->sstr.update);
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
    case SCF_DELETE:{
      /* @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
      convert_condexps_to_conds(sql->sstr.deletion.condition_num, sql->sstr.deletion.condition_exps, sql->sstr.deletion.conditions); 
      StorageEvent *storage_event = new (std::nothrow) StorageEvent(exe_event);
      if (storage_event == nullptr) {
        LOG_ERROR("Failed to new StorageEvent");
        event->done_immediate();
        return;
      }

      default_storage_stage_->handle_event(storage_event);
      /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
    }
    break;
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
  * 检查orderby的错误
  * 检查groupby的错误
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
        if(selects.attributes[j].agg_type != AggType::NOTAGG && !selects.attributes[j].is_attr) { // 比如当前是count(1)，那么就直接跳过这个聚合属性，不用校验
          continue;
        }
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
        AttrType left_at, right_at;
        int left_is_null = false, right_is_null = false; /* @author: huahui  @what for: null-----------------------------------*/
        if (selects.conditions[j].left_is_attr){
            // 条件中不能带*
            if(strcmp("*", selects.conditions[j].left_attr.attribute_name) == 0) {
                return RC::SQL_SYNTAX;
            }
            if(selects.conditions[j].left_attr.relation_name != nullptr && strcmp(selects.conditions[j].left_attr.relation_name, selects.relations[0])!=0) {
                return RC::SQL_SYNTAX;
            }
            const FieldMeta * fieldMeta = table->table_meta().field(selects.conditions[j].left_attr.attribute_name);
            if (fieldMeta == nullptr){
                return RC::SCHEMA_FIELD_NOT_EXIST;
            }
            left_at = fieldMeta->type();
        } else { 
          // 检查date是否符合要求  (其实这个检验应该也没有必要)
          if(selects.conditions[j].left_value.type == AttrType::DATES) {
            const Value *values = (const Value *)(&(selects.conditions[j].left_value));
            rc = check_date_from_values(1, values);
            if(rc != RC::SUCCESS) {
              return rc;
            }
          }

          left_at = selects.conditions[j].left_value.type;
          left_is_null = selects.conditions[j].left_value.is_null;           /* @author: huahui  @what for: null-----------------------------------*/
        }
        if (selects.conditions[j].right_is_attr){
            if(strcmp("*", selects.conditions[j].right_attr.attribute_name) == 0) {
                return RC::SQL_SYNTAX;
            }
            if(selects.conditions[j].right_attr.relation_name != nullptr && strcmp(selects.conditions[j].right_attr.relation_name, selects.relations[0])!=0) {
                return RC::SQL_SYNTAX;
            }
            const FieldMeta * fieldMeta = table->table_meta().field(selects.conditions[j].right_attr.attribute_name);
            if (fieldMeta == nullptr){
                return RC::SCHEMA_FIELD_NOT_EXIST;
            }
            right_at = fieldMeta->type();
        }else {
          // 检查date是否符合要求   (其实这个检验应该也没有必要)
          if(selects.conditions[j].right_value.type == AttrType::DATES) {
            const Value *values = (const Value *)(&(selects.conditions[j].right_value));
            rc = check_date_from_values(1, values);
            if(rc != RC::SUCCESS) {
              return rc;
            }
          }

          right_at = selects.conditions[j].right_value.type;
          right_is_null = selects.conditions[j].right_value.is_null;                       /* @author: huahui  @what for: null-----------------------------------*/
        }
        
        /* @author: huahui  @what for: null--------------------------------------------------------------*/
        // 检查如果有一个是值null，则可以跳过
        if(left_is_null || right_is_null) {
          continue;
        }

        /* @author: huahui  @what for: 元数据校验, where中int和float兼容 -----------------------------------------*/
        // 检查左右两边的类型是否相容，比如int和float就是相容的
        if(left_at != right_at && 
          !(left_at==AttrType::INTS && right_at==AttrType::FLOATS || left_at==AttrType::FLOATS && right_at==AttrType::INTS)) {
          return RC::SQL_SYNTAX;
        }
    }

    // @todo: order-by 校验order- by

    // @todo: group-by 校验group-by

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
    if(selects.attributes[j].agg_type != AggType::NOTAGG && !selects.attributes[j].is_attr) { // 比如当前是count(1)，那么就直接跳过这个聚合属性，不用校验
      continue;
    }
    RC rc2 = check_attr_for_multitable(db, selects, selects.attributes[j]);
    if(rc2 != RC::SUCCESS) {
      return rc2;
    }
  }
  for(size_t j=0; j<selects.condition_num; j++) {
    AttrType left_at, right_at;
    int left_is_null = false, right_is_null = false; /* @author: huahui  @what for: null-----------------------------------*/
    if (selects.conditions[j].left_is_attr){
      if(strcmp("*", selects.conditions[j].left_attr.attribute_name) == 0) {
        return RC::SQL_SYNTAX;
      }
      RC rc2 = check_attr_for_multitable(db, selects, selects.conditions[j].left_attr);
      if(rc2 != RC::SUCCESS) {
        return rc2;
      }
      Table * table = DefaultHandler::get_default().find_table(db,selects.conditions[j].left_attr.relation_name);
      const FieldMeta * fieldMeta = table->table_meta().field(selects.conditions[j].left_attr.attribute_name);
      if (fieldMeta == nullptr){
          return RC::SCHEMA_FIELD_NOT_EXIST;
      }
      left_at = fieldMeta->type();
    } else {
      // 检查date是否符合要求  (其实这个检验应该也没有必要)
      if(selects.conditions[j].left_value.type == AttrType::DATES) {
        const Value *values = (const Value *)(&(selects.conditions[j].left_value));
        rc = check_date_from_values(1, values);
        if(rc != RC::SUCCESS) {
          return rc;
        }
      }

      left_at = selects.conditions[j].left_value.type;
      left_is_null = selects.conditions[j].left_value.is_null;           /* @author: huahui  @what for: null-----------------------------------*/
    }
    if (selects.conditions[j].right_is_attr){
      if(strcmp("*", selects.conditions[j].right_attr.attribute_name) == 0) {
        return RC::SQL_SYNTAX;
      }
      RC rc2 = check_attr_for_multitable(db, selects, selects.conditions[j].right_attr);
      if(rc2 != RC::SUCCESS) {
        return rc2;
      }
      Table * table = DefaultHandler::get_default().find_table(db,selects.conditions[j].right_attr.relation_name);
      const FieldMeta * fieldMeta = table->table_meta().field(selects.conditions[j].right_attr.attribute_name);
      if (fieldMeta == nullptr){
          return RC::SCHEMA_FIELD_NOT_EXIST;
      }
      right_at = fieldMeta->type();
    } else {
      // 检查date是否符合要求  (其实这个检验应该也没有必要)
      if(selects.conditions[j].left_value.type == AttrType::DATES) {
        const Value *values = (const Value *)(&(selects.conditions[j].left_value));
        rc = check_date_from_values(1, values);
        if(rc != RC::SUCCESS) {
          return rc;
        }
      }

      right_at = selects.conditions[j].right_value.type;
      right_is_null = selects.conditions[j].right_value.is_null;                       /* @author: huahui  @what for: null-----------------------------------*/
    }

    /* @author: huahui  @what for: null--------------------------------------------------------------*/
    // 检查如果有一个是值null，则可以跳过后面的判断
    if(left_is_null || right_is_null) {
      continue;
    }

    /* @author: huahui  @what for: 元数据校验, where中int和float兼容 -----------------------------------------*/
    // 检查左右两边的类型是否相容，比如int和float就是相容的
    if(left_at != right_at && 
      !(left_at==AttrType::INTS && right_at==AttrType::FLOATS || left_at==AttrType::FLOATS && right_at==AttrType::INTS)) {
      return RC::SQL_SYNTAX;
    }
  }
  return RC::SUCCESS;
}
/* --------------------------------------------------------------------------------------------------------------------------------*/

/* @author: huahui  @what for: 元数据校验----------------------------------------------------------------------*/
RC ExecuteStage::update_check(const char *db, const Updates &updates) {
  // 查看date是否合法
  const Value *values = (const Value *)(&(updates.value));
  RC rc = check_date_from_values(1, values);
  if(rc != RC::SUCCESS) {
    return rc;
  }

  // check conditon
  Table * table = DefaultHandler::get_default().find_table(db, updates.relation_name);
  if(!table) {
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }
  rc = check_condition(updates.condition_num, updates.conditions, table);
  if(rc != RC::SUCCESS) {
    return rc;
  }

  return RC::SUCCESS;
}
/* -------------------------------------------------------------------------------------------------------------------*/

/*
 * fzh
 * 投影操作
 */
RC ExecuteStage::projection(std::vector<TupleSet> &tuplesets,const Selects &selects) {
    RC rc = RC::SUCCESS;

    for (int i = selects.attr_num-1; i >=0 ; --i){
        if (0 == strcmp("*", selects.attributes[i].attribute_name) && selects.attributes[i].relation_name == nullptr ) {
            return rc;
        }
    }

/*
 * 待优化，因前面已经对大部分情况进行了正常列排序，如果顺序正确且不需要投影应直接返回成功，减少一次投影操作
 *
 *
*/
    TupleSet tmptupset;
    TupleSchema tmpschema;
    int attrindex[40];
    int indexcount=0;
    if (selects.relation_num >1 ){
        for (int i = selects.attr_num-1; i >=0 ; --i) {
            for(auto it_field = tuplesets[0].get_schema().fields().begin();it_field != tuplesets[0].get_schema().fields().end() ; ++it_field){
                if (strcmp(it_field->table_name(),selects.attributes[i].relation_name) == 0 && strcmp(it_field->field_name(),selects.attributes[i].attribute_name) == 0){
                    tmpschema.add_if_not_exists(static_cast<AttrType>(it_field->getAggtype()), it_field->table_name(), it_field->field_name(), true);
                    attrindex[indexcount++]=it_field-tuplesets[0].get_schema().fields().begin();
                    break;
                }
                if (strcmp(it_field->table_name(),selects.attributes[i].relation_name) == 0 && strcmp("*",selects.attributes[i].attribute_name) == 0){
                    tmpschema.add_if_not_exists(static_cast<AttrType>(it_field->getAggtype()), it_field->table_name(), it_field->field_name(), true);
                    attrindex[indexcount++]=it_field-tuplesets[0].get_schema().fields().begin();
                }
            }
        }
        tmptupset.set_schema(tmpschema);
        for(auto it_tuple = tuplesets[0].tuples().begin();it_tuple != tuplesets[0].tuples().end();++it_tuple){
            Tuple tmptuple;
            for (int i = 0; i < indexcount; ++i) {
                tmptuple.add(it_tuple->get_pointer(attrindex[i]));
            }
            tmptupset.add(std::move(tmptuple));
        }
    }else{
        for (int i = selects.attr_num-1; i >=0 ; --i) {
            for(auto it_field = tuplesets[0].get_schema().fields().begin();it_field != tuplesets[0].get_schema().fields().end() ; ++it_field){
                if ( strcmp(it_field->field_name(),selects.attributes[i].attribute_name) == 0){
                    tmpschema.add_if_not_exists(it_field->type(), it_field->table_name(), it_field->field_name(),
                                                false);
                    attrindex[indexcount++]=it_field-tuplesets[0].get_schema().fields().begin();
                    break;
                }
            }
        }
        tmptupset.set_schema(tmpschema);
        for(auto it_tuple = tuplesets[0].tuples().begin();it_tuple != tuplesets[0].tuples().end();++it_tuple){
            Tuple tmptuple;
            for (int i = 0; i < indexcount; ++i) {
                tmptuple.add(it_tuple->get_pointer(attrindex[i]));
            }
            tmptupset.add(std::move(tmptuple));
        }
    }
    tuplesets.pop_back();
    tuplesets.push_back(std::move(tmptupset));
    return rc;
}

//void sel_append_table_name(AdvSelects &adv_selects){
//
//}


// 这里没有对输入的某些信息做合法性校验，比如查询的列名、where条件中的列名等，没有做必要的合法性校验
// 需要补充上这一部分. 校验部分也可以放在resolve，不过跟execution放一起也没有关系
RC ExecuteStage::do_select(const char *db, Query *sql, SessionEvent *session_event,TupleSet &res_tupleset,int curpos) {

  RC rc = RC::SUCCESS;
  Session *session = session_event->get_client()->session;
  Trx *trx = session->current_trx();
  const AdvSelects &adv_selects = sql->sstr.adv_selection[curpos];
  Selects selects;
  if(!convert_to_selects(adv_selects, selects)) {
    // 走表达式路线;
    //TupleSet res_tupleset;
    std::stringstream ss;
    rc = do_advselects(trx, adv_selects, db, res_tupleset);
    if(rc != RC::SUCCESS) {
      end_trx_if_need(session, trx, false);
      return rc;
    }
    res_tupleset.print(ss);
    session_event->set_response(ss.str());
    end_trx_if_need(session, trx, true);
    return RC::SUCCESS;
  } 

  if(sql->sstr.adv_selection[0].select_num == 1){
      rc = select_check(db,selects);
      if ( rc != RC::SUCCESS){
          LOG_ERROR("select error,rc=%d:%s",rc, strrc(rc));
          return rc;
      }
  }
    /* @author: fzh
   * @what for: subselect
   * -----------------------------------------------------------------------------------------------------------------
   */
   std::vector<Condition> sub_sel_conditions;
   rc = check_sub_select(selects,sql,sub_sel_conditions);
   if( rc != RC::SUCCESS){
       LOG_ERROR("sub_select check error,rc=%d:%s",rc, strrc(rc));
       return rc;
   }
  // 把所有的表和只跟这张表关联的condition都拿出来，生成最底层的select 执行节点
  std::vector<SelectExeNode *> select_nodes;
  for (size_t i = 0; i < selects.relation_num; i++) {
    const char *table_name = selects.relations[i];
    SelectExeNode *select_node = new SelectExeNode;
    rc = create_selection_executor(trx, selects, db, table_name, *select_node,sql);
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
  //TupleSet res_tupleset;
  std::stringstream ss;
  if (tuple_sets.size() > 1) {
    // 本次查询了多张表，需要做join操作
    JoinExeNode *joinExeNode = new JoinExeNode;
    rc = joinExeNode->init(trx, selects.conditions, selects.condition_num, db, selects.relations, selects.relation_num);
    if (rc != RC::SUCCESS){
        LOG_ERROR("Join init error");
        delete joinExeNode;
        return rc;
    }
    rc = joinExeNode->execute(tuple_sets);
    if (rc != RC::SUCCESS){
        LOG_ERROR("Join execute error");
        delete joinExeNode;
        return rc;
    }
    /*rc = projection(tuple_sets,selects);
    if (rc != RC::SUCCESS){
        LOG_ERROR("projection error");
        delete joinExeNode;
        return rc;
    }*/
    res_tupleset = std::move(tuple_sets.front());
    delete joinExeNode;
    // 本次查询了多张表，需要做join操作, 结果放在res_tuple_set中
  } else {
    // 当前只查询一张表，直接返回结果即可
    //有子查询递归执行子查询再次过滤当前tuplest
    if(sub_sel_conditions.size() > 0 && tuple_sets.front().size() > 0 ){
        bool need_change = if_need_change(tuple_sets.front(),sql,curpos);
        std::vector<std::pair<TupleSet,TupleSet>> tupleset_pair_list;
        TupleSet tmp_tupleset;
        tmp_tupleset.set_schema(tuple_sets.front().get_schema());
        if (need_change == false){
            rc = do_sub_sel(db,sql,session_event,tupleset_pair_list,sub_sel_conditions);
            if (rc != RC::SUCCESS){
                LOG_ERROR("do_sub_sel error");
                return rc;
            }
        }
        for(int i = 0 ; i < tuple_sets.front().size() ; ++i){
            if (need_change == true){
                change_sub_select(tuple_sets.front(),sql,i,curpos);
                rc = do_sub_sel(db,sql,session_event,tupleset_pair_list,sub_sel_conditions);
                if (rc != RC::SUCCESS){
                    LOG_ERROR("do_sub_sel error");
                    return rc;
                }
            }
            bool final_flag = true;
            for(int j = 0 ; j < sub_sel_conditions.size() ; ++j){
                SubSelConditionFilter sub_sel_filter;
                sub_sel_filter.init(&tuple_sets.front().get(i),&sub_sel_conditions[j],tuple_sets.front().schema());
                rc = sub_sel_filter.check_subsel_tupset(tupleset_pair_list[j]);
                if (rc != RC::SUCCESS){
                    LOG_ERROR("check sub_sel_filter error");
                    return rc;
                }
                final_flag = final_flag&&sub_sel_filter.filter(tupleset_pair_list[j]);
                if(final_flag == false){
                    break;
                }
            }
            if (final_flag == true){
                Tuple tmp_tuple;
                for (int j = 0; j < tuple_sets.front().get(i).size(); ++j) {
                    tmp_tuple.add(tuple_sets.front().get(i).get_pointer(j));
                }
                tmp_tupleset.add(std::move(tmp_tuple));
            }
        }
        tuple_sets.clear();
        tuple_sets.push_back(std::move(tmp_tupleset));
       // projection(tuple_sets,selects);

    }
    projection(tuple_sets,selects);
    res_tupleset = std::move(tuple_sets.front());
  }

  /* @author: huahui  @what for: order-by <<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  // 调用TupleSet的排序函数，但是此时还未projection
  if(selects.order_num > 0) {
    res_tupleset.sortTuples(selects.order_num, selects.order_attrs);
  }
  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

  /* @what for: group-by*/
  if(selects.group_num > 0) {
    OrderAttr *order_attrs = new OrderAttr[selects.group_num];
    for(int i = 0; i < selects.group_num; i++) {
      order_attrs[i].attribute_name = selects.group_attrs[i].attribute_name;
      order_attrs[i].relation_name = selects.group_attrs[i].relation_name;
      order_attrs[i].order_rule = OrderRule::ASCORDER;
    }
    res_tupleset.sortTuples(selects.group_num, order_attrs);
    delete[] order_attrs;
  }
  
  /* @what for: group-by*/
  if(selects.group_num == 0) {
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
      rc = agg_select_from_tupleset(trx, db, selects, res_tupleset, relattrs, agg_res);
      if(rc != RC::SUCCESS) {
        return rc;
      }
      res_tupleset = std::move(agg_res);
    } else {
      std::vector<TupleSet> tuplesets;
      tuplesets.push_back(std::move(res_tupleset));
      projection(tuplesets, selects);
      res_tupleset = std::move(tuplesets.front());
    }
  } else {
    std::vector<const RelAttr *> relattrs;
    have_agg_from_tupleset_for_group(selects, relattrs);
    // 聚合查询，先做合法性校验，比如AVG(birthday)是肯定不对的
    rc = check_agg(db, selects, relattrs);
    if(rc != RC::SUCCESS) {
      LOG_ERROR("aggregation is invalid\n");
      return rc;
    }
    TupleSet group_res;
    rc = groupby_select_from_tupleset(trx, db, selects, res_tupleset, group_res);
    res_tupleset = std::move(group_res);
  }

  if(curpos == 0){
      res_tupleset.print(ss);
      session_event->set_response(ss.str());
      for (SelectExeNode *& tmp_node: select_nodes) {
          delete tmp_node;
      }
      end_trx_if_need(session, trx, true);
      return rc;
  }else{
      for (SelectExeNode *& tmp_node: select_nodes) {
          delete tmp_node;
      }
      end_trx_if_need(session, trx, true);
      return rc;
  }


}
/*
 * @author: fzh
 * @what for: sub_select
 * begin -------------------------------------------------------------------------------------------
 */
bool ExecuteStage::if_need_change(TupleSet &res_tupleset,Query *sql,int curpos){
    bool need_change = false;
    if (res_tupleset.tuples().size() == 0) {
        return need_change;
    }
    for (int i = 0; i < res_tupleset.schema().fields().size(); ++i) {
        for (int j = curpos+1; j < sql->sstr.adv_selection[0].select_num; ++j) {
            for (int k = 0; k < sql->sstr.adv_selection[j].condition_num; ++k) {
                if ((sql->sstr.adv_selection[j].condition_exps[k].left->exp->is_attr == 1)
                    && (sql->sstr.adv_selection[j].condition_exps[k].left->exp->relation_name != nullptr)){
                    if (strcmp(res_tupleset.schema().field(i).table_name(),
                               sql->sstr.adv_selection[j].condition_exps[k].left->exp->relation_name) == 0
                        && strcmp(res_tupleset.schema().field(i).field_name(),
                                  sql->sstr.adv_selection[j].condition_exps[k].left->exp->attribute_name) ==
                           0) {
                        need_change = true;
                    }
                }
                if ((sql->sstr.adv_selection[j].condition_exps[k].right->exp->is_attr == 1)
                    && (sql->sstr.adv_selection[j].condition_exps[k].right->exp->relation_name != nullptr) ){
                    if (strcmp(res_tupleset.schema().field(i).table_name(),
                               sql->sstr.adv_selection[j].condition_exps[k].right->exp->relation_name) == 0
                        && strcmp(res_tupleset.schema().field(i).field_name(),
                                  sql->sstr.adv_selection[j].condition_exps[k].right->exp->attribute_name) ==
                           0) {
                        need_change = true;
                    }
                }

            }
        }
    }
    return need_change;
}
bool ExecuteStage::if_need_change(Selects &selects,TupleSet &res_tupleset,Query *sql){
    bool need_change = false;
    for (size_t i = 0; i < selects.condition_num; i++) {
        if (res_tupleset.tuples().size() == 0) {
            break;
        }
        if (selects.conditions[i].left_select_index != -1) {
            int left_select_index = selects.conditions[i].left_select_index;
            for (int j = 0; j < res_tupleset.schema().fields().size(); ++j) {
                for (int k = 0; k < sql->sstr.adv_selection[left_select_index].condition_num; ++k) {
                    if ((sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->is_attr == 1)
                    && (sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->relation_name != nullptr)) {
                        if (strcmp(res_tupleset.schema().field(j).table_name(),
                                   sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->relation_name) == 0
                            && strcmp(res_tupleset.schema().field(j).field_name(),
                                      sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->attribute_name) ==
                               0) {
                            need_change = true;
                        }
                    }
                    if ((sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->is_attr == 1)
                        && (sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->relation_name != nullptr) ){
                        if (strcmp(res_tupleset.schema().field(j).table_name(),
                                   sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->relation_name) == 0
                            && strcmp(res_tupleset.schema().field(j).field_name(),
                                      sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->attribute_name) ==
                               0) {
                            need_change = true;
                        }
                    }
                }
            }
        }
        if (selects.conditions[i].right_select_index != -1) {
            int right_select_index = selects.conditions[i].right_select_index;
            for (int j = 0; j < res_tupleset.schema().fields().size(); ++j) {
                for (int k = 0; k < sql->sstr.adv_selection[right_select_index].condition_num; ++k) {
                    if ((sql->sstr.adv_selection[right_select_index].condition_exps[k].left->exp->is_attr == 1)
                        && (sql->sstr.adv_selection[right_select_index].condition_exps[k].left->exp->relation_name != nullptr) ){
                        if (strcmp(res_tupleset.schema().field(j).table_name(),
                                   sql->sstr.adv_selection[right_select_index].condition_exps[k].left->exp->relation_name) == 0
                            && strcmp(res_tupleset.schema().field(j).field_name(),
                                      sql->sstr.adv_selection[right_select_index].condition_exps[k].left->exp->attribute_name) ==
                               0) {
                            need_change = true;
                        }
                    }
                    if ((sql->sstr.adv_selection[right_select_index].condition_exps[k].right->exp->is_attr == 1)
                        && (sql->sstr.adv_selection[right_select_index].condition_exps[k].right->exp->relation_name != nullptr) ){
                        if (strcmp(res_tupleset.schema().field(j).table_name(),
                                   sql->sstr.adv_selection[right_select_index].condition_exps[k].right->exp->relation_name) == 0
                            && strcmp(res_tupleset.schema().field(j).field_name(),
                                      sql->sstr.adv_selection[right_select_index].condition_exps[k].right->exp->attribute_name) ==
                               0) {
                            need_change = true;
                        }
                    }
                }

            }
        }
    }
    return need_change;
}
RC ExecuteStage::change_sub_select(TupleSet &res_tupleset,Query *sql,int cur_tuple_pos,int sel_pos){
    for (int i = 0; i < res_tupleset.schema().fields().size(); ++i) {
        for (int j = sel_pos+1; j < sql->sstr.adv_selection[0].select_num; ++j) {
            for (int k = 0; k < sql->sstr.adv_selection[j].condition_num; ++k) {
                if ( (sql->sstr.adv_selection[j].condition_exps[k].left->exp->relation_name != nullptr)){
                    if (strcmp(res_tupleset.schema().field(i).table_name(),
                               sql->sstr.adv_selection[j].condition_exps[k].left->exp->relation_name) == 0
                        && strcmp(res_tupleset.schema().field(i).field_name(),
                                  sql->sstr.adv_selection[j].condition_exps[k].left->exp->attribute_name) ==
                           0) {
                        sql->sstr.adv_selection[j].condition_exps[k].left->exp->is_attr = 0;
                        sql->sstr.adv_selection[j].condition_exps[k].left->exp->value.type = res_tupleset.schema().field(
                                i).type();
                        sql->sstr.adv_selection[j].condition_exps[k].left->exp->value.is_null = 0;
                        if (sql->sstr.adv_selection[j].condition_exps[k].left->exp->value.data !=
                            nullptr){
                            //free(sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->value.data);
                        }
                        sql->sstr.adv_selection[j].condition_exps[k].left->exp->value.data = res_tupleset.tuples()[cur_tuple_pos].get_pointer(
                                i)->get_value();
                    }
                }
                if ( (sql->sstr.adv_selection[j].condition_exps[k].right->exp->relation_name != nullptr) ){
                    if (strcmp(res_tupleset.schema().field(i).table_name(),
                               sql->sstr.adv_selection[j].condition_exps[k].right->exp->relation_name) == 0
                        && strcmp(res_tupleset.schema().field(i).field_name(),
                                  sql->sstr.adv_selection[j].condition_exps[k].right->exp->attribute_name) ==
                           0) {
                        sql->sstr.adv_selection[j].condition_exps[k].right->exp->is_attr = 0;
                        sql->sstr.adv_selection[j].condition_exps[k].right->exp->value.type = res_tupleset.schema().field(
                                i).type();
                        sql->sstr.adv_selection[j].condition_exps[k].right->exp->value.is_null = 0;
                        if(sql->sstr.adv_selection[j].condition_exps[k].right->exp->value.data !=
                           nullptr){
                            //free(sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->value.data);
                        }
                        sql->sstr.adv_selection[j].condition_exps[k].right->exp->value.data = res_tupleset.tuples()[cur_tuple_pos].get_pointer(
                                i)->get_value();
                    }
                }

            }
        }
    }
    return RC::SUCCESS;
}

RC ExecuteStage::change_sub_select(Selects &selects,TupleSet &res_tupleset,Query *sql,int cur_tuple_pos){
    for (size_t i = 0; i < selects.condition_num; i++) {
        if (res_tupleset.tuples().size() == 0) {
            break;
        }
        if (selects.conditions[i].left_select_index != -1) {
            int left_select_index = selects.conditions[i].left_select_index;
            for (int j = 0; j < res_tupleset.schema().fields().size(); ++j) {
                for (int k = 0; k < sql->sstr.adv_selection[left_select_index].condition_num; ++k) {
                    if (strcmp(res_tupleset.schema().field(j).table_name(),
                               sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->relation_name) == 0
                        && strcmp(res_tupleset.schema().field(j).field_name(),
                                  sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->attribute_name) ==
                           0) {
                        sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->is_attr = 0;
                        sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->value.type = res_tupleset.schema().field(
                                j).type();
                        sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->value.is_null = 0;
                        if (sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->value.data !=
                                nullptr){
                            //free(sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->value.data);
                        }
                        sql->sstr.adv_selection[left_select_index].condition_exps[k].left->exp->value.data = res_tupleset.tuples()[cur_tuple_pos].get_pointer(
                                j)->get_value();
                    }
                    if (strcmp(res_tupleset.schema().field(j).table_name(),
                               sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->relation_name) == 0
                        && strcmp(res_tupleset.schema().field(j).field_name(),
                                  sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->attribute_name) ==
                           0) {
                        sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->is_attr = 0;
                        sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->value.type = res_tupleset.schema().field(
                                j).type();
                        sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->value.is_null = 0;
                        if(sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->value.data !=
                                nullptr){
                            //free(sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->value.data);
                        }
                        sql->sstr.adv_selection[left_select_index].condition_exps[k].right->exp->value.data = res_tupleset.tuples()[cur_tuple_pos].get_pointer(
                                j)->get_value();
                    }
                }
            }
        }
        if (selects.conditions[i].right_select_index != -1) {
            int right_select_index = selects.conditions[i].right_select_index;
            for (int j = 0; j < res_tupleset.schema().fields().size(); ++j) {
                for (int k = 0; k < sql->sstr.adv_selection[right_select_index].condition_num; ++k) {
                    if (strcmp(res_tupleset.schema().field(j).table_name(),
                               sql->sstr.adv_selection[right_select_index].condition_exps[k].left->exp->relation_name) == 0
                        && strcmp(res_tupleset.schema().field(j).field_name(),
                                  sql->sstr.adv_selection[right_select_index].condition_exps[k].left->exp->attribute_name) ==
                           0) {
                        sql->sstr.adv_selection[right_select_index].condition_exps[k].left->exp->is_attr = 0;
                        sql->sstr.adv_selection[right_select_index].condition_exps[k].left->exp->value.type = res_tupleset.schema().field(
                                j).type();
                        sql->sstr.adv_selection[right_select_index].condition_exps[k].left->exp->value.is_null = 0;
                        if(sql->sstr.adv_selection[right_select_index].condition_exps[k].left->exp->value.data !=
                                nullptr){
                            //free(sql->sstr.adv_selection[right_select_index].condition_exps[k].left->exp->value.data);
                        }
                        sql->sstr.adv_selection[right_select_index].condition_exps[k].left->exp->value.data = res_tupleset.tuples()[cur_tuple_pos].get_pointer(
                                j)->get_value();
                    }
                    if (strcmp(res_tupleset.schema().field(j).table_name(),
                               sql->sstr.adv_selection[right_select_index].condition_exps[k].right->exp->relation_name) == 0
                        && strcmp(res_tupleset.schema().field(j).field_name(),
                                  sql->sstr.adv_selection[right_select_index].condition_exps[k].right->exp->attribute_name) ==
                           0) {
                        sql->sstr.adv_selection[right_select_index].condition_exps[k].right->exp->is_attr = 0;
                        sql->sstr.adv_selection[right_select_index].condition_exps[k].right->exp->value.type = res_tupleset.schema().field(
                                j).type();
                        sql->sstr.adv_selection[right_select_index].condition_exps[k].right->exp->value.is_null = 0;
                        if(sql->sstr.adv_selection[right_select_index].condition_exps[k].right->exp->value.data !=
                                nullptr){
                            //free(sql->sstr.adv_selection[right_select_index].condition_exps[k].right->exp->value.data);
                        }
                        sql->sstr.adv_selection[right_select_index].condition_exps[k].right->exp->value.data = res_tupleset.tuples()[cur_tuple_pos].get_pointer(
                                j)->get_value();
                    }
                }

            }
        }
    }
    return RC::SUCCESS;
}

RC ExecuteStage::check_sub_select(Selects &selects,Query *sql,std::vector<Condition> &sub_sel_conditions){
    RC rc = RC::SUCCESS;
    for(int i = 0; i < selects.condition_num ;++i){
        if (selects.conditions[i].left_select_index !=-1 || selects.conditions[i].right_select_index !=-1){
            if(selects.conditions[i].left_select_index !=-1){
                int left_select_index = selects.conditions[i].left_select_index;
                if(selects.conditions[i].comp == IN_SUB ||  selects.conditions[i].comp == NOT_IN_SUB){
                    LOG_ERROR("sub_select cant at left of in!");
                    return RC::SCHEMA;
                }
                if(sql->sstr.adv_selection[left_select_index].attr_num != 1){
                    LOG_ERROR("in filed too many!");
                    return RC::SCHEMA;
                }
            }
            if(selects.conditions[i].right_select_index !=-1){
                int right_select_index = selects.conditions[i].right_select_index;
                if(sql->sstr.adv_selection[right_select_index].attr_num != 1){
                    LOG_ERROR("in filed too many!");
                    return RC::SCHEMA;
                }
            }
            if (selects.conditions[i].left_is_attr && selects.conditions[i].left_attr.relation_name== nullptr){
                selects.conditions[i].left_attr.relation_name  = selects.relations[0];
            }
            if (selects.conditions[i].right_is_attr && selects.conditions[i].right_attr.relation_name== nullptr){
                selects.conditions[i].right_attr.relation_name  = selects.relations[0];
            }
            sub_sel_conditions.push_back(selects.conditions[i]);
        }
    }
    return rc;
}
RC ExecuteStage::do_sub_sel(const char *db, Query *sql, SessionEvent *session_event,std::vector<std::pair<TupleSet,TupleSet>> &tupleset_pair_list,std::vector<Condition> &sub_sel_conditions){
    RC rc = RC::SUCCESS;
    tupleset_pair_list.clear();
    for (int i = 0; i < sub_sel_conditions.size(); ++i) {
        std::pair<TupleSet,TupleSet> tmp_pair_set;
        if(sub_sel_conditions[i].left_select_index != -1){
            rc = do_select(db,sql,session_event,tmp_pair_set.first,sub_sel_conditions[i].left_select_index);
            if (rc != RC::SUCCESS){
                LOG_ERROR("do_sub_sel left wrong!");
                return rc;
            }
            if (tmp_pair_set.first.tuples().size() !=1 && sub_sel_conditions[i].comp>=EQUAL_TO && sub_sel_conditions[i].comp <= GREAT_THAN){
                LOG_ERROR("tuples num  wrong!");
                return RC::RECORD;
            }
        }
        if(sub_sel_conditions[i].right_select_index != -1){
            rc = do_select(db,sql,session_event,tmp_pair_set.second,sub_sel_conditions[i].right_select_index);
            if (rc != RC::SUCCESS){
                LOG_ERROR("do_sub_sel right wrong!");
                return rc;
            }
            if (tmp_pair_set.second.tuples().size() !=1 && sub_sel_conditions[i].comp>=EQUAL_TO && sub_sel_conditions[i].comp <= GREAT_THAN){
                LOG_ERROR("tuples num  wrong!");
                return RC::RECORD;
            }
        }
        tupleset_pair_list.push_back(std::move(tmp_pair_set));
    }
    if ( tupleset_pair_list.size() != sub_sel_conditions.size() ){
        LOG_ERROR("tupleset_pair_list.size != sub_sel_conditions.size!");
        rc = RC::SCHEMA;
    }
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
      LOG_ERROR("The date should not exceed February 2038\n");
      return RC::CONSTRAINT_CHECK; // ?这里要返回什么RC
    }
    if(year < 1970) {
      LOG_ERROR("The date should not be earlier than 1970-01-01\n");
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

// 在check_insert_stat中不用检验插入的数据是否合法,不用检查null相关,因为在后面的make_record()中会检查
RC ExecuteStage::check_insert_stat(const Inserts &inserts, SessionEvent *session_event){
  // 校验insert语句中的date字段是否符合要求，即满足日期小于2038年2月，以及满足闰年平年的要求
  for(size_t i = 0; i < inserts.value_list_length; i++){
    const Value *values = (const Value *)(inserts.values[i]);
    RC rc = check_date_from_values(inserts.value_num[i], values);
    if(rc != RC::SUCCESS) {
      char err[207];
      sprintf(err, "FAILURE\n");
      session_event->set_response(err);
      return RC::CONSTRAINT_CHECK; // ?这里要返回什么RC
    }
  }

  return RC::SUCCESS;
}
/*end ----------------------------------------------------------------------------------------------*/

/* @author: huahui 
 * @what for: 必做题，聚合查询 
 * relattrs中的属性肯定都存在，因为在调用这个函数之前进行了元数据校验
 * -----------------------------------------------------------------------------------------------------------------
 */
RC ExecuteStage::check_agg(const char *db, const Selects &selects, std::vector<const RelAttr *> &relattrs) {
  for(size_t i = 0; i < relattrs.size(); i++) {
    if(!relattrs[i]->is_attr) {
      continue;
    }
    // count(*)的情况
    if(strcmp(relattrs[i]->attribute_name,"*")==0 && relattrs[i]->agg_type==AggType::AGGCOUNT){
      continue;
    }

    const char *table_name = (relattrs[i]->relation_name == nullptr ? selects.relations[0] : relattrs[i]->relation_name);
    Table * table = DefaultHandler::get_default().find_table(db, table_name);
    if(!table) {
      return RC::SCHEMA_TABLE_NOT_EXIST;
    }
    const FieldMeta * fieldMeta = table->table_meta().field(relattrs[i]->attribute_name);
    if(strcmp(relattrs[i]->attribute_name,"*") != 0){
      if(!fieldMeta) {
        return RC::SCHEMA_FIELD_MISSING;
      }
    }
    
    // AVG(*)  AVG(birthday)是错的
    if((strcmp(relattrs[i]->attribute_name,"*")==0 || !fieldMeta->addable()) && relattrs[i]->agg_type==AggType::AGGAVG) {
      return RC::SQL_SYNTAX;
    }
    // MAX(*) MIN(*) AVG(*)是错误的
    if((relattrs[i]->agg_type==AggType::AGGMAX || relattrs[i]->agg_type==AggType::AGGMIN || relattrs[i]->agg_type==AggType::AGGAVG) 
        && strcmp(relattrs[i]->attribute_name,"*")==0) {
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
  for(int i = selection.attr_num - 1; i >= 0; i--) {
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
    if(!relattrs[i]->is_attr) {
      schema.add(false, relattrs[i]->agg_type, relattrs[i]->is_attr, relattrs[i]->agg_val_type, relattrs[i]->agg_val);
      continue;
    }
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
  return agg_node->execute(agg_res);
} 
/* ------------------------------------------------------------------------------------------------------------
 */

/* @author: huahui  @what for: null ------------------------------------------------------------------------------*/
// 检验select和update的语句中的where语句是否符合要求
// 检查属性, 表名是否合法, 支持有关null的校验
// 一般在select_check()的单表校验和update_check()函数中被调用
RC ExecuteStage::check_condition(int condition_num, const Condition *conditions, const Table * table) {
  RC rc = RC::SUCCESS;
  for (size_t j = 0; j < condition_num ; ++j) {
    AttrType left_at, right_at;
    int left_is_null = false, right_is_null = false; /* @author: huahui  @what for: null-----------------------------------*/
    if (conditions[j].left_is_attr){
      // 条件中不能带*
      if(strcmp("*", conditions[j].left_attr.attribute_name) == 0) {
          return RC::SQL_SYNTAX;
      }
      if(conditions[j].left_attr.relation_name != nullptr && strcmp(conditions[j].left_attr.relation_name, table->name())!=0) {
          return RC::SQL_SYNTAX;
      }
      const FieldMeta * fieldMeta = table->table_meta().field(conditions[j].left_attr.attribute_name);
      if (fieldMeta == nullptr){
          return RC::SCHEMA_FIELD_NOT_EXIST;
      }
      left_at = fieldMeta->type();
    } else { 
      // 检查date是否符合要求  (其实这个检验应该也没有必要)
      if(conditions[j].left_value.type == AttrType::DATES) {
        const Value *values = (const Value *)(&(conditions[j].left_value));
        rc = check_date_from_values(1, values);
        if(rc != RC::SUCCESS) {
          return rc;
        }
      }

      left_at = conditions[j].left_value.type;
      left_is_null = conditions[j].left_value.is_null;           /* @author: huahui  @what for: null-----------------------------------*/
    }
    if (conditions[j].right_is_attr){
      if(strcmp("*", conditions[j].right_attr.attribute_name) == 0) {
          return RC::SQL_SYNTAX;
      }
      if(conditions[j].right_attr.relation_name != nullptr && strcmp(conditions[j].right_attr.relation_name, table->name())!=0) {
          return RC::SQL_SYNTAX;
      }
      const FieldMeta * fieldMeta = table->table_meta().field(conditions[j].right_attr.attribute_name);
      if (fieldMeta == nullptr){
          return RC::SCHEMA_FIELD_NOT_EXIST;
      }
      right_at = fieldMeta->type();
    }else {
      // 检查date是否符合要求   (其实这个检验应该也没有必要)
      if(conditions[j].right_value.type == AttrType::DATES) {
        const Value *values = (const Value *)(&(conditions[j].right_value));
        rc = check_date_from_values(1, values);
        if(rc != RC::SUCCESS) {
          return rc;
        }
      }

      right_at = conditions[j].right_value.type;
      right_is_null = conditions[j].right_value.is_null;                       /* @author: huahui  @what for: null-----------------------------------*/
    }
    
    /* @author: huahui  @what for: null--------------------------------------------------------------*/
    // 检查如果有一个是值null，则可以跳过
    if(left_is_null || right_is_null) {
      continue;
    }

    /* @author: huahui  @what for: 元数据校验, where中int和float兼容 -----------------------------------------*/
    // 检查左右两边的类型是否相容，比如int和float就是相容的
    if(left_at != right_at && 
      !(left_at==AttrType::INTS && right_at==AttrType::FLOATS || left_at==AttrType::FLOATS && right_at==AttrType::INTS)) {
      return RC::SQL_SYNTAX;
    }
  }

  return RC::SUCCESS;
}
/* --------------------------------------------------------------------------------------------------------------*/

/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
bool ExecuteStage::convert_condexps_to_conds(int condition_num, ConditionExp condition_exps[], Condition conds[]) {
  bool flag;

  for(int i=0; i<condition_num; i++) {
    const ConditionExp &condexp = condition_exps[i];
    Condition cond;
    if(condexp.left->num == 1) {
      const Exp *exp = condexp.left->exp;
      if(!exp->have_brace && !exp->have_negative) {
        cond.left_is_attr = exp->is_attr;
        if(exp->is_attr) {
          RelAttr attr;
          relation_attr_init(&attr, exp->relation_name, exp->attribute_name);
   			  cond.left_attr = attr;
        } else {
          cond.left_value = exp->value;
        }
      }else {
        flag = false;
        break;
      }
    }else {
      flag = false;
      break;
    }

    if(condexp.right->num == 1) {
      const Exp *exp = condexp.right->exp;
      if(!exp->have_brace && !exp->have_negative) {
        cond.right_is_attr = exp->is_attr;
        if(exp->is_attr) {
          RelAttr attr;
          relation_attr_init(&attr, exp->relation_name, exp->attribute_name);
   			  cond.right_attr = attr;
        } else {
          cond.right_value = exp->value;
        }
      }else {
        flag = false;
        break;
      }
    }else {
      flag = false;
      break;
    }

    cond.comp = condexp.comp;

    conds[i] = cond;
  }

  return false;
}

// 如果所有select中的RelAttrExp能够转换为RelAttr并且所有where中的ConditioExp能够转换为Condtion，则转换，并返回true；否则，就返回false
bool ExecuteStage::convert_to_selects(const AdvSelects &adv_selects, Selects &selects) {
  memset(&selects, 0, sizeof(selects));
  bool flag = true;

  // 转换select clause
  for(int i=0; i<adv_selects.attr_num; i++) {
    const RelAttrExp &relattrexp = adv_selects.attr_exps[i];
    if(relattrexp.agg_type != AggType::NOTAGG) {
      RelAttr attr;
      attr.agg_type = relattrexp.agg_type;
      attr.is_attr = relattrexp.is_attr;
      attr.relation_name = relattrexp.agg_relation_name ? strdup(relattrexp.agg_relation_name) : nullptr;
      attr.attribute_name = relattrexp.agg_attribute_name ? strdup(relattrexp.agg_attribute_name) : nullptr;
      attr.agg_val_type = relattrexp.agg_val_type;
      attr.agg_val = relattrexp.agg_val;
      selects_append_attribute(&selects, &attr);
    } else if(relattrexp.is_star) {
      RelAttr attr;
 			relation_attr_init(&attr, relattrexp.relation_name, "*");
 			selects_append_attribute(&selects, &attr);
    } else if(relattrexp.num == 1) {
      const Exp *exp = relattrexp.explist->exp;
      if(!exp->have_brace && exp->is_attr && !exp->have_negative) {
        RelAttr attr;
        relation_attr_init(&attr, exp->relation_name, exp->attribute_name);
   			selects_append_attribute(&selects, &attr);
      }else {
        flag = false;
        break;
      }
    }else {
      flag = false;
      break;
    }
  }

  // 转换where clause
  for(int i=0; i<adv_selects.condition_num; i++) {
    const ConditionExp &condexp = adv_selects.condition_exps[i];
    Condition cond;
    if(condexp.left->num == 1) {
      const Exp *exp = condexp.left->exp;
      if(!exp->have_brace && !exp->have_negative) {
        cond.left_is_attr = exp->is_attr;
        if(exp->is_attr) {
          RelAttr attr;
          relation_attr_init(&attr, exp->relation_name, exp->attribute_name);
          cond.left_attr = attr;
          cond.left_select_index = -1;
        } else if(exp->sub_select_index == -1){
            cond.left_value = exp->value;
            cond.left_select_index = -1;
        } else{
            cond.left_select_index = exp->sub_select_index;
        }
      }else {
        flag = false;
        break;
      }
    }else {
      flag = false;
      break;
    }

    if(condexp.right->num == 1) {
      const Exp *exp = condexp.right->exp;
      if(!exp->have_brace && !exp->have_negative) {
        cond.right_is_attr = exp->is_attr;
        if(exp->is_attr) {
          RelAttr attr;
          relation_attr_init(&attr, exp->relation_name, exp->attribute_name);
          cond.right_attr = attr;
          cond.right_select_index = -1;
        } else if(exp->sub_select_index == -1){
          cond.right_value = exp->value;
          cond.right_select_index = -1;
        } else{
          cond.right_select_index = exp->sub_select_index;
        }
      }else {
        flag = false;
        break;
      }
    }else {
      flag = false;
      break;
    }

    cond.comp = condexp.comp;

    selects.conditions[selects.condition_num++] = cond;
  }
  
  // 转换from clause
  for(int i=0; i<adv_selects.relation_num; i++) {
    selects.relations[selects.relation_num++] = strdup(adv_selects.relations[i]);
  }

  /* @what for: order-by*/
  selects.order_num = adv_selects.order_num;
  for(int i = 0; i < adv_selects.order_num; i++) {
    selects.order_attrs[i] = adv_selects.order_attrs[i];
  }

  /* @what for: group-by*/
  selects.group_num = adv_selects.group_num;
  for(int i = 0; i < adv_selects.group_num; i++) {
    selects.group_attrs[i] = adv_selects.group_attrs[i];
  }
  
  return flag;
}

RC ExecuteStage::do_advselects(Trx *trx, const AdvSelects &adv_selects, const char *db, TupleSet &res_tupleset) {
  ExpSelectExeNode *esnode = new ExpSelectExeNode(trx, adv_selects, db);
  RC rc = RC::SUCCESS;

  rc = esnode->init();
  if(rc != RC::SUCCESS) {
    return rc;
  }
  rc = esnode->execute(res_tupleset);
  if(rc != RC::SUCCESS) {
    LOG_ERROR("ExpSelectExeNode::execute() runs wrong \n");
    delete esnode;
    return rc;
  }

  delete esnode;
  return RC::SUCCESS;
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


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
RC create_selection_executor(Trx *trx, const Selects &selects, const char *db, const char *table_name, SelectExeNode &select_node,Query *sql) {
  // 列出跟这张表关联的Attr
  RC rc = RC::SUCCESS;
  TupleSchema schema;
  Table * table = DefaultHandler::get_default().find_table(db, table_name);
  if (nullptr == table) {
    LOG_WARN("No such table [%s] in db [%s]", table_name, db);
    return RC::SCHEMA_TABLE_NOT_EXIST;
  }

  for (int i = selects.attr_num - 1; i >= 0; i--) {
    const RelAttr &attr = selects.attributes[i];
    // 如果是类似max(1)的情况，attr.attribute_name就是空的
    if (attr.attribute_name == nullptr) {
      continue;
    }
    /* ----------------------------------------------------------------------------------------------*/
    if (nullptr == attr.relation_name || 0 == strcmp(table_name, attr.relation_name)) {
      /* @author: huahui  @what for: 聚合查询  --------------------------------------------------------*/
      if (0 == strcmp("*", attr.attribute_name) || sql->sstr.adv_selection[0].select_num > 1) {
        /* @author: huahui  @what for: 聚合查询 多表查询  -----------------------------------------------*/
        // 列出这张表所有字段
        TupleSchema::from_table(table, schema, (selects.relation_num>1) );
        /* ----------------------------------------------------------------------------------------------*/
        break; // 没有校验，给出* 之后，再写字段的错误
      } else {
        /* @author: huahui  @what for: 聚合查询 多表查询  ---------------------------------------------------*/
        // 列出这张表相关字段
        rc = schema_add_field(table, attr.attribute_name, schema, (selects.relation_num>1));
        /* ---------------------------------------------------------------------------------------------------*/
        if (rc != RC::SUCCESS) {
          return rc;
        }
      }
    }
  }
  //添加条件两边都是列的字段
    for (size_t i = 0; i < selects.condition_num; i++) {
        const Condition &condition = selects.conditions[i];
        if(condition.left_is_attr == 1 && condition.right_is_attr == 1 ) {
            if (match_table(selects, condition.left_attr.relation_name, table_name) && (!match_table(selects, condition.right_attr.relation_name, table_name))){
                rc = schema_add_field(table,condition.left_attr.attribute_name,schema,true);
            }
            if ((!match_table(selects, condition.left_attr.relation_name, table_name)) && match_table(selects, condition.right_attr.relation_name, table_name)){
                rc = schema_add_field(table,condition.right_attr.attribute_name,schema,true);
            }
        }
    }
    /*
     * 可以优化，将schema按照原表的先后顺序排好，后面就很可能少排一次
    */

  // 找出仅与此表相关的过滤条件, 或者都是值的过滤条件
  std::vector<DefaultConditionFilter *> condition_filters;
  for (size_t i = 0; i < selects.condition_num; i++) {
    const Condition &condition = selects.conditions[i];
    if ((condition.left_is_attr == 0 && condition.right_is_attr == 0 && condition.left_select_index == -1 && condition.right_select_index == -1) || // 两边都是值
        (condition.left_is_attr == 1 && condition.right_is_attr == 0 && condition.right_select_index == -1 && match_table(selects, condition.left_attr.relation_name, table_name)) ||  // 左边是属性右边是值
        (condition.left_is_attr == 0 && condition.right_is_attr == 1 && condition.left_select_index == -1 && match_table(selects, condition.right_attr.relation_name, table_name)) ||  // 左边是值，右边是属性名
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

  /* @author: huahui  @what for: order-by <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  // 找出与这个表相关的order-by属性
  for(size_t i = 0; i < selects.order_num; i++) {
    const OrderAttr &order_attr = selects.order_attrs[i];
    if(match_table(selects, order_attr.relation_name, table_name)) {
      schema_add_field(table, order_attr.attribute_name, schema, selects.relation_num>1);
    }
  }
  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

  /* @author: huahui  @what for: group-by <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  // 找出与这个表相关的group-by属性
  for(size_t i = 0; i < selects.group_num; i++) {
    const GroupAttr &group_attr = selects.group_attrs[i];
    if(match_table(selects, group_attr.relation_name, table_name)) {
      schema_add_field(table, group_attr.attribute_name, schema, selects.relation_num>1);
    }
  }
  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

  return select_node.init(trx, table, std::move(schema), std::move(condition_filters));
}

/* @author: huahui  @what for: group-by <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
// 从tuple_set中做分组，甚至在分组内做聚合，结果放在res_tupleset中
// 已经做好了groupby的鲁棒性判断：比如选择的属性必然在groupby属性内
// tuple_set是按照groupby的要求排序好的
RC ExecuteStage::groupby_select_from_tupleset(Trx *trx, const char *db, const Selects &selects, TupleSet &tuple_set, TupleSet &res_tupleset) {
  RC rc = RC::SUCCESS;
  GroupbyExeNode *groupby_node = new GroupbyExeNode(trx, std::move(tuple_set), selects);
  rc = groupby_node->execute(res_tupleset);
  delete groupby_node;
  return rc;
}

// 对于带有groupby的查询，看是否有聚合属性，与have_agg_from_tupleset的不同是，这里可以支持属性和聚合属性同时存在
// 已经对groupby做好了鲁棒性判断
void ExecuteStage::have_agg_from_tupleset_for_group(const Selects &selection, std::vector<const RelAttr *> &relattrs) {
  for(int i = selection.attr_num - 1; i >= 0; i--) {
    const RelAttr &relattr = selection.attributes[i];
    if(relattr.agg_type != AggType::NOTAGG) {
      relattrs.push_back(&(selection.attributes[i]));
    }
  }
}

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
