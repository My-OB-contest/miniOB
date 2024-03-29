/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


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
  size_t condition_length[MAX_NUM];
  size_t from_length;
  size_t value_length;
  size_t value_list_length;
  size_t insert_value_length[MAX_NUM];
  Value values[MAX_NUM];
  Value insert_values[MAX_NUM][MAX_NUM];
  ConditionExp condition_exps[MAX_NUM][MAX_NUM]; // 删掉Condition，增加ConditionExp
  CompOp comp[MAX_NUM];
  size_t comp_deep;
    char id[MAX_NUM];
    DeepStack deep_stack;
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


#line 137 "yacc_sql.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "yacc_sql.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_CREATE = 4,                     /* CREATE  */
  YYSYMBOL_DROP = 5,                       /* DROP  */
  YYSYMBOL_TABLE = 6,                      /* TABLE  */
  YYSYMBOL_TABLES = 7,                     /* TABLES  */
  YYSYMBOL_INDEX = 8,                      /* INDEX  */
  YYSYMBOL_SELECT = 9,                     /* SELECT  */
  YYSYMBOL_DESC = 10,                      /* DESC  */
  YYSYMBOL_SHOW = 11,                      /* SHOW  */
  YYSYMBOL_SYNC = 12,                      /* SYNC  */
  YYSYMBOL_INSERT = 13,                    /* INSERT  */
  YYSYMBOL_DELETE = 14,                    /* DELETE  */
  YYSYMBOL_UPDATE = 15,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 16,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 17,                    /* RBRACE  */
  YYSYMBOL_COMMA = 18,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 19,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 20,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 21,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 22,                     /* INT_T  */
  YYSYMBOL_STRING_T = 23,                  /* STRING_T  */
  YYSYMBOL_FLOAT_T = 24,                   /* FLOAT_T  */
  YYSYMBOL_DATE_T = 25,                    /* DATE_T  */
  YYSYMBOL_TEXT_T = 26,                    /* TEXT_T  */
  YYSYMBOL_HELP = 27,                      /* HELP  */
  YYSYMBOL_EXIT = 28,                      /* EXIT  */
  YYSYMBOL_DOT = 29,                       /* DOT  */
  YYSYMBOL_INTO = 30,                      /* INTO  */
  YYSYMBOL_VALUES = 31,                    /* VALUES  */
  YYSYMBOL_FROM = 32,                      /* FROM  */
  YYSYMBOL_WHERE = 33,                     /* WHERE  */
  YYSYMBOL_AND = 34,                       /* AND  */
  YYSYMBOL_SET = 35,                       /* SET  */
  YYSYMBOL_ON = 36,                        /* ON  */
  YYSYMBOL_LOAD = 37,                      /* LOAD  */
  YYSYMBOL_DATA = 38,                      /* DATA  */
  YYSYMBOL_INFILE = 39,                    /* INFILE  */
  YYSYMBOL_EQ = 40,                        /* EQ  */
  YYSYMBOL_LT = 41,                        /* LT  */
  YYSYMBOL_GT = 42,                        /* GT  */
  YYSYMBOL_LE = 43,                        /* LE  */
  YYSYMBOL_GE = 44,                        /* GE  */
  YYSYMBOL_NE = 45,                        /* NE  */
  YYSYMBOL_COUNT = 46,                     /* COUNT  */
  YYSYMBOL_MAX = 47,                       /* MAX  */
  YYSYMBOL_MIN = 48,                       /* MIN  */
  YYSYMBOL_AVG = 49,                       /* AVG  */
  YYSYMBOL_INNER = 50,                     /* INNER  */
  YYSYMBOL_JOIN = 51,                      /* JOIN  */
  YYSYMBOL_NOT = 52,                       /* NOT  */
  YYSYMBOL_NULL_A = 53,                    /* NULL_A  */
  YYSYMBOL_NULLABLE = 54,                  /* NULLABLE  */
  YYSYMBOL_IS_A = 55,                      /* IS_A  */
  YYSYMBOL_UNIQUE = 56,                    /* UNIQUE  */
  YYSYMBOL_IN = 57,                        /* IN  */
  YYSYMBOL_PLUS = 58,                      /* PLUS  */
  YYSYMBOL_MINUS = 59,                     /* MINUS  */
  YYSYMBOL_DIVIDE = 60,                    /* DIVIDE  */
  YYSYMBOL_ORDER = 61,                     /* ORDER  */
  YYSYMBOL_BY = 62,                        /* BY  */
  YYSYMBOL_ASC = 63,                       /* ASC  */
  YYSYMBOL_GROUP = 64,                     /* GROUP  */
  YYSYMBOL_NNNUMBER = 65,                  /* NNNUMBER  */
  YYSYMBOL_NNFLOAT = 66,                   /* NNFLOAT  */
  YYSYMBOL_ID = 67,                        /* ID  */
  YYSYMBOL_DATE = 68,                      /* DATE  */
  YYSYMBOL_PATH = 69,                      /* PATH  */
  YYSYMBOL_SSS = 70,                       /* SSS  */
  YYSYMBOL_STAR = 71,                      /* STAR  */
  YYSYMBOL_STRING_V = 72,                  /* STRING_V  */
  YYSYMBOL_YYACCEPT = 73,                  /* $accept  */
  YYSYMBOL_commands = 74,                  /* commands  */
  YYSYMBOL_command = 75,                   /* command  */
  YYSYMBOL_exit = 76,                      /* exit  */
  YYSYMBOL_help = 77,                      /* help  */
  YYSYMBOL_sync = 78,                      /* sync  */
  YYSYMBOL_begin = 79,                     /* begin  */
  YYSYMBOL_commit = 80,                    /* commit  */
  YYSYMBOL_rollback = 81,                  /* rollback  */
  YYSYMBOL_drop_table = 82,                /* drop_table  */
  YYSYMBOL_show_tables = 83,               /* show_tables  */
  YYSYMBOL_desc_table = 84,                /* desc_table  */
  YYSYMBOL_create_index = 85,              /* create_index  */
  YYSYMBOL_field_name_list = 86,           /* field_name_list  */
  YYSYMBOL_drop_index = 87,                /* drop_index  */
  YYSYMBOL_create_table = 88,              /* create_table  */
  YYSYMBOL_attr_def_list = 89,             /* attr_def_list  */
  YYSYMBOL_attr_def = 90,                  /* attr_def  */
  YYSYMBOL_number = 91,                    /* number  */
  YYSYMBOL_floatnumber = 92,               /* floatnumber  */
  YYSYMBOL_text = 93,                      /* text  */
  YYSYMBOL_type = 94,                      /* type  */
  YYSYMBOL_ID_get = 95,                    /* ID_get  */
  YYSYMBOL_insert = 96,                    /* insert  */
  YYSYMBOL_values_lists = 97,              /* values_lists  */
  YYSYMBOL_value_tuple = 98,               /* value_tuple  */
  YYSYMBOL_value_list = 99,                /* value_list  */
  YYSYMBOL_insert_value = 100,             /* insert_value  */
  YYSYMBOL_value = 101,                    /* value  */
  YYSYMBOL_delete = 102,                   /* delete  */
  YYSYMBOL_delete_handle = 103,            /* delete_handle  */
  YYSYMBOL_update = 104,                   /* update  */
  YYSYMBOL_update_handle = 105,            /* update_handle  */
  YYSYMBOL_select_clasue = 106,            /* select_clasue  */
  YYSYMBOL_select = 107,                   /* select  */
  YYSYMBOL_select_handle = 108,            /* select_handle  */
  YYSYMBOL_relattrexp = 109,               /* relattrexp  */
  YYSYMBOL_relattrexp2 = 110,              /* relattrexp2  */
  YYSYMBOL_exp_list = 111,                 /* exp_list  */
  YYSYMBOL_exp_list2 = 112,                /* exp_list2  */
  YYSYMBOL_exp_list3 = 113,                /* exp_list3  */
  YYSYMBOL_exps = 114,                     /* exps  */
  YYSYMBOL_exps2 = 115,                    /* exps2  */
  YYSYMBOL_exp = 116,                      /* exp  */
  YYSYMBOL_rel_list = 117,                 /* rel_list  */
  YYSYMBOL_where = 118,                    /* where  */
  YYSYMBOL_condition_exps = 119,           /* condition_exps  */
  YYSYMBOL_condition_exp = 120,            /* condition_exp  */
  YYSYMBOL_join = 121,                     /* join  */
  YYSYMBOL_onwhere = 122,                  /* onwhere  */
  YYSYMBOL_order = 123,                    /* order  */
  YYSYMBOL_order2 = 124,                   /* order2  */
  YYSYMBOL_group = 125,                    /* group  */
  YYSYMBOL_group2 = 126,                   /* group2  */
  YYSYMBOL_comOp = 127,                    /* comOp  */
  YYSYMBOL_load_data = 128                 /* load_data  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int16 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   365

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  73
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  56
/* YYNRULES -- Number of rules.  */
#define YYNRULES  171
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  356

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   327


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   192,   192,   194,   198,   199,   200,   201,   202,   203,
     204,   205,   206,   207,   208,   209,   210,   211,   212,   213,
     214,   218,   223,   228,   234,   240,   246,   252,   258,   264,
     271,   276,   284,   288,   295,   302,   311,   313,   317,   328,
     339,   353,   365,   377,   389,   401,   416,   417,   423,   424,
     437,   441,   442,   443,   444,   447,   457,   478,   481,   486,
     488,   493,   495,   502,   505,   508,   512,   517,   526,   529,
     533,   537,   542,   550,   560,   567,   578,   584,   589,   600,
     612,   623,   635,   646,   661,   671,   679,   697,   702,   707,
     712,   717,   725,   734,   739,   744,   749,   757,   766,   771,
     776,   781,   789,   798,   803,   808,   813,   821,   832,   839,
     848,   850,   857,   880,   883,   891,   902,   905,   919,   937,
     940,   948,   961,   975,   989,  1005,  1021,  1038,  1055,  1071,
    1081,  1097,  1099,  1105,  1107,  1112,  1113,  1116,  1123,  1134,
    1145,  1161,  1165,  1170,  1172,  1179,  1187,  1195,  1203,  1211,
    1219,  1229,  1231,  1239,  1247,  1255,  1263,  1271,  1283,  1290,
    1298,  1300,  1307,  1319,  1320,  1321,  1322,  1323,  1324,  1325,
    1328,  1332
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "CREATE",
  "DROP", "TABLE", "TABLES", "INDEX", "SELECT", "DESC", "SHOW", "SYNC",
  "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE", "COMMA", "TRX_BEGIN",
  "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T", "FLOAT_T", "DATE_T",
  "TEXT_T", "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM", "WHERE",
  "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "EQ", "LT", "GT", "LE",
  "GE", "NE", "COUNT", "MAX", "MIN", "AVG", "INNER", "JOIN", "NOT",
  "NULL_A", "NULLABLE", "IS_A", "UNIQUE", "IN", "PLUS", "MINUS", "DIVIDE",
  "ORDER", "BY", "ASC", "GROUP", "NNNUMBER", "NNFLOAT", "ID", "DATE",
  "PATH", "SSS", "STAR", "STRING_V", "$accept", "commands", "command",
  "exit", "help", "sync", "begin", "commit", "rollback", "drop_table",
  "show_tables", "desc_table", "create_index", "field_name_list",
  "drop_index", "create_table", "attr_def_list", "attr_def", "number",
  "floatnumber", "text", "type", "ID_get", "insert", "values_lists",
  "value_tuple", "value_list", "insert_value", "value", "delete",
  "delete_handle", "update", "update_handle", "select_clasue", "select",
  "select_handle", "relattrexp", "relattrexp2", "exp_list", "exp_list2",
  "exp_list3", "exps", "exps2", "exp", "rel_list", "where",
  "condition_exps", "condition_exp", "join", "onwhere", "order", "order2",
  "group", "group2", "comOp", "load_data", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-302)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -302,   180,  -302,    36,   141,  -302,   -48,    22,    35,    61,
    -302,  -302,    63,   107,   122,   125,   133,   108,  -302,  -302,
    -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,
    -302,  -302,  -302,    70,  -302,    88,  -302,   166,    73,  -302,
      96,   116,   188,   121,   131,   199,   201,  -302,   138,  -302,
    -302,  -302,  -302,  -302,   167,   149,   176,  -302,    31,   202,
     204,   208,   215,  -302,   -12,  -302,  -302,   205,  -302,  -302,
    -302,   210,   225,    92,   -26,   217,   214,   184,   249,   250,
    -302,  -302,   224,   186,   221,   190,   -12,   229,   242,  -302,
     243,    99,   156,   170,   173,   225,    64,   194,    73,  -302,
     -12,   -12,  -302,   -12,   -12,  -302,   195,   196,   228,  -302,
    -302,   251,   235,    41,   263,   230,  -302,   206,  -302,  -302,
     106,  -302,  -302,    -8,   252,   254,   255,    -2,   257,   258,
     259,     1,   260,   261,   262,   101,   264,   265,   266,  -302,
    -302,   225,    -1,  -302,  -302,  -302,  -302,  -302,  -302,   267,
     223,   268,   213,   144,   269,   280,    72,  -302,   234,  -302,
     160,  -302,  -302,   225,    66,   225,   225,   225,   225,   222,
     225,   225,   225,   225,   226,   225,   225,   225,   225,   227,
     225,   225,   225,  -302,   231,   237,   221,   221,   195,   273,
    -302,  -302,  -302,  -302,  -302,   239,     7,   232,   276,  -302,
    -302,  -302,  -302,  -302,   277,   251,   293,   233,  -302,  -302,
    -302,  -302,  -302,  -302,   240,   129,  -302,    41,    41,  -302,
    -302,  -302,  -302,  -302,   221,  -302,   284,   285,  -302,  -302,
    -302,  -302,   286,  -302,  -302,  -302,  -302,   287,  -302,  -302,
    -302,  -302,   288,  -302,  -302,  -302,   289,   241,   -25,   -25,
     267,   303,   256,   -22,   270,  -302,   292,   294,   232,   144,
     295,   269,  -302,   310,  -302,   271,  -302,  -302,  -302,   311,
     225,   225,   225,   225,   225,  -302,   279,   272,   274,  -302,
    -302,  -302,  -302,  -302,  -302,  -302,   253,   299,  -302,   232,
     314,   302,   277,  -302,  -302,  -302,  -302,  -302,  -302,  -302,
    -302,  -302,  -302,    41,   275,   278,   281,   102,  -302,  -302,
     317,  -302,  -302,  -302,     4,    75,   282,  -302,  -302,   304,
     283,   290,   304,  -302,   291,   296,  -302,  -302,  -302,     6,
      42,  -302,   105,   308,   304,   297,   304,  -302,   304,   304,
    -302,   298,  -302,  -302,  -302,    85,  -302,  -302,  -302,   308,
     304,   304,  -302,  -302,  -302,  -302
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,    84,     0,     0,     0,     0,
      74,    76,     0,     0,     0,     0,     0,     0,     3,    20,
      19,    14,    15,    16,    17,     9,    10,    11,    12,    13,
       8,     5,     7,     0,     6,     0,     4,     0,     0,    18,
       0,     0,     0,     0,     0,     0,     0,    23,     0,    24,
      25,    26,    22,    21,     0,     0,     0,    77,     0,     0,
       0,     0,     0,   128,     0,   124,   125,   122,   127,   126,
     108,     0,   110,   113,   119,     0,     0,     0,     0,     0,
      29,    28,     0,     0,   133,     0,     0,   122,     0,   116,
       0,     0,     0,     0,     0,   110,     0,     0,     0,    85,
       0,     0,   112,     0,     0,   118,     0,     0,     0,    27,
      34,    59,     0,   135,     0,     0,   117,     0,   130,   129,
       0,    46,    48,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    86,
     123,   110,   131,   111,   114,   115,   121,   120,    55,    36,
       0,     0,     0,     0,    57,     0,     0,   134,   136,    73,
       0,    47,    49,   110,     0,   110,   110,   110,   110,     0,
     110,   110,   110,   110,     0,   110,   110,   110,   110,     0,
     110,   110,   110,   109,     0,     0,   133,   133,     0,     0,
      51,    52,    53,    54,    50,    40,    39,     0,     0,    67,
      66,    65,    63,    64,    61,    59,     0,     0,   163,   164,
     165,   166,   167,   168,     0,     0,   169,     0,   135,    72,
      71,    70,    68,    69,   133,    87,     0,     0,    88,    91,
      92,    93,     0,    94,    96,    97,    98,     0,    99,   101,
     102,   103,     0,   104,   106,   107,   131,     0,    78,    79,
      36,     0,     0,     0,     0,    45,    32,     0,     0,     0,
       0,    57,    56,     0,   170,     0,   139,   138,   137,     0,
     110,   110,   110,   110,   110,   132,   143,     0,     0,    80,
      82,    81,    83,    37,    35,    44,     0,     0,    43,     0,
       0,     0,    61,    60,    58,   171,   140,    75,    89,    90,
      95,   100,   105,   135,   141,     0,     0,    38,    33,    30,
       0,    62,   144,   142,   151,   160,     0,    42,    31,   151,
       0,     0,   151,   145,     0,     0,   158,    41,   149,   151,
     151,   147,   160,   160,   151,     0,   151,   152,   151,   151,
     146,     0,   161,   159,   156,   151,   154,   150,   148,   160,
     151,   151,   153,   162,   157,   155
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,  -302,
    -302,  -302,  -302,  -238,  -302,  -302,    71,   139,   -86,   -81,
    -302,  -302,  -302,  -302,    67,   124,    38,    74,  -302,  -302,
    -302,  -302,  -302,  -302,   301,  -302,   244,   -95,   -36,  -302,
     -57,    83,  -302,  -302,    86,  -161,  -215,  -302,    27,  -302,
      89,  -177,    90,  -301,  -302,  -302
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,   257,    29,    30,   189,   149,   202,   203,
     195,   196,   150,    31,   206,   154,   260,   204,   224,    32,
      33,    34,    35,    36,    37,    38,    71,    99,    89,   102,
     156,    73,   105,    74,   186,   114,   157,   158,   187,   304,
     279,   323,   280,   326,   217,    39
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     139,    90,    72,   268,    58,   125,   129,   133,   137,   163,
     126,   130,   134,   138,   319,   168,   334,   184,   173,    45,
     291,   164,   320,   253,   320,   248,   249,   169,    95,    46,
     174,   342,   343,   321,   103,   335,   277,   286,    47,   278,
       5,    63,    40,   121,    41,   104,   183,    58,   353,   185,
     116,   308,   338,    65,    66,    87,    68,    58,    69,   254,
     320,   255,    72,   269,   144,   145,    49,   322,   225,   336,
     228,   229,   230,   231,   222,   233,   234,   235,   236,   223,
     238,   239,   240,   241,    63,   243,   244,   245,   312,    58,
      86,    48,    42,   324,    63,   350,    65,    66,    87,    68,
      86,    69,    55,   320,   325,   339,    65,    66,    87,    68,
      50,    69,   208,   209,   210,   211,   212,   213,   178,    59,
      60,    61,    62,   324,   214,    51,    63,   215,    52,   216,
     179,   140,    64,   226,   341,   141,    53,   227,    65,    66,
      67,    68,   328,    69,    70,   331,    54,    43,   351,    44,
     100,   101,   337,   340,   316,    56,   317,   344,   120,   346,
     267,   347,   348,    75,   121,   122,   123,   287,   352,    57,
     124,   161,   162,   354,   355,   298,   299,   300,   301,   302,
       2,   265,   266,    76,     3,     4,   146,   147,    78,     5,
       6,     7,     8,     9,    10,    11,    77,   199,    79,    12,
      13,    14,    80,   120,    81,    82,    83,    15,    16,   121,
     122,    85,   200,   219,   201,   120,    84,    17,    91,   120,
      92,   121,   122,   127,    93,   121,   122,   128,   220,   120,
     221,    94,   120,   106,    96,   121,   122,   131,   121,   122,
     135,   132,    97,    98,   136,   190,   191,   192,   193,   194,
     107,   108,   109,   110,   113,   111,   112,   115,   117,   118,
     119,   142,   148,   151,   152,   155,   159,   153,   218,   165,
     160,   166,   167,   140,   170,   171,   172,   175,   176,   177,
     198,   180,   181,   182,   197,   188,   207,   205,   247,   232,
     251,   252,   258,   237,   242,   259,   262,   264,   246,   256,
     263,   270,   271,   272,   273,   274,   284,   184,   276,   285,
     289,   290,   293,   295,   297,   303,   307,   309,   161,   310,
     318,   283,   320,   288,   296,   185,   324,   250,   294,   261,
     311,   313,   275,   292,   305,   327,   306,     0,   281,   282,
       0,     0,   143,     0,     0,   314,     0,     0,   315,     0,
     329,     0,     0,     0,     0,     0,     0,   330,   332,    88,
       0,     0,     0,   333,   345,   349
};

