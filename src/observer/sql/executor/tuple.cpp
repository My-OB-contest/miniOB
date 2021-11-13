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

#include <algorithm>

#include "sql/parser/parse_defs.h"
#include "sql/executor/tuple.h"
#include "storage/common/table.h"
#include "common/log/log.h"

Tuple::Tuple(const Tuple &other) {
  LOG_PANIC("Copy constructor of tuple is not supported");
  exit(1);
}

Tuple::Tuple(Tuple &&other) noexcept : values_(std::move(other.values_)) {
}

Tuple & Tuple::operator=(Tuple &&other) noexcept {
  if (&other == this) {
    return *this;
  }

  values_.clear();
  values_.swap(other.values_);
  return *this;
}

Tuple::~Tuple() {
}

// add (Value && value)
void Tuple::add(TupleValue *value) {
  values_.emplace_back(value);
}
void Tuple::add(const std::shared_ptr<TupleValue> &other) {
  values_.emplace_back(other);
}
void Tuple::add(int value) {
  add(new IntValue(value));
}

void Tuple::add(float value) {
  add(new FloatValue(value));
}

void Tuple::add(const char *s, int len) {
  add(new StringValue(s, len));
}

////////////////////////////////////////////////////////////////////////////////

std::string TupleField::to_string() const {
  return std::string(table_name_) + "." + field_name_ + std::to_string(type_);
}

/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< 
 */
void print_exp(Exp *exp, std::ostream &os) {
  if(exp->left_exp) {
    print_exp(exp->left_exp, os);
    os << calopToStr(exp->calop);
  }
  if(exp->have_brace) {
    os << "(";
    print_explist(exp->explist, os);
    os << ")";
  } else if(exp->is_attr) {
    if(exp->relation_name) {
      os << exp->relation_name << ".";
    }
    os << exp->attribute_name;
  } else {
    if(exp->value.type == AttrType::INTS) {
      os << *(int *)(exp->value.data);
    } else {
      char *s2 = (char *)(exp->value.data);
      os << (s2 + sizeof(float));
    }
  }
}

