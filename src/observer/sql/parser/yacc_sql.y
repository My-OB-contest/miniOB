
%{

#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.tab.h"
#include "sql/parser/lex.yy.h"
// #include "common/log/log.h" // 包含C++中的头文件

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// insert支持多条插入,修改ParserContext结构 by：xiaoyu
typedef struct ParserContext {
  Query * ssql;
  size_t select_length;
  size_t condition_length;
  size_t from_length;
  size_t value_length;
  size_t value_list_length;
  size_t insert_value_length[MAX_NUM];
  Value values[MAX_NUM];
  Value insert_values[MAX_NUM][MAX_NUM];
  Condition conditions[MAX_NUM];
  CompOp comp;
	char id[MAX_NUM];
} ParserContext;

//获取子串
char *substr(const char *s,int n1,int n2)/*从s中提取下标为n1~n2的字符组成一个新字符串，然后返回这个新串的首地址*/
{
  char *sp = malloc(sizeof(char) * (n2 - n1 + 2));
  int i, j = 0;
  for (i = n1; i <= n2; i++) {
    sp[j++] = s[i];
  }
  sp[j] = 0;
  return sp;
}

void yyerror(yyscan_t scanner, const char *str)
{
  ParserContext *context = (ParserContext *)(yyget_extra(scanner));
  query_reset(context->ssql);
  context->ssql->flag = SCF_ERROR;
  context->condition_length = 0;
  context->from_length = 0;
  context->select_length = 0;
  context->value_length = 0;
  for(size_t i = 0; i < context->value_list_length; i++){
    context->ssql->sstr.insertion.value_num[i] = 0;
  }
  context->value_list_length = 0;
  printf("parse sql failed. error=%s", str);
}


ParserContext *get_context(yyscan_t scanner)
{
  return (ParserContext *)yyget_extra(scanner);
}

#define CONTEXT get_context(scanner)

%}

%define api.pure full
%lex-param { yyscan_t scanner }
%parse-param { void *scanner }

//标识tokens
%token  SEMICOLON
        CREATE
        DROP
        TABLE
        TABLES
        INDEX
        SELECT
        DESC
        SHOW
        SYNC
        INSERT
        DELETE
        UPDATE
        LBRACE
        RBRACE
        COMMA
        TRX_BEGIN
        TRX_COMMIT
        TRX_ROLLBACK
        INT_T
        STRING_T
        FLOAT_T
		DATE_T  /* @author: huahui @what for: 必做题，增加date字段 */
        HELP
        EXIT
        DOT //QUOTE
        INTO
        VALUES
        FROM
        WHERE
        AND
        SET
        ON
        LOAD
        DATA
        INFILE
        EQ
        LT
        GT
        LE
        GE
        NE
		COUNT /* @author: huahui @what for: 必做题，聚合查询 */
		MAX   /* @author: huahui @what for: 必做题，聚合查询 */
		MIN   /* @author: huahui @what for: 必做题，聚合查询 */
		AVG   /* @author: huahui @what for: 必做题，聚合查询 */
		INNER /* @author: fzh @what for: join */
		JOIN  /* @author: fzh @what for: join */
		NOT      /* @author: huahui @what for: null */
		NULL_A     /* @author: huahui @what for: null */
		NULLABLE /* @author: huahui @what for: null */
		IS_A      /* @author: huahui @what for: null */
		UNIQUE  /* @author: fzh @what for: unique index */
        PLUS    /* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
        MINUS
        DIVIDE   /* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
           


/* @author: huahui &what for: 聚合
 * 由于max(1.999)需要完整保留1.999，因此lex_sql.l文件中解析到FLOATS时需要保存float值和字符串
 * -----------------------------------------------------------------------------------------------------
 */
%union {
  struct _Attr *attr;
  struct _Condition *condition1;
  struct _Value *value1;
  char *string;
  int number;
  float floats;
  char *position;
  struct {
    float floats;
    char *str;
  } floatsAndStr;
  void *exp;       /* @author: huahui  @what for: expression */
  void *exp_list;  /* @author: huahui  @what for: expression */
}
/* ----------------------------------------------------------------------------------------------*/

// @what for: expression  
%token <number> NNNUMBER
%token <floatsAndStr> NNFLOAT

%token <string> ID
/* @author: huahui @what for: 必做题，增加date字段 ------------------------------------------------*/
%token <string> DATE
/* -----------------------------------------------------------------------------------------------*/
%token <string> PATH
%token <string> SSS
%token <string> STAR
%token <string> STRING_V
// 非终结符

%type <number> type;
// %type <condition1> condition;
%type <value1> value;
%type <number> number;
/* @author: huahui  @what for: expression <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/
%type <exp> exp;
%type <exp> exps;
%type <exp> exps2;
%type <exp_list> exp_list;
%type <exp_list> exp_list2;
%type <floatsAndStr> floatnumber
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

%%

commands:		//commands or sqls. parser starts here.
    /* empty */
    | commands command
    ;

