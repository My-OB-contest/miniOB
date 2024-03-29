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
#include <math.h>

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
    if (attr_type < CHARS || attr_type > TEXT ) {
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
    case CHARS || TEXT: {  // 字符串都是定长的，直接比较
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
      if((left - right < 1e-6) && (left -right > -1e-6) )
      {
          cmp_result = 0;
      } else{
          if (left-right > 0){
              cmp_result = 1;
          }
          if (left-right < 0){
              cmp_result = -1;
          }
      }

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
    /* @author: huahui  @what for: null字段 -------------------------------------------------------------------------------------------------------------------*/
    // 如果是null属性，则直接返回false
    if(std::dynamic_pointer_cast<NullValue>(tupleleft_->get_pointer(posl_)) || std::dynamic_pointer_cast<NullValue>(tupleright->get_pointer(posr))) {
      return false;
    }
    /* ----------------------------------------------------------------------------------------------------------------------------------------------------------*/
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
    /* @author: huahui  @what for: null字段 -------------------------------------------------------------------------------------------------------------------*/
    // 如果是null属性，则直接返回false
    if(std::dynamic_pointer_cast<NullValue>(tupleleft_->get_pointer(posl_)) || std::dynamic_pointer_cast<NullValue>(tupleright_->get_pointer(posr_))) {
      return false;
    }
    /* ----------------------------------------------------------------------------------------------------------------------------------------------------------*/
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

/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
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
      if(exp->calop==CalOp::DIVIDE_OP && (rightexp_value.type==AttrType::INTS && (*(int *)(rightexp_value.data)) == 0 || rightexp_value.type==AttrType::FLOATS && (*(float *)(rightexp_value.data)) == 0.0)) {
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
    int lv = *(int *)(leftexplist_value.data), rv = *(int *)(rightexplist_value.data);
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
      lv = (float)(*(int *)(leftexplist_value.data));
    } else {
      lv = *(float *)(leftexplist_value.data);
    }
    if(rightexplist_value.type == AttrType::INTS) {
      rv = (float)(*(int *)(rightexplist_value.data));
    } else {
      rv = *(float *)(rightexplist_value.data);
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

ConditionExpsFilter::ConditionExpsFilter(){};
ConditionExpsFilter::~ConditionExpsFilter(){};

void ConditionExpsFilter::init(int condition_num, const ConditionExp cond_exps[], const TupleSchema &tuple_schema) {
  condition_num_ = condition_num;
  for(int i = 0; i < condition_num; i++) {
    // cond_exps_.emplace_back(cond_exps[i]);
    cond_exps_[i] = cond_exps[i];
  }
  tuple_schema_ = tuple_schema;
}

bool ConditionExpsFilter::filter(const Tuple &tuple) const {
  // for(int i = 0; i < cond_exps_.size(); i++) {
  for(int i = 0; i < condition_num_; i++) {
    Value vleft, vright;
    RC rc = RC::SUCCESS;
    rc = cal_explist(cond_exps_[i].left, tuple, tuple_schema_, vleft);
    if(rc != RC::SUCCESS) {
      LOG_ERROR("ConditionExpsFilter::filter error, return false\n");
      return false;
    }
    rc = cal_explist(cond_exps_[i].right, tuple, tuple_schema_, vright);
    if(rc != RC::SUCCESS) {
      LOG_ERROR("ConditionExpsFilter::filter error, return false\n");
      value_destroy(&vleft);
      return false;
    }
    
    if(vleft.is_null || vright.is_null) { 
      if(cond_exps_[i].comp != CompOp::IS && cond_exps_[i].comp != CompOp::ISNOT) {
        value_destroy(&vleft);
        value_destroy(&vright);
        return false;
      }
      if(cond_exps_[i].comp == CompOp::IS && !vleft.is_null) {
        value_destroy(&vleft);
        value_destroy(&vright);
        return false;
      }
      if(cond_exps_[i].comp == CompOp::ISNOT && vleft.is_null) {
        value_destroy(&vleft);
        value_destroy(&vright);
        return false;
      }
      continue;
    }
    
    int cmp_res = 0;
    if(vleft.type==AttrType::INTS && vright.type==AttrType::INTS) {
      int vl, vr;
      vl = *(int *)(vleft.data);
      vr = *(int *)(vright.data);
      cmp_res = vl - vr;
    } else if(vleft.type==AttrType::CHARS && vright.type==AttrType::CHARS) {
      char *left_char = (char *)(vleft.data), *right_char = (char *)(vright.data);
      cmp_res = strcmp(left_char, right_char);
    } else if(vleft.type==AttrType::DATES && vright.type==AttrType::DATES) {
      unsigned char *left_value2 = (unsigned char *)(vleft.data);
      unsigned char *right_value2 = (unsigned char *)(vright.data);
      DateValue left_dv = DateValue(left_value2);
      DateValue right_dv = DateValue(right_value2);
      cmp_res = left_dv.compare(right_dv);
    }else {
      float vl, vr;
      if(vleft.type == AttrType::INTS) {
        vl = (float)(*(int *)(vleft.data));
      } else {
        vl = *(float *)(vleft.data);
      }
      if(vright.type == AttrType::INTS) {
        vr = (float)(*(int *)(vright.data));
      } else {
        vr = *(float *)(vright.data);
      }
      if(abs(vl - vr) <= 1e-5) {
        cmp_res = 0;
      } else if(vl - vr > 1e-5) {
        cmp_res = 1;
      } else {
        cmp_res = -1;
      }
    }

    switch (cond_exps_[i].comp) {
    case EQUAL_TO:
      if(cmp_res != 0) { value_destroy(&vleft); value_destroy(&vright); return false; }
      break;
    case LESS_EQUAL:
      if(cmp_res >0) { value_destroy(&vleft); value_destroy(&vright); return false; }
      break;
    case NOT_EQUAL:
      if(cmp_res == 0) { value_destroy(&vleft); value_destroy(&vright); return false; }
      break;
    case LESS_THAN:
      if(cmp_res >= 0) { value_destroy(&vleft); value_destroy(&vright); return false; }
      break;
    case GREAT_EQUAL:
      if(cmp_res < 0) { value_destroy(&vleft); value_destroy(&vright); return false; }
      break;
    case GREAT_THAN:
      if(cmp_res <= 0) { value_destroy(&vleft); value_destroy(&vright); return false; }
      break;
    default:
      LOG_ERROR("CompOp is invalid\n");
      return false;
    }
  }
  return true;
}

AttrType ConditionExpsFilter::getType(const char *table_name, const char *attribute_name) {
  int index = tuple_schema_.index_of_field(table_name ? table_name : tuple_schema_.field(0).table_name(), attribute_name);
  return tuple_schema_.field(index).type();
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


RC SubSelConditionFilter::init(const Tuple *tuple, Condition *sub_sel_condition,const TupleSchema &tupleSchema){
    comp_op_ = sub_sel_condition->comp;
    if (sub_sel_condition->left_select_index != -1){
        left_.is_select = true;
    }
    if (sub_sel_condition->right_select_index != -1){
        right_.is_select = true;
    }
    if(sub_sel_condition->left_is_attr){
        int index = tupleSchema.index_of_field(sub_sel_condition->left_attr.relation_name,sub_sel_condition->left_attr.attribute_name);
        left_.is_select = false;
        left_.tupleValue_ = tuple->get_pointer(index);
        left_.attrtype = tupleSchema.field(index).type();
    }
    if(sub_sel_condition->right_is_attr){
        int index = tupleSchema.index_of_field(sub_sel_condition->right_attr.relation_name,sub_sel_condition->right_attr.attribute_name);
        right_.is_select = false;
        right_.tupleValue_ = tuple->get_pointer(index);
        right_.attrtype = tupleSchema.field(index).type();
    }
    if (sub_sel_condition->left_is_attr == 0 && sub_sel_condition->left_select_index == -1){
        switch (sub_sel_condition->left_value.type) {
            case INTS: {
                int i = *(int *)sub_sel_condition->left_value.data;
                std::shared_ptr<IntValue> p(new IntValue(i));
                left_.is_select = false;
                left_.tupleValue_ = p;
                left_.attrtype = INTS;
            }
                break;
            case FLOATS: {
                float f = *(float *)sub_sel_condition->left_value.data;
                std::shared_ptr<FloatValue> p(new FloatValue(f));
                left_.is_select = false;
                left_.tupleValue_ = p;
                left_.attrtype = FLOATS;
            }
                break;
            case CHARS: {
                char *s = (char *)sub_sel_condition->left_value.data;
                std::shared_ptr<StringValue> p(new StringValue(s));
                left_.is_select = false;
                left_.tupleValue_ = p;
                left_.attrtype = CHARS;
            }
                break;

            case DATES: {
                const unsigned char *s = (const unsigned char *)(sub_sel_condition->left_value.data);
                std::shared_ptr<DateValue> p(new DateValue(s));
                left_.is_select = false;
                left_.tupleValue_ = p;
                left_.attrtype = DATES;
            }
                break;
                /*end ----------------------------------------------------------------------------------------------*/
            default: {
                LOG_PANIC("Unsupported field type. type=%d", sub_sel_condition->left_value.type);
            }
        }
    }
    if (sub_sel_condition->right_is_attr == 0 && sub_sel_condition->right_select_index == -1){
        switch (sub_sel_condition->right_value.type) {
            case INTS: {
                int i = *(int *)sub_sel_condition->right_value.data;
                std::shared_ptr<IntValue> p(new IntValue(i));
                right_.is_select = false;
                right_.tupleValue_ = p;
                right_.attrtype = INTS;
            }
                break;
            case FLOATS: {
                float f = *(float *)sub_sel_condition->right_value.data;
                std::shared_ptr<FloatValue> p(new FloatValue(f));
                right_.is_select = false;
                right_.tupleValue_ = p;
                right_.attrtype = FLOATS;
            }
                break;
            case CHARS: {
                char *s = (char *)sub_sel_condition->right_value.data;
                std::shared_ptr<StringValue> p(new StringValue(s));
                right_.is_select = false;
                right_.tupleValue_ = p;
                right_.attrtype = CHARS;
            }
                break;

            case DATES: {
                const unsigned char *s = (const unsigned char *)(sub_sel_condition->right_value.data);
                std::shared_ptr<DateValue> p(new DateValue(s));
                right_.is_select = false;
                right_.tupleValue_ = p;
                right_.attrtype = DATES;
            }
                break;
                /*end ----------------------------------------------------------------------------------------------*/
            default: {
                LOG_PANIC("Unsupported field type. type=%d", sub_sel_condition->right_value.type);
            }
        }
    }
    return RC::SUCCESS;
}

RC SubSelConditionFilter::check_subsel_tupset(std::pair<TupleSet,TupleSet> &tupleset_pair) const{
    if (left_.is_select == true && right_.is_select == false){
        if (comp_op_ == IN_SUB || comp_op_ == NOT_IN_SUB){
            LOG_ERROR("left of in cant be sub_select!");
            return RC::SCHEMA;
        }
        if (tupleset_pair.first.schema().fields().size() != 1){
            LOG_ERROR("left filed too many!");
            return RC::SCHEMA;
        }
//        if (tupleset_pair.first.schema().field(0).type() != right_.attrtype){
//            LOG_ERROR("compare type not match!");
//            return RC::SCHEMA;
//        }
    } else if(left_.is_select == false && right_.is_select == true){
        if (tupleset_pair.second.schema().fields().size() != 1){
            LOG_ERROR("right filed too many!");
            return RC::SCHEMA;
        }
//        if (tupleset_pair.second.schema().field(0).type() != left_.attrtype){
//            LOG_ERROR("compare type not match!");
//            return RC::SCHEMA;
//        }
    } else if(left_.is_select == true && right_.is_select == true){
        if (tupleset_pair.first.schema().fields().size() != 1 || tupleset_pair.second.schema().fields().size() != 1){
            LOG_ERROR("left and right filed too many!");
            return RC::SCHEMA;
        }
//        if (tupleset_pair.first.schema().field(0).type() != tupleset_pair.second.schema().field(0).type()){
//            LOG_ERROR("compare type not match!");
//            return RC::SCHEMA;
//        }
    } else{
        LOG_ERROR("filter never go here!");
        return RC::SCHEMA;
    }
    return RC::SUCCESS;
}

bool SubSelConditionFilter::filter(std::pair<TupleSet,TupleSet> &tupleset_pair) const {
    if (comp_op_ >= EQUAL_TO && comp_op_ <= GREAT_THAN){
        int cmp_result;
        if (left_.is_select == true && right_.is_select == false){
            if (tupleset_pair.first.size() !=1 || UNDEFINED==tupleset_pair.first.get(0).get(0).get_type()){
                return false;
            }
            cmp_result = tupleset_pair.first.get(0).get(0).compare(*right_.tupleValue_);
        } else if(left_.is_select == false && right_.is_select == true ){
            if (tupleset_pair.second.size() !=1||UNDEFINED==tupleset_pair.second.get(0).get(0).get_type()){
                return false;
            }
            cmp_result = left_.tupleValue_->compare(tupleset_pair.second.get(0).get(0));
        } else if(left_.is_select == true && right_.is_select == true){
            if (tupleset_pair.first.size() !=1 || tupleset_pair.second.size() !=1){
                return false;
            }
            if(UNDEFINED==tupleset_pair.first.get(0).get(0).get_type() || UNDEFINED==tupleset_pair.second.get(0).get(0).get_type()){
                return false;
            }
            cmp_result = tupleset_pair.first.get(0).get(0).compare(tupleset_pair.second.get(0).get(0));
        } else{
            LOG_ERROR("filter never go here!");
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
                return false;
        }
        LOG_ERROR("should never go here!");
        return false;
    } else if(comp_op_ == IN_SUB ){
        if ( tupleset_pair.second.size() == 0){
            return false;
        }
        for (int i = 0; i < tupleset_pair.second.size(); ++i) {
            if (left_.tupleValue_->compare(tupleset_pair.second.get(i).get(0)) == 0){
                return true;
            }
        }
        return false;
    } else if( comp_op_ == NOT_IN_SUB ){
        if ( tupleset_pair.second.size() == 0){
            return true;
        }
        for (int i = 0; i < tupleset_pair.second.size(); ++i) {
            if (left_.tupleValue_->compare(tupleset_pair.second.get(i).get(0)) != 0){
                continue;
            } else{
                return false;
            }
        }
        return true;
    } else{
        LOG_ERROR("not support null error");
        return false;
    }
}

