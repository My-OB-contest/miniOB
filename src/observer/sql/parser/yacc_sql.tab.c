/* A Bison parser, made by GNU Bison 3.7.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7"

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
  size_t condition_length;
  size_t from_length;
  size_t value_length;
  size_t value_list_length;
  size_t insert_value_length[MAX_NUM];
  Value values[MAX_NUM];
  Value insert_values[MAX_NUM][MAX_NUM];
  ConditionExp condition_exps[MAX_NUM]; // 删掉Condition，增加ConditionExp
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


#line 136 "yacc_sql.tab.c"

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
  YYSYMBOL_HELP = 26,                      /* HELP  */
  YYSYMBOL_EXIT = 27,                      /* EXIT  */
  YYSYMBOL_DOT = 28,                       /* DOT  */
  YYSYMBOL_INTO = 29,                      /* INTO  */
  YYSYMBOL_VALUES = 30,                    /* VALUES  */
  YYSYMBOL_FROM = 31,                      /* FROM  */
  YYSYMBOL_WHERE = 32,                     /* WHERE  */
  YYSYMBOL_AND = 33,                       /* AND  */
  YYSYMBOL_SET = 34,                       /* SET  */
  YYSYMBOL_ON = 35,                        /* ON  */
  YYSYMBOL_LOAD = 36,                      /* LOAD  */
  YYSYMBOL_DATA = 37,                      /* DATA  */
  YYSYMBOL_INFILE = 38,                    /* INFILE  */
  YYSYMBOL_EQ = 39,                        /* EQ  */
  YYSYMBOL_LT = 40,                        /* LT  */
  YYSYMBOL_GT = 41,                        /* GT  */
  YYSYMBOL_LE = 42,                        /* LE  */
  YYSYMBOL_GE = 43,                        /* GE  */
  YYSYMBOL_NE = 44,                        /* NE  */
  YYSYMBOL_COUNT = 45,                     /* COUNT  */
  YYSYMBOL_MAX = 46,                       /* MAX  */
  YYSYMBOL_MIN = 47,                       /* MIN  */
  YYSYMBOL_AVG = 48,                       /* AVG  */
  YYSYMBOL_INNER = 49,                     /* INNER  */
  YYSYMBOL_JOIN = 50,                      /* JOIN  */
  YYSYMBOL_NOT = 51,                       /* NOT  */
  YYSYMBOL_NULL_A = 52,                    /* NULL_A  */
  YYSYMBOL_NULLABLE = 53,                  /* NULLABLE  */
  YYSYMBOL_IS_A = 54,                      /* IS_A  */
  YYSYMBOL_UNIQUE = 55,                    /* UNIQUE  */
  YYSYMBOL_PLUS = 56,                      /* PLUS  */
  YYSYMBOL_MINUS = 57,                     /* MINUS  */
  YYSYMBOL_DIVIDE = 58,                    /* DIVIDE  */
  YYSYMBOL_ORDER = 59,                     /* ORDER  */
  YYSYMBOL_BY = 60,                        /* BY  */
  YYSYMBOL_ASC = 61,                       /* ASC  */
  YYSYMBOL_GROUP = 62,                     /* GROUP  */
  YYSYMBOL_NNNUMBER = 63,                  /* NNNUMBER  */
  YYSYMBOL_NNFLOAT = 64,                   /* NNFLOAT  */
  YYSYMBOL_ID = 65,                        /* ID  */
  YYSYMBOL_DATE = 66,                      /* DATE  */
  YYSYMBOL_PATH = 67,                      /* PATH  */
  YYSYMBOL_SSS = 68,                       /* SSS  */
  YYSYMBOL_STAR = 69,                      /* STAR  */
  YYSYMBOL_STRING_V = 70,                  /* STRING_V  */
  YYSYMBOL_YYACCEPT = 71,                  /* $accept  */
  YYSYMBOL_commands = 72,                  /* commands  */
  YYSYMBOL_command = 73,                   /* command  */
  YYSYMBOL_exit = 74,                      /* exit  */
  YYSYMBOL_help = 75,                      /* help  */
  YYSYMBOL_sync = 76,                      /* sync  */
  YYSYMBOL_begin = 77,                     /* begin  */
  YYSYMBOL_commit = 78,                    /* commit  */
  YYSYMBOL_rollback = 79,                  /* rollback  */
  YYSYMBOL_drop_table = 80,                /* drop_table  */
  YYSYMBOL_show_tables = 81,               /* show_tables  */
  YYSYMBOL_desc_table = 82,                /* desc_table  */
  YYSYMBOL_create_index = 83,              /* create_index  */
  YYSYMBOL_field_name_list = 84,           /* field_name_list  */
  YYSYMBOL_drop_index = 85,                /* drop_index  */
  YYSYMBOL_create_table = 86,              /* create_table  */
  YYSYMBOL_attr_def_list = 87,             /* attr_def_list  */
  YYSYMBOL_attr_def = 88,                  /* attr_def  */
  YYSYMBOL_number = 89,                    /* number  */
  YYSYMBOL_floatnumber = 90,               /* floatnumber  */
  YYSYMBOL_type = 91,                      /* type  */
  YYSYMBOL_ID_get = 92,                    /* ID_get  */
  YYSYMBOL_insert = 93,                    /* insert  */
  YYSYMBOL_values_lists = 94,              /* values_lists  */
  YYSYMBOL_value_tuple = 95,               /* value_tuple  */
  YYSYMBOL_value_list = 96,                /* value_list  */
  YYSYMBOL_insert_value = 97,              /* insert_value  */
  YYSYMBOL_value = 98,                     /* value  */
  YYSYMBOL_delete = 99,                    /* delete  */
  YYSYMBOL_update = 100,                   /* update  */
  YYSYMBOL_select = 101,                   /* select  */
  YYSYMBOL_relattrexp = 102,               /* relattrexp  */
  YYSYMBOL_relattrexp2 = 103,              /* relattrexp2  */
  YYSYMBOL_exp_list = 104,                 /* exp_list  */
  YYSYMBOL_exp_list2 = 105,                /* exp_list2  */
  YYSYMBOL_exp_list3 = 106,                /* exp_list3  */
  YYSYMBOL_exps = 107,                     /* exps  */
  YYSYMBOL_exps2 = 108,                    /* exps2  */
  YYSYMBOL_exp = 109,                      /* exp  */
  YYSYMBOL_rel_list = 110,                 /* rel_list  */
  YYSYMBOL_where = 111,                    /* where  */
  YYSYMBOL_condition_exps = 112,           /* condition_exps  */
  YYSYMBOL_condition_exp = 113,            /* condition_exp  */
  YYSYMBOL_join = 114,                     /* join  */
  YYSYMBOL_onwhere = 115,                  /* onwhere  */
  YYSYMBOL_order = 116,                    /* order  */
  YYSYMBOL_order2 = 117,                   /* order2  */
  YYSYMBOL_group = 118,                    /* group  */
  YYSYMBOL_group2 = 119,                   /* group2  */
  YYSYMBOL_comOp = 120,                    /* comOp  */
  YYSYMBOL_load_data = 121                 /* load_data  */
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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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
#define YYLAST   370

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  71
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  51
/* YYNRULES -- Number of rules.  */
#define YYNRULES  161
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  348

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   325


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
      65,    66,    67,    68,    69,    70
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   185,   185,   187,   191,   192,   193,   194,   195,   196,
     197,   198,   199,   200,   201,   202,   203,   204,   205,   206,
     207,   211,   216,   221,   227,   233,   239,   245,   251,   257,
     264,   269,   277,   281,   288,   295,   304,   306,   310,   321,
     335,   347,   359,   371,   386,   387,   393,   394,   407,   408,
     409,   410,   413,   423,   444,   447,   452,   454,   459,   461,
     468,   471,   474,   478,   483,   492,   495,   499,   503,   508,
     516,   526,   538,   549,   561,   572,   584,   595,   610,   618,
     636,   641,   646,   651,   656,   664,   673,   678,   683,   688,
     696,   705,   710,   715,   720,   728,   737,   742,   747,   752,
     760,   771,   778,   787,   789,   796,   819,   822,   830,   841,
     844,   858,   876,   879,   887,   900,   913,   926,   941,   956,
     972,   988,  1003,  1014,  1016,  1022,  1024,  1029,  1030,  1033,
    1040,  1050,  1062,  1078,  1082,  1087,  1089,  1096,  1104,  1112,
    1120,  1128,  1136,  1146,  1148,  1156,  1164,  1172,  1180,  1188,
    1200,  1207,  1215,  1217,  1224,  1236,  1237,  1238,  1239,  1240,
    1241,  1245
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
  "HELP", "EXIT", "DOT", "INTO", "VALUES", "FROM", "WHERE", "AND", "SET",
  "ON", "LOAD", "DATA", "INFILE", "EQ", "LT", "GT", "LE", "GE", "NE",
  "COUNT", "MAX", "MIN", "AVG", "INNER", "JOIN", "NOT", "NULL_A",
  "NULLABLE", "IS_A", "UNIQUE", "PLUS", "MINUS", "DIVIDE", "ORDER", "BY",
  "ASC", "GROUP", "NNNUMBER", "NNFLOAT", "ID", "DATE", "PATH", "SSS",
  "STAR", "STRING_V", "$accept", "commands", "command", "exit", "help",
  "sync", "begin", "commit", "rollback", "drop_table", "show_tables",
  "desc_table", "create_index", "field_name_list", "drop_index",
  "create_table", "attr_def_list", "attr_def", "number", "floatnumber",
  "type", "ID_get", "insert", "values_lists", "value_tuple", "value_list",
  "insert_value", "value", "delete", "update", "select", "relattrexp",
  "relattrexp2", "exp_list", "exp_list2", "exp_list3", "exps", "exps2",
  "exp", "rel_list", "where", "condition_exps", "condition_exp", "join",
  "onwhere", "order", "order2", "group", "group2", "comOp", "load_data", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325
};
#endif

