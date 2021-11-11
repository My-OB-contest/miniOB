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
// Created by Wangyunlai on 2021/5/14.
//

#include <storage/default/default_handler.h>
#include "sql/executor/execution_node.h"
#include "storage/common/table.h"
#include "common/log/log.h"
#include "sql/executor/value.h"
#include "sql/parser/parse_defs.h"
#include "sql/executor/tuple.h"
#include "storage/common/condition_filter.h"
#include "storage/common/field_meta.h"


SelectExeNode::SelectExeNode() : table_(nullptr) {
}

SelectExeNode::~SelectExeNode() {
  for (DefaultConditionFilter * &filter : condition_filters_) {
    delete filter;
  }
  condition_filters_.clear();
}

JoinExeNode::JoinExeNode() :trx_(nullptr){}



RC SelectExeNode::init(Trx *trx, Table *table, TupleSchema &&tuple_schema, std::vector<DefaultConditionFilter *> &&condition_filters) {
  trx_ = trx;
  table_ = table;
  tuple_schema_ = tuple_schema;
  condition_filters_ = std::move(condition_filters);
  return RC::SUCCESS;
}

void record_reader(const char *data, void *context) {
  TupleRecordConverter *converter = (TupleRecordConverter *)context;
  converter->add_record(data);
}
RC SelectExeNode::execute(TupleSet &tuple_set) {
  CompositeConditionFilter condition_filter;
  condition_filter.init((const ConditionFilter **)condition_filters_.data(), condition_filters_.size());

  tuple_set.clear();
  tuple_set.set_schema(tuple_schema_);
  TupleRecordConverter converter(table_, tuple_set);
  return table_->scan_record(trx_, &condition_filter, -1, (void *)&converter, record_reader);
}

RC JoinExeNode::init(Trx *trx, const _Condition *conditions, int condition_num) {
    this->trx_=trx;
    //只放入左右均为属性，且属性所属不同表的条件（不考虑自连接）
    for (int i = 0; i < condition_num; ++i) {
        if(conditions[i].left_is_attr&&conditions[i].right_is_attr){
            if(strcmp(conditions[i].left_attr.attribute_name,conditions[i].right_attr.relation_name) != 0){
                this->conditions_.push_back(conditions[i]);
            }
        }
    }
    return RC::SUCCESS;
}
RC JoinExeNode::init(Trx *trx, const _Condition *conditions, int condition_num, const char *db, char *const *relations, int relation_num) {
    this->trx_=trx;
    // for (int i = 0; i < condition_num; ++i) {
    //     //因前面select_cehck已做筛选，不考虑表不存在的情况
    //     if (conditions[i].left_is_attr==0 || conditions[i].right_is_attr==0){
    //         continue;
    //     }
    //     Table *tablel = DefaultHandler::get_default().find_table(db,conditions[i].left_attr.relation_name);
    //     Table *tabler = DefaultHandler::get_default().find_table(db,conditions[i].right_attr.relation_name);
    //     const FieldMeta * fieldMetal = tablel->table_meta().field(conditions[i].left_attr.attribute_name);
    //     const FieldMeta * fieldMetar = tabler->table_meta().field(conditions[i].right_attr.attribute_name);
    //     if (fieldMetal->type() != fieldMetar->type()){
    //         LOG_ERROR("some join conditions field type not match");
    //         return RC::SCHEMA_FIELD_TYPE_MISMATCH;
    //     }
    // }
    for (int i = 0; i < relation_num; ++i) {
        selectorder[relations[i]]=i;
    }

    int tablecount=0;
    for (int i = 0; i < condition_num; ++i) {
        //左右必须都为列
        if (conditions[i].left_is_attr==0 || conditions[i].right_is_attr==0){
            continue;
        }
        //遍历所有连接条件找出，将所属同一对表的属性添加到一个数组里，如stu1.id = stu2.id和stu1.old = stu2.old要放一起
        // 此外注意stu2.old=stu1.old也是一样的
        std::string s1(conditions[i].left_attr.relation_name);
        std::string s2(conditions[i].right_attr.relation_name);
        std::string consumname=s1+s2;
        std::string consumnamev=s2+s1;
        if (sameTablecountmap.count(consumname) == 0){
            //如果map中没有则添加表对和对应二维数组位置和数量的map
            //并作初始化
            conditionset[tablecount][0]=conditions[i];
            ++sameTablecountmap[consumname].sameTableconNum;
            ++sameTablecountmap[consumnamev].sameTableconNum;
            sameTablecountmap[consumname].tablePos=tablecount;
            sameTablecountmap[consumnamev].tablePos=tablecount;
            ++tablecount;
        } else{
            //如果map中有，则通过map找到这个表对在二维数组中的位置，然后装入条件并把该表对的条件数量+1
            conditionset[sameTablecountmap[consumname].tablePos][sameTablecountmap[consumname].sameTableconNum]=conditions[i];
            ++sameTablecountmap[consumname].sameTableconNum;
            ++sameTablecountmap[consumnamev].sameTableconNum;
        }
    }
    //二维数组的第一列的大小joinTableNum，即表明一共有多少个不同的表对
    joinTableNum=tablecount;
    for( auto it : sameTablecountmap ){
        //初始化sameTableconNum数组，用来得知每种表对所涉及到的条件数量，用于后面形成and条件过滤器
        sameTableconNum[it.second.tablePos]=it.second.sameTableconNum;
    }

    return RC::SUCCESS;
}


