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
// Created by Longda on 2021/4/13.
//

#include <mutex>
/* @author: huahui @what for: 必做题，增加date字段 */
#include <string.h>
/* ----------------------------------------------*/
#include "sql/parser/parse.h" 
#include "rc.h"
#include "common/log/log.h"
#include "memory.h"

RC parse(char *st, Query *sqln);

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
void relation_attr_init(RelAttr *relation_attr, const char *relation_name, const char *attribute_name) {
  /* @author: huahui @what for: 必做题，聚合查询 ------------------------------------------------------*/
  relation_attr->agg_type = AggType::NOTAGG;
  /* --------------------------------------------------------------------------------------------------*/
  if (relation_name != nullptr) {
    relation_attr->relation_name = strdup(relation_name);
  } else {
    relation_attr->relation_name = nullptr;
  }
  relation_attr->attribute_name = strdup(attribute_name);
}
/* @author: huahui @what for: 必做题，聚合查询 ------------------------------------------------------*/
void relation_agg_attr_init(RelAttr *relation_attr, AggType agg_type, const char *relation_name, const char *attribute_name) {
  relation_attr->agg_type = agg_type;
  relation_attr->is_attr = true;
  if(relation_name != nullptr) {
    relation_attr->relation_name = strdup(relation_name);
  }else{
    relation_attr->relation_name = nullptr;
  }
  relation_attr->attribute_name = strdup(attribute_name);
}
/* --------------------------------------------------------------------------------------------------*/

void relation_attr_destroy(RelAttr *relation_attr) {
  free(relation_attr->relation_name);
  free(relation_attr->attribute_name);
  relation_attr->relation_name = nullptr;
  relation_attr->attribute_name = nullptr;
}

/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
// 对relation_agg_attr_init()的替换，目的初始化RelAttrExp结构的aggregation部分
void relation_agg_relattrexp_init(RelAttrExp *exp, AggType agg_type, const char *relation_name, const char *attribute_name) {
  exp->agg_type = agg_type;
  exp->is_attr = 1;
  if(relation_name != nullptr) {
    exp->agg_relation_name = strdup(relation_name);
  } else {
    exp->agg_relation_name = nullptr;
  }
  exp->agg_attribute_name = strdup(attribute_name);
  exp->explist = nullptr;  // 这是聚合属性，不存在表达式
  memset(&exp->agg_val, 0, sizeof(exp->agg_val));
}

