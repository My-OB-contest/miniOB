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
// Created by wangyunlai.wyl on 2021/6/7.
//

#ifndef __OBSERVER_SQL_PARSER_PARSE_DEFS_H__
#define __OBSERVER_SQL_PARSER_PARSE_DEFS_H__

#include <stddef.h>

#define MAX_NUM 20
#define MAX_REL_NAME 20
#define MAX_ATTR_NAME 20
#define MAX_ERROR_MESSAGE 20
#define MAX_DATA 50

/* @author: huahui @what for: 必做题，聚合查询 ------------------------------------------------*/
typedef enum {NOTAGG, AGGCOUNT, AGGMAX, AGGMIN, AGGAVG} AggType;
typedef enum {AGGNUMBER, AGGFLOAT} AggValType;
typedef struct {int intv; float floatv; char *str; } AggVal;
/* -----------------------------------------------------------------------------------------------*/

//属性结构体
typedef struct {
  /* @author: huahui @what for: 必做题，聚合查询 ------------------------------------------------------*/
  // 当agg_type==NOTAGG时,只有relation_name和attribute_name生效
  // 当agg_type!=NOTAGG && is_attr时, agg_type, relation_name和attribute_name生效
  // 当agg_type!=NOTAGG && !is_attr时, agg_type, agg_val_type和agg_val生效
  AggType agg_type;   // 标识是否是聚合查询以及是什么聚合查询，NOTAGG表示不是
  int is_attr;       // 如果是聚合属性，则这个is_attr判断是属性还是数值
  AggValType agg_val_type;
  AggVal agg_val;
  /* --------------------------------------------------------------------------------------------------*/
  char *relation_name;   // relation name (may be NULL) 表名
  char *attribute_name;  // attribute name              属性名
} RelAttr;

typedef enum {
  EQUAL_TO,     //"="     0
  LESS_EQUAL,   //"<="    1
  NOT_EQUAL,    //"<>"    2
  LESS_THAN,    //"<"     3
  GREAT_EQUAL,  //">="    4
  GREAT_THAN,   //">"     5
  IS,            // is 
  ISNOT,         // is not  /* @author: huahui  @what for: null */
  NO_OP                     /* @author: huahui  @what for: null */
} CompOp;
 
/* @author: huahui @what for: 必做题，增加date字段 ------------------------------------------------*/
//属性值类型
typedef enum { UNDEFINED, CHARS, INTS, FLOATS, DATES } AttrType;
/* -----------------------------------------------------------------------------------------------*/

//属性值
typedef struct _Value {
  int is_null;  /* @author: huahui  @what for: null ------------------------------------------------------------------------------*/
  AttrType type;  // type of value
  void *data;     // value
} Value;

typedef struct _Condition {
  int left_is_attr;    // TRUE if left-hand side is an attribute
                       // 1时，操作符左边是属性名，0时，是属性值
  Value left_value;    // left-hand side value if left_is_attr = FALSE
  RelAttr left_attr;   // left-hand side attribute
  CompOp comp;         // comparison operator
  int right_is_attr;   // TRUE if right-hand side is an attribute
                       // 1时，操作符右边是属性名，0时，是属性值
  RelAttr right_attr;  // right-hand side attribute if right_is_attr = TRUE 右边的属性
  Value right_value;   // right-hand side value if right_is_attr = FALSE
} Condition;


/* @author: huahui  @what for: expression 
 *---------------------------------------------------------------------------------------------------------------------*/
typedef enum {
  STARTCALOP,
  PLUS_OP,
  MINUS_OP,
  TIME_OP,
  DIVIDE_OP,
  ENDCALOP
} CalOp;