RC JoinExeNode::execute(TupleSet &tuple_set) {
    return RC::SUCCESS;
}

//笛卡尔积
void JoinExeNode::Cart(std::vector<TupleSet> &tuple_sets){
    while (tuple_sets.size() > 1){
        //tuple_set在tuple_sets中是反的
        std::vector<TupleSet>::iterator itleft = tuple_sets.end()-1;
        std::vector<TupleSet>::iterator itright = tuple_sets.end()-2;
        TupleSet tmptupset;
        TupleSchema tmpschema;
        bool LorRflag[itleft->get_schema().fields().size()+itright->get_schema().fields().size()];
        int sumcount,leftcount,rightcount;
        sumcount=leftcount=rightcount=0;
        for (; leftcount < itleft->get_schema().fields().size()&&rightcount < itright->get_schema().fields().size(); ) {
            if (selectorder[itleft->get_schema().field(leftcount).table_name()] > selectorder[itright->get_schema().field(rightcount).table_name()]){
                tmpschema.add_if_not_exists(itleft->get_schema().field(leftcount).type(),itleft->get_schema().field(leftcount).table_name(),itleft->get_schema().field(leftcount).field_name(),
                                            true);
                leftcount++;
                LorRflag[sumcount++]=0;
            } else{
                tmpschema.add_if_not_exists(itright->get_schema().field(rightcount).type(),itright->get_schema().field(rightcount).table_name(),itright->get_schema().field(rightcount).field_name(),
                                            true);
                rightcount++;
                LorRflag[sumcount++]=1;
            }
        }
        while(leftcount < itleft->get_schema().fields().size()){
            tmpschema.add_if_not_exists(itleft->get_schema().field(leftcount).type(),itleft->get_schema().field(leftcount).table_name(),itleft->get_schema().field(leftcount).field_name(),
                                        true);
            leftcount++;
            LorRflag[sumcount++]=0;
        }
        while(rightcount < itright->get_schema().fields().size()){
            tmpschema.add_if_not_exists(itright->get_schema().field(rightcount).type(),itright->get_schema().field(rightcount).table_name(),itright->get_schema().field(rightcount).field_name(),
                                        true);
            rightcount++;
            LorRflag[sumcount++]=1;
        }
        /*for(auto it_field = itleft->get_schema().fields().begin();it_field != itleft->get_schema().fields().end(); ++it_field){
            tmpschema.add_if_not_exists(it_field->type(),it_field->table_name(),it_field->field_name(), true);
        }
        for(auto it_field = itright->get_schema().fields().begin();it_field != itright->get_schema().fields().end(); ++it_field){
            tmpschema.add_if_not_exists(it_field->type(),it_field->table_name(),it_field->field_name(), true);
        }
         */
        tmptupset.set_schema(tmpschema);
        for(auto it_tuplel = itleft->tuples().begin();it_tuplel != itleft->tuples().end();++it_tuplel){
            for(auto it_tupler = itright->tuples().begin();it_tupler != itright->tuples().end();++it_tupler){
                Tuple tmptuple;
                /*for (int i = 0; i < it_tuplel->size(); ++i) {
                    tmptuple.add(it_tuplel->get_pointer(i));
                }
                for (int i = 0; i < it_tupler->size() ; ++i) {
                    tmptuple.add(it_tupler->get_pointer(i));
                }
                 */
                int tuplel,tupler;
                tuplel=tupler=0;
                for (int i = 0; i < sumcount ; ++i) {
                    if (LorRflag[i]){
                        tmptuple.add(it_tupler->get_pointer(tupler));
                        tupler++;
                    } else{
                        tmptuple.add(it_tuplel->get_pointer(tuplel));
                        tuplel++;
                    }
                }
                tmptupset.add(std::move(tmptuple));
            }
        }
        tuple_sets.pop_back();
        tuple_sets.pop_back();
        tuple_sets.push_back(std::move(tmptupset));
    }
}

/* @author: fzh
 * 多表查询
 * 目前逻辑：将selects形成的TupleSets连接为一个tupleset给后续聚合使用
 * 连接时先取连接条件（目前未做join on 只是把左右都是不同表且属性相同的条件加到jionnode），
 * 将连接条件涉及的表先条件连接（后续要考虑索引等），然后对剩余的tupleset做笛卡尔积
 * -----------------------------------------------------------------------------------------------------------------
 */