static const yytype_int16 yycheck[] =
{
      95,    58,    38,   218,    16,    91,    92,    93,    94,    17,
      91,    92,    93,    94,    10,    17,    10,    18,    17,    67,
     258,    29,    18,    16,    18,   186,   187,    29,    64,     7,
      29,   332,   333,    29,    60,    29,    61,    59,     3,    64,
       9,    53,     6,    65,     8,    71,   141,    16,   349,    50,
      86,   289,    10,    65,    66,    67,    68,    16,    70,    52,
      18,    54,    98,   224,   100,   101,     3,    63,   163,    63,
     165,   166,   167,   168,   160,   170,   171,   172,   173,   160,
     175,   176,   177,   178,    53,   180,   181,   182,   303,    16,
      59,    30,    56,    18,    53,    10,    65,    66,    67,    68,
      59,    70,    32,    18,    29,    63,    65,    66,    67,    68,
       3,    70,    40,    41,    42,    43,    44,    45,    17,    46,
      47,    48,    49,    18,    52,     3,    53,    55,     3,    57,
      29,    67,    59,    67,    29,    71,     3,    71,    65,    66,
      67,    68,   319,    70,    71,   322,    38,     6,    63,     8,
      58,    59,   329,   330,    52,    67,    54,   334,    59,   336,
     217,   338,   339,    67,    65,    66,    67,   253,   345,     3,
      71,    65,    66,   350,   351,   270,   271,   272,   273,   274,
       0,    52,    53,    67,     4,     5,   103,   104,    67,     9,
      10,    11,    12,    13,    14,    15,     8,    53,    67,    19,
      20,    21,     3,    59,     3,    67,    39,    27,    28,    65,
      66,    35,    68,    53,    70,    59,    67,    37,    16,    59,
      16,    65,    66,    67,    16,    65,    66,    71,    68,    59,
      70,    16,    59,    16,    29,    65,    66,    67,    65,    66,
      67,    71,    32,    18,    71,    22,    23,    24,    25,    26,
      36,    67,     3,     3,    33,    31,    70,    67,    29,    17,
      17,    67,    67,    67,    36,    30,     3,    16,    34,    17,
      40,    17,    17,    67,    17,    17,    17,    17,    17,    17,
      67,    17,    17,    17,    16,    18,     6,    18,    51,    67,
      17,    52,    16,    67,    67,    18,     3,    57,    67,    67,
      67,    17,    17,    17,    17,    17,     3,    18,    67,    53,
      18,    17,    17,     3,     3,    36,    17,     3,    65,    17,
       3,   250,    18,    53,    53,    50,    18,   188,   261,   205,
     292,   304,   246,   259,    62,    53,    62,    -1,   249,   249,
      -1,    -1,    98,    -1,    -1,    67,    -1,    -1,    67,    -1,
      67,    -1,    -1,    -1,    -1,    -1,    -1,    67,    67,    58,
      -1,    -1,    -1,    67,    67,    67
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    74,     0,     4,     5,     9,    10,    11,    12,    13,
      14,    15,    19,    20,    21,    27,    28,    37,    75,    76,
      77,    78,    79,    80,    81,    82,    83,    84,    85,    87,
      88,    96,   102,   103,   104,   105,   106,   107,   108,   128,
       6,     8,    56,     6,     8,    67,     7,     3,    30,     3,
       3,     3,     3,     3,    38,    32,    67,     3,    16,    46,
      47,    48,    49,    53,    59,    65,    66,    67,    68,    70,
      71,   109,   111,   114,   116,    67,    67,     8,    67,    67,
       3,     3,    67,    39,    67,    35,    59,    67,   107,   111,
     113,    16,    16,    16,    16,   111,    29,    32,    18,   110,
      58,    59,   112,    60,    71,   115,    16,    36,    67,     3,
       3,    31,    70,    33,   118,    67,   111,    29,    17,    17,
      59,    65,    66,    67,    71,    91,    92,    67,    71,    91,
      92,    67,    71,    91,    92,    67,    71,    91,    92,   110,
      67,    71,    67,   109,   111,   111,   114,   114,    67,    90,
      95,    67,    36,    16,    98,    30,   113,   119,   120,     3,
      40,    65,    66,    17,    29,    17,    17,    17,    17,    29,
      17,    17,    17,    17,    29,    17,    17,    17,    17,    29,
      17,    17,    17,   110,    18,    50,   117,   121,    18,    89,
      22,    23,    24,    25,    26,    93,    94,    16,    67,    53,
      68,    70,    91,    92,   100,    18,    97,     6,    40,    41,
      42,    43,    44,    45,    52,    55,    57,   127,    34,    53,
      68,    70,    91,    92,   101,   110,    67,    71,   110,   110,
     110,   110,    67,   110,   110,   110,   110,    67,   110,   110,
     110,   110,    67,   110,   110,   110,    67,    51,   118,   118,
      90,    17,    52,    16,    52,    54,    67,    86,    16,    18,
      99,    98,     3,    67,    57,    52,    53,   113,   119,   118,
      17,    17,    17,    17,    17,   117,    67,    61,    64,   123,
     125,   123,   125,    89,     3,    53,    59,    91,    53,    18,
      17,    86,   100,    17,    97,     3,    53,     3,   110,   110,
     110,   110,   110,    36,   122,    62,    62,    17,    86,     3,
      17,    99,   119,   121,    67,    67,    52,    54,     3,    10,
      18,    29,    63,   124,    18,    29,   126,    53,   124,    67,
      67,   124,    67,    67,    10,    29,    63,   124,    10,    63,
     124,    29,   126,   126,   124,    67,   124,   124,   124,    67,
      10,    63,   124,   126,   124,   124
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    73,    74,    74,    75,    75,    75,    75,    75,    75,
      75,    75,    75,    75,    75,    75,    75,    75,    75,    75,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    85,    86,    86,    87,    88,    89,    89,    90,    90,
      90,    90,    90,    90,    90,    90,    91,    91,    92,    92,
      93,    94,    94,    94,    94,    95,    96,    97,    97,    98,
      98,    99,    99,   100,   100,   100,   100,   100,   101,   101,
     101,   101,   101,   102,   103,   104,   105,   106,   107,   107,
     107,   107,   107,   107,   108,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     109,   109,   109,   109,   109,   109,   109,   109,   109,   109,
     110,   110,   111,   112,   112,   112,   113,   113,   114,   115,
     115,   115,   116,   116,   116,   116,   116,   116,   116,   116,
     116,   117,   117,   118,   118,   119,   119,   119,   120,   120,
     120,   121,   121,   122,   122,   123,   123,   123,   123,   123,
     123,   124,   124,   124,   124,   124,   124,   124,   125,   125,
     126,   126,   126,   127,   127,   127,   127,   127,   127,   127,
     127,   128
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
       9,    10,     1,     3,     4,     8,     0,     3,     5,     2,
       2,     7,     6,     4,     4,     3,     1,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     7,     0,     3,     0,
       4,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     1,     8,     1,     2,     6,     6,
       7,     7,     7,     7,     1,     2,     3,     5,     5,     7,
       7,     5,     5,     5,     5,     7,     5,     5,     5,     5,
       7,     5,     5,     5,     5,     7,     5,     5,     1,     4,
       0,     2,     2,     0,     2,     2,     1,     2,     2,     0,
       2,     2,     1,     3,     1,     1,     1,     1,     1,     3,
       3,     0,     3,     0,     2,     0,     1,     3,     3,     3,
       4,     4,     5,     0,     2,     4,     6,     5,     7,     5,
       7,     0,     3,     5,     4,     6,     4,     6,     4,     6,
       0,     3,     5,     1,     1,     1,     1,     1,     1,     1,
       2,     8
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (scanner, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void *scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (scanner);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, void *scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep, scanner);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule, void *scanner)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)], scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, scanner); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, void *scanner)
{
  YY_USE (yyvaluep);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (void *scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex (&yylval, scanner);
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 21: /* exit: EXIT SEMICOLON  */
#line 218 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1511 "yacc_sql.tab.c"
    break;

  case 22: /* help: HELP SEMICOLON  */
#line 223 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1519 "yacc_sql.tab.c"
    break;

  case 23: /* sync: SYNC SEMICOLON  */
#line 228 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1527 "yacc_sql.tab.c"
    break;

  case 24: /* begin: TRX_BEGIN SEMICOLON  */
#line 234 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1535 "yacc_sql.tab.c"
    break;

  case 25: /* commit: TRX_COMMIT SEMICOLON  */
#line 240 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1543 "yacc_sql.tab.c"
    break;

  case 26: /* rollback: TRX_ROLLBACK SEMICOLON  */
#line 246 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1551 "yacc_sql.tab.c"
    break;

  case 27: /* drop_table: DROP TABLE ID SEMICOLON  */
#line 252 "yacc_sql.y"
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1560 "yacc_sql.tab.c"
    break;

  case 28: /* show_tables: SHOW TABLES SEMICOLON  */
#line 258 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1568 "yacc_sql.tab.c"
    break;

  case 29: /* desc_table: DESC ID SEMICOLON  */
#line 264 "yacc_sql.y"
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1577 "yacc_sql.tab.c"
    break;

  case 30: /* create_index: CREATE INDEX ID ON ID LBRACE field_name_list RBRACE SEMICOLON  */
#line 272 "yacc_sql.y"
                {
 			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string) ,0);
 		}