command:
	  select  
	| insert
	| update
	| delete
	| create_table
	| drop_table
	| show_tables
	| desc_table
	| create_index	
	| drop_index
	| sync
	| begin
	| commit
	| rollback
	| load_data
	| help
	| exit
    ;

exit:			
    EXIT SEMICOLON {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    };

help:
    HELP SEMICOLON {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    };

sync:
    SYNC SEMICOLON {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
    ;

begin:
    TRX_BEGIN SEMICOLON {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
    ;

commit:
    TRX_COMMIT SEMICOLON {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
    ;

rollback:
    TRX_ROLLBACK SEMICOLON {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
    ;

drop_table:		/*drop table 语句的语法解析树*/
    DROP TABLE ID SEMICOLON {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, $3);
    };

show_tables:
    SHOW TABLES SEMICOLON {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
    ;

desc_table:
    DESC ID SEMICOLON {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, $2);
    }
    ;

 create_index:		/*create index 语句的语法解析树*/
    CREATE INDEX ID ON ID LBRACE field_name_list RBRACE SEMICOLON
 		{
 			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, $3, $5 ,0);
 		}
	| CREATE UNIQUE INDEX ID ON ID LBRACE field_name_list RBRACE SEMICOLON
 	    {
 	        CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_unique_index";
            create_index_init(&CONTEXT->ssql->sstr.create_index, $4, $6 ,1);
 	    }
    ;

field_name_list:
    ID
        {
            create_index_append_attribute(&CONTEXT->ssql->sstr.create_index,$1);
        }
    |ID COMMA field_name_list
        {
            create_index_append_attribute(&CONTEXT->ssql->sstr.create_index,$1);
        }


drop_index:			/*drop index 语句的语法解析树*/
    DROP INDEX ID  SEMICOLON 
		{
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, $3);
		}
    ;
create_table:		/*create table 语句的语法解析树*/
    CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON 
		{
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, $3);
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
    ;
attr_def_list:
    /* empty */
    | COMMA attr_def attr_def_list {    }
    ;
    
attr_def:
    ID_get type LBRACE number RBRACE 
		{
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, $2, $4);
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name =(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type = $2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length = $4;
			CONTEXT->value_length++;
		}
    |ID_get type
		{
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, $2, 4);
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name=(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type=$2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length=4; // default attribute length
			CONTEXT->value_length++;
		}
	/* @author: huahui  @what for: null
	 * ------------------------------------------------------------------------------------------------------------------------------------------------
	 */
	| ID_get type LBRACE number RBRACE NOT NULL_A 
	    {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, $2, $4);
			attribute.nullable = 0;
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name =(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type = $2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length = $4;
			CONTEXT->value_length++;
		}
	| ID_get type LBRACE number RBRACE NULLABLE 
	    {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, $2, $4);
			attribute.nullable = 1;
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name =(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type = $2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length = $4;
			CONTEXT->value_length++;
		}
	| ID_get type NOT NULL_A 
	    {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, $2, 4);
			attribute.nullable = 0;
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name=(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type=$2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length=4; // default attribute length
			CONTEXT->value_length++;
		}
	| ID_get type NULLABLE 
	    {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, $2, 4);
			attribute.nullable = 1;
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name=(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type=$2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length=4; // default attribute length
			CONTEXT->value_length++;
		}
	/* --------------------------------------------------------------------------------------------------------------------------------------------*/
    ;
number:
    NNNUMBER {$$ = $1;}
    | MINUS NNNUMBER {
      $$ = -($2); 
    }
		;

floatnumber:
  NNFLOAT {$$ = $1; }
  | MINUS NNFLOAT { 
    $$ = $2;
    $$.floats = -($$.floats);
    char *s = $$.str;
    char *tmp = (char *)malloc(strlen(s)+5);
    tmp[0] = '-';
    strcpy(tmp+1, $$.str);
    free($$.str);
    $$.str = strdup(tmp);
  }
  ;

type:
	INT_T { $$=INTS; }
       | STRING_T { $$=CHARS; }
       | FLOAT_T { $$=FLOATS; }
	   | DATE_T { $$=DATES; } /* @author: huahui @what for: 必做题，增加date字段 */
       ;
ID_get:
	ID 
	{
		char *temp=$1; 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
	;


/* @author: xiaoyu @what for: 选做题，插入多个值 ------------------------------------------------*/
insert:				/* insert   语句的语法解析树*/
    INSERT INTO ID VALUES value_tuple values_lists SEMICOLON
		{
			// CONTEXT->values[CONTEXT->value_length++] = *$6;

			CONTEXT->ssql->flag=SCF_INSERT;//"insert";
			// CONTEXT->ssql->sstr.insertion.relation_name = $3;
			// CONTEXT->ssql->sstr.insertion.value_num = CONTEXT->value_length;
			// for(i = 0; i < CONTEXT->value_length; i++){
			// 	CONTEXT->ssql->sstr.insertion.values[i] = CONTEXT->values[i];
      // }
            //CONTEXT->value_list_length++;
			inserts_init(&CONTEXT->ssql->sstr.insertion, $3, CONTEXT->insert_values, CONTEXT->insert_value_length, CONTEXT->value_list_length);

      //临时变量清零
      for(int i=0; i<CONTEXT->value_list_length; i++){
        CONTEXT->insert_value_length[i] = 0;
      }
      CONTEXT->value_list_length = 0;
    }

values_lists:
    /* empty */{
        //CONTEXT->value_list_length++;
    }
    | COMMA value_tuple values_lists{
  	     //CONTEXT->value_list_length++;
	  }
    ;

value_tuple:
    /* empty */
    | LBRACE insert_value value_list RBRACE  {
        CONTEXT->value_list_length++;
	  }
    ;

value_list:
    /* empty */
    | COMMA insert_value value_list{
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
    ;

insert_value:
    // insert支持多条插入,修改CONTEXT结构 by：xiaoyu
    number {
  		value_init_integer(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], $1);
		}
    |floatnumber {
  		value_init_float(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], ($1).floats);
		}
    |SSS {
		$1 = substr($1,1,strlen($1)-2);
  		value_init_string(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], $1);
		}
	|DATE {
		$1 = substr($1,1,strlen($1)-2);
  		value_init_date(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], $1);
	}
	/* @author: huahui  @what for: null----------------------------------------------------------------*/
	|NULL_A {
		// value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
		value_init_null(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++]); // 多条插入
	}
	/* -----------------------------------------------------------------------------------------------*/
    ;