RC JoinExeNode::execute(std::vector<TupleSet> &tuplesets) {
    RC rc = RC::SUCCESS;
    //遍历所有左右均有列名的条件
    for (int i_table = 0; i_table < joinTableNum; ++i_table) {
        std::vector<TupleSet>::iterator itleft;
        std::vector<TupleSet>::iterator itright;
        //找到condition涉及的tuoleset在tuplesets的位置(itleft/itright)
        for(auto it_tuple = tuplesets.begin();it_tuple !=tuplesets.end(); ++it_tuple){
            for(auto it_field = it_tuple->get_schema().fields().begin();it_field != it_tuple->get_schema().fields().end(); ++it_field){
                if (strcmp(it_field->table_name(),conditionset[i_table][0].left_attr.relation_name) ==0 ){
                    itleft = it_tuple;
                    break;
                }
            }

        }
        for(auto it_tuple = tuplesets.begin();it_tuple !=tuplesets.end(); ++it_tuple){
            for(auto it_field = it_tuple->get_schema().fields().begin();it_field != it_tuple->get_schema().fields().end(); ++it_field){
                if (strcmp(it_field->table_name(),conditionset[i_table][0].right_attr.relation_name) == 0 ){
                    itright = it_tuple;
                    break;
                }
            }

        }
        TupleSet tmptupset;
        TupleSchema tmpschema;
        //合并两个tupleset中的属性
        //按照select from或者join的顺序归并两个schema，同时记录每次合并的是左表还是右表，后续合并tuple使用
        bool LorRflag[itleft->get_schema().fields().size()+itright->get_schema().fields().size()];
        int sumcount,leftcount,rightcount;
        sumcount=leftcount=rightcount=0;
        for (; leftcount < itleft->get_schema().fields().size()&&rightcount < itright->get_schema().fields().size(); ) {
            if (selectorder[itleft->get_schema().field(leftcount).table_name()] > selectorder[itright->get_schema().field(rightcount).table_name()]){
                tmpschema.add_if_not_exists(itleft->get_schema().field(leftcount).type(),itleft->get_schema().field(leftcount).table_name(),itleft->get_schema().field(leftcount).field_name(),
                                            true);
                leftcount++;
                LorRflag[sumcount++]=0;
            } else{
                tmpschema.add_if_not_exists(itright->get_schema().field(rightcount).type(),itright->get_schema().field(rightcount).table_name(),itright->get_schema().field(rightcount).field_name(),
                                            true);
                rightcount++;
                LorRflag[sumcount++]=1;
            }
        }
        while(leftcount < itleft->get_schema().fields().size()){
            tmpschema.add_if_not_exists(itleft->get_schema().field(leftcount).type(),itleft->get_schema().field(leftcount).table_name(),itleft->get_schema().field(leftcount).field_name(),
                                        true);
            leftcount++;
            LorRflag[sumcount++]=0;
        }
        while(rightcount < itright->get_schema().fields().size()){
            tmpschema.add_if_not_exists(itright->get_schema().field(rightcount).type(),itright->get_schema().field(rightcount).table_name(),itright->get_schema().field(rightcount).field_name(),
                                        true);
            rightcount++;
            LorRflag[sumcount++]=1;
        }
        tmptupset.set_schema(tmpschema);
        for(auto it_tuplel = itleft->tuples().begin();it_tuplel != itleft->tuples().end();++it_tuplel){
            for(auto it_tupler = itright->tuples().begin();it_tupler != itright->tuples().end();++it_tupler){

                //将这对tupleset所涉及的连接条件综合，目前只支持and
                bool finalret = true;
                for (int i_sametable = 0; i_sametable < sameTableconNum[i_table]; ++i_sametable) {
                    TupleConditionFilter tupleFilter;
                    rc = tupleFilter.init(&(*it_tuplel),&(*it_tupler),&conditionset[i_table][i_sametable],itleft->get_schema(),itright->get_schema());
                    if (rc != RC::SUCCESS){
                        return rc;
                    }
                    finalret = finalret&&tupleFilter.filter();
                }

                if (finalret){
                    Tuple tmptuple;
                    int tuplel,tupler;
                    tuplel=tupler=0;
                    for (int i = 0; i < sumcount ; ++i) {
                        if (LorRflag[i]){
                            tmptuple.add(it_tupler->get_pointer(tupler));
                            tupler++;
                        } else{
                            tmptuple.add(it_tuplel->get_pointer(tuplel));
                            tuplel++;
                        }
                    }
                    tmptupset.add(std::move(tmptuple));
                }
            }
        }
        int dis = itleft-itright;
        std::vector<TupleSet>::iterator itleft2 = tuplesets.erase(itleft);
        if (dis>0){
            tuplesets.erase(itleft2-dis);
        }
        if (dis<0){
            tuplesets.erase(itleft2-dis-1);
        }
        tuplesets.push_back(std::move(tmptupset));
        if (tuplesets.size() == 1){
            break;
        }

    }
    if(tuplesets.size()>1){
        this->Cart(tuplesets);
    }
    return rc;
}





/* @author: huahui 
 * @what for: 必做题，聚合查询 
 * 这是一个聚合查询结点，根据多表join的结果来进行聚合查询
 * -----------------------------------------------------------------------------------------------------------------
 */