#define YYPACT_NINF (-307)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -307,   171,  -307,     5,    50,    73,   -35,    30,    74,    17,
     103,    70,   157,   158,   159,   160,   167,   135,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -307,  -307,  -307,   108,   109,   169,   113,
     114,    46,   172,   179,   183,   189,  -307,    81,  -307,  -307,
     186,  -307,  -307,  -307,   156,   193,    43,   -33,   215,   217,
    -307,   168,   170,   188,  -307,  -307,  -307,  -307,  -307,   185,
     218,   201,   177,   234,   240,    81,   219,  -307,   228,    59,
     137,   152,   175,   193,    36,   181,    73,  -307,    81,    81,
    -307,    81,    81,  -307,  -307,  -307,   220,   216,   184,   190,
     191,   192,   224,  -307,  -307,  -307,   195,  -307,    63,  -307,
    -307,    10,   235,   236,   237,    44,   238,   244,   245,    87,
     246,   247,   248,   112,   249,   250,   251,  -307,  -307,   193,
       8,  -307,  -307,  -307,  -307,  -307,   253,    46,   267,   212,
     242,  -307,   254,   128,   257,   209,  -307,  -307,   193,    48,
     193,   193,   193,   193,   210,   193,   193,   193,   193,   211,
     193,   193,   193,   193,   213,   193,   193,   193,  -307,   214,
     227,   216,   216,   -24,   262,   187,  -307,   252,  -307,    91,
     275,   191,   265,  -307,  -307,  -307,  -307,    -1,   221,   268,
    -307,   266,   270,  -307,  -307,  -307,  -307,   271,  -307,  -307,
    -307,  -307,   272,  -307,  -307,  -307,  -307,   273,  -307,  -307,
    -307,   274,   226,    28,    29,  -307,  -307,  -307,  -307,  -307,
     276,   253,   290,  -307,  -307,  -307,  -307,  -307,  -307,    56,
      46,    46,  -307,  -307,  -307,  -307,  -307,   216,   230,   254,
     293,   -34,   255,  -307,   279,   281,   221,   193,   193,   193,
     193,   193,  -307,   264,  -307,   241,   243,   297,   299,  -307,
     301,   302,   -24,   289,   262,  -307,   256,  -307,  -307,  -307,
     306,   307,  -307,  -307,   258,   294,  -307,   221,   309,   296,
    -307,  -307,  -307,  -307,  -307,    46,   269,   259,   260,  -307,
    -307,  -307,  -307,   276,  -307,  -307,  -307,  -307,  -307,   105,
    -307,  -307,   311,  -307,  -307,     6,    27,  -307,   263,  -307,
    -307,   298,   261,   277,   298,  -307,   278,   280,  -307,  -307,
    -307,    31,     4,  -307,    35,   304,   298,   282,   298,  -307,
     298,   298,  -307,   283,  -307,  -307,  -307,    33,  -307,  -307,
    -307,   304,   298,   298,  -307,  -307,  -307,  -307
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,    20,
      19,    14,    15,    16,    17,     9,    10,    11,    12,    13,
       8,     5,     7,     6,     4,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   121,     0,   117,   118,
     115,   120,   119,   101,     0,   103,   106,   112,     0,     0,
      23,     0,     0,     0,    24,    25,    26,    22,    21,     0,
       0,     0,     0,     0,     0,     0,   115,   109,     0,     0,
       0,     0,     0,   103,     0,     0,     0,    78,     0,     0,
     105,     0,     0,   111,    29,    28,     0,   125,     0,     0,
       0,     0,     0,    27,    34,   110,     0,   122,     0,    44,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    79,   116,   103,
     123,   104,   107,   108,   114,   113,    56,   127,     0,     0,
       0,    52,    36,     0,     0,     0,    45,    47,   103,     0,
     103,   103,   103,   103,     0,   103,   103,   103,   103,     0,
     103,   103,   103,   103,     0,   103,   103,   103,   102,     0,
       0,   125,   125,     0,    54,     0,   126,   128,    70,     0,
       0,     0,     0,    48,    49,    50,    51,    39,     0,     0,
      80,     0,     0,    81,    84,    85,    86,     0,    87,    89,
      90,    91,     0,    92,    94,    95,    96,     0,    97,    99,
     100,   123,     0,     0,     0,    64,    63,    62,    60,    61,
      58,    56,     0,   155,   156,   157,   158,   159,   160,     0,
       0,   127,    69,    68,    67,    65,    66,   125,     0,    36,
       0,     0,     0,    43,    32,     0,     0,   103,   103,   103,
     103,   103,   124,   135,    72,     0,     0,     0,     0,    73,
       0,     0,     0,     0,    54,    53,     0,   131,   130,   129,
       0,     0,    37,    35,     0,     0,    42,     0,     0,     0,
      82,    83,    88,    93,    98,   127,   133,     0,     0,    74,
      76,    75,    77,    58,    57,    55,   132,    71,   161,    38,
      33,    30,     0,   136,   134,   143,   152,    59,     0,    41,
      31,   143,     0,     0,   143,   137,     0,     0,   150,    40,
     141,   143,   143,   139,   152,   152,   143,     0,   143,   144,
     143,   143,   138,     0,   153,   151,   148,   143,   146,   142,
     140,   152,   143,   143,   145,   154,   149,   147
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,  -307,
    -307,  -307,  -307,  -229,  -307,  -307,    78,   138,   -77,   -73,
    -307,  -307,  -307,    64,    99,    34,    61,  -307,  -307,  -307,
    -307,   284,   -82,     7,  -307,   -41,    40,  -307,  -307,   118,
    -151,  -221,  -307,    45,  -307,   116,  -118,   119,  -306,  -307,
    -307
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,   245,    29,    30,   182,   142,   218,   219,
     187,   143,    31,   222,   174,   263,   220,   237,    32,    33,
      34,    54,    87,    77,    90,   175,    56,    93,    57,   171,
     138,   176,   177,   172,   286,   257,   315,   258,   318,   230,
      35
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      78,   127,   113,   117,   121,   125,   114,   118,   122,   126,
     269,    36,    55,    37,   330,   241,   311,   279,   334,   335,
     213,   214,   312,   274,   312,    91,   169,   148,   215,   109,
      58,   254,   259,   108,   313,   345,    92,    59,   149,   109,
     110,   326,   216,   342,   217,   316,    61,   168,   300,   312,
     242,   312,   243,   316,    83,   317,    39,   170,    40,   327,
      38,   153,    41,   333,   303,   331,   190,   314,   193,   194,
     195,   196,   154,   198,   199,   200,   201,    60,   203,   204,
     205,   206,   105,   208,   209,   210,   270,   255,   255,    41,
     256,   256,   328,    55,   343,   132,   133,    41,    46,    88,
      89,   128,   235,    75,   158,   129,   236,   266,   267,    48,
      49,    76,    51,   191,    52,   159,   108,   192,    42,    43,
      44,    45,   109,   110,   111,    46,   146,   147,   112,   163,
      47,   134,   135,    46,    62,    63,    48,    49,    50,    51,
     164,    52,    53,   232,    48,    49,    76,    51,   108,    52,
     183,   184,   185,   186,   109,   110,   308,   233,   309,   234,
      64,    65,    66,    67,   275,   280,   281,   282,   283,   284,
      68,     2,    69,    70,    71,     3,     4,    72,    73,    74,
       5,     6,     7,     8,     9,    10,    11,    85,    79,   268,
      12,    13,    14,   320,   108,    80,   323,    15,    16,    81,
     109,   110,   115,   329,   332,    82,   116,    17,   336,   108,
     338,    86,   339,   340,    84,   109,   110,   119,    94,   344,
      95,   120,    98,    99,   346,   347,   223,   224,   225,   226,
     227,   228,   108,    96,   100,    97,   101,   103,   109,   110,
     123,   229,   102,   104,   124,   107,   130,   106,   137,   139,
     136,   179,   150,   151,   152,   155,   141,   144,   140,   145,
     128,   156,   157,   160,   161,   162,   165,   166,   167,   173,
     178,   180,   181,   188,   189,   197,   202,   212,   207,   211,
     221,   238,   240,   247,   246,   231,   244,   248,   249,   250,
     251,   253,   169,   265,   262,   271,   273,   277,   278,   285,
     289,   287,   290,   288,   291,   292,   294,   276,   296,   297,
     298,   299,   301,   302,   310,   319,   312,   272,   170,   239,
     264,   146,   316,   293,   305,   306,   321,   307,   295,   252,
     260,   304,     0,   261,     0,     0,     0,     0,     0,     0,
       0,     0,   322,   324,     0,   325,     0,   337,   341,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     131
};