#line 1586 "yacc_sql.tab.c"
    break;

  case 31: /* create_index: CREATE UNIQUE INDEX ID ON ID LBRACE field_name_list RBRACE SEMICOLON  */
#line 277 "yacc_sql.y"
            {
 	        CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_unique_index";
            create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string) ,1);
 	    }
#line 1595 "yacc_sql.tab.c"
    break;

  case 32: /* field_name_list: ID  */
#line 285 "yacc_sql.y"
        {
            create_index_append_attribute(&CONTEXT->ssql->sstr.create_index,(yyvsp[0].string));
        }
#line 1603 "yacc_sql.tab.c"
    break;

  case 33: /* field_name_list: ID COMMA field_name_list  */
#line 289 "yacc_sql.y"
        {
            create_index_append_attribute(&CONTEXT->ssql->sstr.create_index,(yyvsp[-2].string));
        }
#line 1611 "yacc_sql.tab.c"
    break;

  case 34: /* drop_index: DROP INDEX ID SEMICOLON  */
#line 296 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1620 "yacc_sql.tab.c"
    break;

  case 35: /* create_table: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON  */
#line 303 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
#line 1632 "yacc_sql.tab.c"
    break;

  case 37: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 313 "yacc_sql.y"
                                   {    }
#line 1638 "yacc_sql.tab.c"
    break;

  case 38: /* attr_def: ID_get type LBRACE number RBRACE  */