value:
    // insert支持多条插入,修改CONTEXT结构 by：xiaoyu
    number {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], $1);
		}
    |floatnumber{
  		// value_init_float(&CONTEXT->values[CONTEXT->value_length++], ($1).floats);
		value_init_float2(&CONTEXT->values[CONTEXT->value_length++], ($1).floats, ($1).str);
	}
    |SSS {
			$1 = substr($1,1,strlen($1)-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], $1);
		}
	|DATE {
		$1 = substr($1,1,strlen($1)-2);
  		value_init_date(&CONTEXT->values[CONTEXT->value_length++], $1);
	}
	/* @author: huahui  @what for: null----------------------------------------------------------------*/
	|NULL_A {
		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
	}
	/* -----------------------------------------------------------------------------------------------*/
    ;

    
delete:		/*  delete 语句的语法解析树*/
    DELETE FROM ID where SEMICOLON 
		{
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, $3);
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;	
    }
    ;
update:			/*  update 语句的语法解析树*/
    UPDATE ID SET ID EQ value where SEMICOLON
		{
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, $2, $4, value, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
		}
    ;

/* @author: huahui  @what for: expression
 * 将select的语法进行了几乎全部的更改
 * <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */
// select:				/*  select 语句的语法解析树*/
//     SELECT select_attr FROM ID rel_list where SEMICOLON
// 		{
// 			// CONTEXT->ssql->sstr.selection.relations[CONTEXT->from_length++]=$4;
// 			selects_append_relation(&CONTEXT->ssql->sstr.selection, $4);

// 			selects_append_conditions(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);

// 			CONTEXT->ssql->flag=SCF_SELECT;//"select";
// 			// CONTEXT->ssql->sstr.selection.attr_num = CONTEXT->select_length;

// 			//临时变量清零
// 			CONTEXT->condition_length=0;
// 			CONTEXT->from_length=0;
// 			CONTEXT->select_length=0;
// 			CONTEXT->value_length = 0;
// 	}
// 	|SELECT select_attr FROM ID join where SEMICOLON
// 	    {
// 			selects_append_relation(&CONTEXT->ssql->sstr.selection, $4);
// 			selects_append_conditions(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);
// 			CONTEXT->ssql->flag=SCF_SELECT;//"select";
// 			CONTEXT->condition_length=0;
// 			CONTEXT->from_length=0;
// 			CONTEXT->select_length=0;
// 			CONTEXT->value_length = 0;

// 	    }
// 	;

