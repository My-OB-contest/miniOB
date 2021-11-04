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
// Created by wangyunlai.wyl on 2021/5/18.
//

#include "storage/common/index_meta.h"
#include "storage/common/field_meta.h"
#include "storage/common/table_meta.h"
#include "common/lang/string.h"
#include "common/log/log.h"
#include "rc.h"
#include "json/json.h"

const static Json::StaticString FIELD_NAME("name");
const static Json::StaticString FIELD_FIELD_NAME("fields_name");
const static Json::StaticString UNIQUE_FLAG("isunique");

//fzh改，支持多列索引
std::vector<std::string> split(std::string str, std::string pattern)
{
    std::string::size_type pos;
    std::vector<std::string> result;
    str += pattern;//扩展字符串以方便操作
    int size = str.size();
    for (int i = 0; i < size; i++)
    {
        pos = str.find(pattern, i);
        if (pos < size)
        {
            std::string s = str.substr(i, pos - i);
            result.push_back(s);
            i = pos + pattern.size() - 1;
        }
    }
    return result;
}
RC IndexMeta::init(const char *name, std::vector<FieldMeta> &fields_meta, int isunique) {
  if (nullptr == name || common::is_blank(name)) {
    return RC::INVALID_ARGUMENT;
  }

  name_ = name;
  for(auto it : fields_meta){
      fields_.push_back(it.name());
  }
  field_num_ = fields_.size();
  isunique_ = isunique;
  return RC::SUCCESS;
}

void IndexMeta::to_json(Json::Value &json_value) const {
  json_value[FIELD_NAME] = name_;
  std::string totalstr;
  for(int i = 0;i < field_num_ ;i++ ){
      if (i != field_num_-1){
          totalstr=totalstr+fields_[i]+",";
      } else{
          totalstr=totalstr+fields_[i];
      }
  }
  json_value[FIELD_FIELD_NAME] = totalstr;
  json_value[UNIQUE_FLAG] = isunique_;
}

RC IndexMeta::from_json(const TableMeta &table, const Json::Value &json_value, IndexMeta &index) {
  const Json::Value &name_value = json_value[FIELD_NAME];
  const Json::Value &field_value = json_value[FIELD_FIELD_NAME];
  const Json::Value &unique_value = json_value[UNIQUE_FLAG];
  if (!name_value.isString()) {
    LOG_ERROR("Index name is not a string. json value=%s", name_value.toStyledString().c_str());
    return RC::GENERIC_ERROR;
  }

  if (!field_value.isString()) {
    LOG_ERROR("Field name of index [%s] is not a string. json value=%s",
              name_value.asCString(), field_value.toStyledString().c_str());
    return RC::GENERIC_ERROR;
  }
  std::string beforesplit(field_value.asCString());
  std::vector<std::string> fields = split(beforesplit,",");
  std::vector<FieldMeta> fieldsmeta;
  for (int i = 0; i < fields.size(); ++i) {
    const FieldMeta *field = table.field(fields[i].c_str());
    if (nullptr == field) {
        LOG_ERROR("Deserialize index [%s]: no such field: %s", name_value.asCString(), field_value.asCString());
        return RC::SCHEMA_FIELD_MISSING;
    }
    fieldsmeta.push_back(std::move(*field));
  }

  return index.init(name_value.asCString(), fieldsmeta,unique_value.asInt());
}

const char *IndexMeta::name() const {
  return name_.c_str();
}

const char *IndexMeta::field(int i) const {
  return fields_[i].c_str();
}
const int IndexMeta::field_num() const{
    return field_num_;
}

const int IndexMeta::isunique() const {
    return isunique_;
}

void IndexMeta::desc(std::ostream &os) const {
  os << "index name=" << name_
  << ", fields=" ;
  for(int i = 0 ; i < field_num_ ; ++i){
      os <<  fields_[i]<<",";
  }
  os << " isunique=" << isunique_;
}

std::vector<std::string> IndexMeta::fields() {
    return fields_;
}