#line 318 "yacc_sql.y"
                {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-3].number), (yyvsp[-1].number));
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name =(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type = $2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length = $4;
			CONTEXT->value_length++;
		}
#line 1653 "yacc_sql.tab.c"
    break;

  case 39: /* attr_def: ID_get type  */
#line 329 "yacc_sql.y"
                {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[0].number), 4);
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name=(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type=$2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length=4; // default attribute length
			CONTEXT->value_length++;
		}
#line 1668 "yacc_sql.tab.c"
    break;

  case 40: /* attr_def: ID_get text  */
#line 340 "yacc_sql.y"
                {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[0].number), 16);
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name=(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type=$2;
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length=4; // default attribute length
			CONTEXT->value_length++;
		}
#line 1683 "yacc_sql.tab.c"
    break;

  case 41: /* attr_def: ID_get type LBRACE number RBRACE NOT NULL_A  */
#line 354 "yacc_sql.y"
            {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-5].number), (yyvsp[-3].number));
			attribute.nullable = 0;
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name =(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type = $2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length = $4;
			CONTEXT->value_length++;
		}
#line 1699 "yacc_sql.tab.c"
    break;

  case 42: /* attr_def: ID_get type LBRACE number RBRACE NULLABLE  */
#line 366 "yacc_sql.y"
            {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-4].number), (yyvsp[-2].number));
			attribute.nullable = 1;
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name =(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type = $2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length = $4;
			CONTEXT->value_length++;
		}
#line 1715 "yacc_sql.tab.c"
    break;

  case 43: /* attr_def: ID_get type NOT NULL_A  */
#line 378 "yacc_sql.y"
        {
        	AttrInfo attribute;
        	attr_info_init(&attribute, CONTEXT->id, (yyvsp[-2].number), 4);
        	attribute.nullable = 0;
        	create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
        	// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name=(char*)malloc(sizeof(char));
        	// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id);
        	// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type=$2;
        	// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length=4; // default attribute length
        	CONTEXT->value_length++;
        }
#line 1731 "yacc_sql.tab.c"
    break;

  case 44: /* attr_def: ID_get text NOT NULL_A  */
#line 390 "yacc_sql.y"
            {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-2].number), 16);
			attribute.nullable = 0;
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name=(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type=$2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length=4; // default attribute length
			CONTEXT->value_length++;
		}
#line 1747 "yacc_sql.tab.c"
    break;

  case 45: /* attr_def: ID_get type NULLABLE  */
#line 402 "yacc_sql.y"
            {
			AttrInfo attribute;
			attr_info_init(&attribute, CONTEXT->id, (yyvsp[-1].number), 4);
			attribute.nullable = 1;
			create_table_append_attribute(&CONTEXT->ssql->sstr.create_table, &attribute);
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name=(char*)malloc(sizeof(char));
			// strcpy(CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].name, CONTEXT->id); 
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].type=$2;  
			// CONTEXT->ssql->sstr.create_table.attributes[CONTEXT->value_length].length=4; // default attribute length
			CONTEXT->value_length++;
		}