struct ExpList;
// 5; a; a*5; a/5; a/b; 5/b; *等只含有乘除法，不含加减法的表达式
typedef struct Exp {
  // 若have_brace==true，则Exp里面只有explist有效
  // is_attr为真，relation_name和attribute_name有效
  // is_attr为假，value有效
  // left_exp默认为nullptr, 若为nullptr，则calop属性失效
  int have_brace;                 // 表达式外面是否有括号包围
  int have_negative;              // 表达式前面是否有负号，默认是没有的，一般在Exp树的根结点中使用have_negative表示这个表达式是否有负号
  // have_brace == true
    struct ExpList *explist;               // 若have_brace==true，则Exp里面只有explist有效
  // have_brace == false
    int is_attr;           // 标识是否是属性
    // is_attr == 1
      char *relation_name;   // relation name (may be NULL) 表名
      char *attribute_name;  // attribute name              属性名
    // is_attr == 0
      Value value;           // 表达式中的常量值
  struct Exp *left_exp;        // 为nullptr,表示这个结点是表达式树的叶子结点，而且后面的CalOp属性失效
  // left_exp != nullptr
    CalOp calop;           // 只能是乘除号
    
  int num;                  // 表达式中item的个数，这个item可以是*, id或value
} Exp;

// 可以包含一个Exp，也可以用加减号将多个ExpList连接在一起
typedef struct ExpList {
  // left_explist为假时, calop失效
  Exp *exp;
  struct ExpList *left_explist;
  CalOp calop;                    // 只能是加减号
    
  int num;                  // 表达式中item的个数，这个item可以是*, id或value
} ExpList;

// RelAttrExp有以下三种情况
// select * from t; 或者 select t.* from t;
// select (i+f)-4.5, i-f from t;
// select count(i), avg(f) from t;
// ----------------------------------------------
// 当agg_type!=NOTAGG 或者 is_star==1 或者 num==1并且没有括号并且没有负号 则说明这个RelAttrExp是简单的，就可以转换为RelAttr结构
// RelAttrExp有三种种类：1. star 2. 表达式 3. 非数值类型,如日期和字符串
typedef struct {
  AggType agg_type;   // 标识是否是聚合查询以及是什么聚合查询，NOTAGG表示不是
  // agg_type == NOTAGG
    int is_star;              // 表示是不是类似于*, t.*的情况，如果是，则is_star = true
    // is_star == 1
      char *relation_name;      // 如果agg_type==NOTAGG并且is_star==1，则这个属性有效  
    // is_star == 0
      ExpList *explist;    // 表达式
      int num;                  // 当agg_type==NOTAGG并且is_star==0时有效, 表达式中item的个数，这个item可以是*, id或value
  // agg_type != NOTAGG
    int is_attr;        // 如果是聚合属性并且is_star==0，则这个is_attr判断是属性还是数值
    char *agg_relation_name;  // 聚合属性的表名
    char *agg_attribute_name; // 聚合属性的属性名
    AggValType agg_val_type;  // 聚合属性的数值类型
    AggVal agg_val;           // 聚合属性的数值
}RelAttrExp;

typedef struct {
  ExpList *left;
  ExpList *right;
  CompOp comp; 
} ConditionExp; // 对Condition的替换
/* -----------------------------------------------------------------------------------------------------------------------*/


// struct of select
typedef struct {
  size_t    attr_num;               // Length of attrs in Select clause
  RelAttr   attributes[MAX_NUM];    // attrs in Select clause
  size_t    relation_num;           // Length of relations in Fro clause
  char *    relations[MAX_NUM];     // relations in From clause
  size_t    condition_num;          // Length of conditions in Where clause
  Condition conditions[MAX_NUM];    // conditions in Where clause
} Selects;

/* @author: huahui  @what for: expression -----------------------------------------------------------------------*/
typedef struct {
  size_t       attr_num;
  RelAttrExp   attr_exps[MAX_NUM];  // 删掉RelAttr，增加RelAttrExp
  size_t       relation_num;
  char *       relations[MAX_NUM];
  size_t       condition_num;
  ConditionExp condition_exps[MAX_NUM]; // 删掉Condition，增加ConditionExp
}AdvSelects; // advanced selects: 支持条件表达式
/* --------------------------------------------------------------------------------------------------------------*/