static const yytype_int16 yycheck[] =
{
      41,    83,    79,    80,    81,    82,    79,    80,    81,    82,
     231,     6,     5,     8,    10,    16,    10,   246,   324,   325,
     171,   172,    18,    57,    18,    58,    18,    17,    52,    63,
      65,     3,     3,    57,    28,   341,    69,     7,    28,    63,
      64,    10,    66,    10,    68,    18,    29,   129,   277,    18,
      51,    18,    53,    18,    47,    28,     6,    49,     8,    28,
      55,    17,    16,    28,   285,    61,   148,    61,   150,   151,
     152,   153,    28,   155,   156,   157,   158,     3,   160,   161,
     162,   163,    75,   165,   166,   167,   237,    59,    59,    16,
      62,    62,    61,    86,    61,    88,    89,    16,    52,    56,
      57,    65,   179,    57,    17,    69,   179,    51,    52,    63,
      64,    65,    66,    65,    68,    28,    57,    69,    45,    46,
      47,    48,    63,    64,    65,    52,    63,    64,    69,    17,
      57,    91,    92,    52,    31,    65,    63,    64,    65,    66,
      28,    68,    69,    52,    63,    64,    65,    66,    57,    68,
      22,    23,    24,    25,    63,    64,    51,    66,    53,    68,
       3,     3,     3,     3,   241,   247,   248,   249,   250,   251,
       3,     0,    37,    65,    65,     4,     5,     8,    65,    65,
       9,    10,    11,    12,    13,    14,    15,    31,    16,   230,
      19,    20,    21,   311,    57,    16,   314,    26,    27,    16,
      63,    64,    65,   321,   322,    16,    69,    36,   326,    57,
     328,    18,   330,   331,    28,    63,    64,    65,     3,   337,
       3,    69,    34,    38,   342,   343,    39,    40,    41,    42,
      43,    44,    57,    65,    16,    65,    35,     3,    63,    64,
      65,    54,    65,     3,    69,    17,    65,    28,    32,    65,
      30,    39,    17,    17,    17,    17,    65,    65,    68,    35,
      65,    17,    17,    17,    17,    17,    17,    17,    17,    16,
       3,    29,    18,    16,    65,    65,    65,    50,    65,    65,
      18,     6,    17,    17,    16,    33,    65,    17,    17,    17,
      17,    65,    18,     3,    18,    65,     3,    18,    17,    35,
       3,    60,     3,    60,     3,     3,    17,    52,    52,     3,
       3,    17,     3,    17,     3,    52,    18,   239,    49,   181,
     221,    63,    18,   262,    65,    65,    65,   293,   264,   211,
     214,   286,    -1,   214,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    65,    65,    -1,    65,    -1,    65,    65,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      86
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    72,     0,     4,     5,     9,    10,    11,    12,    13,
      14,    15,    19,    20,    21,    26,    27,    36,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    85,
      86,    93,    99,   100,   101,   121,     6,     8,    55,     6,
       8,    16,    45,    46,    47,    48,    52,    57,    63,    64,
      65,    66,    68,    69,   102,   104,   107,   109,    65,     7,
       3,    29,    31,    65,     3,     3,     3,     3,     3,    37,
      65,    65,     8,    65,    65,    57,    65,   104,   106,    16,
      16,    16,    16,   104,    28,    31,    18,   103,    56,    57,
     105,    58,    69,   108,     3,     3,    65,    65,    34,    38,
      16,    35,    65,     3,     3,   104,    28,    17,    57,    63,
      64,    65,    69,    89,    90,    65,    69,    89,    90,    65,
      69,    89,    90,    65,    69,    89,    90,   103,    65,    69,
      65,   102,   104,   104,   107,   107,    30,    32,   111,    65,
      68,    65,    88,    92,    65,    35,    63,    64,    17,    28,
      17,    17,    17,    17,    28,    17,    17,    17,    17,    28,
      17,    17,    17,    17,    28,    17,    17,    17,   103,    18,
      49,   110,   114,    16,    95,   106,   112,   113,     3,    39,
      29,    18,    87,    22,    23,    24,    25,    91,    16,    65,
     103,    65,    69,   103,   103,   103,   103,    65,   103,   103,
     103,   103,    65,   103,   103,   103,   103,    65,   103,   103,
     103,    65,    50,   111,   111,    52,    66,    68,    89,    90,
      97,    18,    94,    39,    40,    41,    42,    43,    44,    54,
     120,    33,    52,    66,    68,    89,    90,    98,     6,    88,
      17,    16,    51,    53,    65,    84,    16,    17,    17,    17,
      17,    17,   110,    65,     3,    59,    62,   116,   118,     3,
     116,   118,    18,    96,    95,     3,    51,    52,   106,   112,
     111,    65,    87,     3,    57,    89,    52,    18,    17,    84,
     103,   103,   103,   103,   103,    35,   115,    60,    60,     3,
       3,     3,     3,    97,    17,    94,    52,     3,     3,    17,
      84,     3,    17,   112,   114,    65,    65,    96,    51,    53,
       3,    10,    18,    28,    61,   117,    18,    28,   119,    52,
     117,    65,    65,   117,    65,    65,    10,    28,    61,   117,
      10,    61,   117,    28,   119,   119,   117,    65,   117,   117,
     117,    65,    10,    61,   117,   119,   117,   117
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    71,    72,    72,    73,    73,    73,    73,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73,    73,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    83,    84,    84,    85,    86,    87,    87,    88,    88,
      88,    88,    88,    88,    89,    89,    90,    90,    91,    91,
      91,    91,    92,    93,    94,    94,    95,    95,    96,    96,
      97,    97,    97,    97,    97,    98,    98,    98,    98,    98,
      99,   100,   101,   101,   101,   101,   101,   101,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102,   102,
     102,   102,   102,   103,   103,   104,   105,   105,   105,   106,
     106,   107,   108,   108,   108,   109,   109,   109,   109,   109,
     109,   109,   109,   110,   110,   111,   111,   112,   112,   112,
     113,   113,   113,   114,   114,   115,   115,   116,   116,   116,
     116,   116,   116,   117,   117,   117,   117,   117,   117,   117,
     118,   118,   119,   119,   119,   120,   120,   120,   120,   120,
     120,   121
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
       9,    10,     1,     3,     4,     8,     0,     3,     5,     2,
       7,     6,     4,     3,     1,     2,     1,     2,     1,     1,
       1,     1,     1,     7,     0,     3,     0,     4,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     8,     7,     7,     8,     8,     8,     8,     2,     3,
       5,     5,     7,     7,     5,     5,     5,     5,     7,     5,
       5,     5,     5,     7,     5,     5,     5,     5,     7,     5,
       5,     1,     4,     0,     2,     2,     0,     2,     2,     1,
       2,     2,     0,     2,     2,     1,     3,     1,     1,     1,
       1,     1,     3,     0,     3,     0,     2,     0,     1,     3,
       3,     3,     4,     4,     5,     0,     2,     4,     6,     5,
       7,     5,     7,     0,     3,     5,     4,     6,     4,     6,
       4,     6,     0,     3,     5,     1,     1,     1,     1,     1,
       1,     8
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


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

/* This macro is provided for backward compatibility. */
# ifndef YY_LOCATION_PRINT
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif


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
  YYUSE (yyoutput);
  YYUSE (scanner);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
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
  YYUSE (yyvaluep);
  YYUSE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
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
    goto yyexhaustedlab;
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
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
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
#line 211 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1499 "yacc_sql.tab.c"
    break;

  case 22: /* help: HELP SEMICOLON  */
#line 216 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1507 "yacc_sql.tab.c"
    break;

  case 23: /* sync: SYNC SEMICOLON  */
#line 221 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1515 "yacc_sql.tab.c"
    break;

  case 24: /* begin: TRX_BEGIN SEMICOLON  */
#line 227 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1523 "yacc_sql.tab.c"
    break;

  case 25: /* commit: TRX_COMMIT SEMICOLON  */
#line 233 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1531 "yacc_sql.tab.c"
    break;

  case 26: /* rollback: TRX_ROLLBACK SEMICOLON  */
#line 239 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1539 "yacc_sql.tab.c"
    break;

  case 27: /* drop_table: DROP TABLE ID SEMICOLON  */
#line 245 "yacc_sql.y"
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1548 "yacc_sql.tab.c"
    break;

  case 28: /* show_tables: SHOW TABLES SEMICOLON  */
#line 251 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1556 "yacc_sql.tab.c"
    break;

  case 29: /* desc_table: DESC ID SEMICOLON  */
#line 257 "yacc_sql.y"
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1565 "yacc_sql.tab.c"
    break;

  case 30: /* create_index: CREATE INDEX ID ON ID LBRACE field_name_list RBRACE SEMICOLON  */
#line 265 "yacc_sql.y"
                {
 			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string) ,0);
 		}
