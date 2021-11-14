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

#ifndef __OBSERVER_SQL_EXECUTOR_TUPLE_H_
#define __OBSERVER_SQL_EXECUTOR_TUPLE_H_

#include <memory>
#include <vector>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <string>

#include "sql/parser/parse.h"
#include "sql/executor/value.h"
#include "storage/common/record_manager.h"

class Table;
struct TextRecord;

class Tuple {
public:
  Tuple() = default;

  Tuple(const Tuple &other);

  ~Tuple();

  Tuple(Tuple &&other) noexcept ;
  Tuple & operator=(Tuple &&other) noexcept ;

  void add(TupleValue *value);
  void add(const std::shared_ptr<TupleValue> &other);
  void add(int value);
  void add(float value);
  void add(const char *s, int len);

  const std::vector<std::shared_ptr<TupleValue>> &values() const {
    return values_;
  }

  int size() const {
    return values_.size();
  }

  const TupleValue &get(int index) const {
    return *values_[index];
  }

  const std::shared_ptr<TupleValue> &get_pointer(int index) const {
    return values_[index];
  }

private:
  std::vector<std::shared_ptr<TupleValue>>  values_;
};


/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
 */
void print_exp(Exp *exp, std::ostream &os);
void print_explist(ExpList *explist, std::ostream &os);
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
class TupleField {
public:
  TupleField(AttrType type, const char *table_name, const char *field_name) :
          type_(type), table_name_(table_name), field_name_(field_name){
    /* @author: huahui @what for: 必做题，聚合查询 TupleField支持聚合属性了
	   * -----------------------------------------------------------------------------------------------------------------
	   */
    aggtype_ = AggType::NOTAGG;
    /* -----------------------------------------------------------------------------------------------------------------*/
  
    is_explist_ = 0;  /* @author: huahui  @what for: expression */
  }

  /* @author: huahui  @what for: expression */
  TupleField(ExpList *explist) : explist_(explist) {
    is_explist_ = 1;
  }

  AttrType  type() const{
    return type_;
  }

  const char *table_name() const {
    return table_name_.c_str();
  }
  const char *field_name() const {
    return field_name_.c_str();
  }
  /* @author: huahui @what for: 必做题，聚合查询 TupleField支持聚合属性了
	 * -----------------------------------------------------------------------------------------------------------------
	 */
  void set_have_table_name(bool have_table_name) {
    have_table_name_ = have_table_name;
  }
  void set_aggtype(AggType aggtype) {
    aggtype_ = aggtype;
  }
  void set_is_attr(bool is_attr) {
    is_attr_ = is_attr;
  }
  void set_agg_val_type(AggValType agg_val_type) {
    agg_val_type_ = agg_val_type;
  }
  void set_agg_val(AggVal agg_val) {
    agg_val_ = agg_val;
  }
  void print(std::ostream &os) const;
  AggType getAggtype() const;
  bool get_is_attr() const;
  AggValType get_agg_val_type() const;
  AggVal get_agg_val() const;
  /* -----------------------------------------------------------------------------------------------------------------*/
  
  int get_is_explist() const;  /* @author: huahui  @what for: expression */
  ExpList *get_explist() const; /* @author: huahui  @what for: expression */

  std::string to_string() const;
private:
  /* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  int is_explist_;   // 如果is_explist_，则explist_有效，其他都无效
  ExpList *explist_;
  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

  /* @author: huahui @what for: 必做题，聚合查询 TupleField支持聚合属性了
   * have_table_name_表示这个TupleField输出时应不应该带表名 
   * aggtype_表示聚合类型，为NOTAGG表示不是聚合类型，是正常类型
	 * -----------------------------------------------------------------------------------------------------------------
	 */
  bool have_table_name_;
  AggType aggtype_;
  bool is_attr_;       // 如果是聚合属性，则这个is_attr判断是属性还是数值
  AggValType agg_val_type_;
  AggVal agg_val_;
  /* -----------------------------------------------------------------------------------------------------------------*/
  AttrType  type_;
  std::string table_name_;
  std::string field_name_;
};

class TupleSchema {
public:
  TupleSchema() = default;
  ~TupleSchema() = default;

  /* @author: huahui @what for: 聚合查询, 多表查询  -----------------------------------------------------------
   * have_table_name表示加入到schema中的TupleField在输出时应该不应该带表名
   */
  void add(AttrType type, const char *table_name, const char *field_name);
  void add(AttrType type, const char *table_name, const char *field_name, bool have_table_name);
  void add(AttrType type, const char *table_name, const char *field_name, bool have_table_name, AggType aggtype);
  void add(bool have_table_name, AggType aggtype, bool is_attr, AggValType agg_val_type_, AggVal agg_val_);
  void add_if_not_exists(AttrType type, const char *table_name, const char *field_name);
  void add_if_not_exists(AttrType type, const char *table_name, const char *field_name, bool have_table_name);
  /* ---------------------------------------------------------------------------------------------------------------*/
  
  /* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  void add_explist(ExpList *explist);
  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

  // void merge(const TupleSchema &other);
  void append(const TupleSchema &other);

  const std::vector<TupleField> &fields() const {
    return fields_;
  }

  const TupleField &field(int index) const {
    return fields_[index];
  }

  int index_of_field(const char *table_name, const char *field_name) const;
  void clear() {
    fields_.clear();
  }

  void print(std::ostream &os) const;
public:
  /* @author: huahui @what for: 聚合查询, 多表查询  -----------------------------------------------
   * have_table_name表示加入到schema中的TupleField在输出时应该不应该带表名
   */
  static void from_table(const Table *table, TupleSchema &schema, bool have_table_name);
  /* ---------------------------------------------------------------------------------------------*/
private:
  std::vector<TupleField> fields_;
};

class TupleSet {
public:
  TupleSet() = default;
  TupleSet(TupleSet &&other);
  explicit TupleSet(const TupleSchema &schema) : schema_(schema) {
  }
  TupleSet &operator =(TupleSet &&other);

  ~TupleSet() = default;

  void set_schema(const TupleSchema &schema);

  const TupleSchema &get_schema() const;

  void add(Tuple && tuple);

  void clear();

  bool is_empty() const;
  int size() const;
  const Tuple &get(int index) const;
  const std::vector<Tuple> &tuples() const;

  void print(std::ostream &os) const;

  /* @author: huahui  @what for: order-by */
  void sortTuples(int order_num, const OrderAttr *order_attrs);

public:
  const TupleSchema &schema() const {
    return schema_;
  }
private:
  std::vector<Tuple> tuples_;
  TupleSchema schema_;
};

class TupleRecordConverter {
public:
  TupleRecordConverter(Table *table, TupleSet &tuple_set);

  void add_record(const char *record);
private:
  Table *table_;
  TupleSet &tuple_set_;
};


/* @author: huahui  @what for: 浮点数默认格式化---------------------------------------------
  */
std::string formatFloat(float f);
/* -------------------------------------------------------------------------------------*/

#endif //__OBSERVER_SQL_EXECUTOR_TUPLE_H_