// struct of insert
// insert支持多条插入,修改Inserts结构 by：xiaoyu
typedef struct {
  char *relation_name;    // Relation to insert into
  size_t value_list_length;
  size_t value_num[MAX_NUM];       // Length of values
  Value values[MAX_NUM][MAX_NUM];  // values to insert
} Inserts;

// struct of delete
typedef struct {
  char *relation_name;            // Relation to delete from
  size_t condition_num;           // Length of conditions in Where clause
  Condition conditions[MAX_NUM];  // conditions in Where clause
} Deletes;

// struct of update
typedef struct {
  char *relation_name;            // Relation to update
  char *attribute_name;           // Attribute to update
  Value value;                    // update value
  size_t condition_num;           // Length of conditions in Where clause
  Condition conditions[MAX_NUM];  // conditions in Where clause
} Updates;

typedef struct {
  char *name;     // Attribute name
  AttrType type;  // Type of attribute
  size_t length;  // Length of attribute
  /* @author: huahui  @what for: null  ----------------------------------------------------------------------------*/
  int nullable;   
  /* -------------------------------------------------------------------------------------------------------------*/
} AttrInfo;

// struct of craete_table
typedef struct {
  char *relation_name;           // Relation name
  size_t attribute_count;        // Length of attribute
  AttrInfo attributes[MAX_NUM];  // attributes
} CreateTable;

// struct of drop_table
typedef struct {
  char *relation_name;  // Relation name
} DropTable;

// struct of create_index
typedef struct {
    char *index_name;      // Index name
    char *relation_name;   // Relation name
    char* attribute_name[MAX_NUM];  // Attribute name
    int   isunique;
    int   attr_num;
} CreateIndex;


// struct of  drop_index
typedef struct {
  const char *index_name;  // Index name
} DropIndex;

typedef struct {
  const char *relation_name;
} DescTable;

typedef struct {
  const char *relation_name;
  const char *file_name;
} LoadData;

union Queries {
  Selects selection;
  AdvSelects adv_selection; /* @author: huahui  @what for: expression -----------------------------------------*/
  Inserts insertion;
  Deletes deletion;
  Updates update;
  CreateTable create_table;
  DropTable drop_table;
  CreateIndex create_index;
  DropIndex drop_index;
  DescTable desc_table;
  LoadData load_data;
  char *errors;
};

// 修改yacc中相关数字编码为宏定义
enum SqlCommandFlag {
  SCF_ERROR = 0,
  SCF_SELECT,
  SCF_INSERT,
  SCF_UPDATE,
  SCF_DELETE,
  SCF_CREATE_TABLE,
  SCF_DROP_TABLE,
  SCF_CREATE_INDEX,
  SCF_DROP_INDEX,
  SCF_SYNC,
  SCF_SHOW_TABLES,
  SCF_DESC_TABLE,
  SCF_BEGIN,
  SCF_COMMIT,
  SCF_ROLLBACK,
  SCF_LOAD_DATA,
  SCF_HELP,
  SCF_EXIT
};
// struct of flag and sql_struct
typedef struct Query {
  enum SqlCommandFlag flag;
  union Queries sstr;
} Query;

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

void relation_attr_init(RelAttr *relation_attr, const char *relation_name, const char *attribute_name);
/* @author: huahui @what for: 必做题，聚合查询 ------------------------------------------------------*/
void relation_agg_attr_init(RelAttr *relation_attr, AggType agg_type, const char *relation_name, const char *attribute_name);
/* --------------------------------------------------------------------------------------------------*/
void relation_attr_destroy(RelAttr *relation_attr);

/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
// 对relation_agg_attr_init()的替换，目的初始化RelAttrExp结构的aggregation部分
void relation_agg_relattrexp_init(RelAttrExp *exp, AggType agg_type, const char *relation_name, const char *attribute_name);

void explist_init_for_null(ExpList *explist);

/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