AggExeNode::AggExeNode() {};
AggExeNode::~AggExeNode() {};
RC AggExeNode::init(Trx *trx, TupleSchema && tuple_schema, TupleSet && tuple_set) {
  trx_ = trx;
  tuple_schema_ = std::move(tuple_schema);
  tuple_set_ = std::move(tuple_set);
  return RC::SUCCESS;
}
RC AggExeNode::execute(TupleSet &res_tupleset) {
  res_tupleset.set_schema(tuple_schema_);
  int agg_num = tuple_schema_.fields().size();
  
  // 保存结果,如果是max, min, 就保存下标在first中，如果是count，就保存计数器在first中，如果是avg，就保存计数器和累加值在first和second中
  // 如果是max(5)，则保存5在first中，如果是min(5.6)则保存5.6在second中
  // 如果是count(*)，则保存tuple_set_的大小在first里面
  // 如果是count(attr)，则初始化first为0
  std::vector<std::pair<int, double> > res(agg_num);
  /* @what for: null  用于判断某个聚合属性是否有结果, 比如max(id)，但是id全是null，这样max(id)就没有结果了*/
  std::vector<int> have_res(agg_num, 0);
  // 如果是类似于count(1)这种形式的聚合，可以直接把结果算出来
  bool have_agg_attr = false; // 还存在类似于max(id)这种聚合属性
  for(int i = 0; i < agg_num; i++) {
    const TupleField &tuple_field = tuple_schema_.field(i);
    if(!tuple_field.get_is_attr()) {
      have_res[i] = 1;                                              /* @what for: null */
      if(tuple_field.getAggtype() == AggType::AGGMAX || tuple_field.getAggtype() == AggType::AGGMIN) {
        if(tuple_field.get_agg_val_type() == AggValType::AGGNUMBER) {
          res[i].first = tuple_field.get_agg_val().intv;
        }else{
          res[i].second = tuple_field.get_agg_val().floatv;
        }
      }else if(tuple_field.getAggtype() == AggType::AGGCOUNT) {
        res[i].first = tuple_set_.size();
      }else {
        if(tuple_field.get_agg_val_type() == AggValType::AGGNUMBER) {
          res[i].second = tuple_field.get_agg_val().intv;
        }else{
          res[i].second = tuple_field.get_agg_val().floatv;
        }
      }
    } else if(tuple_field.getAggtype()==AggType::AGGCOUNT) { // 注意，这个时候聚合函数只能是count，否则在前面的判断阶段会出错
      have_res[i] = 1;  
      if(strcmp(tuple_field.field_name(), "*") == 0) {
        res[i].first = tuple_set_.size();
      } else {
        have_agg_attr = true;
        res[i].first = 0;
      }
    } else{
      have_agg_attr = true;
    }
  }

  for(int i = 0; i < tuple_set_.size() && have_agg_attr; i++) {
    const Tuple &tuple = tuple_set_.get(i);
    for(int j = 0; j < agg_num; j++) {
      const TupleField &tuple_field = tuple_schema_.field(j);
      int idx = index_of_field(tuple_field);
      if(!tuple_field.get_is_attr()) {
        continue;
      }
      AggType aggtype = tuple_field.getAggtype();
      if(aggtype == AggType::NOTAGG) {
        return RC::SQL_SYNTAX;
      }
      if(strcmp(tuple_field.field_name(), "*") == 0) { // 注意，这个时候聚合函数只能是count，否则在前面的判断阶段会出错
        continue;
      }
      /* @author: huahui  @what for: null --------------------------------------------------------*/
      // 当聚合某个属性的时候，如果这个属性是null，可以直接跳过
      if(strcmp(tuple_field.field_name(), "*")!=0 && std::dynamic_pointer_cast<NullValue>(tuple.get_pointer(idx))) {
        continue;
      }
      /* -------------------------------------------------------------------------------------------*/
                                                   
      if(aggtype == AggType::AGGMAX) {
        if(tuple_field.type() == AttrType::UNDEFINED){
          return RC::SQL_SYNTAX;
        }
        if(!have_res[j]) {
          res[j].first = i;
        }else{
          if(tuple.get_pointer(idx)->compare(tuple_set_.get(res[j].first).get(idx)) > 0) {
            res[j].first = i;
          }
        }
      }else if(aggtype == AggType::AGGMIN) {
        if(tuple_field.type() == AttrType::UNDEFINED){
          return RC::SQL_SYNTAX;
        }
        if(!have_res[j]) {
          res[j].first = i;
        } else {
          if(tuple.get_pointer(idx)->compare(tuple_set_.get(res[j].first).get(idx)) < 0) {
            res[j].first = i;
          }
        }
      } else if(aggtype == AggType::AGGCOUNT) {
        if(tuple_field.type() == AttrType::UNDEFINED){
          return RC::SQL_SYNTAX;
        }
        if(!have_res[j]) {
          res[j].first = 1; 
        }else {
          res[j].first += 1; 
        }
      }else {
        if(tuple_field.type() == AttrType::CHARS || tuple_field.type() == AttrType::DATES || tuple_field.type() == AttrType::UNDEFINED) {
          return RC::SQL_SYNTAX;
        }
        if(!have_res[j]) {
          res[j].first = 1;
          res[j].second = 0.0;
          res[j].second += getNum(tuple.get_pointer(idx), tuple_field.type());
        }else{
          res[j].first += 1;
          res[j].second += getNum(tuple.get_pointer(idx), tuple_field.type());
        }
      }
      have_res[j] = 1;         /* @what for: null */
    }
  }

  Tuple tuple;
  for(int j = 0; j < agg_num; j++) {
    /* @what for: null */
    if(!have_res[j]) {
      tuple.add(new NullValue());
      continue;
    }

    const TupleField &tuple_field = tuple_schema_.field(j);
    if(!tuple_field.get_is_attr()) {
      if(tuple_field.getAggtype() == AggType::AGGMAX || tuple_field.getAggtype() == AggType::AGGMIN) {
        if(tuple_field.get_agg_val_type() == AggValType::AGGNUMBER) {
          tuple.add(res[j].first);
        }else{
          tuple.add((float)res[j].second);
        }
      }else if(tuple_field.getAggtype() == AggType::AGGCOUNT) {
        tuple.add(res[j].first);
      }else{
        tuple.add((float)res[j].second);
      }
      continue;
    }

    if(tuple_schema_.field(j).getAggtype() == AggType::AGGMAX || tuple_schema_.field(j).getAggtype() == AggType::AGGMIN) {
      const TupleField &tuple_field = tuple_schema_.field(j);
      int idx = index_of_field(tuple_field);
      if(tuple_field.type() == AttrType::INTS) {
        std::shared_ptr<IntValue> tv = std::static_pointer_cast<IntValue>(tuple_set_.get(res[j].first).get_pointer(idx));
        tuple.add(tv);
      }else if(tuple_field.type() == AttrType::FLOATS) {
        std::shared_ptr<FloatValue> tv = std::static_pointer_cast<FloatValue>(tuple_set_.get(res[j].first).get_pointer(idx));
        tuple.add(tv);
      }else if(tuple_field.type() == AttrType::CHARS) {
        std::shared_ptr<StringValue> tv = std::static_pointer_cast<StringValue>(tuple_set_.get(res[j].first).get_pointer(idx));
        tuple.add(tv);
      }else {
        std::shared_ptr<DateValue> tv = std::static_pointer_cast<DateValue>(tuple_set_.get(res[j].first).get_pointer(idx));
        tuple.add(tv);
      }
    }else if(tuple_schema_.field(j).getAggtype() == AggType::AGGCOUNT) {
      tuple.add(res[j].first);
    }else {
      tuple.add((float)res[j].second / res[j].first);
    }
  }

  res_tupleset.add(std::move(tuple));

  return RC::SUCCESS;
}

