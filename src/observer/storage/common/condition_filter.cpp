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

#include <stddef.h>
#include <sql/executor/value.h>
#include "condition_filter.h"
#include "record_manager.h"
#include "common/log/log.h"
#include "storage/common/table.h"

using namespace common;

ConditionFilter::~ConditionFilter()
{}

DefaultConditionFilter::DefaultConditionFilter()
{
  left_.is_attr = false;
  left_.attr_length = 0;
  left_.attr_offset = 0;
  left_.value = nullptr;

  right_.is_attr = false;
  right_.attr_length = 0;
  right_.attr_offset = 0;
  right_.value = nullptr;
}
DefaultConditionFilter::~DefaultConditionFilter()
{}

RC DefaultConditionFilter::init(const ConDesc &left, const ConDesc &right, AttrType attr_type, CompOp comp_op)
{
  //fzh改，添加新可比较属性需要更改
  /* @author: huahui  @what for: null  ------------------------------------------------------------------*/
  if(!left.is_null && !right.is_null){
    if (attr_type < CHARS || attr_type > DATES ) {
      LOG_ERROR("Invalid condition with unsupported attribute type: %d", attr_type);
      return RC::INVALID_ARGUMENT;
    }
  }
  if (comp_op < EQUAL_TO || comp_op >= NO_OP) {
    LOG_ERROR("Invalid condition with unsupported compare operation: %d", comp_op);
    return RC::INVALID_ARGUMENT;
  }

  left_ = left;
  right_ = right;
  attr_type_ = attr_type;
  comp_op_ = comp_op;
  return RC::SUCCESS;
}