#line 1574 "yacc_sql.tab.c"
    break;

  case 31: /* create_index: CREATE UNIQUE INDEX ID ON ID LBRACE field_name_list RBRACE SEMICOLON  */
#line 270 "yacc_sql.y"
            {
 	        CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_unique_index";
            create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string) ,1);
 	    }
#line 1583 "yacc_sql.tab.c"
    break;

  case 32: /* field_name_list: ID  */
#line 278 "yacc_sql.y"
        {
            create_index_append_attribute(&CONTEXT->ssql->sstr.create_index,(yyvsp[0].string));
        }
#line 1591 "yacc_sql.tab.c"
    break;

  case 33: /* field_name_list: ID COMMA field_name_list  */
#line 282 "yacc_sql.y"
        {
            create_index_append_attribute(&CONTEXT->ssql->sstr.create_index,(yyvsp[-2].string));
        }
#line 1599 "yacc_sql.tab.c"
    break;

  case 34: /* drop_index: DROP INDEX ID SEMICOLON  */
#line 289 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1608 "yacc_sql.tab.c"
    break;

  case 35: /* create_table: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON  */
#line 296 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
#line 1620 "yacc_sql.tab.c"
    break;

  case 37: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 306 "yacc_sql.y"
                                   {    }
#line 1626 "yacc_sql.tab.c"
    break;

  case 38: /* attr_def: ID_get type LBRACE number RBRACE  */