// 由于tuple_schema_和tuple_set_.tuple_schema_是不一样的，比如：
// 对于查询select max(f), min(f) from t, tuple_chema_包含两个tuple_field: (max(f), min(f))，
// tuple_set_.tuple_schema_仅包含一个tuple_filed: (f)
// 所以要找出tuple_chema_中的tuple_field对应于tuple_set_.tuple_schema_中的下标
int AggExeNode::index_of_field(const TupleField &tuple_field) {
  return tuple_set_.get_schema().index_of_field(tuple_field.table_name(), tuple_field.field_name());
}
/*end ----------------------------------------------------------------------------------------------*/


/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

// 计算exp的结果，exp中未知数的结果在tuple中
// exp中relation_name可能为空，若为空，则说明
// 注意：exp和tuple_schema必须合法
RC cal_exp(const Exp *exp, const Tuple &tuple, const TupleSchema &tuple_schema, Value &value) {
  RC rc = RC::SUCCESS;
  Value leftexp_value, rightexp_value;
  if(exp->left_exp) {
    rc = cal_exp(exp->left_exp, tuple, tuple_schema, leftexp_value);
    if(rc != RC::SUCCESS) {
      return rc;
    }
  }
  if(exp->have_brace) {
    rc = cal_explist(exp->explist, tuple, tuple_schema, rightexp_value);
    if(rc != RC::SUCCESS) return rc;
  } else {
    if(exp->is_attr) {
      int field_index = tuple_schema.index_of_field((exp->relation_name?exp->relation_name:tuple_schema.field(0).table_name()), exp->attribute_name);
      const std::shared_ptr<TupleValue> &tv = tuple.get_pointer(field_index);
      /*if(tuple_schema.field(field_index).type() != AttrType::INTS && tuple_schema.field(field_index).type() != AttrType::FLOATS) {
        LOG_ERROR("%s.%s is not type of int or float\n", tuple_schema.field(field_index).table_name(), tuple_schema.field(field_index).field_name());
        return RC::SQL_SYNTAX;
      }*/
      if(std::dynamic_pointer_cast<NullValue>(tv)) {
        rightexp_value.is_null = 1;
        LOG_INFO("the field of expression: %s.%s is NULL\n", exp->relation_name?exp->relation_name:tuple_schema.field(0).table_name(), exp->attribute_name);
      }else {
        if(tuple_schema.field(field_index).type() == AttrType::INTS) {
          std::shared_ptr<IntValue> inttv = std::dynamic_pointer_cast<IntValue>(tv);
          rightexp_value.is_null = 0;
          rightexp_value.type = AttrType::INTS;
          rightexp_value.data = malloc(sizeof(int));
          int v = inttv->getValue();
          memcpy(rightexp_value.data, &v, sizeof(int));
        } else if(tuple_schema.field(field_index).type() == AttrType::FLOATS) {
          std::shared_ptr<FloatValue> floattv = std::dynamic_pointer_cast<FloatValue>(tv);
          rightexp_value.is_null = 0;
          rightexp_value.type = AttrType::FLOATS;
          rightexp_value.data = malloc(sizeof(float));
          float v = floattv->getValue();
          memcpy(rightexp_value.data, &v, sizeof(float));
        } else if(tuple_schema.field(field_index).type() == AttrType::CHARS) {
          std::shared_ptr<StringValue> strtv = std::dynamic_pointer_cast<StringValue>(tv);
          rightexp_value.is_null = 0;
          rightexp_value.type = AttrType::CHARS;
          rightexp_value.data = strtv->get_value();
        } else if(tuple_schema.field(field_index).type() == AttrType::DATES) {
          std::shared_ptr<DateValue> datetv = std::dynamic_pointer_cast<DateValue>(tv);
          rightexp_value.is_null = 0;
          rightexp_value.type = AttrType::DATES;
          rightexp_value.data = datetv->get_value();
        }
      }
    } else {
      /*if(exp->value.is_null) {
        LOG_INFO("some item in expression is NULL\n");
      }else if(exp->value.type!=AttrType::INTS && exp->value.type!=AttrType::FLOATS) {
        LOG_ERROR("some constant in expression is not type of int or float\n");
        if(exp->left_exp) value_destroy(&leftexp_value);
        return RC::SQL_SYNTAX;
      }*/
      if(exp->value.is_null) {
        rightexp_value.is_null = 1;
      } else if(exp->value.type == AttrType::INTS) {
        rightexp_value.is_null = 0;
        rightexp_value.type = AttrType::INTS;
        rightexp_value.data = malloc(sizeof(int));
        memcpy(rightexp_value.data, exp->value.data, sizeof(int));
      } else if(exp->value.type == AttrType::FLOATS){
        rightexp_value.is_null = 0;
        rightexp_value.type = AttrType::FLOATS;
        rightexp_value.data = malloc(sizeof(float));
        memcpy(rightexp_value.data, exp->value.data, sizeof(float));
      } else if(exp->value.type == AttrType::CHARS){
        rightexp_value.is_null = 0;
        rightexp_value.type = AttrType::CHARS;
        char *s = strdup((char *)(exp->value.data));
        rightexp_value.data = (void *)s;
      } else if(exp->value.type == AttrType::DATES) {
        rightexp_value.is_null = 0;
        rightexp_value.type = AttrType::DATES;
        rightexp_value.data = malloc(4);
        memcpy(rightexp_value.data, exp->value.data, 4);
      }
    }
  }

  if((exp->left_exp && leftexp_value.is_null) || rightexp_value.is_null) {
    if(exp->left_exp) {
      value_destroy(&leftexp_value);
    }
    value_destroy(&rightexp_value);
    value.is_null = 1;
    return RC::SUCCESS;
  }else {
    if(!exp->left_exp) {
      value = rightexp_value;
      return RC::SUCCESS;
    } else {
      // 解决除0错误
      if(exp->calop==CalOp::DIVIDE_OP && (rightexp_value.type==AttrType::INTS && (*(int *)(rightexp_value.data)) == 0 || rightexp_value.type==AttrType::FLOATS && (*(float *)(leftexp_value.data)) == 0.0)) {
        value.is_null = 1;
        value_destroy(&leftexp_value);
        value_destroy(&rightexp_value);
        return RC::SUCCESS;
      }
      if(leftexp_value.type==AttrType::INTS && rightexp_value.type==AttrType::INTS 
         && (exp->calop!=CalOp::DIVIDE_OP || (*(int *)(leftexp_value.data)) % (*(int *)(rightexp_value.data)) == 0)) {
        int res;
        int lv = (*(int *)(leftexp_value.data)), rv = (*(int *)(rightexp_value.data));
        switch(exp->calop) {
        case CalOp::DIVIDE_OP:
          res = lv / rv;
          break;
        case CalOp::TIME_OP:
          res = lv * rv;
          break;
        default:
          LOG_ERROR("invalid calop, Exp struct must only use DIVIDE or TIME \n");
          value_destroy(&leftexp_value);
          value_destroy(&rightexp_value);
          return RC::SQL_SYNTAX;
        }
        value.is_null = 0;
        value.type = AttrType::INTS;
        value.data = malloc(sizeof(int));
        memcpy(value.data, &res, sizeof(int));
      } else {
        float res;
        float lv, rv;
        if(leftexp_value.type == AttrType::INTS) {
          lv = (float)(*(int *)(leftexp_value.data));
        } else {
          lv = (*(float *)(leftexp_value.data));
        }
        if(rightexp_value.type == AttrType::INTS) {
          rv = (float)(*(int *)(rightexp_value.data));
        } else {
          rv = (*(float *)(rightexp_value.data));
        }
        switch(exp->calop) {
        case CalOp::DIVIDE_OP:
          res = lv / rv;
          break;
        case CalOp::TIME_OP:
          res = lv * rv;
          break;
        default:
          LOG_ERROR("invalid calop, Exp struct must only use DIVIDE or TIME \n");
          value_destroy(&leftexp_value);
          value_destroy(&rightexp_value);
          return RC::SQL_SYNTAX;
        }
        value.is_null = 0;
        value.type = AttrType::FLOATS;
        value.data = malloc(sizeof(float));
        memcpy(value.data, &res, sizeof(float));
      }
      value_destroy(&leftexp_value);
      value_destroy(&rightexp_value);
      return RC::SUCCESS;
    }
  }
}