// select_attr:
//     STAR {  
// 			RelAttr attr;
// 			relation_attr_init(&attr, NULL, "*");
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
//     | ID attr_list {
// 			RelAttr attr;
// 			relation_attr_init(&attr, NULL, $1);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
//   	| ID DOT ID attr_list {
// 			RelAttr attr;
// 			relation_attr_init(&attr, $1, $3);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 	/* @author: fzh
// 	 * @what for:  支持t.*
// 	 * -----------------------------------------------------------------------------------------------------------------
// 	 */
//     | ID DOT STAR attr_list {
//             RelAttr attr;
//             relation_attr_init(&attr, $1, "*");
//             selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
//         }
// 	/* @author: huahui 
// 	 * @what for: 必做题，聚合查询 
// 	 * -----------------------------------------------------------------------------------------------------------------
// 	 */
// 	| COUNT LBRACE ID RBRACE attr_list {
//             RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGCOUNT, NULL, $3);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 	    }
// 	    | COUNT LBRACE STAR RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGCOUNT, NULL, "*");
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| COUNT LBRACE ID DOT ID RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGCOUNT, $3, $5);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| COUNT LBRACE ID DOT STAR RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGCOUNT, $3, "*");
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| COUNT LBRACE number RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGCOUNT, NULL, "*");
// 			attr.is_attr = 0;
// 			attr.agg_val_type = AGGNUMBER;
// 			attr.agg_val.intv = $3;
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| COUNT LBRACE FLOAT RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGCOUNT, NULL, "*");
// 			attr.is_attr = 0;
// 			attr.agg_val_type = AGGFLOAT;
// 			attr.agg_val.floatv = ($3).floats;
// 			attr.agg_val.str = strdup(($3).str);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 	| MAX LBRACE ID RBRACE attr_list {
//             RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGMAX, NULL, $3);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 	    }
// 	    | MAX LBRACE STAR RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGMAX, NULL, "*");
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| MAX LBRACE ID DOT ID RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGMAX, $3, $5);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| MAX LBRACE number RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGMAX, NULL, "*");
// 			attr.is_attr = 0;
// 			attr.agg_val_type = AGGNUMBER;
// 			attr.agg_val.intv = $3;
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| MAX LBRACE FLOAT RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGMAX, NULL, "*");
// 			attr.is_attr = 0;
// 			attr.agg_val_type = AGGFLOAT;
// 			attr.agg_val.floatv = ($3).floats;
// 			attr.agg_val.str = strdup(($3).str);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 	| MIN LBRACE ID RBRACE attr_list {
//             RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGMIN, NULL, $3);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 	    }
// 	    | MIN LBRACE STAR RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGMIN, NULL, "*");
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| MIN LBRACE ID DOT ID RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGMIN, $3, $5);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| MIN LBRACE number RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGMIN, NULL, "*");
// 			attr.is_attr = 0;
// 			attr.agg_val_type = AGGNUMBER;
// 			attr.agg_val.intv = $3;
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| MIN LBRACE FLOAT RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGMIN, NULL, "*");
// 			attr.is_attr = 0;
// 			attr.agg_val_type = AGGFLOAT;
// 			attr.agg_val.floatv = ($3).floats;
// 			attr.agg_val.str = strdup(($3).str);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 	| AVG LBRACE ID RBRACE attr_list {
//             RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGAVG, NULL, $3);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 	    }
// 	    | AVG LBRACE STAR RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGAVG, NULL, "*");
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| AVG LBRACE ID DOT ID RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGAVG, $3, $5);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| AVG LBRACE number RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGAVG, NULL, "*");
// 			attr.is_attr = 0;
// 			attr.agg_val_type = AGGNUMBER;
// 			attr.agg_val.intv = $3;
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 		| AVG LBRACE FLOAT RBRACE attr_list {
// 			RelAttr attr;
// 			relation_agg_attr_init(&attr, AGGAVG, NULL, "*");
// 			attr.is_attr = 0;
// 			attr.agg_val_type = AGGFLOAT;
// 			attr.agg_val.floatv = ($3).floats;
// 			attr.agg_val.str = strdup(($3).str);
// 			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
// 		}
// 	/* ------------------------------------------------------------------------------------------------------------
// 	 */
//     ;
// attr_list:
//     /* empty */
//     | COMMA select_attr {
		
// 	}
// 	/* ------------------------------------------------------------------------------------------------------------
// 	 */
//   	;

// rel_list:
//     /* empty */
//     | COMMA ID rel_list {	
// 				selects_append_relation(&CONTEXT->ssql->sstr.selection, $2);
// 		  }
//     ;

// join:
//     INNER JOIN ID onwhere {
//         selects_append_relation(&CONTEXT->ssql->sstr.selection, $3);
//     }
//     |INNER JOIN ID onwhere join {
//         selects_append_relation(&CONTEXT->ssql->sstr.selection, $3);
//     }
//     ;
// onwhere:
//     /* empty */
//     | ON condition condition_list{

//     }
//     ;
// where:
//     /* empty */ 
//     | WHERE condition condition_list {	
// 				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
// 			}
//     ;
// condition_list:
//     /* empty */
//     | AND condition condition_list {
// 				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
// 			}
//     ;
// condition:
//     ID comOp value 
// 		{
// 			RelAttr left_attr;
// 			relation_attr_init(&left_attr, NULL, $1);

// 			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

// 			Condition condition;
// 			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
// 			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
// 			// $$ = ( Condition *)malloc(sizeof( Condition));
// 			// $$->left_is_attr = 1;
// 			// $$->left_attr.relation_name = NULL;
// 			// $$->left_attr.attribute_name= $1;
// 			// $$->comp = CONTEXT->comp;
// 			// $$->right_is_attr = 0;
// 			// $$->right_attr.relation_name = NULL;
// 			// $$->right_attr.attribute_name = NULL;
// 			// $$->right_value = *$3;

// 		}
// 		|value comOp value 
// 		{
// 			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
// 			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

// 			Condition condition;
// 			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, NULL, right_value);
// 			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
// 			// $$ = ( Condition *)malloc(sizeof( Condition));
// 			// $$->left_is_attr = 0;
// 			// $$->left_attr.relation_name=NULL;
// 			// $$->left_attr.attribute_name=NULL;
// 			// $$->left_value = *$1;
// 			// $$->comp = CONTEXT->comp;
// 			// $$->right_is_attr = 0;
// 			// $$->right_attr.relation_name = NULL;
// 			// $$->right_attr.attribute_name = NULL;
// 			// $$->right_value = *$3;

// 		}
// 		|ID comOp ID 
// 		{
// 			RelAttr left_attr;
// 			relation_attr_init(&left_attr, NULL, $1);
// 			RelAttr right_attr;
// 			relation_attr_init(&right_attr, NULL, $3);

// 			Condition condition;
// 			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, &right_attr, NULL);
// 			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
// 			// $$=( Condition *)malloc(sizeof( Condition));
// 			// $$->left_is_attr = 1;
// 			// $$->left_attr.relation_name=NULL;
// 			// $$->left_attr.attribute_name=$1;
// 			// $$->comp = CONTEXT->comp;
// 			// $$->right_is_attr = 1;
// 			// $$->right_attr.relation_name=NULL;
// 			// $$->right_attr.attribute_name=$3;

// 		}
//     |value comOp ID
// 		{
// 			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
// 			RelAttr right_attr;
// 			relation_attr_init(&right_attr, NULL, $3);

// 			Condition condition;
// 			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 1, &right_attr, NULL);
// 			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

// 			// $$=( Condition *)malloc(sizeof( Condition));
// 			// $$->left_is_attr = 0;
// 			// $$->left_attr.relation_name=NULL;
// 			// $$->left_attr.attribute_name=NULL;
// 			// $$->left_value = *$1;
// 			// $$->comp=CONTEXT->comp;
			
// 			// $$->right_is_attr = 1;
// 			// $$->right_attr.relation_name=NULL;
// 			// $$->right_attr.attribute_name=$3;
		
// 		}
//     |ID DOT ID comOp value
// 		{
// 			RelAttr left_attr;
// 			relation_attr_init(&left_attr, $1, $3);
// 			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

// 			Condition condition;
// 			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
// 			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

// 			// $$=( Condition *)malloc(sizeof( Condition));
// 			// $$->left_is_attr = 1;
// 			// $$->left_attr.relation_name=$1;
// 			// $$->left_attr.attribute_name=$3;
// 			// $$->comp=CONTEXT->comp;
// 			// $$->right_is_attr = 0;   //属性值
// 			// $$->right_attr.relation_name=NULL;
// 			// $$->right_attr.attribute_name=NULL;
// 			// $$->right_value =*$5;			
							
//     }
//     |value comOp ID DOT ID
// 		{
// 			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

// 			RelAttr right_attr;
// 			relation_attr_init(&right_attr, $3, $5);

// 			Condition condition;
// 			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 1, &right_attr, NULL);
// 			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
// 			// $$=( Condition *)malloc(sizeof( Condition));
// 			// $$->left_is_attr = 0;//属性值
// 			// $$->left_attr.relation_name=NULL;
// 			// $$->left_attr.attribute_name=NULL;
// 			// $$->left_value = *$1;
// 			// $$->comp =CONTEXT->comp;
// 			// $$->right_is_attr = 1;//属性
// 			// $$->right_attr.relation_name = $3;
// 			// $$->right_attr.attribute_name = $5;
									
//     }
//     |ID DOT ID comOp ID DOT ID
// 		{
// 			RelAttr left_attr;
// 			relation_attr_init(&left_attr, $1, $3);
// 			RelAttr right_attr;
// 			relation_attr_init(&right_attr, $5, $7);

// 			Condition condition;
// 			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, &right_attr, NULL);
// 			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
// 			// $$=( Condition *)malloc(sizeof( Condition));
// 			// $$->left_is_attr = 1;		//属性
// 			// $$->left_attr.relation_name=$1;
// 			// $$->left_attr.attribute_name=$3;
// 			// $$->comp =CONTEXT->comp;
// 			// $$->right_is_attr = 1;		//属性
// 			// $$->right_attr.relation_name=$5;
// 			// $$->right_attr.attribute_name=$7;
//     } 
// 	/* @author: huahui  @what for: null ------------------------------------------------------------------------*/
// 	| ID IS_A NULL_A {
// 		RelAttr left_attr;
// 		relation_attr_init(&left_attr, NULL, $1);
// 		CONTEXT->comp = IS;
// 		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
// 		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

// 		Condition condition;
// 		condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
// 		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
// 	}
// 	| ID DOT ID IS_A NULL_A {
// 		RelAttr left_attr;
// 		relation_attr_init(&left_attr, $1, $3);
// 		CONTEXT->comp = IS;
// 		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
// 		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

// 		Condition condition;
// 		condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
// 		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
// 	}
// 	| ID IS_A NOT NULL_A {
// 		RelAttr left_attr;
// 		relation_attr_init(&left_attr, NULL, $1);
// 		CONTEXT->comp = ISNOT;
// 		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
// 		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

// 		Condition condition;
// 		condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
// 		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
// 	}
// 	| ID DOT ID IS_A NOT NULL_A {
// 		RelAttr left_attr;
// 		relation_attr_init(&left_attr, $1, $3);
// 		CONTEXT->comp = ISNOT;
// 		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
// 		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

// 		Condition condition;
// 		condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
// 		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
// 	}
// 	| value IS_A NULL_A {
// 		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
// 		CONTEXT->comp = IS;
// 		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
// 		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

// 		Condition condition;
// 		condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, NULL, right_value);
// 		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
// 	}
// 	| value IS_A NOT NULL_A {
// 		Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
// 		CONTEXT->comp = ISNOT;
// 		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
// 		Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

// 		Condition condition;
// 		condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, NULL, right_value);
// 		CONTEXT->conditions[CONTEXT->condition_length++] = condition;
// 	}
// 	/* ------------------------------------------------------------------------------------------------------------*/
//     ;

select:
  SELECT relattrexp FROM ID rel_list where SEMICOLON {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    adv_selects->relations[adv_selects->relation_num++] = strdup($4);
    CONTEXT->ssql->flag=SCF_SELECT;
    //临时变量清零
    CONTEXT->condition_length=0;
    CONTEXT->from_length=0;
    CONTEXT->select_length=0;
    CONTEXT->value_length = 0;
  }
  | SELECT relattrexp FROM ID join where SEMICOLON
  {
     AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
     adv_selects->relations[adv_selects->relation_num++] = strdup($4);
     CONTEXT->ssql->flag=SCF_SELECT;
     //临时变量清零
     CONTEXT->condition_length=0;
     CONTEXT->from_length=0;
     CONTEXT->select_length=0;
     CONTEXT->value_length = 0;
  }
  ;
  
relattrexp:
  exp_list relattrexp2 {
    RelAttrExp exp;
    exp.agg_type = NOTAGG;
    exp.is_star = 0;
    exp.explist = (ExpList *)($1);
    exp.num = exp.explist->num;
    advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
  }
  | MINUS exp_list relattrexp2 {
    ExpList *explist = (ExpList *)($2);
    while(explist->left_explist) {
      explist = explist->left_explist;
    }
    explist->exp->have_negative = 1;

    RelAttrExp exp;
    exp.agg_type = NOTAGG;
    exp.is_star = 0;
    exp.explist = (ExpList *)($2);
    exp.num = exp.explist->num;
    advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
  }
    /* @author: huahui 
     * @what for: 必做题，聚合查询 
     * -----------------------------------------------------------------------------------------------------------------
     */
    | COUNT LBRACE ID RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, $3);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | COUNT LBRACE STAR RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | COUNT LBRACE ID DOT ID RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, $3, $5);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | COUNT LBRACE ID DOT STAR RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, $3, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | COUNT LBRACE number RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = $3;
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | COUNT LBRACE floatnumber RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ($3).floats;
            exp.agg_val.str = strdup(($3).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
    | MAX LBRACE ID RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, $3);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | MAX LBRACE STAR RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | MAX LBRACE ID DOT ID RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, $3, $5);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | MAX LBRACE number RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = $3;
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | MAX LBRACE floatnumber RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ($3).floats;
            exp.agg_val.str = strdup(($3).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
    | MIN LBRACE ID RBRACE relattrexp2 {  
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, $3);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | MIN LBRACE STAR RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | MIN LBRACE ID DOT ID RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, $3, $5);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | MIN LBRACE number RBRACE relattrexp2 { 
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = $3;
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | MIN LBRACE floatnumber RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ($3).floats;
            exp.agg_val.str = strdup(($3).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
    | AVG LBRACE ID RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, $3);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | AVG LBRACE STAR RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | AVG LBRACE ID DOT ID RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, $3, $5);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | AVG LBRACE number RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = $3;
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
        | AVG LBRACE floatnumber RBRACE relattrexp2 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ($3).floats;
            exp.agg_val.str = strdup(($3).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
    /* ------------------------------------------------------------------------------------------------------------
     */
    | STAR {
      RelAttrExp exp;
      exp.agg_type = NOTAGG;
      exp.is_star = 1;
      exp.relation_name = NULL;
      advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
    }
    | ID DOT STAR relattrexp2 {
      RelAttrExp exp;
      exp.agg_type = NOTAGG;
      exp.is_star = 1;
      exp.relation_name = strdup($1);
      advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
    }
    ;
    
relattrexp2:
  /* empty*/
  | COMMA relattrexp
  ;

/* exp_list是一个完整的支持加减乘除的表达式，它用加减将至少一个的exps连接起来
 */
exp_list:
  exps exp_list2 {
    ExpList *explist_left = (ExpList *)malloc(sizeof(ExpList));
    explist_left->exp = (Exp *)($1);
    explist_left->left_explist = NULL;
    explist_left->calop = STARTCALOP;
    explist_left->num = explist_left->exp->num;
    if($2){
      ExpList *explist_right = (ExpList *)($2);
      while(explist_right->left_explist) {
        explist_right->num += explist_left->num;
        explist_right = explist_right->left_explist;
      }
      explist_right->num += explist_left->num;
      explist_right->left_explist = explist_left;
      $$ = $2;
    }
    else {
      $$ = (void *)explist_left;
    }
  }
  ;
  
exp_list2:
  /* empty*/ {
    $$ = NULL;
  }
  | PLUS exp_list {
    ExpList *explist_right = (ExpList *)($2);
    while(explist_right->left_explist) {
      explist_right = explist_right->left_explist;
    }
    explist_right->calop = PLUS_OP;
    $$ = $2;
  }
  | MINUS exp_list {
    ExpList *explist_right = (ExpList *)($2);
    while(explist_right->left_explist) {
      explist_right = explist_right->left_explist;
    }
    explist_right->calop = MINUS_OP;
    $$ = $2;
  }
  ;


/* exps是一个完整的支持乘除的表达式，它用乘除将很多exp连接起来，exp可以是属性，值或者括号括进来的东西exp_list
 */
exps:
  exp exps2 {
    if($2){
      Exp *exp_left = (Exp *)($1);
      Exp *exp_right = (Exp *)($2);
      while(exp_right->left_exp) {
        exp_right->num += exp_left->num;
        exp_right = exp_right->left_exp;
      }
      exp_right->num += exp_left->num;
      exp_right->left_exp = exp_left;
      $$ = $2;
    } else {
      $$ = $1;
    }
  }
  ;

exps2:
  /* empty*/ {
    $$ = NULL;
  }
  | STAR exps {
    Exp *exp_right = (Exp *)($2);
    while(exp_right->left_exp) {
      exp_right = exp_right->left_exp;
    }
    exp_right->calop = TIME_OP;
    $$ = $2;
  }
  | DIVIDE exps {
    Exp *exp_right = (Exp *)($2);
    while(exp_right->left_exp) {
      exp_right = exp_right->left_exp;
    }
    exp_right->calop = DIVIDE_OP;
    $$ = $2;
  }
  ;

/* exp可以是属性，值或者括号括进来的东西exp_list
 */
exp:
  ID {
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 0;
    exp->have_negative = 0;
    exp->explist = NULL;
    exp->is_attr = 1;
    exp->relation_name = NULL;
    exp->attribute_name = strdup($1);
    exp->left_exp = NULL;
    exp->calop = STARTCALOP;
    exp->num = 1;
    $$ = (void *)exp;
  }
  | ID DOT ID {
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 0;
    exp->have_negative = 0;
    exp->explist = NULL;
    exp->is_attr = 1;
    exp->relation_name = strdup($1);
    exp->attribute_name = strdup($3);
    exp->left_exp = NULL;
    exp->calop = STARTCALOP;
    exp->num = 1;
    $$ = (void *)exp;
  }
  | NNNUMBER {
    value_init_integer(&CONTEXT->values[CONTEXT->value_length++], $1);
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 0;
    exp->have_negative = 0;
    exp->is_attr = 0;
    exp->explist = NULL;
    exp->relation_name = NULL;
    exp->attribute_name = NULL;
    exp->value = CONTEXT->values[CONTEXT->value_length - 1];
    exp->left_exp = NULL;
    exp->calop = STARTCALOP;
    exp->num = 1;
    $$ = (void *)exp;
  }
  | NNFLOAT {
    value_init_float2(&CONTEXT->values[CONTEXT->value_length++], ($1).floats, ($1).str);
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 0;
    exp->have_negative = 0;
    exp->is_attr = 0;
    exp->explist = NULL;
    exp->relation_name = NULL;
    exp->attribute_name = NULL;
    exp->value = CONTEXT->values[CONTEXT->value_length - 1];
    exp->left_exp = NULL;
    exp->calop = STARTCALOP;
    exp->num = 1;
    $$ = (void *)exp;
  }
  | SSS {
    $1 = substr($1,1,strlen($1)-2);
  	value_init_string(&CONTEXT->values[CONTEXT->value_length++], $1);
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 0;
    exp->have_negative = 0;
    exp->is_attr = 0;
    exp->explist = NULL;
    exp->relation_name = NULL;
    exp->attribute_name = NULL;
    exp->value = CONTEXT->values[CONTEXT->value_length - 1];
    exp->left_exp = NULL;
    exp->calop = STARTCALOP;
    exp->num = 1;
    $$ = (void *)exp;
  }
  | DATE {
    $1 = substr($1,1,strlen($1)-2);
  	value_init_date(&CONTEXT->values[CONTEXT->value_length++], $1);
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 0;
    exp->have_negative = 0;
    exp->is_attr = 0;
    exp->explist = NULL;
    exp->relation_name = NULL;
    exp->attribute_name = NULL;
    exp->value = CONTEXT->values[CONTEXT->value_length - 1];
    exp->left_exp = NULL;
    exp->calop = STARTCALOP;
    exp->num = 1;
    $$ = (void *)exp;
  }
  | NULL_A {
    value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 0;
    exp->have_negative = 0;
    exp->is_attr = 0;
    exp->explist = NULL;
    exp->relation_name = NULL;
    exp->attribute_name = NULL;
    exp->value = CONTEXT->values[CONTEXT->value_length - 1];
    exp->left_exp = NULL;
    exp->calop = STARTCALOP;
    exp->num = 1;
    $$ = (void *)exp;
  }
  | LBRACE exp_list RBRACE {
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 1;
    exp->have_negative = 0;
    exp->explist = (ExpList *)($2);
    exp->num = exp->explist->num;
    exp->left_exp = NULL;
    $$ = (void *)exp;
  }
  ;

rel_list:
    /* empty */
    | COMMA ID rel_list {	
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
        adv_selects->relations[adv_selects->relation_num++] = strdup($2);
		  }
    ;

where:
  /* empty*/
  | WHERE condition_exps {
  
  }
  ;
  
condition_exps:
  | condition_exp {
  
  }
  | condition_exp AND condition_exps {
  
  }
  ;
  
condition_exp: 
  exp_list comOp exp_list {
    ExpList *explist_left = (ExpList *)($1);
    ExpList *explist_right = (ExpList *)($3);
    ConditionExp cond_exp;
    cond_exp.left = explist_left;
    cond_exp.right = explist_right;
    cond_exp.comp = CONTEXT->comp;
    advselects_append_conditionexp(&CONTEXT->ssql->sstr.adv_selection, &cond_exp);
  }
  /* @author: huahui  @what for: null ------------------------------------------------------------------------*/
	| exp_list IS_A NULL_A {
		ExpList *explist_left = (ExpList *)($1);
		ExpList *explist_right = (ExpList *)malloc(sizeof(ExpList));
		
		explist_init_for_null(explist_right);
		
		ConditionExp cond_exp;
		cond_exp.left = explist_left;
		cond_exp.right = explist_right;
		cond_exp.comp = IS;
		advselects_append_conditionexp(&CONTEXT->ssql->sstr.adv_selection, &cond_exp);
	}
	| exp_list IS_A NOT NULL_A {
		ExpList *explist_left = (ExpList *)($1);
		ExpList *explist_right = (ExpList *)malloc(sizeof(ExpList));
		
		explist_init_for_null(explist_right);
		
		ConditionExp cond_exp;
		cond_exp.left = explist_left;
		cond_exp.right = explist_right;
		cond_exp.comp = ISNOT;
		advselects_append_conditionexp(&CONTEXT->ssql->sstr.adv_selection, &cond_exp);
	}
	/* ------------------------------------------------------------------------------------------------------------*/
    ;

join:
    INNER JOIN ID onwhere {
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
        adv_selects->relations[adv_selects->relation_num++] = strdup($3);
    }
    |INNER JOIN ID onwhere join {
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
        adv_selects->relations[adv_selects->relation_num++] = strdup($3);
    }
    ;
onwhere:
    /* empty */
    | ON condition_exps {

    }
    ;
/* >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

comOp:
  	  EQ { CONTEXT->comp = EQUAL_TO; }
    | LT { CONTEXT->comp = LESS_THAN; }
    | GT { CONTEXT->comp = GREAT_THAN; }
    | LE { CONTEXT->comp = LESS_EQUAL; }
    | GE { CONTEXT->comp = GREAT_EQUAL; }
    | NE { CONTEXT->comp = NOT_EQUAL; }
    ;

load_data:
		LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON
		{
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, $7, $4);
		}
		;
%%
//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, Query *sqls){
	ParserContext context;
	memset(&context, 0, sizeof(context));

	yyscan_t scanner;
	yylex_init_extra(&context, &scanner);
	context.ssql = sqls;
	scan_string(s, scanner);
	int result = yyparse(scanner);
	yylex_destroy(scanner);
	return result;
}
