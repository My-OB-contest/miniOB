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
// Created by Wangyunlai on 2021/5/13.
//

#ifndef __OBSERVER_SQL_EXECUTOR_EXECUTION_NODE_H_
#define __OBSERVER_SQL_EXECUTOR_EXECUTION_NODE_H_

#include <vector>
#include "storage/common/condition_filter.h"
#include "sql/executor/tuple.h"

class Table;
class Trx;

class ExecutionNode {
public:
  ExecutionNode() = default;
  virtual ~ExecutionNode() = default;

  virtual RC execute(TupleSet &tuple_set) = 0;
};

class SelectExeNode : public ExecutionNode {
public:
  SelectExeNode();
  virtual ~SelectExeNode();

  RC init(Trx *trx, Table *table, TupleSchema && tuple_schema, std::vector<DefaultConditionFilter *> &&condition_filters);

  RC execute(TupleSet &tuple_set) override;
private:
  Trx *trx_ = nullptr;
  Table  * table_;
  TupleSchema  tuple_schema_;
  std::vector<DefaultConditionFilter *> condition_filters_;
};
class Tablemapinfo{

public:
    int sameTableconNum = 0;
    int tablePos = 0;
};

class JoinExeNode : public ExecutionNode {
public:
    JoinExeNode();


    RC init(Trx *trx, const _Condition *conditions, int condition_num);

    RC init(Trx *trx, const _Condition *conditions, int condition_num,const char *db);

    RC execute(TupleSet &tuple_set) override;

    RC execute(std::vector<TupleSet> &tuple_set) ;

    void Cart(std::vector<TupleSet> &tuple_set);
private:
    Trx *trx_ = nullptr;
    std::vector<Condition> conditions_;
    Condition conditionset[MAX_NUM][MAX_NUM];
    //joinTableNum表明conditionset第一列的数量
    int joinTableNum;

    //sameTableconNum数组表明conditionset第二列的数量
    int sameTableconNum[MAX_NUM];

    //map保存了一对表名和它在conditionset的位置以及这对表涉及条件的数量的映射
    std::map<std::string,Tablemapinfo> sameTablecountmap;
};



/* @author: huahui 
 * @what for: 必做题，聚合查询 
 * 这是一个聚合查询结点，根据多表join的结果来进行聚合查询
 * -----------------------------------------------------------------------------------------------------------------
 */
class AggExeNode : public ExecutionNode {
public:
  AggExeNode();
  virtual ~AggExeNode();

  RC init(Trx *trx, TupleSchema && tuple_schema, TupleSet && tuple_set);

  RC execute(TupleSet &res_tupleset);

private:
  Trx *trx_ = nullptr;
  TupleSchema tuple_schema_;
  TupleSet tuple_set_;
};
/*end ----------------------------------------------------------------------------------------------*/


#endif //__OBSERVER_SQL_EXECUTOR_EXECUTION_NODE_H_