// 计算explist的结果，explist中未知数的值在tuple中
// 结果输出在value_type和value中
// 注意：explist和tuple_schema必须合法
RC cal_explist(const ExpList *explist, const Tuple &tuple, const TupleSchema &tuple_schema, Value &value) {
  RC rc = RC::SUCCESS;
  Value leftexplist_value, rightexplist_value;
  if(explist->left_explist) {
    rc = cal_explist(explist->left_explist, tuple, tuple_schema, leftexplist_value);
    if(rc != RC::SUCCESS) {
      return rc;
    }
  }
  rc = cal_exp(explist->exp, tuple, tuple_schema, rightexplist_value);
  if(rc != RC::SUCCESS) {
    return rc;
  }
  if((explist->left_explist && leftexplist_value.is_null) || rightexplist_value.is_null) {
    if(explist->left_explist) {
      value_destroy(&leftexplist_value);
    }
    value_destroy(&rightexplist_value);
    value.is_null = 1;
    return RC::SUCCESS;
  }
  
  if(explist->exp->have_negative) {
    LOG_INFO("the leftmost Exp of Explist has negative sign\n");
    if(rightexplist_value.type == AttrType::INTS) {
      int rv = *(int *)rightexplist_value.data;
      rv = -rv;
      memcpy(rightexplist_value.data, &rv, sizeof(int));
    } else {
      float rv = *(float *)rightexplist_value.data;
      rv = -rv;
      memcpy(rightexplist_value.data, &rv, sizeof(float));
    }
  }
  if(!explist->left_explist) {
    value = rightexplist_value;
    return RC::SUCCESS;
  }

  if(leftexplist_value.type==AttrType::INTS && rightexplist_value.type==AttrType::INTS) {
    int res;
    int lv = *(int *)leftexplist_value.data, rv = *(int *)rightexplist_value.data;
    switch(explist->calop) {
    case CalOp::PLUS_OP:
      res = lv + rv;
      break;
    case CalOp::MINUS_OP:
      res = lv - rv;
      break;
    default:
      LOG_ERROR("invalid calop, ExpList struct must only use PLUS or MINUS \n");
      value_destroy(&leftexplist_value);
      value_destroy(&rightexplist_value);
      return RC::SQL_SYNTAX;
    }
    value.is_null = 0;
    value.type = AttrType::INTS;
    value.data = malloc(sizeof(int));
    memcpy(value.data, &res, sizeof(int));
  } else {
    float res;
    float lv, rv;
    if(leftexplist_value.type == AttrType::INTS) {
      lv = (float)(*(int *)leftexplist_value.data);
    } else {
      lv = *(float *)leftexplist_value.data;
    }
    if(rightexplist_value.type == AttrType::INTS) {
      lv = (float)(*(int *)rightexplist_value.data);
    } else {
      rv = *(float *)rightexplist_value.data;
    }
    switch(explist->calop) {
    case CalOp::PLUS_OP:
      res = lv + rv;
      break;
    case CalOp::MINUS_OP:
      res = lv - rv;
      break;
    default:
      LOG_ERROR("invalid calop, ExpList struct must only use PLUS or MINUS \n");
      value_destroy(&leftexplist_value);
      value_destroy(&rightexplist_value);
      return RC::SQL_SYNTAX;
    }
    value.is_null = 0;
    value.type = AttrType::FLOATS;
    value.data = malloc(sizeof(float));
    memcpy(value.data, &res, sizeof(float));
  }
  value_destroy(&leftexplist_value);
  value_destroy(&rightexplist_value);
  return RC::SUCCESS;
}
// RelAttrExp是表达式种类吗，不包含非数值的单属性类型，比如date, string
bool relattrexp_isexp(const RelAttrExp &relattrexp, const char *first_table_name, const char *db) {
  if(relattrexp.agg_type != AggType::NOTAGG) {
    return false;
  }
  if(relattrexp.is_star) {
    return false;
  }
  if(relattrexp.num == 1 && !relattrexp.explist->exp->have_brace && !relattrexp.explist->exp->have_negative && relattrexp.explist->exp->is_attr) {
    const char *table_name = relattrexp.explist->exp->relation_name ? relattrexp.explist->exp->relation_name : first_table_name;
    Table *table = DefaultHandler::get_default().find_table(db, table_name);
    const FieldMeta *field_meta = table->table_meta().field(relattrexp.explist->exp->attribute_name);
    if(field_meta->type() != AttrType::INTS && field_meta->type() != AttrType::FLOATS) { // 虽然是单个属性，但是不是Int或float，所以应该是非表达式
      return false;
    } else {
      return true;
    }
  } else {
    return true;
  }
}
// RelAttrExp是单个的属性吗
bool relattrexp_issingle(const RelAttrExp &relattrexp, const char *first_table_name, const char *db) {
  if(relattrexp.agg_type != AggType::NOTAGG) {
    return false;
  }
  if(relattrexp.is_star) {
    return false;
  }
  if(relattrexp.num == 1 && !relattrexp.explist->exp->have_brace && !relattrexp.explist->exp->have_negative && relattrexp.explist->exp->is_attr) {
    const char *table_name = relattrexp.explist->exp->relation_name ? relattrexp.explist->exp->relation_name : first_table_name;
    Table *table = DefaultHandler::get_default().find_table(db, table_name);
    const FieldMeta *field_meta = table->table_meta().field(relattrexp.explist->exp->attribute_name);
    if(field_meta->type() != AttrType::INTS && field_meta->type() != AttrType::FLOATS) { // 虽然是单个属性，但是不是Int或float，所以应该是非表达式
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

ExpSelectExeNode::~ExpSelectExeNode(){
  if(filter_) delete filter_;
};

RC ExpSelectExeNode::init() {
  filter_ = nullptr;
  // your code
}

// 返回的res_tupleset不含有schema，因为表达式和TupleSchema结构不兼容
RC ExpSelectExeNode::execute(TupleSet &res_tupleset) {
  RC rc = RC::SUCCESS;

  std::vector<SelectExeNode *> select_nodes;
  for(int i=0; i<adv_selects_.relation_num; i++) {
    const char *table_name = adv_selects_.relations[i];
    SelectExeNode *select_node = new SelectExeNode();
    Table * table = DefaultHandler::get_default().find_table(db_, table_name);
    TupleSchema schema;
    std::vector<DefaultConditionFilter *> condition_filters;
    TupleSchema::from_table(table, schema, adv_selects_.relation_num>1);
    select_node->init(trx_, table, std::move(schema), std::move(condition_filters));
    select_nodes.push_back(select_node);
  }
  
  std::vector<TupleSet> tuple_sets;
  for (SelectExeNode *&node: select_nodes) {
    TupleSet tuple_set;
    rc = node->execute(tuple_set);
    if (rc != RC::SUCCESS) {
      for (SelectExeNode *& tmp_node: select_nodes) {
        delete tmp_node;
      }
      return rc;
    } else {
      tuple_sets.push_back(std::move(tuple_set));
    }
  }

  cart(tuple_sets);

  // 初始化filter
  filter_ = new ConditionExpsFilter();
  filter_->init(adv_selects_.condition_num, adv_selects_.condition_exps, tuple_sets[0].get_schema(), cal_explist);
  
  TupleSchema res_schema;
  for(int i = adv_selects_.attr_num-1; i >= 0; i--) {
    const RelAttrExp &relattrexp = adv_selects_.attr_exps[i];
    if(relattrexp.is_star) {
      if(relattrexp.relation_name) {
        Table * table = DefaultHandler::get_default().find_table(db_, relattrexp.relation_name);
        TupleSchema::from_table(table, res_schema, adv_selects_.relation_num>1);
      } else {
        for(int j = adv_selects_.relation_num - 1; j >= 0; j--) {
          Table *table = DefaultHandler::get_default().find_table(db_, adv_selects_.relations[j]);
          TupleSchema::from_table(table, res_schema, adv_selects_.relation_num>1);
        }
      }
    } else { 
      if(relattrexp_isexp(relattrexp, adv_selects_.relations[0], db_)) {
        res_schema.add_explist(relattrexp.explist);
      } else {
        const char *table_name = relattrexp.explist->exp->relation_name ? relattrexp.explist->exp->relation_name : adv_selects_.relations[0];
        Table *table = DefaultHandler::get_default().find_table(db_, table_name);
        const FieldMeta *field_meta = table->table_meta().field(relattrexp.explist->exp->attribute_name);
        res_schema.add_if_not_exists(field_meta->type(), table_name, relattrexp.explist->exp->attribute_name, adv_selects_.relation_num>1);
      }
    }
  }

  res_tupleset.set_schema(res_schema);
  for(int i = 0; i < tuple_sets[0].size(); i++) {
    if(filter_->filter(tuple_sets[0].get(i))) {
      Tuple tuple;
      for(int j = 0; j < res_tupleset.get_schema().fields().size(); j++) {
        const TupleField &tuple_field = res_tupleset.get_schema().field(j);
        if(tuple_field.get_is_explist()){
          Value value;
          rc = cal_explist(tuple_field.get_explist(), tuple_sets[0].tuples()[i], tuple_sets[0].get_schema(), value);
          if(rc != RC::SUCCESS) {
            LOG_ERROR("expression calculation invalid\n");
            return rc;
          }
          if(value.is_null) {
            tuple.add(new NullValue());
          } else if(value.type == AttrType::INTS) {
            tuple.add((*(int *)(value.data)));
          } else {
            tuple.add((*(float *)(value.data)));
          }
        }else {
          int index = tuple_sets[0].get_schema().index_of_field(tuple_field.table_name(), tuple_field.field_name());
          tuple.add(tuple_sets[0].tuples()[i].get_pointer(index));
        }
      }
      res_tupleset.add(std::move(tuple));
    }
  }

  return RC::SUCCESS;
}

void ExpSelectExeNode::cart(std::vector<TupleSet> &tuple_sets){
    while (tuple_sets.size() > 1){
        //tuple_set在tuple_sets中是反的
        std::vector<TupleSet>::iterator itleft = tuple_sets.end()-1;
        std::vector<TupleSet>::iterator itright = tuple_sets.end()-2;
        TupleSet tmptupset;
        TupleSchema tmpschema;
        
        int leftcount = 0, rightcount = 0;
        for (leftcount=0; leftcount < itleft->get_schema().fields().size(); leftcount++) {
            tmpschema.add_if_not_exists(itleft->get_schema().field(leftcount).type(),itleft->get_schema().field(leftcount).table_name(),itleft->get_schema().field(leftcount).field_name(),
                                            true);
        }
        for (rightcount=0; rightcount < itright->get_schema().fields().size(); rightcount++) {
            tmpschema.add_if_not_exists(itright->get_schema().field(rightcount).type(),itright->get_schema().field(rightcount).table_name(),itright->get_schema().field(rightcount).field_name(),
                                            true);
        }
        
        tmptupset.set_schema(tmpschema);
        for(auto it_tuplel = itleft->tuples().begin();it_tuplel != itleft->tuples().end();++it_tuplel){
            for(auto it_tupler = itright->tuples().begin();it_tupler != itright->tuples().end();++it_tupler){
                Tuple tmptuple;
                int tuplel,tupler;
                tuplel=tupler=0;
                
                for(tuplel=0; tuplel<it_tuplel->size(); tuplel++) {
                  tmptuple.add(it_tuplel->get_pointer(tuplel));
                }
                for(tupler=0; tupler<it_tupler->size(); tupler++) {
                  tmptuple.add(it_tupler->get_pointer(tupler));
                }
                tmptupset.add(std::move(tmptuple));
            }
        }
        tuple_sets.pop_back();
        tuple_sets.pop_back();
        tuple_sets.push_back(std::move(tmptupset));
    }
}

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