#line 311 "yacc_sql.y"
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
#line 1641 "yacc_sql.tab.c"
    break;

  case 39: /* attr_def: ID_get type  */
#line 322 "yacc_sql.y"
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
#line 1656 "yacc_sql.tab.c"
    break;

  case 40: /* attr_def: ID_get type LBRACE number RBRACE NOT NULL_A  */
#line 336 "yacc_sql.y"
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
#line 1672 "yacc_sql.tab.c"
    break;

  case 41: /* attr_def: ID_get type LBRACE number RBRACE NULLABLE  */
#line 348 "yacc_sql.y"
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
#line 1688 "yacc_sql.tab.c"
    break;

  case 42: /* attr_def: ID_get type NOT NULL_A  */
#line 360 "yacc_sql.y"
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
#line 1704 "yacc_sql.tab.c"
    break;

  case 43: /* attr_def: ID_get type NULLABLE  */
#line 372 "yacc_sql.y"
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
#line 1720 "yacc_sql.tab.c"
    break;

  case 44: /* number: NNNUMBER  */
#line 386 "yacc_sql.y"
             {(yyval.number) = (yyvsp[0].number);}
#line 1726 "yacc_sql.tab.c"
    break;

  case 45: /* number: MINUS NNNUMBER  */
#line 387 "yacc_sql.y"
                     {
      (yyval.number) = -((yyvsp[0].number)); 
    }
#line 1734 "yacc_sql.tab.c"
    break;

  case 46: /* floatnumber: NNFLOAT  */
#line 393 "yacc_sql.y"
          {(yyval.floatsAndStr) = (yyvsp[0].floatsAndStr); }
#line 1740 "yacc_sql.tab.c"
    break;

  case 47: /* floatnumber: MINUS NNFLOAT  */
#line 394 "yacc_sql.y"
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
#line 1755 "yacc_sql.tab.c"
    break;

  case 48: /* type: INT_T  */
#line 407 "yacc_sql.y"
              { (yyval.number)=INTS; }
#line 1761 "yacc_sql.tab.c"
    break;

  case 49: /* type: STRING_T  */
#line 408 "yacc_sql.y"
                  { (yyval.number)=CHARS; }
#line 1767 "yacc_sql.tab.c"
    break;

  case 50: /* type: FLOAT_T  */
#line 409 "yacc_sql.y"
                 { (yyval.number)=FLOATS; }
#line 1773 "yacc_sql.tab.c"
    break;

  case 51: /* type: DATE_T  */
#line 410 "yacc_sql.y"
                    { (yyval.number)=DATES; }
#line 1779 "yacc_sql.tab.c"
    break;

  case 52: /* ID_get: ID  */
#line 414 "yacc_sql.y"
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1788 "yacc_sql.tab.c"
    break;

  case 53: /* insert: INSERT INTO ID VALUES value_tuple values_lists SEMICOLON  */
#line 424 "yacc_sql.y"
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
#line 1811 "yacc_sql.tab.c"
    break;

  case 54: /* values_lists: %empty  */
#line 444 "yacc_sql.y"
               {
        //CONTEXT->value_list_length++;
    }
#line 1819 "yacc_sql.tab.c"
    break;

  case 55: /* values_lists: COMMA value_tuple values_lists  */
#line 447 "yacc_sql.y"
                                    {
  	     //CONTEXT->value_list_length++;
	  }
#line 1827 "yacc_sql.tab.c"
    break;

  case 57: /* value_tuple: LBRACE insert_value value_list RBRACE  */
#line 454 "yacc_sql.y"
                                             {
        CONTEXT->value_list_length++;
	  }
#line 1835 "yacc_sql.tab.c"
    break;

  case 59: /* value_list: COMMA insert_value value_list  */
#line 461 "yacc_sql.y"
                                   {
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1843 "yacc_sql.tab.c"
    break;

  case 60: /* insert_value: number  */
#line 468 "yacc_sql.y"
           {
  		value_init_integer(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], (yyvsp[0].number));
		}
#line 1851 "yacc_sql.tab.c"
    break;

  case 61: /* insert_value: floatnumber  */
#line 471 "yacc_sql.y"
                 {
  		value_init_float(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], ((yyvsp[0].floatsAndStr)).floats);
		}
#line 1859 "yacc_sql.tab.c"
    break;

  case 62: /* insert_value: SSS  */
#line 474 "yacc_sql.y"
         {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], (yyvsp[0].string));
		}
#line 1868 "yacc_sql.tab.c"
    break;

  case 63: /* insert_value: DATE  */
#line 478 "yacc_sql.y"
              {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_date(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], (yyvsp[0].string));
	}
#line 1877 "yacc_sql.tab.c"
    break;

  case 64: /* insert_value: NULL_A  */
#line 483 "yacc_sql.y"
                {
		// value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
		value_init_null(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++]); // 多条插入
	}
#line 1886 "yacc_sql.tab.c"
    break;

  case 65: /* value: number  */
#line 492 "yacc_sql.y"
           {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number));
		}
#line 1894 "yacc_sql.tab.c"
    break;

  case 66: /* value: floatnumber  */
#line 495 "yacc_sql.y"
                {
  		// value_init_float(&CONTEXT->values[CONTEXT->value_length++], ($1).floats);
		value_init_float2(&CONTEXT->values[CONTEXT->value_length++], ((yyvsp[0].floatsAndStr)).floats, ((yyvsp[0].floatsAndStr)).str);
	}
#line 1903 "yacc_sql.tab.c"
    break;

  case 67: /* value: SSS  */
#line 499 "yacc_sql.y"
         {
			(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
		}
#line 1912 "yacc_sql.tab.c"
    break;

  case 68: /* value: DATE  */
#line 503 "yacc_sql.y"
              {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_date(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
	}
#line 1921 "yacc_sql.tab.c"
    break;

  case 69: /* value: NULL_A  */
#line 508 "yacc_sql.y"
                {
		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
	}
#line 1929 "yacc_sql.tab.c"
    break;

  case 70: /* delete: DELETE FROM ID where SEMICOLON  */
#line 517 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->condition_exps, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;	
    }
#line 1941 "yacc_sql.tab.c"
    break;

  case 71: /* update: UPDATE ID SET ID EQ value where SEMICOLON  */
#line 527 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value, 
					CONTEXT->condition_exps, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
		}
#line 1953 "yacc_sql.tab.c"
    break;

  case 72: /* select: SELECT relattrexp FROM ID rel_list where SEMICOLON  */
