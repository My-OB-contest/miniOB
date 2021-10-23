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

#include "sql/parser/parse_defs.h"

class TupleValue {
public:
  TupleValue() = default;
  virtual ~TupleValue() = default;

  virtual void to_string(std::ostream &os) const = 0;
  virtual int compare(const TupleValue &other) const = 0;
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
    const IntValue & int_other = (const IntValue &)other;
    return value_ - int_other.value_;
  }
  int getValue() const {
    return value_;
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
    const FloatValue & float_other = (const FloatValue &)other;
    float result = value_ - float_other.value_;
    if (result > 0) { // 浮点数没有考虑精度问题
      return 1;
    }
    if (result < 0) {
      return -1;
    }
    return 0;
  }
  double getValue() const {
    return value_;
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
private:
  int year, month, day; // 从字节数组中解析出的year, month, day
};

/*end ----------------------------------------------------------------------------------------------*/

/* @author: huahui @what for: 必做题，聚合查询，
 * -----------------------------------------------------------------------------------------------------------------
 */
double getNum(const std::shared_ptr<TupleValue> tv, AttrType at);
/* --------------------------------------------------------------------------------------------------------*/

#endif //__OBSERVER_SQL_EXECUTOR_VALUE_H_