void explist_init_for_null(ExpList *explist) {
  Exp *exp = (Exp *)malloc(sizeof(Exp));
  exp->have_brace = 0;
  exp->have_negative = 0;
  exp->explist = NULL;
  exp->is_attr = 0;
  exp->relation_name = NULL;
  exp->attribute_name = NULL;
  value_init_null(&(exp->value));
  exp->left_exp = NULL;
  exp->calop = STARTCALOP;
  exp->num = 1;
  
  explist->exp = exp;
  explist->left_explist = NULL;
  explist->calop = STARTCALOP;
  explist->num = explist->exp->num;
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

void value_init_integer(Value *value, int v) {
  value->is_null = 0;       /* @author: huahui  @what for: null--------------------------------------*/
  value->type = INTS;
  value->data = malloc(sizeof(v));
  memcpy(value->data, &v, sizeof(v));
}
void value_init_float(Value *value, float v) {
  value->is_null = 0;      /* @author: huahui  @what for: null--------------------------------------*/
  value->type = FLOATS;
  value->data = malloc(sizeof(v));
  memcpy(value->data, &v, sizeof(v));
}

/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
// 不同于value_init_float，这个版本的可以把浮点数的字符串也保存在value->data中
// value->data分成两部分，第一部分是4个字节，保存浮点数，第二部分是后面的字符串
void value_init_float2(Value *value, float v, const char *str) {
  value->is_null = 0;   /* @author: huahui  @what for: null */
  value->type = FLOATS;
  value->data = malloc(sizeof(v) + strlen(str) + 5); // 5是为了多申请一些空间
  memcpy(value->data, &v, sizeof(v));
  char *s2 = (char *)(value->data) + sizeof(v);
  strcpy(s2, str);
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/


void value_init_string(Value *value, const char *v) {
  value->is_null = 0;     /* @author: huahui  @what for: null--------------------------------------*/
  value->type = CHARS;
  value->data = strdup(v);
}
/* @author: huahui @what for: 必做题，增加date字段 ------------------------------------------------
 * 例如：将"1024-08-16"转换为Value存储, Value:{type: DATE_T, data: @{byte(00000100 0000000 00001000 00010000)}}
 *   前两个字节表示1024，第三个字节表示8，第四个字节表示16
 */
unsigned int stoi(const char *s, int len){
  unsigned int ans = 0;
  for(int i=0; i<len; i++){
    ans *= 10;
    ans += (s[i]-'0');
  }
  return ans;
}
void value_init_date(Value *value, const char *v){
  value->is_null = 0;    /* @author: huahui  @what for: null--------------------------------------*/
  value->type = DATES;
  value->data = (char*)malloc(4+strlen(v)+5);
  // 将字符串编码成合适的存储结构
  unsigned int year, month, day;
  year = stoi(v, 4);
  int i = 5;
  if(v[i+1] == '-'){
    month = stoi(v+i, 1);
    i += 2;
  }else{
    month = stoi(v+i, 2);
    i += 3;
  }
  day = stoi(v+i, strlen(v)-i);

  unsigned char *scratch = (unsigned char*)(value->data);
  scratch[0] = (year>>8);
  scratch[1] = year;
  scratch[2] = month;
  scratch[3] = day;

  // 这里要保存字符串的原本值
  char *s2 = (char *)(value->data) + 4;
  strcpy(s2, v);
}
/* -----------------------------------------------------------------------------------------------*/

/* @author: huahui  @what for: null----------------------------------------------------------------*/
void value_init_null(Value *value) {
  value->is_null = 1;
  value->type = AttrType::UNDEFINED;
  value->data = nullptr;
}
/* -----------------------------------------------------------------------------------------------*/
void value_destroy(Value *value) {
  value->type = UNDEFINED;
  /* @author: huahui  @what for: null字段 -------------------------------------------------------*/
  if(value->is_null) {
    return;
  }
  free(value->data);
  value->data = nullptr;
}


void condition_init(Condition *condition, CompOp comp, 
                    int left_is_attr, RelAttr *left_attr, Value *left_value,
                    int right_is_attr, RelAttr *right_attr, Value *right_value) {
  condition->comp = comp;
  /* @author: huahui  @what for: date字段 
   * 解决select * from t where "2010-1-10"="2015-10-15"的问题，应该是字符串比较，但是搞成了日期比较
   * -----------------------------------------------------------------------------------------
   */
  if(!left_is_attr && !right_is_attr) {
    if(left_value->type == AttrType::DATES) {
      left_value->type = AttrType::CHARS;
      char *s = (char *)(left_value->data);
      char *tmp = (char*)malloc(strlen(s+4) + 5);
      strcpy(tmp, s+4);
      strcpy(s, tmp);
      free(tmp);
    }
    if(right_value->type == AttrType::DATES) {
      right_value->type = AttrType::CHARS;
      char *s = (char *)(right_value->data);
      char *tmp = (char*)malloc(strlen(s+4) + 5);
      strcpy(tmp, s+4);
      strcpy(s, tmp);
      free(tmp);
    }
  }
  /* ---------------------------------------------------------------------------------------------*/

  condition->left_is_attr = left_is_attr;
  if (left_is_attr) {
    condition->left_attr = *left_attr;
  } else {
    condition->left_value = *left_value;
  }

  condition->right_is_attr = right_is_attr;
  if (right_is_attr) {
    condition->right_attr = *right_attr;
  } else {
    condition->right_value = *right_value;
  }
}
void condition_destroy(Condition *condition) {
  if (condition->left_is_attr) {
    relation_attr_destroy(&condition->left_attr);
  } else {
    value_destroy(&condition->left_value);
  }
  if (condition->right_is_attr) {
    relation_attr_destroy(&condition->right_attr);
  } else {
    value_destroy(&condition->right_value);
  }
}

void attr_info_init(AttrInfo *attr_info, const char *name, AttrType type, size_t length) {
  attr_info->name = strdup(name);
  attr_info->type = type;
  attr_info->length = length;
  /* @author: huahui  @what for: null -----------------------------------------------------------------------------*/
  attr_info->nullable = 0;  // 默认属性不支持nullable
  /* ---------------------------------------------------------------------------------------------------------------*/
}
void attr_info_destroy(AttrInfo *attr_info) {
  free(attr_info->name);
  attr_info->name = nullptr;
}

void selects_init(Selects *selects, ...);
void selects_append_attribute(Selects *selects, RelAttr *rel_attr) {
  selects->attributes[selects->attr_num++] = *rel_attr;
}
void selects_append_relation(Selects *selects, const char *relation_name) {
  selects->relations[selects->relation_num++] = strdup(relation_name);
}

void selects_append_conditions(Selects *selects, Condition conditions[], size_t condition_num) {
  assert(condition_num <= sizeof(selects->conditions)/sizeof(selects->conditions[0]));
  for (size_t i = 0; i < condition_num; i++) {
    selects->conditions[i] = conditions[i];
  }
  selects->condition_num = condition_num;
}

void selects_destroy(Selects *selects) {
  for (size_t i = 0; i < selects->attr_num; i++) {
    relation_attr_destroy(&selects->attributes[i]);
  }
  selects->attr_num = 0;

  for (size_t i = 0; i < selects->relation_num; i++) {
    free(selects->relations[i]);
    selects->relations[i] = NULL;
  }
  selects->relation_num = 0;

  for (size_t i = 0; i < selects->condition_num; i++) {
    condition_destroy(&selects->conditions[i]);
  }
  selects->condition_num = 0;
}

/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void exp_destroy(Exp *exp) {
  if(exp->left_exp) {
    exp_destroy(exp->left_exp);
  }
  if(exp->have_brace) {
    if(exp->explist) explist_destroy(exp->explist);
  } else {
    if(exp->is_attr) {
      if(exp->relation_name) {
        free(exp->relation_name);
      }
      if(exp->attribute_name) {
        free(exp->attribute_name);
      }
    } else {
      value_destroy(&(exp->value));
    }
  }
  free(exp);
}
void explist_destroy(ExpList *explist) {
  if(explist->left_explist) {
    explist_destroy(explist->left_explist);
  }
  exp_destroy(explist->exp);
  free(explist);
}
void relattrexp_destroy(RelAttrExp *relattrexp) {
  if(relattrexp->agg_type == AggType::NOTAGG) {
    if(relattrexp->is_star) {
      if(relattrexp->relation_name) free(relattrexp->relation_name);
    }else {
      explist_destroy(relattrexp->explist);
    }
  } else {
    if(relattrexp->is_attr) {
      if(relattrexp->agg_relation_name) free(relattrexp->agg_relation_name);
      if(relattrexp->agg_attribute_name) free(relattrexp->agg_attribute_name);
    } else {
      if(relattrexp->agg_val_type == AggValType::AGGFLOAT && relattrexp->agg_val.str) free(relattrexp->agg_val.str);
    }
  }
}
void advselects_destroy(AdvSelects *adv_selects) {
  for(int i=0; i<adv_selects->attr_num; i++) {
    relattrexp_destroy(&(adv_selects->attr_exps[i]));
  }
  adv_selects->attr_num = 0;
  for(int i=0; i<adv_selects->relation_num; i++) {
    free(adv_selects->relations[i]);
    adv_selects->relations[i] = NULL;
  }
  adv_selects->relation_num = 0;
  for(int i=0; i<adv_selects->condition_num; i++) {
    if(adv_selects->condition_exps[i].left) explist_destroy(adv_selects->condition_exps[i].left);
    if(adv_selects->condition_exps[i].right) explist_destroy(adv_selects->condition_exps[i].right);
  }
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
// 对selects_append_attribute的替换，将RelAttrExp压入到adv_selection.attr_exps中
void advselects_append_relattrexp(AdvSelects *adv_selection, RelAttrExp *exp) {
  adv_selection->attr_exps[adv_selection->attr_num++] = *exp;
}
// 向AdvSelects中的condition_exps中push一个ConditionExp
void advselects_append_conditionexp(AdvSelects *adv_selection, ConditionExp *cond_exp) {
  assert(adv_selection->condition_num < sizeof(adv_selection->condition_exps)/sizeof(adv_selection->condition_exps[0]));
  adv_selection->condition_exps[adv_selection->condition_num++] = *cond_exp;
}
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

// insert支持多条插入 by：xiaoyu
void inserts_init(Inserts *inserts, const char *relation_name, Value values[][MAX_NUM], size_t insert_value_length[], size_t value_list_length) {
  for(size_t i = 0; i < value_list_length; i++) {
    assert(insert_value_length[i] <= sizeof(inserts->values) / sizeof(inserts->values[0]));

    for (size_t j = 0; j < insert_value_length[i]; j++) {
      inserts->values[i][j] = values[i][j];
    }
    inserts->value_num[i] = insert_value_length[i];
  }
  inserts->value_list_length = value_list_length;
  inserts->relation_name = strdup(relation_name);
}

// insert支持多条插入,修改Inserts结构 by：xiaoyu
void inserts_destroy(Inserts *inserts) {
  free(inserts->relation_name);
  inserts->relation_name = nullptr;

  for (size_t i = 0; i < inserts->value_list_length; i++) {
    for (size_t j = 0; j < inserts->value_num[i]; j++) {
      value_destroy(&inserts->values[i][j]);
    }
    inserts->value_num[i] = 0;
  }
}

void deletes_init_relation(Deletes *deletes, const char *relation_name) {
  deletes->relation_name = strdup(relation_name);
}

void deletes_set_conditions(Deletes *deletes, Condition conditions[], size_t condition_num) {
  assert(condition_num <= sizeof(deletes->conditions)/sizeof(deletes->conditions[0]));
  for (size_t i = 0; i < condition_num; i++) {
    deletes->conditions[i] = conditions[i];
  }
  deletes->condition_num = condition_num;
}
void deletes_destroy(Deletes *deletes) {
  for (size_t i = 0; i < deletes->condition_num; i++) {
    condition_destroy(&deletes->conditions[i]);
  }
  deletes->condition_num = 0;
  free(deletes->relation_name);
  deletes->relation_name = nullptr;
}

void updates_init(Updates *updates, const char *relation_name, const char *attribute_name,
                  Value *value, Condition conditions[], size_t condition_num) {
  updates->relation_name = strdup(relation_name);
  updates->attribute_name = strdup(attribute_name);
  updates->value = *value;

  assert(condition_num <= sizeof(updates->conditions)/sizeof(updates->conditions[0]));
  for (size_t i = 0; i < condition_num; i++) {
    updates->conditions[i] = conditions[i];
  }
  updates->condition_num = condition_num;
}

void updates_destroy(Updates *updates) {
  free(updates->relation_name);
  free(updates->attribute_name);
  updates->relation_name = nullptr;
  updates->attribute_name = nullptr;

  value_destroy(&updates->value);

  for (size_t i = 0; i < updates->condition_num; i++) {
    condition_destroy(&updates->conditions[i]);
  }
  updates->condition_num = 0;
}

void create_table_append_attribute(CreateTable *create_table, AttrInfo *attr_info) {
  create_table->attributes[create_table->attribute_count++] = *attr_info;
}
void create_table_init_name(CreateTable *create_table, const char *relation_name) {
  create_table->relation_name = strdup(relation_name);
}
void create_table_destroy(CreateTable *create_table) {
  for (size_t i = 0; i < create_table->attribute_count; i++) {
    attr_info_destroy(&create_table->attributes[i]);
  }
  create_table->attribute_count = 0;
  free(create_table->relation_name);
  create_table->relation_name = nullptr;
}

void drop_table_init(DropTable *drop_table, const char *relation_name) {
  drop_table->relation_name = strdup(relation_name);
}
void drop_table_destroy(DropTable *drop_table) {
  free(drop_table->relation_name);
  drop_table->relation_name = nullptr;
}


/* @author: fzh  @what for: unique index  --------------------------------------------------------------*/

void create_index_init(CreateIndex *create_index, const char *index_name,const char *relation_name, const char *attr_name ,int isunique) {
  create_index->index_name = strdup(index_name);
  create_index->relation_name = strdup(relation_name);
  create_index->attribute_name = strdup(attr_name);
  create_index->isunique = isunique;
}

/* ----------------------------------------------------------------------------------------------------*/

void create_index_destroy(CreateIndex *create_index) {
  free(create_index->index_name);
  free(create_index->relation_name);
  free(create_index->attribute_name);

  create_index->index_name = nullptr;
  create_index->relation_name = nullptr;
  create_index->attribute_name = nullptr;
}

void drop_index_init(DropIndex *drop_index, const char *index_name) {
  drop_index->index_name = strdup(index_name);
}
void drop_index_destroy(DropIndex *drop_index) {
  free((char *)drop_index->index_name);
  drop_index->index_name = nullptr;
}

void desc_table_init(DescTable *desc_table, const char *relation_name) {
  desc_table->relation_name = strdup(relation_name);
}

void desc_table_destroy(DescTable *desc_table) {
  free((char *)desc_table->relation_name);
  desc_table->relation_name = nullptr;
}

void load_data_init(LoadData *load_data, const char *relation_name, const char *file_name) {
  load_data->relation_name = strdup(relation_name);

  if (file_name[0] == '\'' || file_name[0] == '\"') {
    file_name++;
  }
  char *dup_file_name = strdup(file_name);
  int len = strlen(dup_file_name);
  if (dup_file_name[len - 1] == '\'' || dup_file_name[len - 1] == '\"') {
    dup_file_name[len - 1] = 0;
  }
  load_data->file_name = dup_file_name;
}

void load_data_destroy(LoadData *load_data) {
  free((char *)load_data->relation_name);
  free((char *)load_data->file_name);
  load_data->relation_name = nullptr;
  load_data->file_name = nullptr;
}

void query_init(Query *query) {
  query->flag = SCF_ERROR;
  memset(&query->sstr, 0, sizeof(query->sstr));
}

Query *query_create() {
  Query *query = (Query *)malloc(sizeof(Query));
  if (nullptr == query) {
    LOG_ERROR("Failed to alloc memroy for query. size=%ld", sizeof(Query));
    return nullptr;
  }

  query_init(query);
  return query;
}

void query_reset(Query *query) {
  switch (query->flag) {
    case SCF_SELECT: {
      advselects_destroy(&query->sstr.adv_selection);
    }
    break;
    case SCF_INSERT: {
      inserts_destroy(&query->sstr.insertion);
    }
    break;
    case SCF_DELETE: {
      deletes_destroy(&query->sstr.deletion);
    }
    break;
    case SCF_UPDATE: {
      updates_destroy(&query->sstr.update);
    }
    break;
    case SCF_CREATE_TABLE: {
      create_table_destroy(&query->sstr.create_table);
    }
    break;
    case SCF_DROP_TABLE: {
      drop_table_destroy(&query->sstr.drop_table);
    }
    break;
    case SCF_CREATE_INDEX: {
      create_index_destroy(&query->sstr.create_index);
    }
    break;
    case SCF_DROP_INDEX: {
      drop_index_destroy(&query->sstr.drop_index);
    }
    break;
    case SCF_SYNC: {

    }
    break;
    case SCF_SHOW_TABLES:
    break;

    case SCF_DESC_TABLE: {
      desc_table_destroy(&query->sstr.desc_table);
    }
    break;

    case SCF_LOAD_DATA: {
      load_data_destroy(&query->sstr.load_data);
    }
    break;
    case SCF_BEGIN:
    case SCF_COMMIT:
    case SCF_ROLLBACK:
    case SCF_HELP:
    case SCF_EXIT:
    case SCF_ERROR:
    break;
  }
}

void query_destroy(Query *query) {
  query_reset(query);
  free(query);
}

/* @author: huahui  @what for: 聚合查询  --------------------------------------------------------------*/
char * aggtypeToStr(AggType aggtype) {
  char res[100];
  if(aggtype == AggType::AGGCOUNT) {
    strcpy(res, "COUNT");
  }else if(aggtype == AggType::AGGMAX) {
    strcpy(res, "MAX");
  }else if(aggtype == AggType::AGGMIN) {
    strcpy(res, "MIN");
  }else if(aggtype == AggType::AGGAVG) {
    strcpy(res, "AVG");
  }else {
    strcpy(res, "NOTAGG");
  }
  return strdup(res);
}
/* ------------------------------------------------------------------------------------------------------ */

char *calopToStr(CalOp calop) {
  if(calop == CalOp::STARTCALOP || calop == CalOp::ENDCALOP) {
    return "";
  }
  if(calop == CalOp::PLUS_OP) {
    return "+";
  } else if(calop == CalOp::MINUS_OP) {
    return "-";
  } else if(calop == CalOp::TIME_OP) {
    return "*";
  } else if(calop == CalOp::DIVIDE_OP) {
    return "/";
  }
  return "";
}

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

////////////////////////////////////////////////////////////////////////////////

extern "C" int sql_parse(const char *st, Query  *sqls);

RC parse(const char *st, Query *sqln) {
  sql_parse(st, sqln);

  if (sqln->flag == SCF_ERROR)
    return SQL_SYNTAX;
  else
    return SUCCESS;
}