void value_init_integer(Value *value, int v);
void value_init_float(Value *value, float v); 
/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void value_init_float2(Value *value, float v, const char *str);
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
void value_init_string(Value *value, const char *str);
/* @author: huahui @what for: 必做题，增加date字段 ------------------------------------------------*/
void value_init_date(Value *value, const char *v);
/* -----------------------------------------------------------------------------------------------*/
/* @author: huahui  @what for: null----------------------------------------------------------------*/
void value_init_null(Value *value);
/* -----------------------------------------------------------------------------------------------*/
void value_destroy(Value *value);

void condition_init(Condition *condition, CompOp comp, int left_is_attr, RelAttr *left_attr, Value *left_value,
    int right_is_attr, RelAttr *right_attr, Value *right_value);
void condition_destroy(Condition *condition);


void attr_info_init(AttrInfo *attr_info, const char *name, AttrType type, size_t length);
void attr_info_destroy(AttrInfo *attr_info);

void selects_init(Selects *selects, ...);
void selects_append_attribute(Selects *selects, RelAttr *rel_attr);
void selects_append_relation(Selects *selects, const char *relation_name);
void selects_append_conditions(Selects *selects, Condition conditions[], size_t condition_num);
void selects_destroy(Selects *selects);
/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
void exp_destroy(Exp *exp);
void explist_destroy(ExpList *explist);
void relattrexp_destroy(RelAttrExp *relattrexp);
void advselects_destroy(AdvSelects *adv_selects);   // 将AdvSelects销毁掉
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
// 对selects_append_attribute的替换，将RelAttrExp压入到adv_selection.attr_exps中
void advselects_append_relattrexp(AdvSelects *adv_selection, RelAttrExp *exp);
// 向AdvSelects中的condition_exps中push一个ConditionExp
void advselects_append_conditionexp(AdvSelects *adv_selection, ConditionExp *cond_exp);
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

// insert支持多条插入 by：xiaoyu
void inserts_init(Inserts *inserts, const char *relation_name, Value values[][MAX_NUM], size_t insert_value_length[], size_t value_list_length);
void inserts_destroy(Inserts *inserts);

void deletes_init_relation(Deletes *deletes, const char *relation_name);
void deletes_set_conditions(Deletes *deletes, Condition conditions[], size_t condition_num);
void deletes_destroy(Deletes *deletes);

void updates_init(Updates *updates, const char *relation_name, const char *attribute_name, Value *value,
    Condition conditions[], size_t condition_num);
void updates_destroy(Updates *updates);

void create_table_append_attribute(CreateTable *create_table, AttrInfo *attr_info);
void create_table_init_name(CreateTable *create_table, const char *relation_name);
void create_table_destroy(CreateTable *create_table);
/* @author: fzh  @what for: unique index multi index  --------------------------------------------------------------*/
void create_index_init(CreateIndex *create_index, const char *index_name, const char *relation_name,int isunique);
void create_index_append_attribute(CreateIndex *create_index, const char *attr_name);

/* ----------------------------------------------------------------------------------------------------*/


void drop_table_init(DropTable *drop_table, const char *relation_name);
void drop_table_destroy(DropTable *drop_table);


void create_index_destroy(CreateIndex *create_index);

void drop_index_init(DropIndex *drop_index, const char *index_name);
void drop_index_destroy(DropIndex *drop_index);

void desc_table_init(DescTable *desc_table, const char *relation_name);
void desc_table_destroy(DescTable *desc_table);

void load_data_init(LoadData *load_data, const char *relation_name, const char *file_name);
void load_data_destroy(LoadData *load_data);

void query_init(Query *query);
Query *query_create();  // create and init
void query_reset(Query *query);
void query_destroy(Query *query);  // reset and delete

/* @author: huahui  @what for: 聚合查询  --------------------------------------------------------------*/
char * aggtypeToStr(AggType aggtype);
/* ----------------------------------------------------------------------------------------------------*/

char *calopToStr(CalOp calop);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // __OBSERVER_SQL_PARSER_PARSE_DEFS_H__