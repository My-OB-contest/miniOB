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

#ifndef __OBSERVER_SQL_EXECUTOR_VALUE_H_
#define __OBSERVER_SQL_EXECUTOR_VALUE_H_

#include <string.h>
#include <stdio.h>

#include <string>
#include <ostream>
#include <sstream>
#include <memory>
#include <cmath>

#include "sql/parser/parse_defs.h"
class TupleValue {
public:
  TupleValue() = default;
  virtual ~TupleValue() = default;

  virtual void to_string(std::ostream &os) const = 0;
  virtual int compare(const TupleValue &other) const = 0;
  virtual void *get_value() const  = 0;
  virtual AttrType get_type() const = 0;
private:
};

class IntValue : public TupleValue {
public:
  explicit IntValue(int value) : value_(value) {
  }

  void to_string(std::ostream &os) const override {
    os << value_;
  }

  int compare(const TupleValue &other) const override {
    if (other.get_type()==FLOATS){
        float  left = (float )value_;
        float result;
        void *right_pr = other.get_value();
        float  right = *(float *)(right_pr);
        result = left - right;
        free(right_pr);
        if (result < 1e-6 && result > -1e-6) {
            return 0;
        }
        return result > 0 ? 1: -1;
    }
    const IntValue & int_other = (const IntValue &)other;
    return value_ - int_other.value_;
  }
  int getValue() const {
    return value_;
  }
  void *get_value() const{
      int *i = (int *)malloc(sizeof(int));
      *i = value_;
      return (void *)i;
  }
  AttrType get_type() const {
      return INTS;
  }
private:
  int value_;
};
class FloatValue : public TupleValue {
public:
    explicit FloatValue(float value) : value_(value) {
    }

    void to_string(std::ostream &os) const override {
        os << formatFloat(value_);
    }

    int compare(const TupleValue &other) const override {
        float result;
        if (other.get_type()==INTS){
            const IntValue & int_other = (const IntValue &)other;
            int int_value = int_other.getValue();
            result = value_ - (float)int_value;
        }else{
            const FloatValue & float_other = (const FloatValue &)other;
            result = value_ - float_other.value_;
        }
        if (result < 1e-6 && result > -1e-6) {
            return 0;
        }
        return result > 0 ? 1: -1;
    }
    double getValue() const {
        return value_;
    }
    void *get_value() const {
        float *f = (float *)malloc(sizeof(float));
        *f = value_;
        return (void *)f;
    }
    AttrType get_type() const {
        return FLOATS;
    }
private:
    /* @author: huahui  @what for: 浮点数默认格式化---------------------------------------------
     */
    std::string formatFloat(float f) const {
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
    float value_;
};


class StringValue : public TupleValue {
public:
  StringValue(const char *value, int len) : value_(value, len){
  }
  explicit StringValue(const char *value) : value_(value) {
  }

  void to_string(std::ostream &os) const override {
    os << value_;
  }

  int compare(const TupleValue &other) const override {
    const StringValue &string_other = (const StringValue &)other;
    return strcmp(value_.c_str(), string_other.value_.c_str());
  }

  /* @what for: expression */
  void *get_value() const {
    char *s = strdup(value_.c_str());
    return (void *)s;
  }
  AttrType get_type() const {
    return CHARS;
  }
private:
  std::string value_;
};

/*
 * @author: huahui
 * @what for: 必做题，增加date字段
 * begin -------------------------------------------------------------------------------------------
 */
class DateValue : public TupleValue {
public:
  DateValue(){};
  DateValue(const unsigned char *value){
    year = (int)value[0]*256 + (int)value[1];
    month = (int)value[2];
    day = (int)value[3];

    value_ = malloc(4);
    memcpy(value_, (const void *)value, 4);
  }
  void to_string(std::ostream &os) const override {
    if(year / 1000 == 0){
      os << "0";
    }
    if(year / 100 == 0){
      os << "0";
    }
    if(year / 10 == 0){
      os << "0";
    }
    os << year << "-";
    if(month / 10 == 0){
      os << "0";
    }
    os << month << "-";
    if(day / 10 == 0){
      os << "0";
    }
    os << day;
  }
  int compare(const TupleValue &other) const override {
    std::stringstream ss;
    to_string(ss);
    std::string s1 = ss.str();
    ss.str("");
    other.to_string(ss);
    std::string s2 = ss.str();
    return strcmp(s1.c_str(), s2.c_str());
  }
  /* @what for: expression*/
  void *get_value() const {
    return value_;
  }
  AttrType get_type() const {
        return DATES;
  }
private:
  int year, month, day; // 从字节数组中解析出的year, month, day
  void *value_;
};

/* @author: huahui  @what for: null
 * TupleValue的子类，用于表示null
 * -------------------------------------------------------------------------------------------------------------------
 */
class NullValue : public TupleValue {
public:
  NullValue(){};
  void to_string(std::ostream &os) const override {
    os << "NULL";
  }
  // 这里要注意，虽然compare函数返回0，但并不是表示null与任何值比较都是相等，而正好相反
  // 在condition的filter中要注意
  int compare(const TupleValue &other) const override {
    return 0;
  }
  void *get_value() const {
      return nullptr;
  }
  AttrType get_type() const {
      return UNDEFINED;
  }
};
/*end -----------------------------------------------------------------------------------------------------------------*/

/* @author: huahui @what for: 必做题，聚合查询，
 * -----------------------------------------------------------------------------------------------------------------
 */
double getNum(const std::shared_ptr<TupleValue> tv, AttrType at);
/* --------------------------------------------------------------------------------------------------------*/

#endif //__OBSERVER_SQL_EXECUTOR_VALUE_H_
