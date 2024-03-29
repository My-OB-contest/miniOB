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
// Created by Wangyunlai on 2021/5/7.
//

#ifndef __OBSERVER_STORAGE_COMMON_CONDITION_FILTER_H_
#define __OBSERVER_STORAGE_COMMON_CONDITION_FILTER_H_

#include <sql/executor/tuple.h>
#include "rc.h"
#include "sql/parser/parse.h"
#include "sql/parser/parse_defs.h"

struct Record;
class Table;

struct ConDesc {
  bool   is_attr;     // 是否属性，false 表示是值
  int    attr_length; // 如果是属性，表示属性值长度
  int    attr_offset; // 如果是属性，表示在记录中的偏移量
  void * value;       // 如果是值类型，这里记录值的数据
  AttrType attrtype; // 属性类型
  /* @author: huahui  @what for: null -----------------------------------------------------------------------------*/
  int    null_tag_offset;  
  int    is_null;
  /* ----------------------------------------------------------------------------------------------------------------*/
};
struct SubSelConDesc {
    bool   is_select= false;   //是否是子查询
    std::shared_ptr<TupleValue> tupleValue_; //
    AttrType attrtype; // 属性类型
};

class ConditionFilter {
public:
  virtual ~ConditionFilter();

  /**
   * Filter one record
   * @param rec
   * @return true means match condition, false means failed to match.
   */
  virtual bool filter(const Record &rec) const = 0;
};

class DefaultConditionFilter : public ConditionFilter {
public:
  DefaultConditionFilter();
  virtual ~DefaultConditionFilter();

  RC init(const ConDesc &left, const ConDesc &right, AttrType attr_type, CompOp comp_op);
  RC init(Table &table, const Condition &condition);

  virtual bool filter(const Record &rec) const;

public:
  const ConDesc &left() const {
    return left_;
  }

  const ConDesc &right() const {
    return right_;
  }

  CompOp comp_op() const {
    return comp_op_;
  }

private:
  ConDesc  left_;
  ConDesc  right_;
  AttrType attr_type_ = UNDEFINED;
  CompOp   comp_op_ = NO_OP;
};


class CompositeConditionFilter : public ConditionFilter {
public:
  CompositeConditionFilter() = default;
  virtual ~CompositeConditionFilter();

  RC init(const ConditionFilter *filters[], int filter_num);
  RC init(Table &table, const Condition *conditions, int condition_num);
  virtual bool filter(const Record &rec) const;

public:
  int filter_num() const {
    return filter_num_;
  }
  const ConditionFilter &filter(int index) const {
    return *filters_[index];
  }

private:
  RC init(const ConditionFilter *filters[], int filter_num, bool own_memory);
private:
  const ConditionFilter **      filters_ = nullptr;
  int                           filter_num_ = 0;
  bool                          memory_owner_ = false; // filters_的内存是否由自己来控制
};


class TupleConditionFilter {

public:
    TupleConditionFilter();
    ~TupleConditionFilter();
    RC init(const Tuple *tuple, Condition *condition,TupleSchema tupleSchema);
    RC init(const Tuple *tuplel,const Tuple *tupler, Condition *condition,TupleSchema tupleSchemal,TupleSchema tupleSchemar);
    RC init(const Tuple *tuple, Condition *condition,int pos);
    bool filter(const Tuple *tuple,int posr) const;
    bool filter() const;

private:
    const Tuple *tupleleft_;
    const Tuple *tupleright_;
    int posl_;
    int posr_;
    CompOp comp_op_ = NO_OP;
};

// fzh for sub_select
class SubSelConditionFilter {
public:
    RC init(const Tuple *tuple, Condition *sub_sel_condition,const TupleSchema &tupleSchema);
    RC check_subsel_tupset(std::pair<TupleSet,TupleSet> &tupleset_pair) const;
    bool filter(std::pair<TupleSet,TupleSet> &tupleset_pair) const;

private:
    SubSelConDesc left_;
    SubSelConDesc right_;
    CompOp comp_op_ = NO_OP;
};


/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
RC cal_exp(const Exp *exp, const Tuple &tuple, const TupleSchema &tuple_schema, Value &value);
RC cal_explist(const ExpList *explist, const Tuple &tuple, const TupleSchema &tuple_schema, Value &value);

// 处理表达式的比较以及简单的比较如 d="1998-12-9"
class ConditionExpsFilter {
public:
  ConditionExpsFilter();
  ~ConditionExpsFilter();
  void init(int condition_num, const ConditionExp cond_exps[], const TupleSchema &tuple_schema);
  bool filter(const Tuple &tuple) const;
private:
  // std::vector<ConditionExp> cond_exps_;
  int condition_num_;
  ConditionExp cond_exps_[100];
  TupleSchema tuple_schema_;
  AttrType getType(const char *table_name, const char *attribute_name);
};

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

#endif // __OBSERVER_STORAGE_COMMON_CONDITION_FILTER_H_