#line 1763 "yacc_sql.tab.c"
    break;

  case 46: /* number: NNNUMBER  */
#line 416 "yacc_sql.y"
             {(yyval.number) = (yyvsp[0].number);}
#line 1769 "yacc_sql.tab.c"
    break;

  case 47: /* number: MINUS NNNUMBER  */
#line 417 "yacc_sql.y"
                     {
      (yyval.number) = -((yyvsp[0].number)); 
    }
#line 1777 "yacc_sql.tab.c"
    break;

  case 48: /* floatnumber: NNFLOAT  */
#line 423 "yacc_sql.y"
          {(yyval.floatsAndStr) = (yyvsp[0].floatsAndStr); }
#line 1783 "yacc_sql.tab.c"
    break;

  case 49: /* floatnumber: MINUS NNFLOAT  */
#line 424 "yacc_sql.y"
                  { 
    (yyval.floatsAndStr) = (yyvsp[0].floatsAndStr);
    (yyval.floatsAndStr).floats = -((yyval.floatsAndStr).floats);
    char *s = (yyval.floatsAndStr).str;
    char *tmp = (char *)malloc(strlen(s)+5);
    tmp[0] = '-';
    strcpy(tmp+1, (yyval.floatsAndStr).str);
    free((yyval.floatsAndStr).str);
    (yyval.floatsAndStr).str = strdup(tmp);
  }
#line 1798 "yacc_sql.tab.c"
    break;

  case 50: /* text: TEXT_T  */
#line 437 "yacc_sql.y"
           { (yyval.number)=TEXT; }
#line 1804 "yacc_sql.tab.c"
    break;

  case 51: /* type: INT_T  */
#line 441 "yacc_sql.y"
              { (yyval.number)=INTS; }
#line 1810 "yacc_sql.tab.c"
    break;

  case 52: /* type: STRING_T  */
#line 442 "yacc_sql.y"
                  { (yyval.number)=CHARS; }
#line 1816 "yacc_sql.tab.c"
    break;

  case 53: /* type: FLOAT_T  */
#line 443 "yacc_sql.y"
                 { (yyval.number)=FLOATS; }
#line 1822 "yacc_sql.tab.c"
    break;

  case 54: /* type: DATE_T  */
#line 444 "yacc_sql.y"
                    { (yyval.number)=DATES; }
#line 1828 "yacc_sql.tab.c"
    break;

  case 55: /* ID_get: ID  */
#line 448 "yacc_sql.y"
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1837 "yacc_sql.tab.c"
    break;

  case 56: /* insert: INSERT INTO ID VALUES value_tuple values_lists SEMICOLON  */
#line 458 "yacc_sql.y"
                {
			// CONTEXT->values[CONTEXT->value_length++] = *$6;

			CONTEXT->ssql->flag=SCF_INSERT;//"insert";
			// CONTEXT->ssql->sstr.insertion.relation_name = $3;
			// CONTEXT->ssql->sstr.insertion.value_num = CONTEXT->value_length;
			// for(i = 0; i < CONTEXT->value_length; i++){
			// 	CONTEXT->ssql->sstr.insertion.values[i] = CONTEXT->values[i];
      // }
            //CONTEXT->value_list_length++;
			inserts_init(&CONTEXT->ssql->sstr.insertion, (yyvsp[-4].string), CONTEXT->insert_values, CONTEXT->insert_value_length, CONTEXT->value_list_length);

      //临时变量清零
      for(int i=0; i<CONTEXT->value_list_length; i++){
        CONTEXT->insert_value_length[i] = 0;
      }
      CONTEXT->value_list_length = 0;
    }
#line 1860 "yacc_sql.tab.c"
    break;

  case 57: /* values_lists: %empty  */
#line 478 "yacc_sql.y"
               {
        //CONTEXT->value_list_length++;
    }
#line 1868 "yacc_sql.tab.c"
    break;

  case 58: /* values_lists: COMMA value_tuple values_lists  */
#line 481 "yacc_sql.y"
                                    {
  	     //CONTEXT->value_list_length++;
	  }
#line 1876 "yacc_sql.tab.c"
    break;

  case 60: /* value_tuple: LBRACE insert_value value_list RBRACE  */
#line 488 "yacc_sql.y"
                                             {
        CONTEXT->value_list_length++;
	  }
#line 1884 "yacc_sql.tab.c"
    break;

  case 62: /* value_list: COMMA insert_value value_list  */
#line 495 "yacc_sql.y"
                                   {
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1892 "yacc_sql.tab.c"
    break;

  case 63: /* insert_value: number  */
#line 502 "yacc_sql.y"
           {
  		value_init_integer(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], (yyvsp[0].number));
		}
#line 1900 "yacc_sql.tab.c"
    break;

  case 64: /* insert_value: floatnumber  */
#line 505 "yacc_sql.y"
                 {
  		value_init_float(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], ((yyvsp[0].floatsAndStr)).floats);
		}
#line 1908 "yacc_sql.tab.c"
    break;

  case 65: /* insert_value: SSS  */
#line 508 "yacc_sql.y"
         {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], (yyvsp[0].string));
		}
#line 1917 "yacc_sql.tab.c"
    break;

  case 66: /* insert_value: DATE  */
#line 512 "yacc_sql.y"
              {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_date(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], (yyvsp[0].string));
	}
#line 1926 "yacc_sql.tab.c"
    break;

  case 67: /* insert_value: NULL_A  */
#line 517 "yacc_sql.y"
                {
		// value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
		value_init_null(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++]); // 多条插入
	}
#line 1935 "yacc_sql.tab.c"
    break;

  case 68: /* value: number  */
#line 526 "yacc_sql.y"
           {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number));
		}
#line 1943 "yacc_sql.tab.c"
    break;

  case 69: /* value: floatnumber  */
#line 529 "yacc_sql.y"
                {
  		// value_init_float(&CONTEXT->values[CONTEXT->value_length++], ($1).floats);
		value_init_float2(&CONTEXT->values[CONTEXT->value_length++], ((yyvsp[0].floatsAndStr)).floats, ((yyvsp[0].floatsAndStr)).str);
	}
#line 1952 "yacc_sql.tab.c"
    break;

  case 70: /* value: SSS  */
#line 533 "yacc_sql.y"
         {
			(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
		}
#line 1961 "yacc_sql.tab.c"
    break;

  case 71: /* value: DATE  */
#line 537 "yacc_sql.y"
              {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_date(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
	}
#line 1970 "yacc_sql.tab.c"
    break;

  case 72: /* value: NULL_A  */
#line 542 "yacc_sql.y"
                {
		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
	}
#line 1978 "yacc_sql.tab.c"
    break;

  case 73: /* delete: delete_handle FROM ID where SEMICOLON  */
#line 551 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->condition_exps[0], CONTEXT->condition_length[0]);
			CONTEXT->condition_length[0] = 0;
    }
#line 1990 "yacc_sql.tab.c"
    break;

  case 74: /* delete_handle: DELETE  */
#line 560 "yacc_sql.y"
          {
        CONTEXT->comp_deep++;
    }
#line 1998 "yacc_sql.tab.c"
    break;

  case 75: /* update: update_handle ID SET ID EQ value where SEMICOLON  */
#line 568 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value, 
					CONTEXT->condition_exps[0], CONTEXT->condition_length[0]);
			CONTEXT->condition_length[0] = 0;
		}
#line 2010 "yacc_sql.tab.c"
    break;

  case 76: /* update_handle: UPDATE  */
#line 578 "yacc_sql.y"
          {
        CONTEXT->comp_deep++;
    }
#line 2018 "yacc_sql.tab.c"
    break;

  case 77: /* select_clasue: select SEMICOLON  */
#line 584 "yacc_sql.y"
                    {
        CONTEXT->ssql->flag=SCF_SELECT;
    }
#line 2026 "yacc_sql.tab.c"
    break;

  case 78: /* select: select_handle relattrexp FROM ID rel_list where  */
#line 589 "yacc_sql.y"
                                                   {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
    adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-2].string));
    advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps[CONTEXT->comp_deep-1], CONTEXT->condition_length[CONTEXT->comp_deep-1]);
    //临时变量清零
     CONTEXT->from_length=0;
     int tmp = CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1];
     CONTEXT->deep_stack.top--;
     CONTEXT->comp_deep--;
     (yyval.number)=tmp;
  }
#line 2042 "yacc_sql.tab.c"
    break;

  case 79: /* select: select_handle relattrexp FROM ID join where  */