#line 538 "yacc_sql.y"
                                                     {  
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-3].string));
    CONTEXT->ssql->flag=SCF_SELECT;
    advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps, CONTEXT->condition_length);
    //临时变量清零
    CONTEXT->condition_length=0;
    CONTEXT->from_length=0;
    CONTEXT->select_length=0;
    CONTEXT->value_length = 0;
  }
#line 1969 "yacc_sql.tab.c"
    break;

  case 73: /* select: SELECT relattrexp FROM ID join where SEMICOLON  */
#line 550 "yacc_sql.y"
  {
     AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
     adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-3].string));
     CONTEXT->ssql->flag=SCF_SELECT;
     advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps, CONTEXT->condition_length);
     //临时变量清零
     CONTEXT->condition_length=0;
     CONTEXT->from_length=0;
     CONTEXT->select_length=0;
     CONTEXT->value_length = 0;
  }
#line 1985 "yacc_sql.tab.c"
    break;

  case 74: /* select: SELECT relattrexp FROM ID rel_list where order SEMICOLON  */
#line 561 "yacc_sql.y"
                                                             {  
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-4].string));
    CONTEXT->ssql->flag=SCF_SELECT;
    advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps, CONTEXT->condition_length);
    //临时变量清零
    CONTEXT->condition_length=0;
    CONTEXT->from_length=0;
    CONTEXT->select_length=0;
    CONTEXT->value_length = 0;
  }
#line 2001 "yacc_sql.tab.c"
    break;

  case 75: /* select: SELECT relattrexp FROM ID join where order SEMICOLON  */
#line 573 "yacc_sql.y"
  {
     AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
     adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-4].string));
     CONTEXT->ssql->flag=SCF_SELECT;
     advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps, CONTEXT->condition_length);
     //临时变量清零
     CONTEXT->condition_length=0;
     CONTEXT->from_length=0;
     CONTEXT->select_length=0;
     CONTEXT->value_length = 0;
  }
#line 2017 "yacc_sql.tab.c"
    break;

  case 76: /* select: SELECT relattrexp FROM ID rel_list where group SEMICOLON  */
#line 584 "yacc_sql.y"
                                                             {  
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-4].string));
    CONTEXT->ssql->flag=SCF_SELECT;
    advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps, CONTEXT->condition_length);
    //临时变量清零
    CONTEXT->condition_length=0;
    CONTEXT->from_length=0;
    CONTEXT->select_length=0;
    CONTEXT->value_length = 0;
  }
#line 2033 "yacc_sql.tab.c"
    break;

  case 77: /* select: SELECT relattrexp FROM ID join where group SEMICOLON  */
#line 596 "yacc_sql.y"
  {
     AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
     adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-4].string));
     CONTEXT->ssql->flag=SCF_SELECT;
     advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps, CONTEXT->condition_length);
     //临时变量清零
     CONTEXT->condition_length=0;
     CONTEXT->from_length=0;
     CONTEXT->select_length=0;
     CONTEXT->value_length = 0;
  }
#line 2049 "yacc_sql.tab.c"
    break;

  case 78: /* relattrexp: exp_list relattrexp2  */
#line 610 "yacc_sql.y"
                       {
    RelAttrExp exp;
    exp.agg_type = NOTAGG;
    exp.is_star = 0;
    exp.explist = (ExpList *)((yyvsp[-1].exp_list));
    exp.num = exp.explist->num;
    advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
  }
#line 2062 "yacc_sql.tab.c"
    break;

  case 79: /* relattrexp: MINUS exp_list relattrexp2  */
#line 618 "yacc_sql.y"
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
    advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
  }
#line 2081 "yacc_sql.tab.c"
    break;

  case 80: /* relattrexp: COUNT LBRACE ID RBRACE relattrexp2  */
