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

#include "sql/executor/execution_node.h"
#include "storage/common/table.h"
#include "common/log/log.h"

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
        for(auto it_field = itleft->get_schema().fields().begin();it_field != itleft->get_schema().fields().end(); ++it_field){
            tmpschema.add_if_not_exists(it_field->type(),it_field->table_name(),it_field->field_name());
        }
        for(auto it_field = itright->get_schema().fields().begin();it_field != itright->get_schema().fields().end(); ++it_field){
            tmpschema.add_if_not_exists(it_field->type(),it_field->table_name(),it_field->field_name());
        }
        tmptupset.set_schema(tmpschema);
        for(auto it_tuplel = itleft->tuples().begin();it_tuplel != itleft->tuples().end();++it_tuplel){
            for(auto it_tupler = itright->tuples().begin();it_tupler != itright->tuples().end();++it_tupler){
                Tuple tmptuple;
                for (int i = 0; i < it_tuplel->size(); ++i) {
                    tmptuple.add(it_tuplel->get_pointer(i));
                }
                for (int i = 0; i < it_tupler->size() ; ++i) {
                    tmptuple.add(it_tupler->get_pointer(i));
                }
                tmptupset.add(std::move(tmptuple));
            }
        }
        tuple_sets.pop_back();
        tuple_sets.pop_back();
        tuple_sets.push_back(std::move(tmptupset));
    }
}

RC JoinExeNode::execute(std::vector<TupleSet> &tuplesets) {
    RC rc = RC::SUCCESS;
    //遍历所有左右均有列名的条件
    for (auto it_condition = conditions_.begin();it_condition != conditions_.end();){
        std::vector<TupleSet>::iterator itleft;
        std::vector<TupleSet>::iterator itright;
        int indexl=-1;
        int indexr=-1;
        //找到condition涉及的tuoleset在tuplesets的位置(itleft/itright)以及列位置(indexl/indexr)
        for(auto it_tuple = tuplesets.begin();it_tuple !=tuplesets.end(); ++it_tuple){
            int count=0;
            for(auto it_field = it_tuple->get_schema().fields().begin();it_field != it_tuple->get_schema().fields().end(); ++it_field){
                if (strcmp(it_field->table_name(),it_condition->left_attr.relation_name) == 0 && strcmp(it_field->field_name(),it_condition->left_attr.attribute_name) == 0){
                    itleft = it_tuple;
                    indexl=count;
                    break;
                }
                count++;
            }
            if (indexl != -1){
                break;
            }
        }
        for(auto it_tuple = tuplesets.begin();it_tuple !=tuplesets.end(); ++it_tuple){
            int count=0;
            for(auto it_field = it_tuple->get_schema().fields().begin();it_field != it_tuple->get_schema().fields().end(); ++it_field){
                if (strcmp(it_field->table_name(),it_condition->right_attr.relation_name) == 0 && strcmp(it_field->field_name(),it_condition->right_attr.attribute_name) == 0){
                    itright = it_tuple;
                    indexr=count;
                    break;
                }
                count++;
            }
            if (indexr != -1){
                break;
            }
        }
        if (indexl == -1 || indexr == -1){
            continue;
        }
        TupleSet tmptupset;
        TupleSchema tmpschema;
        //合并两个tupleset中的属性
        for(auto it_field = itleft->get_schema().fields().begin();it_field != itleft->get_schema().fields().end(); ++it_field){
            tmpschema.add_if_not_exists(it_field->type(),it_field->table_name(),it_field->field_name());
        }
        for(auto it_field = itright->get_schema().fields().begin();it_field != itright->get_schema().fields().end(); ++it_field){
            tmpschema.add_if_not_exists(it_field->type(),it_field->table_name(),it_field->field_name());
        }
        tmptupset.set_schema(tmpschema);

        //合并两tupleset满足条件的元组数据
        for(auto it_tuplel = itleft->tuples().begin();it_tuplel != itleft->tuples().end();++it_tuplel){
            for(auto it_tupler = itright->tuples().begin();it_tupler != itright->tuples().end();++it_tupler){
                if (it_tuplel->get(indexl).compare(it_tupler->get(indexr)) == 0){
                    Tuple tmptuple;
                    for (int i = 0; i < it_tuplel->size(); ++i) {
                        tmptuple.add(it_tuplel->get_pointer(i));
                    }
                    for (int i = 0; i < it_tupler->size() ; ++i) {
                        tmptuple.add(it_tupler->get_pointer(i));
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
        it_condition=conditions_.erase(it_condition);
        if (tuplesets.size() == 1){
            break;
        }
    }
    if(tuplesets.size()>1){
        this->Cart(tuplesets);
    }
    return rc;
}