void print_explist(ExpList *explist, std::ostream &os) {
  if(explist->left_explist) {
    print_explist(explist->left_explist, os);
    os << calopToStr(explist->calop);
  }
  if(explist->exp->have_negative) {
    os << "-";
  }
  print_exp(explist->exp, os);
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

/* @author: huahui @what for: 聚合查询, 多表查询  -----------------------------------------------
 */
void TupleField::print(std::ostream &os) const {
  /* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
  if(is_explist_) {
    print_explist(explist_, os);
    return;
  }
  /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

  if(!is_attr_) {
    char *s = aggtypeToStr(aggtype_);
    if(agg_val_type_ == AggValType::AGGNUMBER) {
      os << std::string(s) << "(" << agg_val_.intv << ")";
    }else {
      os << std::string(s) << "(" << std::string(agg_val_.str) << ")";
    }
    return;
  }

  if(have_table_name_) {
    if(aggtype_ != AggType::NOTAGG) {
      char *s = aggtypeToStr(aggtype_);
      os << std::string(s) + "(" + table_name_ + "." + field_name_ + ")";
    }else {
      os << table_name_ + "." + field_name_;
    }
  } else {
    if(aggtype_ != AggType::NOTAGG) {
      char *s = aggtypeToStr(aggtype_);
      os << std::string(s) + "(" + field_name_ + ")";
    }else {
      os << field_name_;
    }
  }
}

AggType TupleField::getAggtype() const {
  return aggtype_;
}

bool TupleField::get_is_attr() const {
  return is_attr_;
}
AggValType TupleField::get_agg_val_type() const {
  return agg_val_type_;
}
AggVal TupleField::get_agg_val() const {
  return agg_val_;
}
/* ---------------------------------------------------------------------------------------------*/

/* @author: huahui  @what for: expression */
int TupleField::get_is_explist() const {
  return is_explist_;
}
ExpList *TupleField::get_explist() const {
  return explist_;
}

////////////////////////////////////////////////////////////////////////////////
/* @author: huahui @what for: 聚合查询, 多表查询  -----------------------------------------------
 * have_table_name表示加入到schema中的TupleField在输出时应该不应该带表名
 */
void TupleSchema::from_table(const Table *table, TupleSchema &schema, bool have_table_name) {
  const char *table_name = table->name();
  const TableMeta &table_meta = table->table_meta();
  const int field_num = table_meta.field_num();
  for (int i = 0; i < field_num; i++) {
    const FieldMeta *field_meta = table_meta.field(i);
    if (field_meta->visible()) {
      schema.add(field_meta->type(), table_name, field_meta->name(), have_table_name);
    }
  }
}
/* ---------------------------------------------------------------------------------------------*/

void TupleSchema::add(AttrType type, const char *table_name, const char *field_name) {
  fields_.emplace_back(type, table_name, field_name);
}
/* @author: huahui @what for: 聚合查询, 多表查询  ---------------------------------------------------------------------------------------
 * have_table_name表示加入到schema中的TupleField在输出时应该不应该带表名
 */
void TupleSchema::add(AttrType type, const char *table_name, const char *field_name, bool have_table_name) {
  fields_.emplace_back(type, table_name, field_name);
  fields_.back().set_have_table_name(have_table_name);
  fields_.back().set_is_attr(true);
  fields_.back().set_aggtype = AggType::NOTAGG;
}
void TupleSchema::add(AttrType type, const char *table_name, const char *field_name, bool have_table_name, AggType aggtype) {
  fields_.emplace_back(type, table_name, field_name);
  fields_.back().set_have_table_name(have_table_name);
  fields_.back().set_aggtype(aggtype);
  fields_.back().set_is_attr(true);
}
void TupleSchema::add(bool have_table_name, AggType aggtype, bool is_attr, AggValType agg_val_type, AggVal agg_val) {
  fields_.emplace_back(AttrType::UNDEFINED, "", "");
  fields_.back().set_have_table_name(have_table_name);
  fields_.back().set_aggtype(aggtype);
  fields_.back().set_is_attr(is_attr);
  fields_.back().set_agg_val_type(agg_val_type);
  fields_.back().set_agg_val(agg_val);
}
/* ------------------------------------------------------------------------------------------------------------------------------------*/

void TupleSchema::add_if_not_exists(AttrType type, const char *table_name, const char *field_name) {
  for (const auto &field: fields_) {
    if(field.get_is_explist()) continue;  // @what for: expression
    if (0 == strcmp(field.table_name(), table_name) &&
        0 == strcmp(field.field_name(), field_name)) {
      return;
    }
  }

  add(type, table_name, field_name);
}
/* @author: huahui @what for: 聚合查询, 多表查询  --------------------------------------------------------------------------
 * have_table_name表示加入到schema中的TupleField在输出时应该不应该带表名
 */
void TupleSchema::add_if_not_exists(AttrType type, const char *table_name, const char *field_name, bool have_table_name) {
  for (const auto &field: fields_) {
    if(field.get_is_explist()) continue;  // @what for: expression
    if (0 == strcmp(field.table_name(), table_name) &&
        0 == strcmp(field.field_name(), field_name)) {
      return;
    }
  }

  add(type, table_name, field_name, have_table_name);
}
/* --------------------------------------------------------------------------------------------------------------------------*/

/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void TupleSchema::add_explist(ExpList *explist) {
  fields_.emplace_back(explist);
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


void TupleSchema::append(const TupleSchema &other) {
  fields_.reserve(fields_.size() + other.fields_.size());
  for (const auto &field: other.fields_) {
    fields_.emplace_back(field);
  }
}

int TupleSchema::index_of_field(const char *table_name, const char *field_name) const {
  const int size = fields_.size();
  for (int i = 0; i < size; i++) {
    const TupleField &field = fields_[i];
    if(field.get_is_explist()) continue;  // @what for: expression
    if (0 == strcmp(field.table_name(), table_name) && 0 == strcmp(field.field_name(), field_name)) {
      return i;
    }
  }
  return -1;
}

/* @author: huahui @what for: 聚合查询, 多表查询  
 * ------------------------------------------------------------------------------------------------*/
void TupleSchema::print(std::ostream &os) const {
  if(fields_.size() <= 0) {
    return;
  }
  for (std::vector<TupleField>::const_iterator iter = fields_.begin(), end = --fields_.end();
       iter != end; ++iter) {
    iter->print(os);
    os << " | ";
  }
  fields_.back().print(os);
  os << std::endl;
}
/* --------------------------------------------------------------------------------------------------------------*/

/////////////////////////////////////////////////////////////////////////////
TupleSet::TupleSet(TupleSet &&other) : tuples_(std::move(other.tuples_)), schema_(other.schema_){
  other.schema_.clear();
}

TupleSet &TupleSet::operator=(TupleSet &&other) {
  if (this == &other) {
    return *this;
  }

  schema_.clear();
  schema_.append(other.schema_);
  other.schema_.clear();

  tuples_.clear();
  tuples_.swap(other.tuples_);
  return *this;
}

void TupleSet::add(Tuple &&tuple) {
  tuples_.emplace_back(std::move(tuple));
}

void TupleSet::clear() {
  tuples_.clear();
  schema_.clear();
}

void TupleSet::print(std::ostream &os) const {
  if (schema_.fields().empty()) {
    LOG_WARN("Got empty schema");
    return;
  }

  schema_.print(os);

  for (const Tuple &item : tuples_) {
    const std::vector<std::shared_ptr<TupleValue>> &values = item.values();
    for (std::vector<std::shared_ptr<TupleValue>>::const_iterator iter = values.begin(), end = --values.end();
          iter != end; ++iter) {
      (*iter)->to_string(os);
      os << " | ";
    }
    values.back()->to_string(os);
    os << std::endl;
  }
}

/* @author: huahui  @what for: order-by <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void TupleSet::sortTuples(int order_num, const OrderAttr *order_attrs) {
  if(order_num <= 0) return;
  auto cmp = [&](const Tuple &t1, const Tuple &t2) -> bool {
    for(int i = order_num - 1; i >= 0; i--) {
      const char *table_name = order_attrs[i].relation_name ? order_attrs[i].relation_name : schema_.field(0).table_name();
      int index = schema_.index_of_field(table_name, order_attrs[i].attribute_name);
      int flag = t1.get_pointer(index)->compare(t2.get(index));
      if(flag != 0 || i == 0) {
        if(order_attrs[i].order_rule == OrderRule::ASCORDER) {
          return flag < 0;
        } else {
          return flag > 0;
        }
      }
    }
  };
  std::sort(tuples_.begin(), tuples_.end(), cmp);
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

void TupleSet::set_schema(const TupleSchema &schema) {
  schema_ = schema;
}

const TupleSchema &TupleSet::get_schema() const {
  return schema_;
}

bool TupleSet::is_empty() const {
  return tuples_.empty();
}

int TupleSet::size() const {
  return tuples_.size();
}

const Tuple &TupleSet::get(int index) const {
  return tuples_[index];
}

const std::vector<Tuple> &TupleSet::tuples() const {
  return tuples_;
}

/////////////////////////////////////////////////////////////////////////////
TupleRecordConverter::TupleRecordConverter(Table *table, TupleSet &tuple_set) :
      table_(table), tuple_set_(tuple_set){
}

void TupleRecordConverter::add_record(const char *record) {
  const TupleSchema &schema = tuple_set_.schema();
  Tuple tuple;
  const TableMeta &table_meta = table_->table_meta();
  for (const TupleField &field : schema.fields()) {
    const FieldMeta *field_meta = table_meta.field(field.field_name());
    assert(field_meta != nullptr);
    /* @author: huahui  @what for: null -----------------------------------------------------------------------*/
    // 如果这个属性是null，则直接在tuple中加入NullValue
    if(record[field_meta->get_null_tag_offset()]) {
      tuple.add(new NullValue());
      continue;
    }
    /* --------------------------------------------------------------------------------------------------------*/
    switch (field_meta->type()) {
      case INTS: {
        int value = *(int*)(record + field_meta->offset());
        tuple.add(value);
      }
      break;
      case FLOATS: {
        float value = *(float *)(record + field_meta->offset());
        tuple.add(value);
      }
        break;
      case CHARS: {
        const char *s = record + field_meta->offset();  // 现在当做Cstring来处理
        tuple.add(s, strlen(s));
      }
      break;
      /*
       * @author: huahui
       * @what for: 必做题，增加date字段
       * begin -------------------------------------------------------------------------------------------
       */
      case DATES: {
        const unsigned char *s = (const unsigned char *)(record + field_meta->offset());
        std::shared_ptr<DateValue> p(new DateValue(s));
        tuple.add(p);
      }
      break;
      /*end ----------------------------------------------------------------------------------------------*/
      default: {
        LOG_PANIC("Unsupported field type. type=%d", field_meta->type());
      }
    }
  }

  tuple_set_.add(std::move(tuple));
}

/* @author: huahui  @what for: 浮点数默认格式化---------------------------------------------
  */
std::string formatFloat(float f) {
  char s[100];
  sprintf(s, "%.2f", f);
  for(int i=strlen(s)-1; s[i]!='.'; i--) {
    if(s[i]=='0') {
      s[i] = '\0';
    }
  }
  if(s[strlen(s)-1] == '.') {
    s[strlen(s)-1] = '\0';
  }
  return std::string(s);
}
/* -------------------------------------------------------------------------------------*/