#line 601 "yacc_sql.y"
  {
     AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
     adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-2].string));
     advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps[CONTEXT->comp_deep-1], CONTEXT->condition_length[CONTEXT->comp_deep-1]);
     //临时变量清零
     CONTEXT->from_length=0;
     int tmp = CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1];
     CONTEXT->deep_stack.top--;
     CONTEXT->comp_deep--;
     (yyval.number)=tmp;
  }
#line 2058 "yacc_sql.tab.c"
    break;

  case 80: /* select: select_handle relattrexp FROM ID rel_list where order  */
#line 612 "yacc_sql.y"
                                                           {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
    adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-3].string));
    advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps[CONTEXT->comp_deep-1], CONTEXT->condition_length[CONTEXT->comp_deep-1]);
    //临时变量清零
     CONTEXT->from_length=0;
     int tmp = CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1];
     CONTEXT->deep_stack.top--;
     CONTEXT->comp_deep--;
     (yyval.number)=tmp;
  }
#line 2074 "yacc_sql.tab.c"
    break;

  case 81: /* select: select_handle relattrexp FROM ID join where order  */
#line 624 "yacc_sql.y"
  {
     AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
     adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-3].string));
     advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps[CONTEXT->comp_deep-1], CONTEXT->condition_length[CONTEXT->comp_deep-1]);
     //临时变量清零
     CONTEXT->from_length=0;
     int tmp = CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1];
     CONTEXT->deep_stack.top--;
     CONTEXT->comp_deep--;
     (yyval.number)=tmp;
  }
#line 2090 "yacc_sql.tab.c"
    break;

  case 82: /* select: select_handle relattrexp FROM ID rel_list where group  */
#line 635 "yacc_sql.y"
                                                           {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
    adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-3].string));
    advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps[CONTEXT->comp_deep-1], CONTEXT->condition_length[CONTEXT->comp_deep-1]);
    //临时变量清零
     CONTEXT->from_length=0;
     int tmp = CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1];
     CONTEXT->deep_stack.top--;
     CONTEXT->comp_deep--;
     (yyval.number)=tmp;
  }
#line 2106 "yacc_sql.tab.c"
    break;

  case 83: /* select: select_handle relattrexp FROM ID join where group  */
#line 647 "yacc_sql.y"
  {
     AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
     adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-3].string));
     advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps[CONTEXT->comp_deep-1], CONTEXT->condition_length[CONTEXT->comp_deep-1]);
     //临时变量清零
     CONTEXT->from_length=0;
     int tmp = CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1];
     CONTEXT->deep_stack.top--;
     CONTEXT->comp_deep--;
     (yyval.number)=tmp;
  }
#line 2122 "yacc_sql.tab.c"
    break;

  case 84: /* select_handle: SELECT  */
#line 662 "yacc_sql.y"
  {
     CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top] = CONTEXT->select_length;
     CONTEXT->deep_stack.top++;
     CONTEXT->comp_deep++;
     CONTEXT->select_length++;
     CONTEXT->ssql->sstr.adv_selection[0].select_num=CONTEXT->select_length;
  }
#line 2134 "yacc_sql.tab.c"
    break;

  case 85: /* relattrexp: exp_list relattrexp2  */
#line 671 "yacc_sql.y"
                       {
    RelAttrExp exp;
    exp.agg_type = NOTAGG;
    exp.is_star = 0;
    exp.explist = (ExpList *)((yyvsp[-1].exp_list));
    exp.num = exp.explist->num;
    advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
  }
#line 2147 "yacc_sql.tab.c"
    break;

  case 86: /* relattrexp: MINUS exp_list relattrexp2  */
#line 679 "yacc_sql.y"
                               {
    ExpList *explist = (ExpList *)((yyvsp[-1].exp_list));
    while(explist->left_explist) {
      explist = explist->left_explist;
    }
    explist->exp->have_negative = 1;

    RelAttrExp exp;
    exp.agg_type = NOTAGG;
    exp.is_star = 0;
    exp.explist = (ExpList *)((yyvsp[-1].exp_list));
    exp.num = exp.explist->num;
    advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
  }
#line 2166 "yacc_sql.tab.c"
    break;

  case 87: /* relattrexp: COUNT LBRACE ID RBRACE relattrexp2  */
