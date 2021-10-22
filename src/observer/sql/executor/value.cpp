/* Copyright (c) 2021 Xie Meiyi(xiemeiyi@hust.edu.cn) and OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "sql/executor/value.h"
/* @author: huahui @what for: 必做题，聚合查询，
 * -----------------------------------------------------------------------------------------------------------------
 */
// 保证at肯定是INTS或者FLOATS中的一个
double getNum(const std::shared_ptr<TupleValue> tv, AttrType at) {
  if(at == AttrType::INTS) {
    std::shared_ptr<IntValue> tv2 = std::static_pointer_cast<IntValue>(tv);
    return tv2->getValue();
  }else{
    std::shared_ptr<FloatValue> tv2 = std::static_pointer_cast<FloatValue>(tv);
    return tv2->getValue();
  }
}
/* --------------------------------------------------------------------------------------------------------*/