RC DefaultConditionFilter::init(Table &table, const Condition &condition)
{
  const TableMeta &table_meta = table.table_meta();
  ConDesc left;
  ConDesc right;

  AttrType type_left = UNDEFINED;
  AttrType type_right = UNDEFINED;

  if (1 == condition.left_is_attr) {
    left.is_attr = true;
    const FieldMeta *field_left = table_meta.field(condition.left_attr.attribute_name);
    if (nullptr == field_left) {
      LOG_WARN("No such field in condition. %s.%s", table.name(), condition.left_attr.attribute_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
    left.attr_length = field_left->len();
    left.attr_offset = field_left->offset();

    left.value = nullptr;

    type_left = field_left->type();
    left.attrtype = type_left;

    left.null_tag_offset = field_left->get_null_tag_offset(); /* @author: huahui  @what for: null -----------------------------------------------*/
  } else {
    left.is_attr = false;
    left.is_null = condition.left_value.is_null;   /* @author: huahui  @what for: null -----------------------------------------------*/
    left.value = condition.left_value.data;  // 校验type 或者转换类型
    type_left = condition.left_value.type;

    left.attr_length = 0;
    left.attr_offset = 0;
    left.attrtype = type_left;
  }

  if (1 == condition.right_is_attr) {
    right.is_attr = true;
    const FieldMeta *field_right = table_meta.field(condition.right_attr.attribute_name);
    if (nullptr == field_right) {
      LOG_WARN("No such field in condition. %s.%s", table.name(), condition.right_attr.attribute_name);
      return RC::SCHEMA_FIELD_MISSING;
    }
    right.attr_length = field_right->len();
    right.attr_offset = field_right->offset();
    type_right = field_right->type();

    right.value = nullptr;
    right.attrtype = type_right;

    right.null_tag_offset = field_right->get_null_tag_offset(); /* @author: huahui  @what for: null -----------------------------------------------*/
  } else {
    right.is_attr = false;
    right.is_null = condition.right_value.is_null;         /* @author: huahui  @what for: null -----------------------------------------------*/
    right.value = condition.right_value.data;
    type_right = condition.right_value.type;

    right.attr_length = 0;
    right.attr_offset = 0;
    right.attrtype = type_right;
  }

  // 校验和转换
  //  if (!field_type_compare_compatible_table[type_left][type_right]) {
  //    // 不能比较的两个字段， 要把信息传给客户端
  //    return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  //  }
  // NOTE：这里没有实现不同类型的数据比较，比如整数跟浮点数之间的对比
  // 但是选手们还是要实现。这个功能在预选赛中会出现
  // if (type_left != type_right 
  //    && !(type_left==AttrType::INTS && type_right==AttrType::FLOATS || type_left==AttrType::FLOATS && type_right==AttrType::INTS)) {    /* @author: huahui  @what for: 元数据校验, where中int和float兼容*/
  //   return RC::SCHEMA_FIELD_TYPE_MISMATCH;
  // }

  return init(left, right, (type_left==AttrType::FLOATS || type_right==AttrType::FLOATS)?AttrType::FLOATS:type_left, condition.comp);   /* @author: huahui  @what for: 元数据校验, where中int和float兼容*/
}

bool DefaultConditionFilter::filter(const Record &rec) const
{
  char *left_value = nullptr;
  char *right_value = nullptr;

  /* @author: huahui  @what for: null -----------------------------------------------------------------------------*/
  // 解决类似于 where id is null 或者 where id is not null, where value is null, where value is not null情况
  if(comp_op_ == CompOp::IS) { // 此时比较符号右边肯定是null
    if(left_.is_attr){
      if(rec.data[left_.null_tag_offset]) {
        return true;
      }else{
        return false;
      }
    }else {
      if(left_.is_null) {
        return true;
      }else {
        return false;
      }
    }
  }
  if(comp_op_ == CompOp::ISNOT) { // 此时比较符号右边肯定是null
    if(left_.is_attr) {
      if(!(rec.data[left_.null_tag_offset])) {
        return true;
      }else{
        return false;
      }
    }else {
      if(left_.is_null) {
        return false;
      } else {
        return true;
      }
    }
  }
  /* -------------------------------------------------------------------------------------------------------------*/

  if (left_.is_attr) {  // value
    if(rec.data[left_.null_tag_offset]) { /* @author: huahui  @what for: null -----------------------------------------------------------------------------*/
      return false;
    }
    left_value = (char *)(rec.data + left_.attr_offset);
  } else {
    if(left_.is_null) { /* @author: huahui  @what for: null -----------------------------------------------------------------------------*/
      return false;
    }
    left_value = (char *)left_.value;
  }

  if (right_.is_attr) { 
    if(rec.data[right_.null_tag_offset]) { /* @author: huahui  @what for: null -----------------------------------------------------------------------------*/
      return false;
    }
    right_value = (char *)(rec.data + right_.attr_offset);
  } else {
    if(right_.is_null) { /* @author: huahui  @what for: null -----------------------------------------------------------------------------*/
      return false;
    }
    right_value = (char *)right_.value;
  }

  int cmp_result = 0;

  //增加属性字段需要修改
  switch (attr_type_) {
    case CHARS: {  // 字符串都是定长的，直接比较
      // 按照C字符串风格来定
      cmp_result = strcmp(left_value, right_value);
    } break;
    case INTS: {
      // 没有考虑大小端问题
      // 对int和float，要考虑字节对齐问题,有些平台下直接转换可能会跪
      int left = *(int *)left_value;
      int right = *(int *)right_value;
      cmp_result = left - right;
    } break;
    /* @author: huahui @what for: 元数据校验
     * -----------------------------------------------------------------------
     */
    case FLOATS: {
      float left, right;
      if(left_.attrtype == AttrType::INTS){
        left = float(*((int *)left_value));
      }else{
        left = *(float *)left_value;
      }
      if(right_.attrtype == AttrType::INTS){
        right = float(*((int *)right_value));
      }else{
        right = *(float *)right_value;
      }
      cmp_result = (int)(left - right);
    } break;
    /* ------------------------------------------------------------------------------*/
    case DATES: {
      unsigned char *left_value2 = (unsigned char *)left_value;
      unsigned char *right_value2 = (unsigned char *)right_value;
      DateValue left_dv = DateValue(left_value2);
      DateValue right_dv = DateValue(right_value2);
      cmp_result = left_dv.compare(right_dv);
    }
    default: {
    }
  }

  switch (comp_op_) {
    case EQUAL_TO:
      return 0 == cmp_result;
    case LESS_EQUAL:
      return cmp_result <= 0;
    case NOT_EQUAL:
      return cmp_result != 0;
    case LESS_THAN:
      return cmp_result < 0;
    case GREAT_EQUAL:
      return cmp_result >= 0;
    case GREAT_THAN:
      return cmp_result > 0;

    default:
      break;
  }

  LOG_PANIC("Never should print this.");
  return cmp_result;  // should not go here
}

CompositeConditionFilter::~CompositeConditionFilter()
{
  if (memory_owner_) {
    delete[] filters_;
    filters_ = nullptr;
  }
}
TupleConditionFilter::TupleConditionFilter(){
    posl_=-1;
    posr_=-1;
    tupleleft_= nullptr;
    tupleright_= nullptr;
}
TupleConditionFilter::~TupleConditionFilter(){}

RC TupleConditionFilter::init(const Tuple *tuple, Condition *condition, int pos) {
    comp_op_ = condition->comp;
    tupleleft_ = tuple;
    posl_ = pos;
    return RC::SUCCESS;
}
RC TupleConditionFilter::init(const Tuple *tuple, Condition *condition,TupleSchema tupleSchema){
    comp_op_ = condition->comp;
    tupleleft_ = tuple;
    int count=0;
    bool flag= false;
    for(auto it_field = tupleSchema.fields().begin();it_field != tupleSchema.fields().end(); ++it_field) {
        if (strcmp(it_field->table_name(), condition->left_attr.relation_name) == 0 &&
            strcmp(it_field->field_name(), condition->left_attr.attribute_name) == 0) {
            posl_ = count;
            flag = true;
            break;
        }
        count++;
    }
    if (flag == false){
        LOG_ERROR("TupleConditionFilter init fail,because tuple not match condition");
        return RC::SCHEMA_FIELD_MISSING;
    }
    return RC::SUCCESS;
}
RC TupleConditionFilter::init(const Tuple *tuplel,const Tuple *tupler, Condition *condition,TupleSchema tupleSchemal,TupleSchema tupleSchemar){
    comp_op_ = condition->comp;
    int count = 0;
    bool flag = false;
    bool verse = false;
    for(auto it_field = tupleSchemal.fields().begin();it_field != tupleSchemal.fields().end(); ++it_field) {
        if (strcmp(it_field->table_name(), condition->left_attr.relation_name) == 0 &&
            strcmp(it_field->field_name(), condition->left_attr.attribute_name) == 0) {
            posl_ = count;
            flag = true;
            break;
        }
        //可能and连接的两个条件表顺序相反，如t1.id=t2.id and t2.old>t1.old;
        if (strcmp(it_field->table_name(), condition->right_attr.relation_name) == 0 &&
            strcmp(it_field->field_name(), condition->right_attr.attribute_name) == 0) {
            posr_ = count;
            flag = true;
            verse = true;
            break;
        }
        count++;
    }
    if (flag == false){
        LOG_ERROR("TupleConditionFilter init fail,because tuple not match condition");
        return RC::SCHEMA_FIELD_MISSING;
    }
    count = 0;
    flag = false;
    for(auto it_field = tupleSchemar.fields().begin();it_field != tupleSchemar.fields().end(); ++it_field) {
        if (strcmp(it_field->table_name(), condition->right_attr.relation_name) == 0 &&
            strcmp(it_field->field_name(), condition->right_attr.attribute_name) == 0) {
            posr_ = count;
            flag = true;
            break;
        }
        if (strcmp(it_field->table_name(), condition->left_attr.relation_name) == 0 &&
            strcmp(it_field->field_name(), condition->left_attr.attribute_name) == 0) {
            posl_ = count;
            flag = true;
            verse = true;
            break;
        }
        count++;
    }
    if (flag == false){
        LOG_ERROR("TupleConditionFilter init fail,because tuple not match condition");
        return RC::SCHEMA_FIELD_MISSING;
    }
    if (verse == false){
        tupleleft_ = tuplel;
        tupleright_ = tupler;
    } else{
        tupleleft_ = tupler;
        tupleright_ = tuplel;
    }

    return RC::SUCCESS;
}

bool TupleConditionFilter::filter(const Tuple *tupleright,int posr) const {
    if (tupleleft_ != nullptr&&tupleright_ != nullptr){
        LOG_ERROR("execute wrong filter method,you should init with one tuple");
    }
    int cmp_result=tupleleft_->get(posl_).compare(tupleright->get(posr));

    switch (comp_op_) {
        case EQUAL_TO:
            return 0 == cmp_result;
        case LESS_EQUAL:
            return cmp_result <= 0;
        case NOT_EQUAL:
            return cmp_result != 0;
        case LESS_THAN:
            return cmp_result < 0;
        case GREAT_EQUAL:
            return cmp_result >= 0;
        case GREAT_THAN:
            return cmp_result > 0;
        default:
            break;
    }
    LOG_PANIC("Never should print this.");
    return cmp_result;  // should not go here
}

bool TupleConditionFilter::filter() const {
    if (tupleleft_== nullptr||tupleright_== nullptr){
        LOG_ERROR("execute wrong filter method,you should init with two tuple");
    }
    int cmp_result=tupleleft_->get(posl_).compare(tupleright_->get(posr_));

    switch (comp_op_) {
        case EQUAL_TO:
            return 0 == cmp_result;
        case LESS_EQUAL:
            return cmp_result <= 0;
        case NOT_EQUAL:
            return cmp_result != 0;
        case LESS_THAN:
            return cmp_result < 0;
        case GREAT_EQUAL:
            return cmp_result >= 0;
        case GREAT_THAN:
            return cmp_result > 0;
        default:
            break;
    }
    LOG_PANIC("Never should print this.");
    return cmp_result;  // should not go here
}

RC CompositeConditionFilter::init(const ConditionFilter *filters[], int filter_num, bool own_memory)
{
  filters_ = filters;
  filter_num_ = filter_num;
  memory_owner_ = own_memory;
  return RC::SUCCESS;
}
RC CompositeConditionFilter::init(const ConditionFilter *filters[], int filter_num)
{
  return init(filters, filter_num, false);
}

RC CompositeConditionFilter::init(Table &table, const Condition *conditions, int condition_num)
{
  if (condition_num == 0) {
    return RC::SUCCESS;
  }
  if (conditions == nullptr) {
    return RC::INVALID_ARGUMENT;
  }

  RC rc = RC::SUCCESS;
  ConditionFilter **condition_filters = new ConditionFilter *[condition_num];
  for (int i = 0; i < condition_num; i++) {
    DefaultConditionFilter *default_condition_filter = new DefaultConditionFilter();
    rc = default_condition_filter->init(table, conditions[i]);
    if (rc != RC::SUCCESS) {
      delete default_condition_filter;
      for (int j = i - 1; j >= 0; j--) {
        delete condition_filters[j];
        condition_filters[j] = nullptr;
      }
      delete[] condition_filters;
      condition_filters = nullptr;
      return rc;
    }
    condition_filters[i] = default_condition_filter;
  }
  return init((const ConditionFilter **)condition_filters, condition_num, true);
}

bool CompositeConditionFilter::filter(const Record &rec) const
{
  for (int i = 0; i < filter_num_; i++) {
    if (!filters_[i]->filter(rec)) {
      return false;
    }
  }
  return true;
}