#line 636 "yacc_sql.y"
                                         {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2091 "yacc_sql.tab.c"
    break;

  case 81: /* relattrexp: COUNT LBRACE STAR RBRACE relattrexp2  */
#line 641 "yacc_sql.y"
                                               {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2101 "yacc_sql.tab.c"
    break;

  case 82: /* relattrexp: COUNT LBRACE ID DOT ID RBRACE relattrexp2  */
#line 646 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2111 "yacc_sql.tab.c"
    break;

  case 83: /* relattrexp: COUNT LBRACE ID DOT STAR RBRACE relattrexp2  */
#line 651 "yacc_sql.y"
                                                      {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, (yyvsp[-4].string), "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2121 "yacc_sql.tab.c"
    break;

  case 84: /* relattrexp: COUNT LBRACE number RBRACE relattrexp2  */
#line 656 "yacc_sql.y"
                                                 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2134 "yacc_sql.tab.c"
    break;

  case 85: /* relattrexp: COUNT LBRACE floatnumber RBRACE relattrexp2  */
#line 664 "yacc_sql.y"
                                                      {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2148 "yacc_sql.tab.c"
    break;

  case 86: /* relattrexp: MAX LBRACE ID RBRACE relattrexp2  */
#line 673 "yacc_sql.y"
                                       {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2158 "yacc_sql.tab.c"
    break;

  case 87: /* relattrexp: MAX LBRACE STAR RBRACE relattrexp2  */
#line 678 "yacc_sql.y"
                                             {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2168 "yacc_sql.tab.c"
    break;

  case 88: /* relattrexp: MAX LBRACE ID DOT ID RBRACE relattrexp2  */
#line 683 "yacc_sql.y"
                                                  {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2178 "yacc_sql.tab.c"
    break;

  case 89: /* relattrexp: MAX LBRACE number RBRACE relattrexp2  */
#line 688 "yacc_sql.y"
                                               {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2191 "yacc_sql.tab.c"
    break;

  case 90: /* relattrexp: MAX LBRACE floatnumber RBRACE relattrexp2  */
#line 696 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2205 "yacc_sql.tab.c"
    break;

  case 91: /* relattrexp: MIN LBRACE ID RBRACE relattrexp2  */
#line 705 "yacc_sql.y"
                                       {  
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2215 "yacc_sql.tab.c"
    break;

  case 92: /* relattrexp: MIN LBRACE STAR RBRACE relattrexp2  */
#line 710 "yacc_sql.y"
                                             {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2225 "yacc_sql.tab.c"
    break;

  case 93: /* relattrexp: MIN LBRACE ID DOT ID RBRACE relattrexp2  */
#line 715 "yacc_sql.y"
                                                  {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2235 "yacc_sql.tab.c"
    break;

  case 94: /* relattrexp: MIN LBRACE number RBRACE relattrexp2  */
#line 720 "yacc_sql.y"
                                               { 
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2248 "yacc_sql.tab.c"
    break;

  case 95: /* relattrexp: MIN LBRACE floatnumber RBRACE relattrexp2  */
#line 728 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2262 "yacc_sql.tab.c"
    break;

  case 96: /* relattrexp: AVG LBRACE ID RBRACE relattrexp2  */
#line 737 "yacc_sql.y"
                                       {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2272 "yacc_sql.tab.c"
    break;

  case 97: /* relattrexp: AVG LBRACE STAR RBRACE relattrexp2  */
#line 742 "yacc_sql.y"
                                             {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2282 "yacc_sql.tab.c"
    break;

  case 98: /* relattrexp: AVG LBRACE ID DOT ID RBRACE relattrexp2  */
#line 747 "yacc_sql.y"
                                                  {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2292 "yacc_sql.tab.c"
    break;

  case 99: /* relattrexp: AVG LBRACE number RBRACE relattrexp2  */
#line 752 "yacc_sql.y"
                                               {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2305 "yacc_sql.tab.c"
    break;

  case 100: /* relattrexp: AVG LBRACE floatnumber RBRACE relattrexp2  */
#line 760 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2319 "yacc_sql.tab.c"
    break;

  case 101: /* relattrexp: STAR  */
#line 771 "yacc_sql.y"
           {
      RelAttrExp exp;
      exp.agg_type = NOTAGG;
      exp.is_star = 1;
      exp.relation_name = NULL;
      advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
    }
#line 2331 "yacc_sql.tab.c"
    break;

  case 102: /* relattrexp: ID DOT STAR relattrexp2  */
#line 778 "yacc_sql.y"
                              {
      RelAttrExp exp;
      exp.agg_type = NOTAGG;
      exp.is_star = 1;
      exp.relation_name = strdup((yyvsp[-3].string));
      advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
    }
#line 2343 "yacc_sql.tab.c"
    break;

  case 105: /* exp_list: exps exp_list2  */
#line 796 "yacc_sql.y"
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
#line 2368 "yacc_sql.tab.c"
    break;

  case 106: /* exp_list2: %empty  */
#line 819 "yacc_sql.y"
             {
    (yyval.exp_list) = NULL;
  }
#line 2376 "yacc_sql.tab.c"
    break;

  case 107: /* exp_list2: PLUS exp_list  */
#line 822 "yacc_sql.y"
                  {
    ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
    while(explist_right->left_explist) {
      explist_right = explist_right->left_explist;
    }
    explist_right->calop = PLUS_OP;
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2389 "yacc_sql.tab.c"
    break;

  case 108: /* exp_list2: MINUS exp_list  */
#line 830 "yacc_sql.y"
                   {
    ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
    while(explist_right->left_explist) {
      explist_right = explist_right->left_explist;
    }
    explist_right->calop = MINUS_OP;
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2402 "yacc_sql.tab.c"
    break;

  case 109: /* exp_list3: exp_list  */
#line 841 "yacc_sql.y"
           {
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2410 "yacc_sql.tab.c"
    break;

  case 110: /* exp_list3: MINUS exp_list  */
#line 844 "yacc_sql.y"
                   {
    ExpList *explist = (ExpList *)((yyvsp[0].exp_list));
    while(explist->left_explist) {
      explist = explist->left_explist;
    }
    explist->exp->have_negative = 1;
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2423 "yacc_sql.tab.c"
    break;

  case 111: /* exps: exp exps2  */
#line 858 "yacc_sql.y"
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
#line 2443 "yacc_sql.tab.c"
    break;

  case 112: /* exps2: %empty  */
#line 876 "yacc_sql.y"
             {
    (yyval.exp) = NULL;
  }
#line 2451 "yacc_sql.tab.c"
    break;

  case 113: /* exps2: STAR exps  */
#line 879 "yacc_sql.y"
              {
    Exp *exp_right = (Exp *)((yyvsp[0].exp));
    while(exp_right->left_exp) {
      exp_right = exp_right->left_exp;
    }
    exp_right->calop = TIME_OP;
    (yyval.exp) = (yyvsp[0].exp);
  }
#line 2464 "yacc_sql.tab.c"
    break;

  case 114: /* exps2: DIVIDE exps  */
#line 887 "yacc_sql.y"
                {
    Exp *exp_right = (Exp *)((yyvsp[0].exp));
    while(exp_right->left_exp) {
      exp_right = exp_right->left_exp;
    }
    exp_right->calop = DIVIDE_OP;
    (yyval.exp) = (yyvsp[0].exp);
  }
#line 2477 "yacc_sql.tab.c"
    break;

  case 115: /* exp: ID  */
#line 900 "yacc_sql.y"
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
    (yyval.exp) = (void *)exp;
  }
#line 2495 "yacc_sql.tab.c"
    break;

  case 116: /* exp: ID DOT ID  */
#line 913 "yacc_sql.y"
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
    (yyval.exp) = (void *)exp;
  }
#line 2513 "yacc_sql.tab.c"
    break;

  case 117: /* exp: NNNUMBER  */
#line 926 "yacc_sql.y"
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
    (yyval.exp) = (void *)exp;
  }
#line 2533 "yacc_sql.tab.c"
    break;

  case 118: /* exp: NNFLOAT  */
#line 941 "yacc_sql.y"
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
    (yyval.exp) = (void *)exp;
  }
#line 2553 "yacc_sql.tab.c"
    break;

  case 119: /* exp: SSS  */
#line 956 "yacc_sql.y"
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
    (yyval.exp) = (void *)exp;
  }
#line 2574 "yacc_sql.tab.c"
    break;

  case 120: /* exp: DATE  */
#line 972 "yacc_sql.y"
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
    (yyval.exp) = (void *)exp;
  }
#line 2595 "yacc_sql.tab.c"
    break;

  case 121: /* exp: NULL_A  */
#line 988 "yacc_sql.y"
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
    (yyval.exp) = (void *)exp;
  }
#line 2615 "yacc_sql.tab.c"
    break;

  case 122: /* exp: LBRACE exp_list3 RBRACE  */
#line 1003 "yacc_sql.y"
                            {
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 1;
    exp->have_negative = 0;
    exp->explist = (ExpList *)((yyvsp[-1].exp_list));
    exp->num = exp->explist->num;
    exp->left_exp = NULL;
    (yyval.exp) = (void *)exp;
  }
#line 2629 "yacc_sql.tab.c"
    break;

  case 124: /* rel_list: COMMA ID rel_list  */
#line 1016 "yacc_sql.y"
                        {	
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
        adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-1].string));
		  }
#line 2638 "yacc_sql.tab.c"
    break;

  case 126: /* where: WHERE condition_exps  */
#line 1024 "yacc_sql.y"
                         {
  
  }
#line 2646 "yacc_sql.tab.c"
    break;

  case 128: /* condition_exps: condition_exp  */
#line 1030 "yacc_sql.y"
                  {
  
  }
#line 2654 "yacc_sql.tab.c"
    break;

  case 129: /* condition_exps: condition_exp AND condition_exps  */
#line 1033 "yacc_sql.y"
                                     {
  
  }
#line 2662 "yacc_sql.tab.c"
    break;

  case 130: /* condition_exp: exp_list3 comOp exp_list3  */
#line 1040 "yacc_sql.y"
                            {
    ExpList *explist_left = (ExpList *)((yyvsp[-2].exp_list));
    ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
    ConditionExp cond_exp;
    cond_exp.left = explist_left;
    cond_exp.right = explist_right;
    cond_exp.comp = CONTEXT->comp;
    CONTEXT->condition_exps[CONTEXT->condition_length++] = cond_exp;
  }
#line 2676 "yacc_sql.tab.c"
    break;

  case 131: /* condition_exp: exp_list3 IS_A NULL_A  */
#line 1050 "yacc_sql.y"
                                {
		ExpList *explist_left = (ExpList *)((yyvsp[-2].exp_list));
		ExpList *explist_right = (ExpList *)malloc(sizeof(ExpList));
		
		explist_init_for_null(explist_right);
		
		ConditionExp cond_exp;
		cond_exp.left = explist_left;
		cond_exp.right = explist_right;
		cond_exp.comp = IS;
		CONTEXT->condition_exps[CONTEXT->condition_length++] = cond_exp;
	}
#line 2693 "yacc_sql.tab.c"
    break;

  case 132: /* condition_exp: exp_list3 IS_A NOT NULL_A  */
#line 1062 "yacc_sql.y"
                                    {
		ExpList *explist_left = (ExpList *)((yyvsp[-3].exp_list));
		ExpList *explist_right = (ExpList *)malloc(sizeof(ExpList));
		
		explist_init_for_null(explist_right);
		
		ConditionExp cond_exp;
		cond_exp.left = explist_left;
		cond_exp.right = explist_right;
		cond_exp.comp = ISNOT;
		CONTEXT->condition_exps[CONTEXT->condition_length++] = cond_exp;
	}
#line 2710 "yacc_sql.tab.c"
    break;

  case 133: /* join: INNER JOIN ID onwhere  */
#line 1078 "yacc_sql.y"
                          {
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
        adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-1].string));
    }
#line 2719 "yacc_sql.tab.c"
    break;

  case 134: /* join: INNER JOIN ID onwhere join  */
#line 1082 "yacc_sql.y"
                                {
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
        adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-2].string));
    }
#line 2728 "yacc_sql.tab.c"
    break;

  case 136: /* onwhere: ON condition_exps  */
#line 1089 "yacc_sql.y"
                        {

    }
#line 2736 "yacc_sql.tab.c"
    break;

  case 137: /* order: ORDER BY ID order2  */
#line 1096 "yacc_sql.y"
                     {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2749 "yacc_sql.tab.c"
    break;

  case 138: /* order: ORDER BY ID DOT ID order2  */
#line 1104 "yacc_sql.y"
                              {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = strdup((yyvsp[-3].string));
    order_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2762 "yacc_sql.tab.c"
    break;

  case 139: /* order: ORDER BY ID ASC order2  */
#line 1112 "yacc_sql.y"
                           {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2775 "yacc_sql.tab.c"
    break;

  case 140: /* order: ORDER BY ID DOT ID ASC order2  */
#line 1120 "yacc_sql.y"
                                  {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = strdup((yyvsp[-4].string));
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2788 "yacc_sql.tab.c"
    break;

  case 141: /* order: ORDER BY ID DESC order2  */
#line 1128 "yacc_sql.y"
                            {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = DESCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2801 "yacc_sql.tab.c"
    break;

  case 142: /* order: ORDER BY ID DOT ID DESC order2  */
#line 1136 "yacc_sql.y"
                                   {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = DESCORDER;
    order_attr.relation_name = strdup((yyvsp[-4].string));
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2814 "yacc_sql.tab.c"
    break;

  case 144: /* order2: COMMA ID order2  */
#line 1148 "yacc_sql.y"
                  {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2827 "yacc_sql.tab.c"
    break;

  case 145: /* order2: COMMA ID DOT ID order2  */
#line 1156 "yacc_sql.y"
                           {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = strdup((yyvsp[-3].string));
    order_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2840 "yacc_sql.tab.c"
    break;

  case 146: /* order2: COMMA ID ASC order2  */
#line 1164 "yacc_sql.y"
                        {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2853 "yacc_sql.tab.c"
    break;

  case 147: /* order2: COMMA ID DOT ID ASC order2  */
#line 1172 "yacc_sql.y"
                               {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = strdup((yyvsp[-4].string));
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2866 "yacc_sql.tab.c"
    break;

  case 148: /* order2: COMMA ID DESC order2  */
#line 1180 "yacc_sql.y"
                         {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = DESCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2879 "yacc_sql.tab.c"
    break;

  case 149: /* order2: COMMA ID DOT ID DESC order2  */
#line 1188 "yacc_sql.y"
                                {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = DESCORDER;
    order_attr.relation_name = strdup((yyvsp[-4].string));
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2892 "yacc_sql.tab.c"
    break;

  case 150: /* group: GROUP BY ID group2  */
#line 1200 "yacc_sql.y"
                     {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    GroupAttr group_attr;
    group_attr.relation_name = NULL;
    group_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->group_attrs[adv_selects->group_num++] = group_attr;
  }
#line 2904 "yacc_sql.tab.c"
    break;

  case 151: /* group: GROUP BY ID DOT ID group2  */
#line 1207 "yacc_sql.y"
                              {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    GroupAttr group_attr;
    group_attr.relation_name = strdup((yyvsp[-3].string));
    group_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->group_attrs[adv_selects->group_num++] = group_attr;
  }
#line 2916 "yacc_sql.tab.c"
    break;

  case 153: /* group2: COMMA ID group2  */
#line 1217 "yacc_sql.y"
                    {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    GroupAttr group_attr;
    group_attr.relation_name = NULL;
    group_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->group_attrs[adv_selects->group_num++] = group_attr;
  }
#line 2928 "yacc_sql.tab.c"
    break;

  case 154: /* group2: COMMA ID DOT ID group2  */
#line 1224 "yacc_sql.y"
                           {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    GroupAttr group_attr;
    group_attr.relation_name = strdup((yyvsp[-3].string));
    group_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->group_attrs[adv_selects->group_num++] = group_attr;
  }
#line 2940 "yacc_sql.tab.c"
    break;

  case 155: /* comOp: EQ  */
#line 1236 "yacc_sql.y"
             { CONTEXT->comp = EQUAL_TO; }
#line 2946 "yacc_sql.tab.c"
    break;

  case 156: /* comOp: LT  */
#line 1237 "yacc_sql.y"
         { CONTEXT->comp = LESS_THAN; }
#line 2952 "yacc_sql.tab.c"
    break;

  case 157: /* comOp: GT  */
#line 1238 "yacc_sql.y"
         { CONTEXT->comp = GREAT_THAN; }
#line 2958 "yacc_sql.tab.c"
    break;

  case 158: /* comOp: LE  */
#line 1239 "yacc_sql.y"
         { CONTEXT->comp = LESS_EQUAL; }
#line 2964 "yacc_sql.tab.c"
    break;

  case 159: /* comOp: GE  */
#line 1240 "yacc_sql.y"
         { CONTEXT->comp = GREAT_EQUAL; }
#line 2970 "yacc_sql.tab.c"
    break;

  case 160: /* comOp: NE  */
#line 1241 "yacc_sql.y"
         { CONTEXT->comp = NOT_EQUAL; }
#line 2976 "yacc_sql.tab.c"
    break;

  case 161: /* load_data: LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON  */
#line 1246 "yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 2985 "yacc_sql.tab.c"
    break;


#line 2989 "yacc_sql.tab.c"

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
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (scanner, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
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

#line 1251 "yacc_sql.y"

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