#line 697 "yacc_sql.y"
                                         {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2176 "yacc_sql.tab.c"
    break;

  case 88: /* relattrexp: COUNT LBRACE STAR RBRACE relattrexp2  */
#line 702 "yacc_sql.y"
                                               {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2186 "yacc_sql.tab.c"
    break;

  case 89: /* relattrexp: COUNT LBRACE ID DOT ID RBRACE relattrexp2  */
#line 707 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2196 "yacc_sql.tab.c"
    break;

  case 90: /* relattrexp: COUNT LBRACE ID DOT STAR RBRACE relattrexp2  */
#line 712 "yacc_sql.y"
                                                      {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, (yyvsp[-4].string), "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2206 "yacc_sql.tab.c"
    break;

  case 91: /* relattrexp: COUNT LBRACE number RBRACE relattrexp2  */
#line 717 "yacc_sql.y"
                                                 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2219 "yacc_sql.tab.c"
    break;

  case 92: /* relattrexp: COUNT LBRACE floatnumber RBRACE relattrexp2  */
#line 725 "yacc_sql.y"
                                                      {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2233 "yacc_sql.tab.c"
    break;

  case 93: /* relattrexp: MAX LBRACE ID RBRACE relattrexp2  */
#line 734 "yacc_sql.y"
                                       {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2243 "yacc_sql.tab.c"
    break;

  case 94: /* relattrexp: MAX LBRACE STAR RBRACE relattrexp2  */
#line 739 "yacc_sql.y"
                                             {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2253 "yacc_sql.tab.c"
    break;

  case 95: /* relattrexp: MAX LBRACE ID DOT ID RBRACE relattrexp2  */
#line 744 "yacc_sql.y"
                                                  {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2263 "yacc_sql.tab.c"
    break;

  case 96: /* relattrexp: MAX LBRACE number RBRACE relattrexp2  */
#line 749 "yacc_sql.y"
                                               {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2276 "yacc_sql.tab.c"
    break;

  case 97: /* relattrexp: MAX LBRACE floatnumber RBRACE relattrexp2  */
#line 757 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2290 "yacc_sql.tab.c"
    break;

  case 98: /* relattrexp: MIN LBRACE ID RBRACE relattrexp2  */
#line 766 "yacc_sql.y"
                                       {  
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2300 "yacc_sql.tab.c"
    break;

  case 99: /* relattrexp: MIN LBRACE STAR RBRACE relattrexp2  */
#line 771 "yacc_sql.y"
                                             {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2310 "yacc_sql.tab.c"
    break;

  case 100: /* relattrexp: MIN LBRACE ID DOT ID RBRACE relattrexp2  */
#line 776 "yacc_sql.y"
                                                  {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2320 "yacc_sql.tab.c"
    break;

  case 101: /* relattrexp: MIN LBRACE number RBRACE relattrexp2  */
#line 781 "yacc_sql.y"
                                               { 
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2333 "yacc_sql.tab.c"
    break;

  case 102: /* relattrexp: MIN LBRACE floatnumber RBRACE relattrexp2  */
#line 789 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2347 "yacc_sql.tab.c"
    break;

  case 103: /* relattrexp: AVG LBRACE ID RBRACE relattrexp2  */
#line 798 "yacc_sql.y"
                                       {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2357 "yacc_sql.tab.c"
    break;

  case 104: /* relattrexp: AVG LBRACE STAR RBRACE relattrexp2  */
#line 803 "yacc_sql.y"
                                             {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2367 "yacc_sql.tab.c"
    break;

  case 105: /* relattrexp: AVG LBRACE ID DOT ID RBRACE relattrexp2  */
#line 808 "yacc_sql.y"
                                                  {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2377 "yacc_sql.tab.c"
    break;

  case 106: /* relattrexp: AVG LBRACE number RBRACE relattrexp2  */
#line 813 "yacc_sql.y"
                                               {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2390 "yacc_sql.tab.c"
    break;

  case 107: /* relattrexp: AVG LBRACE floatnumber RBRACE relattrexp2  */
#line 821 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2404 "yacc_sql.tab.c"
    break;

  case 108: /* relattrexp: STAR  */
#line 832 "yacc_sql.y"
           {
      RelAttrExp exp;
      exp.agg_type = NOTAGG;
      exp.is_star = 1;
      exp.relation_name = NULL;
      advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
    }
#line 2416 "yacc_sql.tab.c"
    break;

  case 109: /* relattrexp: ID DOT STAR relattrexp2  */
#line 839 "yacc_sql.y"
                              {
      RelAttrExp exp;
      exp.agg_type = NOTAGG;
      exp.is_star = 1;
      exp.relation_name = strdup((yyvsp[-3].string));
      advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
    }
#line 2428 "yacc_sql.tab.c"
    break;

  case 112: /* exp_list: exps exp_list2  */
#line 857 "yacc_sql.y"
                 {
    ExpList *explist_left = (ExpList *)malloc(sizeof(ExpList));
    explist_left->exp = (Exp *)((yyvsp[-1].exp));
    explist_left->left_explist = NULL;
    explist_left->calop = STARTCALOP;
    explist_left->num = explist_left->exp->num;
    if((yyvsp[0].exp_list)){
      ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
      while(explist_right->left_explist) {
        explist_right->num += explist_left->num;
        explist_right = explist_right->left_explist;
      }
      explist_right->num += explist_left->num;
      explist_right->left_explist = explist_left;
      (yyval.exp_list) = (yyvsp[0].exp_list);
    }
    else {
      (yyval.exp_list) = (void *)explist_left;
    }
  }
#line 2453 "yacc_sql.tab.c"
    break;

  case 113: /* exp_list2: %empty  */
#line 880 "yacc_sql.y"
             {
    (yyval.exp_list) = NULL;
  }
#line 2461 "yacc_sql.tab.c"
    break;

  case 114: /* exp_list2: PLUS exp_list  */
#line 883 "yacc_sql.y"
                  {
    ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
    while(explist_right->left_explist) {
      explist_right = explist_right->left_explist;
    }
    explist_right->calop = PLUS_OP;
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2474 "yacc_sql.tab.c"
    break;

  case 115: /* exp_list2: MINUS exp_list  */
#line 891 "yacc_sql.y"
                   {
    ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
    while(explist_right->left_explist) {
      explist_right = explist_right->left_explist;
    }
    explist_right->calop = MINUS_OP;
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2487 "yacc_sql.tab.c"
    break;

  case 116: /* exp_list3: exp_list  */
#line 902 "yacc_sql.y"
           {
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2495 "yacc_sql.tab.c"
    break;

  case 117: /* exp_list3: MINUS exp_list  */
#line 905 "yacc_sql.y"
                   {
    ExpList *explist = (ExpList *)((yyvsp[0].exp_list));
    while(explist->left_explist) {
      explist = explist->left_explist;
    }
    explist->exp->have_negative = 1;
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2508 "yacc_sql.tab.c"
    break;

  case 118: /* exps: exp exps2  */
#line 919 "yacc_sql.y"
            {
    if((yyvsp[0].exp)){
      Exp *exp_left = (Exp *)((yyvsp[-1].exp));
      Exp *exp_right = (Exp *)((yyvsp[0].exp));
      while(exp_right->left_exp) {
        exp_right->num += exp_left->num;
        exp_right = exp_right->left_exp;
      }
      exp_right->num += exp_left->num;
      exp_right->left_exp = exp_left;
      (yyval.exp) = (yyvsp[0].exp);
    } else {
      (yyval.exp) = (yyvsp[-1].exp);
    }
  }
#line 2528 "yacc_sql.tab.c"
    break;

  case 119: /* exps2: %empty  */
#line 937 "yacc_sql.y"
             {
    (yyval.exp) = NULL;
  }
#line 2536 "yacc_sql.tab.c"
    break;

  case 120: /* exps2: STAR exps  */
#line 940 "yacc_sql.y"
              {
    Exp *exp_right = (Exp *)((yyvsp[0].exp));
    while(exp_right->left_exp) {
      exp_right = exp_right->left_exp;
    }
    exp_right->calop = TIME_OP;
    (yyval.exp) = (yyvsp[0].exp);
  }
#line 2549 "yacc_sql.tab.c"
    break;

  case 121: /* exps2: DIVIDE exps  */
#line 948 "yacc_sql.y"
                {
    Exp *exp_right = (Exp *)((yyvsp[0].exp));
    while(exp_right->left_exp) {
      exp_right = exp_right->left_exp;
    }
    exp_right->calop = DIVIDE_OP;
    (yyval.exp) = (yyvsp[0].exp);
  }
#line 2562 "yacc_sql.tab.c"
    break;

  case 122: /* exp: ID  */
#line 961 "yacc_sql.y"
     {
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 0;
    exp->have_negative = 0;
    exp->explist = NULL;
    exp->is_attr = 1;
    exp->relation_name = NULL;
    exp->attribute_name = strdup((yyvsp[0].string));
    exp->left_exp = NULL;
    exp->calop = STARTCALOP;
    exp->num = 1;
    exp->sub_select_index=-1;
    (yyval.exp) = (void *)exp;
  }
#line 2581 "yacc_sql.tab.c"
    break;

  case 123: /* exp: ID DOT ID  */
#line 975 "yacc_sql.y"
              {
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 0;
    exp->have_negative = 0;
    exp->explist = NULL;
    exp->is_attr = 1;
    exp->relation_name = strdup((yyvsp[-2].string));
    exp->attribute_name = strdup((yyvsp[0].string));
    exp->left_exp = NULL;
    exp->calop = STARTCALOP;
    exp->num = 1;
    exp->sub_select_index=-1;
    (yyval.exp) = (void *)exp;
  }
#line 2600 "yacc_sql.tab.c"
    break;

  case 124: /* exp: NNNUMBER  */
#line 989 "yacc_sql.y"
             {
    value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number));
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
    exp->sub_select_index=-1;
    (yyval.exp) = (void *)exp;
  }
#line 2621 "yacc_sql.tab.c"
    break;

  case 125: /* exp: NNFLOAT  */
#line 1005 "yacc_sql.y"
            {
    value_init_float2(&CONTEXT->values[CONTEXT->value_length++], ((yyvsp[0].floatsAndStr)).floats, ((yyvsp[0].floatsAndStr)).str);
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
    exp->sub_select_index=-1;
    (yyval.exp) = (void *)exp;
  }
#line 2642 "yacc_sql.tab.c"
    break;

  case 126: /* exp: SSS  */
#line 1021 "yacc_sql.y"
        {
    (yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  	value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
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
    exp->sub_select_index=-1;
    (yyval.exp) = (void *)exp;
  }
#line 2664 "yacc_sql.tab.c"
    break;

  case 127: /* exp: DATE  */
#line 1038 "yacc_sql.y"
         {
    (yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  	value_init_date(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
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
    exp->sub_select_index=-1;
    (yyval.exp) = (void *)exp;
  }
#line 2686 "yacc_sql.tab.c"
    break;

  case 128: /* exp: NULL_A  */
#line 1055 "yacc_sql.y"
           {
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
    exp->sub_select_index=-1;
    (yyval.exp) = (void *)exp;
  }
#line 2707 "yacc_sql.tab.c"
    break;

  case 129: /* exp: LBRACE exp_list3 RBRACE  */
#line 1071 "yacc_sql.y"
                            {
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 1;
    exp->have_negative = 0;
    exp->explist = (ExpList *)((yyvsp[-1].exp_list));
    exp->num = exp->explist->num;
    exp->left_exp = NULL;
    exp->sub_select_index=-1;
    (yyval.exp) = (void *)exp;
  }
#line 2722 "yacc_sql.tab.c"
    break;

  case 130: /* exp: LBRACE select RBRACE  */
#line 1081 "yacc_sql.y"
                         {
      Exp *exp = (Exp *)malloc(sizeof(Exp));
      exp->have_brace = 0;
      exp->have_negative = 0;
      exp->explist = NULL;
      exp->is_attr = 0;
      exp->relation_name = NULL;
      exp->attribute_name = NULL;
      exp->left_exp = NULL;
      exp->calop = STARTCALOP;
      exp->num = 1;
      exp->sub_select_index=(yyvsp[-1].number);
      (yyval.exp) = (void *)exp;
  }
#line 2741 "yacc_sql.tab.c"
    break;

  case 132: /* rel_list: COMMA ID rel_list  */
#line 1099 "yacc_sql.y"
                        {	
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
        adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-1].string));
		  }
#line 2750 "yacc_sql.tab.c"
    break;

  case 134: /* where: WHERE condition_exps  */
#line 1107 "yacc_sql.y"
                         {
  
  }
#line 2758 "yacc_sql.tab.c"
    break;

  case 136: /* condition_exps: condition_exp  */
#line 1113 "yacc_sql.y"
                  {
  
  }
#line 2766 "yacc_sql.tab.c"
    break;

  case 137: /* condition_exps: condition_exp AND condition_exps  */
#line 1116 "yacc_sql.y"
                                     {
  
  }
#line 2774 "yacc_sql.tab.c"
    break;

  case 138: /* condition_exp: exp_list3 comOp exp_list3  */
#line 1123 "yacc_sql.y"
                            {
    ExpList *explist_left = (ExpList *)((yyvsp[-2].exp_list));
    ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
    ConditionExp cond_exp;
    cond_exp.left = explist_left;
    cond_exp.right = explist_right;
    cond_exp.comp = CONTEXT->comp[CONTEXT->comp_deep-1];
    //printf("%d\n\n",CONTEXT->comp[CONTEXT->comp_deep-1]);
    CONTEXT->condition_exps[CONTEXT->comp_deep-1][CONTEXT->condition_length[CONTEXT->comp_deep-1]++] = cond_exp;
  }
#line 2789 "yacc_sql.tab.c"
    break;

  case 139: /* condition_exp: exp_list3 IS_A NULL_A  */
#line 1134 "yacc_sql.y"
                                {
		ExpList *explist_left = (ExpList *)((yyvsp[-2].exp_list));
		ExpList *explist_right = (ExpList *)malloc(sizeof(ExpList));
		
		explist_init_for_null(explist_right);
		ConditionExp cond_exp;
		cond_exp.left = explist_left;
		cond_exp.right = explist_right;
		cond_exp.comp = IS;
		CONTEXT->condition_exps[CONTEXT->comp_deep-1][CONTEXT->condition_length[CONTEXT->comp_deep-1]++] = cond_exp;
	}
#line 2805 "yacc_sql.tab.c"
    break;

  case 140: /* condition_exp: exp_list3 IS_A NOT NULL_A  */
#line 1145 "yacc_sql.y"
                                    {
		ExpList *explist_left = (ExpList *)((yyvsp[-3].exp_list));
		ExpList *explist_right = (ExpList *)malloc(sizeof(ExpList));
		
		explist_init_for_null(explist_right);
		
		ConditionExp cond_exp;
		cond_exp.left = explist_left;
		cond_exp.right = explist_right;
		cond_exp.comp = ISNOT;
		CONTEXT->condition_exps[CONTEXT->comp_deep-1][CONTEXT->condition_length[CONTEXT->comp_deep-1]++] = cond_exp;
	}
#line 2822 "yacc_sql.tab.c"
    break;

  case 141: /* join: INNER JOIN ID onwhere  */
#line 1161 "yacc_sql.y"
                          {
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
        adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-1].string));
    }
#line 2831 "yacc_sql.tab.c"
    break;

  case 142: /* join: INNER JOIN ID onwhere join  */
#line 1165 "yacc_sql.y"
                                {
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
        adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-2].string));
    }
#line 2840 "yacc_sql.tab.c"
    break;

  case 144: /* onwhere: ON condition_exps  */
#line 1172 "yacc_sql.y"
                        {

    }
#line 2848 "yacc_sql.tab.c"
    break;

  case 145: /* order: ORDER BY ID order2  */
#line 1179 "yacc_sql.y"
                     {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2861 "yacc_sql.tab.c"
    break;

  case 146: /* order: ORDER BY ID DOT ID order2  */
#line 1187 "yacc_sql.y"
                              {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = strdup((yyvsp[-3].string));
    order_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2874 "yacc_sql.tab.c"
    break;

  case 147: /* order: ORDER BY ID ASC order2  */
#line 1195 "yacc_sql.y"
                           {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2887 "yacc_sql.tab.c"
    break;

  case 148: /* order: ORDER BY ID DOT ID ASC order2  */
#line 1203 "yacc_sql.y"
                                  {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = strdup((yyvsp[-4].string));
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2900 "yacc_sql.tab.c"
    break;

  case 149: /* order: ORDER BY ID DESC order2  */
#line 1211 "yacc_sql.y"
                            {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = DESCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2913 "yacc_sql.tab.c"
    break;

  case 150: /* order: ORDER BY ID DOT ID DESC order2  */
#line 1219 "yacc_sql.y"
                                   {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = DESCORDER;
    order_attr.relation_name = strdup((yyvsp[-4].string));
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2926 "yacc_sql.tab.c"
    break;

  case 152: /* order2: COMMA ID order2  */
#line 1231 "yacc_sql.y"
                  {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2939 "yacc_sql.tab.c"
    break;

  case 153: /* order2: COMMA ID DOT ID order2  */
#line 1239 "yacc_sql.y"
                           {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = strdup((yyvsp[-3].string));
    order_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2952 "yacc_sql.tab.c"
    break;

  case 154: /* order2: COMMA ID ASC order2  */
#line 1247 "yacc_sql.y"
                        {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2965 "yacc_sql.tab.c"
    break;

  case 155: /* order2: COMMA ID DOT ID ASC order2  */
#line 1255 "yacc_sql.y"
                               {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = strdup((yyvsp[-4].string));
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2978 "yacc_sql.tab.c"
    break;

  case 156: /* order2: COMMA ID DESC order2  */
#line 1263 "yacc_sql.y"
                         {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = DESCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2991 "yacc_sql.tab.c"
    break;

  case 157: /* order2: COMMA ID DOT ID DESC order2  */
#line 1271 "yacc_sql.y"
                                {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = DESCORDER;
    order_attr.relation_name = strdup((yyvsp[-4].string));
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 3004 "yacc_sql.tab.c"
    break;

  case 158: /* group: GROUP BY ID group2  */
#line 1283 "yacc_sql.y"
                     {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    GroupAttr group_attr;
    group_attr.relation_name = NULL;
    group_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->group_attrs[adv_selects->group_num++] = group_attr;
  }
#line 3016 "yacc_sql.tab.c"
    break;

  case 159: /* group: GROUP BY ID DOT ID group2  */
#line 1290 "yacc_sql.y"
                              {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    GroupAttr group_attr;
    group_attr.relation_name = strdup((yyvsp[-3].string));
    group_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->group_attrs[adv_selects->group_num++] = group_attr;
  }
#line 3028 "yacc_sql.tab.c"
    break;

  case 161: /* group2: COMMA ID group2  */
#line 1300 "yacc_sql.y"
                    {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    GroupAttr group_attr;
    group_attr.relation_name = NULL;
    group_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->group_attrs[adv_selects->group_num++] = group_attr;
  }
#line 3040 "yacc_sql.tab.c"
    break;

  case 162: /* group2: COMMA ID DOT ID group2  */
#line 1307 "yacc_sql.y"
                           {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    GroupAttr group_attr;
    group_attr.relation_name = strdup((yyvsp[-3].string));
    group_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->group_attrs[adv_selects->group_num++] = group_attr;
  }
#line 3052 "yacc_sql.tab.c"
    break;

  case 163: /* comOp: EQ  */
#line 1319 "yacc_sql.y"
             { CONTEXT->comp[CONTEXT->comp_deep-1] = EQUAL_TO; }
#line 3058 "yacc_sql.tab.c"
    break;

  case 164: /* comOp: LT  */
#line 1320 "yacc_sql.y"
         { CONTEXT->comp[CONTEXT->comp_deep-1] = LESS_THAN; }
#line 3064 "yacc_sql.tab.c"
    break;

  case 165: /* comOp: GT  */
#line 1321 "yacc_sql.y"
         { CONTEXT->comp[CONTEXT->comp_deep-1] = GREAT_THAN; }
#line 3070 "yacc_sql.tab.c"
    break;

  case 166: /* comOp: LE  */
#line 1322 "yacc_sql.y"
         { CONTEXT->comp[CONTEXT->comp_deep-1] = LESS_EQUAL; }
#line 3076 "yacc_sql.tab.c"
    break;

  case 167: /* comOp: GE  */
#line 1323 "yacc_sql.y"
         { CONTEXT->comp[CONTEXT->comp_deep-1] = GREAT_EQUAL; }
#line 3082 "yacc_sql.tab.c"
    break;

  case 168: /* comOp: NE  */
#line 1324 "yacc_sql.y"
         { CONTEXT->comp[CONTEXT->comp_deep-1] = NOT_EQUAL; }
#line 3088 "yacc_sql.tab.c"
    break;

  case 169: /* comOp: IN  */
#line 1325 "yacc_sql.y"
         { CONTEXT->comp[CONTEXT->comp_deep-1] = IN_SUB;
            printf("%d\n\n",CONTEXT->comp[CONTEXT->comp_deep-1]);
            }
#line 3096 "yacc_sql.tab.c"
    break;

  case 170: /* comOp: NOT IN  */
#line 1328 "yacc_sql.y"
             { CONTEXT->comp[CONTEXT->comp_deep-1] = NOT_IN_SUB;}
#line 3102 "yacc_sql.tab.c"
    break;

  case 171: /* load_data: LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON  */
#line 1333 "yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 3111 "yacc_sql.tab.c"
    break;


#line 3115 "yacc_sql.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (scanner, YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, scanner);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, scanner);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, scanner);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 1338 "yacc_sql.y"

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
