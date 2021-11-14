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
  YYSYMBOL_PLUS = 57,                      /* PLUS  */
  YYSYMBOL_MINUS = 58,                     /* MINUS  */
  YYSYMBOL_DIVIDE = 59,                    /* DIVIDE  */
  YYSYMBOL_ORDER = 60,                     /* ORDER  */
  YYSYMBOL_BY = 61,                        /* BY  */
  YYSYMBOL_ASC = 62,                       /* ASC  */
  YYSYMBOL_GROUP = 63,                     /* GROUP  */
  YYSYMBOL_NNNUMBER = 64,                  /* NNNUMBER  */
  YYSYMBOL_NNFLOAT = 65,                   /* NNFLOAT  */
  YYSYMBOL_ID = 66,                        /* ID  */
  YYSYMBOL_DATE = 67,                      /* DATE  */
  YYSYMBOL_PATH = 68,                      /* PATH  */
  YYSYMBOL_SSS = 69,                       /* SSS  */
  YYSYMBOL_STAR = 70,                      /* STAR  */
  YYSYMBOL_STRING_V = 71,                  /* STRING_V  */
  YYSYMBOL_YYACCEPT = 72,                  /* $accept  */
  YYSYMBOL_commands = 73,                  /* commands  */
  YYSYMBOL_command = 74,                   /* command  */
  YYSYMBOL_exit = 75,                      /* exit  */
  YYSYMBOL_help = 76,                      /* help  */
  YYSYMBOL_sync = 77,                      /* sync  */
  YYSYMBOL_begin = 78,                     /* begin  */
  YYSYMBOL_commit = 79,                    /* commit  */
  YYSYMBOL_rollback = 80,                  /* rollback  */
  YYSYMBOL_drop_table = 81,                /* drop_table  */
  YYSYMBOL_show_tables = 82,               /* show_tables  */
  YYSYMBOL_desc_table = 83,                /* desc_table  */
  YYSYMBOL_create_index = 84,              /* create_index  */
  YYSYMBOL_field_name_list = 85,           /* field_name_list  */
  YYSYMBOL_drop_index = 86,                /* drop_index  */
  YYSYMBOL_create_table = 87,              /* create_table  */
  YYSYMBOL_attr_def_list = 88,             /* attr_def_list  */
  YYSYMBOL_attr_def = 89,                  /* attr_def  */
  YYSYMBOL_number = 90,                    /* number  */
  YYSYMBOL_floatnumber = 91,               /* floatnumber  */
  YYSYMBOL_text = 92,                      /* text  */
  YYSYMBOL_type = 93,                      /* type  */
  YYSYMBOL_ID_get = 94,                    /* ID_get  */
  YYSYMBOL_insert = 95,                    /* insert  */
  YYSYMBOL_values_lists = 96,              /* values_lists  */
  YYSYMBOL_value_tuple = 97,               /* value_tuple  */
  YYSYMBOL_value_list = 98,                /* value_list  */
  YYSYMBOL_insert_value = 99,              /* insert_value  */
  YYSYMBOL_value = 100,                    /* value  */
  YYSYMBOL_delete = 101,                   /* delete  */
  YYSYMBOL_update = 102,                   /* update  */
  YYSYMBOL_select = 103,                   /* select  */
  YYSYMBOL_relattrexp = 104,               /* relattrexp  */
  YYSYMBOL_relattrexp2 = 105,              /* relattrexp2  */
  YYSYMBOL_exp_list = 106,                 /* exp_list  */
  YYSYMBOL_exp_list2 = 107,                /* exp_list2  */
  YYSYMBOL_exp_list3 = 108,                /* exp_list3  */
  YYSYMBOL_exps = 109,                     /* exps  */
  YYSYMBOL_exps2 = 110,                    /* exps2  */
  YYSYMBOL_exp = 111,                      /* exp  */
  YYSYMBOL_rel_list = 112,                 /* rel_list  */
  YYSYMBOL_where = 113,                    /* where  */
  YYSYMBOL_condition_exps = 114,           /* condition_exps  */
  YYSYMBOL_condition_exp = 115,            /* condition_exp  */
  YYSYMBOL_join = 116,                     /* join  */
  YYSYMBOL_onwhere = 117,                  /* onwhere  */
  YYSYMBOL_order = 118,                    /* order  */
  YYSYMBOL_order2 = 119,                   /* order2  */
  YYSYMBOL_group = 120,                    /* group  */
  YYSYMBOL_group2 = 121,                   /* group2  */
  YYSYMBOL_comOp = 122,                    /* comOp  */
  YYSYMBOL_load_data = 123                 /* load_data  */
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
#define YYLAST   373

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  52
/* YYNRULES -- Number of rules.  */
#define YYNRULES  164
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  352

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   326


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
      65,    66,    67,    68,    69,    70,    71
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   187,   187,   189,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   213,   218,   223,   229,   235,   241,   247,   253,   259,
     266,   271,   279,   283,   290,   297,   306,   308,   312,   323,
     334,   348,   360,   372,   384,   396,   411,   412,   418,   419,
     432,   436,   437,   438,   439,   442,   452,   473,   476,   481,
     483,   488,   490,   497,   500,   503,   507,   512,   521,   524,
     528,   532,   537,   545,   555,   567,   578,   590,   601,   613,
     624,   639,   647,   665,   670,   675,   680,   685,   693,   702,
     707,   712,   717,   725,   734,   739,   744,   749,   757,   766,
     771,   776,   781,   789,   800,   807,   816,   818,   825,   848,
     851,   859,   870,   873,   887,   905,   908,   916,   929,   942,
     955,   970,   985,  1001,  1017,  1032,  1043,  1045,  1051,  1053,
    1058,  1059,  1062,  1069,  1079,  1091,  1107,  1111,  1116,  1118,
    1125,  1133,  1141,  1149,  1157,  1165,  1175,  1177,  1185,  1193,
    1201,  1209,  1217,  1229,  1236,  1244,  1246,  1253,  1265,  1266,
    1267,  1268,  1269,  1270,  1274
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
  "NULL_A", "NULLABLE", "IS_A", "UNIQUE", "PLUS", "MINUS", "DIVIDE",
  "ORDER", "BY", "ASC", "GROUP", "NNNUMBER", "NNFLOAT", "ID", "DATE",
  "PATH", "SSS", "STAR", "STRING_V", "$accept", "commands", "command",
  "exit", "help", "sync", "begin", "commit", "rollback", "drop_table",
  "show_tables", "desc_table", "create_index", "field_name_list",
  "drop_index", "create_table", "attr_def_list", "attr_def", "number",
  "floatnumber", "text", "type", "ID_get", "insert", "values_lists",
  "value_tuple", "value_list", "insert_value", "value", "delete", "update",
  "select", "relattrexp", "relattrexp2", "exp_list", "exp_list2",
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
     325,   326
};
#endif

#define YYPACT_NINF (-305)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -305,   173,  -305,     8,   130,    61,    33,    30,   115,    70,
      91,    78,   151,   160,   162,   176,   177,   138,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -305,  -305,  -305,   123,   124,   187,   132,
     136,    76,   165,   192,   198,   201,  -305,    82,  -305,  -305,
     194,  -305,  -305,  -305,   193,   203,   -14,   -44,   223,   232,
    -305,   170,   171,   204,  -305,  -305,  -305,  -305,  -305,   199,
     224,   205,   179,   239,   240,    82,   217,  -305,   230,    -7,
      47,   139,   154,   203,   -10,   182,    61,  -305,    82,    82,
    -305,    82,    82,  -305,  -305,  -305,   218,   219,   184,   185,
     190,   191,   215,  -305,  -305,  -305,   195,  -305,    -3,  -305,
    -305,    23,   236,   238,   241,    72,   242,   243,   245,    86,
     246,   247,   248,    87,   249,   250,   251,  -305,  -305,   203,
      -1,  -305,  -305,  -305,  -305,  -305,   253,    76,   267,   231,
     244,  -305,   254,   134,   257,   209,  -305,  -305,   203,    54,
     203,   203,   203,   203,   210,   203,   203,   203,   203,   211,
     203,   203,   203,   203,   212,   203,   203,   203,  -305,   213,
     229,   219,   219,   -31,   263,   189,  -305,   252,  -305,    97,
     276,   190,   266,  -305,  -305,  -305,  -305,  -305,   233,    -6,
     221,   268,  -305,   271,   272,  -305,  -305,  -305,  -305,   273,
    -305,  -305,  -305,  -305,   274,  -305,  -305,  -305,  -305,   275,
    -305,  -305,  -305,   277,   227,    27,    28,  -305,  -305,  -305,
    -305,  -305,   278,   253,   291,  -305,  -305,  -305,  -305,  -305,
    -305,   100,    76,    76,  -305,  -305,  -305,  -305,  -305,   219,
     234,   254,   294,   255,   -29,   256,  -305,   280,   282,   221,
     203,   203,   203,   203,   203,  -305,   265,  -305,   258,   259,
     299,   300,  -305,   301,   302,   -31,   289,   263,  -305,   260,
    -305,  -305,  -305,   304,   307,  -305,  -305,  -305,   261,   295,
    -305,   221,   308,   297,  -305,  -305,  -305,  -305,  -305,    76,
     279,   262,   264,  -305,  -305,  -305,  -305,   278,  -305,  -305,
    -305,  -305,  -305,    85,  -305,  -305,   312,  -305,  -305,     3,
     103,  -305,   269,  -305,  -305,   298,   270,   281,   298,  -305,
     283,   284,  -305,  -305,  -305,    10,    32,  -305,   104,   303,
     298,   285,   298,  -305,   298,   298,  -305,   286,  -305,  -305,
    -305,    35,  -305,  -305,  -305,   303,   298,   298,  -305,  -305,
    -305,  -305
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
       0,     0,     0,     0,     0,     0,   124,     0,   120,   121,
     118,   123,   122,   104,     0,   106,   109,   115,     0,     0,
      23,     0,     0,     0,    24,    25,    26,    22,    21,     0,
       0,     0,     0,     0,     0,     0,   118,   112,     0,     0,
       0,     0,     0,   106,     0,     0,     0,    81,     0,     0,
     108,     0,     0,   114,    29,    28,     0,   128,     0,     0,
       0,     0,     0,    27,    34,   113,     0,   125,     0,    46,
      48,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    82,   119,   106,
     126,   107,   110,   111,   117,   116,    59,   130,     0,     0,
       0,    55,    36,     0,     0,     0,    47,    49,   106,     0,
     106,   106,   106,   106,     0,   106,   106,   106,   106,     0,
     106,   106,   106,   106,     0,   106,   106,   106,   105,     0,
       0,   128,   128,     0,    57,     0,   129,   131,    73,     0,
       0,     0,     0,    51,    52,    53,    54,    50,    40,    39,
       0,     0,    83,     0,     0,    84,    87,    88,    89,     0,
      90,    92,    93,    94,     0,    95,    97,    98,    99,     0,
     100,   102,   103,   126,     0,     0,     0,    67,    66,    65,
      63,    64,    61,    59,     0,   158,   159,   160,   161,   162,
     163,     0,     0,   130,    72,    71,    70,    68,    69,   128,
       0,    36,     0,     0,     0,     0,    45,    32,     0,     0,
     106,   106,   106,   106,   106,   127,   138,    75,     0,     0,
       0,     0,    76,     0,     0,     0,     0,    57,    56,     0,
     134,   133,   132,     0,     0,    37,    35,    44,     0,     0,
      43,     0,     0,     0,    85,    86,    91,    96,   101,   130,
     136,     0,     0,    77,    79,    78,    80,    61,    60,    58,
     135,    74,   164,    38,    33,    30,     0,   139,   137,   146,
     155,    62,     0,    42,    31,   146,     0,     0,   146,   140,
       0,     0,   153,    41,   144,   146,   146,   142,   155,   155,
     146,     0,   146,   147,   146,   146,   141,     0,   156,   154,
     151,   146,   149,   145,   143,   155,   146,   146,   148,   157,
     152,   150
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,  -305,
    -305,  -305,  -305,  -226,  -305,  -305,    77,   142,   -77,   -73,
    -305,  -305,  -305,  -305,    50,   101,    29,    62,  -305,  -305,
    -305,  -305,   287,   -82,     7,  -305,   -41,    83,  -305,  -305,
     118,  -153,  -222,  -305,    42,  -305,   117,  -119,   119,  -304,
    -305,  -305
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,   248,    29,    30,   182,   142,   220,   221,
     188,   189,   143,    31,   224,   174,   266,   222,   239,    32,
      33,    34,    54,    87,    77,    90,   175,    56,    93,    57,
     171,   138,   176,   177,   172,   290,   260,   319,   261,   322,
     232,    35
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      78,   127,   113,   117,   121,   125,   114,   118,   122,   126,
     244,   272,    55,   315,    36,    91,    37,   169,   215,   216,
     330,   316,   217,   283,   338,   339,    92,   108,   316,   278,
     257,   262,   317,   109,   110,   109,   218,    59,   219,   331,
     148,   349,   334,    88,    89,   346,   245,   168,   246,   170,
     316,   108,   149,   316,    83,   304,   128,   109,   110,   111,
     129,   146,   147,   112,    38,   318,   192,   307,   195,   196,
     197,   198,   332,   200,   201,   202,   203,    41,   205,   206,
     207,   208,   105,   210,   211,   212,   273,   258,   258,   153,
     259,   259,    41,    55,   335,   132,   133,   347,    41,    58,
      61,   154,   237,   158,   163,   108,   238,    42,    43,    44,
      45,   109,   110,   115,    46,   159,   164,   116,    60,    47,
     193,   320,   320,    62,   194,    48,    49,    50,    51,    46,
      52,    53,   321,   337,    75,    46,    39,   312,    40,   313,
      48,    49,    76,    51,    63,    52,    48,    49,    76,    51,
     234,    52,   269,   270,    64,   108,   183,   184,   185,   186,
     187,   109,   110,    65,   235,    66,   236,   279,   284,   285,
     286,   287,   288,     2,   134,   135,    69,     3,     4,    67,
      68,    79,     5,     6,     7,     8,     9,    10,    11,    70,
      71,   271,    12,    13,    14,    72,   324,   108,    73,   327,
      15,    16,    74,   109,   110,   119,   333,   336,    80,   120,
      17,   340,   108,   342,    81,   343,   344,    82,   109,   110,
     123,    86,   348,    84,   124,    85,    94,   350,   351,   225,
     226,   227,   228,   229,   230,    95,    96,    97,    99,    98,
     100,   101,   103,   104,   231,   102,   106,   107,   130,   136,
     139,   145,   137,   150,   140,   151,   141,   144,   152,   155,
     156,   128,   157,   160,   161,   162,   165,   166,   167,   173,
     178,   179,   181,   190,   180,   191,   199,   204,   209,   213,
     214,   223,   240,   242,   249,   243,   233,   247,   250,   251,
     252,   253,   254,   256,   268,   169,   265,   276,   281,   282,
     274,   289,   293,   294,   295,   296,   298,   301,   277,   280,
     302,   305,   303,   300,   306,   314,   316,   299,   275,   291,
     292,   320,   323,   241,   267,   146,   311,   297,   309,   170,
     310,   255,   308,   263,     0,   264,   325,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   326,     0,   328,
     329,   341,   345,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   131
};

static const yytype_int16 yycheck[] =
{
      41,    83,    79,    80,    81,    82,    79,    80,    81,    82,
      16,   233,     5,    10,     6,    59,     8,    18,   171,   172,
      10,    18,    53,   249,   328,   329,    70,    58,    18,    58,
       3,     3,    29,    64,    65,    64,    67,     7,    69,    29,
      17,   345,    10,    57,    58,    10,    52,   129,    54,    50,
      18,    58,    29,    18,    47,   281,    66,    64,    65,    66,
      70,    64,    65,    70,    56,    62,   148,   289,   150,   151,
     152,   153,    62,   155,   156,   157,   158,    16,   160,   161,
     162,   163,    75,   165,   166,   167,   239,    60,    60,    17,
      63,    63,    16,    86,    62,    88,    89,    62,    16,    66,
      30,    29,   179,    17,    17,    58,   179,    46,    47,    48,
      49,    64,    65,    66,    53,    29,    29,    70,     3,    58,
      66,    18,    18,    32,    70,    64,    65,    66,    67,    53,
      69,    70,    29,    29,    58,    53,     6,    52,     8,    54,
      64,    65,    66,    67,    66,    69,    64,    65,    66,    67,
      53,    69,    52,    53,     3,    58,    22,    23,    24,    25,
      26,    64,    65,     3,    67,     3,    69,   244,   250,   251,
     252,   253,   254,     0,    91,    92,    38,     4,     5,     3,
       3,    16,     9,    10,    11,    12,    13,    14,    15,    66,
      66,   232,    19,    20,    21,     8,   315,    58,    66,   318,
      27,    28,    66,    64,    65,    66,   325,   326,    16,    70,
      37,   330,    58,   332,    16,   334,   335,    16,    64,    65,
      66,    18,   341,    29,    70,    32,     3,   346,   347,    40,
      41,    42,    43,    44,    45,     3,    66,    66,    39,    35,
      16,    36,     3,     3,    55,    66,    29,    17,    66,    31,
      66,    36,    33,    17,    69,    17,    66,    66,    17,    17,
      17,    66,    17,    17,    17,    17,    17,    17,    17,    16,
       3,    40,    18,    16,    30,    66,    66,    66,    66,    66,
      51,    18,     6,    17,    16,    52,    34,    66,    17,    17,
      17,    17,    17,    66,     3,    18,    18,     3,    18,    17,
      66,    36,     3,     3,     3,     3,    17,     3,    53,    53,
       3,     3,    17,    53,    17,     3,    18,   267,   241,    61,
      61,    18,    53,   181,   223,    64,   297,   265,    66,    50,
      66,   213,   290,   216,    -1,   216,    66,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    66,    -1,    66,
      66,    66,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    86
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    73,     0,     4,     5,     9,    10,    11,    12,    13,
      14,    15,    19,    20,    21,    27,    28,    37,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    86,
      87,    95,   101,   102,   103,   123,     6,     8,    56,     6,
       8,    16,    46,    47,    48,    49,    53,    58,    64,    65,
      66,    67,    69,    70,   104,   106,   109,   111,    66,     7,
       3,    30,    32,    66,     3,     3,     3,     3,     3,    38,
      66,    66,     8,    66,    66,    58,    66,   106,   108,    16,
      16,    16,    16,   106,    29,    32,    18,   105,    57,    58,
     107,    59,    70,   110,     3,     3,    66,    66,    35,    39,
      16,    36,    66,     3,     3,   106,    29,    17,    58,    64,
      65,    66,    70,    90,    91,    66,    70,    90,    91,    66,
      70,    90,    91,    66,    70,    90,    91,   105,    66,    70,
      66,   104,   106,   106,   109,   109,    31,    33,   113,    66,
      69,    66,    89,    94,    66,    36,    64,    65,    17,    29,
      17,    17,    17,    17,    29,    17,    17,    17,    17,    29,
      17,    17,    17,    17,    29,    17,    17,    17,   105,    18,
      50,   112,   116,    16,    97,   108,   114,   115,     3,    40,
      30,    18,    88,    22,    23,    24,    25,    26,    92,    93,
      16,    66,   105,    66,    70,   105,   105,   105,   105,    66,
     105,   105,   105,   105,    66,   105,   105,   105,   105,    66,
     105,   105,   105,    66,    51,   113,   113,    53,    67,    69,
      90,    91,    99,    18,    96,    40,    41,    42,    43,    44,
      45,    55,   122,    34,    53,    67,    69,    90,    91,   100,
       6,    89,    17,    52,    16,    52,    54,    66,    85,    16,
      17,    17,    17,    17,    17,   112,    66,     3,    60,    63,
     118,   120,     3,   118,   120,    18,    98,    97,     3,    52,
      53,   108,   114,   113,    66,    88,     3,    53,    58,    90,
      53,    18,    17,    85,   105,   105,   105,   105,   105,    36,
     117,    61,    61,     3,     3,     3,     3,    99,    17,    96,
      53,     3,     3,    17,    85,     3,    17,   114,   116,    66,
      66,    98,    52,    54,     3,    10,    18,    29,    62,   119,
      18,    29,   121,    53,   119,    66,    66,   119,    66,    66,
      10,    29,    62,   119,    10,    62,   119,    29,   121,   121,
     119,    66,   119,   119,   119,    66,    10,    62,   119,   121,
     119,   119
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    72,    73,    73,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      84,    84,    85,    85,    86,    87,    88,    88,    89,    89,
      89,    89,    89,    89,    89,    89,    90,    90,    91,    91,
      92,    93,    93,    93,    93,    94,    95,    96,    96,    97,
      97,    98,    98,    99,    99,    99,    99,    99,   100,   100,
     100,   100,   100,   101,   102,   103,   103,   103,   103,   103,
     103,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   104,   104,   104,   104,
     104,   104,   104,   104,   104,   104,   105,   105,   106,   107,
     107,   107,   108,   108,   109,   110,   110,   110,   111,   111,
     111,   111,   111,   111,   111,   111,   112,   112,   113,   113,
     114,   114,   114,   115,   115,   115,   116,   116,   117,   117,
     118,   118,   118,   118,   118,   118,   119,   119,   119,   119,
     119,   119,   119,   120,   120,   121,   121,   121,   122,   122,
     122,   122,   122,   122,   123
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
       9,    10,     1,     3,     4,     8,     0,     3,     5,     2,
       2,     7,     6,     4,     4,     3,     1,     2,     1,     2,
       1,     1,     1,     1,     1,     1,     7,     0,     3,     0,
       4,     0,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     5,     8,     7,     7,     8,     8,     8,
       8,     2,     3,     5,     5,     7,     7,     5,     5,     5,
       5,     7,     5,     5,     5,     5,     7,     5,     5,     5,
       5,     7,     5,     5,     1,     4,     0,     2,     2,     0,
       2,     2,     1,     2,     2,     0,     2,     2,     1,     3,
       1,     1,     1,     1,     1,     3,     0,     3,     0,     2,
       0,     1,     3,     3,     3,     4,     4,     5,     0,     2,
       4,     6,     5,     7,     5,     7,     0,     3,     5,     4,
       6,     4,     6,     4,     6,     0,     3,     5,     1,     1,
       1,     1,     1,     1,     8
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
#line 213 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1505 "yacc_sql.tab.c"
    break;

  case 22: /* help: HELP SEMICOLON  */
#line 218 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1513 "yacc_sql.tab.c"
    break;

  case 23: /* sync: SYNC SEMICOLON  */
#line 223 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1521 "yacc_sql.tab.c"
    break;

  case 24: /* begin: TRX_BEGIN SEMICOLON  */
#line 229 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1529 "yacc_sql.tab.c"
    break;

  case 25: /* commit: TRX_COMMIT SEMICOLON  */
#line 235 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1537 "yacc_sql.tab.c"
    break;

  case 26: /* rollback: TRX_ROLLBACK SEMICOLON  */
#line 241 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1545 "yacc_sql.tab.c"
    break;

  case 27: /* drop_table: DROP TABLE ID SEMICOLON  */
#line 247 "yacc_sql.y"
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1554 "yacc_sql.tab.c"
    break;

  case 28: /* show_tables: SHOW TABLES SEMICOLON  */
#line 253 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1562 "yacc_sql.tab.c"
    break;

  case 29: /* desc_table: DESC ID SEMICOLON  */
#line 259 "yacc_sql.y"
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1571 "yacc_sql.tab.c"
    break;

  case 30: /* create_index: CREATE INDEX ID ON ID LBRACE field_name_list RBRACE SEMICOLON  */
#line 267 "yacc_sql.y"
                {
 			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string) ,0);
 		}
#line 1580 "yacc_sql.tab.c"
    break;

  case 31: /* create_index: CREATE UNIQUE INDEX ID ON ID LBRACE field_name_list RBRACE SEMICOLON  */
#line 272 "yacc_sql.y"
            {
 	        CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_unique_index";
            create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string) ,1);
 	    }
#line 1589 "yacc_sql.tab.c"
    break;

  case 32: /* field_name_list: ID  */
#line 280 "yacc_sql.y"
        {
            create_index_append_attribute(&CONTEXT->ssql->sstr.create_index,(yyvsp[0].string));
        }
#line 1597 "yacc_sql.tab.c"
    break;

  case 33: /* field_name_list: ID COMMA field_name_list  */
#line 284 "yacc_sql.y"
        {
            create_index_append_attribute(&CONTEXT->ssql->sstr.create_index,(yyvsp[-2].string));
        }
#line 1605 "yacc_sql.tab.c"
    break;

  case 34: /* drop_index: DROP INDEX ID SEMICOLON  */
#line 291 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1614 "yacc_sql.tab.c"
    break;

  case 35: /* create_table: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON  */
#line 298 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
#line 1626 "yacc_sql.tab.c"
    break;

  case 37: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 308 "yacc_sql.y"
                                   {    }
#line 1632 "yacc_sql.tab.c"
    break;

  case 38: /* attr_def: ID_get type LBRACE number RBRACE  */
#line 313 "yacc_sql.y"
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
#line 1647 "yacc_sql.tab.c"
    break;

  case 39: /* attr_def: ID_get type  */
#line 324 "yacc_sql.y"
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
#line 1662 "yacc_sql.tab.c"
    break;

  case 40: /* attr_def: ID_get text  */
#line 335 "yacc_sql.y"
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
#line 1677 "yacc_sql.tab.c"
    break;

  case 41: /* attr_def: ID_get type LBRACE number RBRACE NOT NULL_A  */
#line 349 "yacc_sql.y"
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
#line 1693 "yacc_sql.tab.c"
    break;

  case 42: /* attr_def: ID_get type LBRACE number RBRACE NULLABLE  */
#line 361 "yacc_sql.y"
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
#line 1709 "yacc_sql.tab.c"
    break;

  case 43: /* attr_def: ID_get type NOT NULL_A  */
#line 373 "yacc_sql.y"
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
#line 1725 "yacc_sql.tab.c"
    break;

  case 44: /* attr_def: ID_get text NOT NULL_A  */
#line 385 "yacc_sql.y"
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
#line 1741 "yacc_sql.tab.c"
    break;

  case 45: /* attr_def: ID_get type NULLABLE  */
#line 397 "yacc_sql.y"
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
#line 1757 "yacc_sql.tab.c"
    break;

  case 46: /* number: NNNUMBER  */
#line 411 "yacc_sql.y"
             {(yyval.number) = (yyvsp[0].number);}
#line 1763 "yacc_sql.tab.c"
    break;

  case 47: /* number: MINUS NNNUMBER  */
#line 412 "yacc_sql.y"
                     {
      (yyval.number) = -((yyvsp[0].number)); 
    }
#line 1771 "yacc_sql.tab.c"
    break;

  case 48: /* floatnumber: NNFLOAT  */
#line 418 "yacc_sql.y"
          {(yyval.floatsAndStr) = (yyvsp[0].floatsAndStr); }
#line 1777 "yacc_sql.tab.c"
    break;

  case 49: /* floatnumber: MINUS NNFLOAT  */
#line 419 "yacc_sql.y"
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
#line 1792 "yacc_sql.tab.c"
    break;

  case 50: /* text: TEXT_T  */
#line 432 "yacc_sql.y"
           { (yyval.number)=TEXT; }
#line 1798 "yacc_sql.tab.c"
    break;

  case 51: /* type: INT_T  */
#line 436 "yacc_sql.y"
              { (yyval.number)=INTS; }
#line 1804 "yacc_sql.tab.c"
    break;

  case 52: /* type: STRING_T  */
#line 437 "yacc_sql.y"
                  { (yyval.number)=CHARS; }
#line 1810 "yacc_sql.tab.c"
    break;

  case 53: /* type: FLOAT_T  */
#line 438 "yacc_sql.y"
                 { (yyval.number)=FLOATS; }
#line 1816 "yacc_sql.tab.c"
    break;

  case 54: /* type: DATE_T  */
#line 439 "yacc_sql.y"
                    { (yyval.number)=DATES; }
#line 1822 "yacc_sql.tab.c"
    break;

  case 55: /* ID_get: ID  */
#line 443 "yacc_sql.y"
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1831 "yacc_sql.tab.c"
    break;

  case 56: /* insert: INSERT INTO ID VALUES value_tuple values_lists SEMICOLON  */
#line 453 "yacc_sql.y"
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
#line 1854 "yacc_sql.tab.c"
    break;

  case 57: /* values_lists: %empty  */
#line 473 "yacc_sql.y"
               {
        //CONTEXT->value_list_length++;
    }
#line 1862 "yacc_sql.tab.c"
    break;

  case 58: /* values_lists: COMMA value_tuple values_lists  */
#line 476 "yacc_sql.y"
                                    {
  	     //CONTEXT->value_list_length++;
	  }
#line 1870 "yacc_sql.tab.c"
    break;

  case 60: /* value_tuple: LBRACE insert_value value_list RBRACE  */
#line 483 "yacc_sql.y"
                                             {
        CONTEXT->value_list_length++;
	  }
#line 1878 "yacc_sql.tab.c"
    break;

  case 62: /* value_list: COMMA insert_value value_list  */
#line 490 "yacc_sql.y"
                                   {
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1886 "yacc_sql.tab.c"
    break;

  case 63: /* insert_value: number  */
#line 497 "yacc_sql.y"
           {
  		value_init_integer(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], (yyvsp[0].number));
		}
#line 1894 "yacc_sql.tab.c"
    break;

  case 64: /* insert_value: floatnumber  */
#line 500 "yacc_sql.y"
                 {
  		value_init_float(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], ((yyvsp[0].floatsAndStr)).floats);
		}
#line 1902 "yacc_sql.tab.c"
    break;

  case 65: /* insert_value: SSS  */
#line 503 "yacc_sql.y"
         {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], (yyvsp[0].string));
		}
#line 1911 "yacc_sql.tab.c"
    break;

  case 66: /* insert_value: DATE  */
#line 507 "yacc_sql.y"
              {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_date(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], (yyvsp[0].string));
	}
#line 1920 "yacc_sql.tab.c"
    break;

  case 67: /* insert_value: NULL_A  */
#line 512 "yacc_sql.y"
                {
		// value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
		value_init_null(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++]); // 多条插入
	}
#line 1929 "yacc_sql.tab.c"
    break;

  case 68: /* value: number  */
#line 521 "yacc_sql.y"
           {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number));
		}
#line 1937 "yacc_sql.tab.c"
    break;

  case 69: /* value: floatnumber  */
#line 524 "yacc_sql.y"
                {
  		// value_init_float(&CONTEXT->values[CONTEXT->value_length++], ($1).floats);
		value_init_float2(&CONTEXT->values[CONTEXT->value_length++], ((yyvsp[0].floatsAndStr)).floats, ((yyvsp[0].floatsAndStr)).str);
	}
#line 1946 "yacc_sql.tab.c"
    break;

  case 70: /* value: SSS  */
#line 528 "yacc_sql.y"
         {
			(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
		}
#line 1955 "yacc_sql.tab.c"
    break;

  case 71: /* value: DATE  */
#line 532 "yacc_sql.y"
              {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_date(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
	}
#line 1964 "yacc_sql.tab.c"
    break;

  case 72: /* value: NULL_A  */
#line 537 "yacc_sql.y"
                {
		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
	}
#line 1972 "yacc_sql.tab.c"
    break;

  case 73: /* delete: DELETE FROM ID where SEMICOLON  */
#line 546 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->condition_exps, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;	
    }
#line 1984 "yacc_sql.tab.c"
    break;

  case 74: /* update: UPDATE ID SET ID EQ value where SEMICOLON  */
#line 556 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value, 
					CONTEXT->condition_exps, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
		}
#line 1996 "yacc_sql.tab.c"
    break;

  case 75: /* select: SELECT relattrexp FROM ID rel_list where SEMICOLON  */
#line 567 "yacc_sql.y"
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
#line 2012 "yacc_sql.tab.c"
    break;

  case 76: /* select: SELECT relattrexp FROM ID join where SEMICOLON  */
#line 579 "yacc_sql.y"
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
#line 2028 "yacc_sql.tab.c"
    break;

  case 77: /* select: SELECT relattrexp FROM ID rel_list where order SEMICOLON  */
#line 590 "yacc_sql.y"
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
#line 2044 "yacc_sql.tab.c"
    break;

  case 78: /* select: SELECT relattrexp FROM ID join where order SEMICOLON  */
#line 602 "yacc_sql.y"
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
#line 2060 "yacc_sql.tab.c"
    break;

  case 79: /* select: SELECT relattrexp FROM ID rel_list where group SEMICOLON  */
#line 613 "yacc_sql.y"
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
#line 2076 "yacc_sql.tab.c"
    break;

  case 80: /* select: SELECT relattrexp FROM ID join where group SEMICOLON  */
#line 625 "yacc_sql.y"
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
#line 2092 "yacc_sql.tab.c"
    break;

  case 81: /* relattrexp: exp_list relattrexp2  */
#line 639 "yacc_sql.y"
                       {
    RelAttrExp exp;
    exp.agg_type = NOTAGG;
    exp.is_star = 0;
    exp.explist = (ExpList *)((yyvsp[-1].exp_list));
    exp.num = exp.explist->num;
    advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
  }
#line 2105 "yacc_sql.tab.c"
    break;

  case 82: /* relattrexp: MINUS exp_list relattrexp2  */
#line 647 "yacc_sql.y"
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
#line 2124 "yacc_sql.tab.c"
    break;

  case 83: /* relattrexp: COUNT LBRACE ID RBRACE relattrexp2  */
#line 665 "yacc_sql.y"
                                         {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2134 "yacc_sql.tab.c"
    break;

  case 84: /* relattrexp: COUNT LBRACE STAR RBRACE relattrexp2  */
#line 670 "yacc_sql.y"
                                               {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2144 "yacc_sql.tab.c"
    break;

  case 85: /* relattrexp: COUNT LBRACE ID DOT ID RBRACE relattrexp2  */
#line 675 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2154 "yacc_sql.tab.c"
    break;

  case 86: /* relattrexp: COUNT LBRACE ID DOT STAR RBRACE relattrexp2  */
#line 680 "yacc_sql.y"
                                                      {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, (yyvsp[-4].string), "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2164 "yacc_sql.tab.c"
    break;

  case 87: /* relattrexp: COUNT LBRACE number RBRACE relattrexp2  */
#line 685 "yacc_sql.y"
                                                 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2177 "yacc_sql.tab.c"
    break;

  case 88: /* relattrexp: COUNT LBRACE floatnumber RBRACE relattrexp2  */
#line 693 "yacc_sql.y"
                                                      {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2191 "yacc_sql.tab.c"
    break;

  case 89: /* relattrexp: MAX LBRACE ID RBRACE relattrexp2  */
#line 702 "yacc_sql.y"
                                       {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2201 "yacc_sql.tab.c"
    break;

  case 90: /* relattrexp: MAX LBRACE STAR RBRACE relattrexp2  */
#line 707 "yacc_sql.y"
                                             {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2211 "yacc_sql.tab.c"
    break;

  case 91: /* relattrexp: MAX LBRACE ID DOT ID RBRACE relattrexp2  */
#line 712 "yacc_sql.y"
                                                  {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2221 "yacc_sql.tab.c"
    break;

  case 92: /* relattrexp: MAX LBRACE number RBRACE relattrexp2  */
#line 717 "yacc_sql.y"
                                               {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2234 "yacc_sql.tab.c"
    break;

  case 93: /* relattrexp: MAX LBRACE floatnumber RBRACE relattrexp2  */
#line 725 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2248 "yacc_sql.tab.c"
    break;

  case 94: /* relattrexp: MIN LBRACE ID RBRACE relattrexp2  */
#line 734 "yacc_sql.y"
                                       {  
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2258 "yacc_sql.tab.c"
    break;

  case 95: /* relattrexp: MIN LBRACE STAR RBRACE relattrexp2  */
#line 739 "yacc_sql.y"
                                             {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2268 "yacc_sql.tab.c"
    break;

  case 96: /* relattrexp: MIN LBRACE ID DOT ID RBRACE relattrexp2  */
#line 744 "yacc_sql.y"
                                                  {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2278 "yacc_sql.tab.c"
    break;

  case 97: /* relattrexp: MIN LBRACE number RBRACE relattrexp2  */
#line 749 "yacc_sql.y"
                                               { 
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2291 "yacc_sql.tab.c"
    break;

  case 98: /* relattrexp: MIN LBRACE floatnumber RBRACE relattrexp2  */
#line 757 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2305 "yacc_sql.tab.c"
    break;

  case 99: /* relattrexp: AVG LBRACE ID RBRACE relattrexp2  */
#line 766 "yacc_sql.y"
                                       {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2315 "yacc_sql.tab.c"
    break;

  case 100: /* relattrexp: AVG LBRACE STAR RBRACE relattrexp2  */
#line 771 "yacc_sql.y"
                                             {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2325 "yacc_sql.tab.c"
    break;

  case 101: /* relattrexp: AVG LBRACE ID DOT ID RBRACE relattrexp2  */
#line 776 "yacc_sql.y"
                                                  {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2335 "yacc_sql.tab.c"
    break;

  case 102: /* relattrexp: AVG LBRACE number RBRACE relattrexp2  */
#line 781 "yacc_sql.y"
                                               {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2348 "yacc_sql.tab.c"
    break;

  case 103: /* relattrexp: AVG LBRACE floatnumber RBRACE relattrexp2  */
#line 789 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
        }
#line 2362 "yacc_sql.tab.c"
    break;

  case 104: /* relattrexp: STAR  */
#line 800 "yacc_sql.y"
           {
      RelAttrExp exp;
      exp.agg_type = NOTAGG;
      exp.is_star = 1;
      exp.relation_name = NULL;
      advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
    }
#line 2374 "yacc_sql.tab.c"
    break;

  case 105: /* relattrexp: ID DOT STAR relattrexp2  */
#line 807 "yacc_sql.y"
                              {
      RelAttrExp exp;
      exp.agg_type = NOTAGG;
      exp.is_star = 1;
      exp.relation_name = strdup((yyvsp[-3].string));
      advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection, &exp);
    }
#line 2386 "yacc_sql.tab.c"
    break;

  case 108: /* exp_list: exps exp_list2  */
#line 825 "yacc_sql.y"
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
#line 2411 "yacc_sql.tab.c"
    break;

  case 109: /* exp_list2: %empty  */
#line 848 "yacc_sql.y"
             {
    (yyval.exp_list) = NULL;
  }
#line 2419 "yacc_sql.tab.c"
    break;

  case 110: /* exp_list2: PLUS exp_list  */
#line 851 "yacc_sql.y"
                  {
    ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
    while(explist_right->left_explist) {
      explist_right = explist_right->left_explist;
    }
    explist_right->calop = PLUS_OP;
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2432 "yacc_sql.tab.c"
    break;

  case 111: /* exp_list2: MINUS exp_list  */
#line 859 "yacc_sql.y"
                   {
    ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
    while(explist_right->left_explist) {
      explist_right = explist_right->left_explist;
    }
    explist_right->calop = MINUS_OP;
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2445 "yacc_sql.tab.c"
    break;

  case 112: /* exp_list3: exp_list  */
#line 870 "yacc_sql.y"
           {
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2453 "yacc_sql.tab.c"
    break;

  case 113: /* exp_list3: MINUS exp_list  */
#line 873 "yacc_sql.y"
                   {
    ExpList *explist = (ExpList *)((yyvsp[0].exp_list));
    while(explist->left_explist) {
      explist = explist->left_explist;
    }
    explist->exp->have_negative = 1;
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2466 "yacc_sql.tab.c"
    break;

  case 114: /* exps: exp exps2  */
#line 887 "yacc_sql.y"
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
#line 2486 "yacc_sql.tab.c"
    break;

  case 115: /* exps2: %empty  */
#line 905 "yacc_sql.y"
             {
    (yyval.exp) = NULL;
  }
#line 2494 "yacc_sql.tab.c"
    break;

  case 116: /* exps2: STAR exps  */
#line 908 "yacc_sql.y"
              {
    Exp *exp_right = (Exp *)((yyvsp[0].exp));
    while(exp_right->left_exp) {
      exp_right = exp_right->left_exp;
    }
    exp_right->calop = TIME_OP;
    (yyval.exp) = (yyvsp[0].exp);
  }
#line 2507 "yacc_sql.tab.c"
    break;

  case 117: /* exps2: DIVIDE exps  */
#line 916 "yacc_sql.y"
                {
    Exp *exp_right = (Exp *)((yyvsp[0].exp));
    while(exp_right->left_exp) {
      exp_right = exp_right->left_exp;
    }
    exp_right->calop = DIVIDE_OP;
    (yyval.exp) = (yyvsp[0].exp);
  }
#line 2520 "yacc_sql.tab.c"
    break;

  case 118: /* exp: ID  */
#line 929 "yacc_sql.y"
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
#line 2538 "yacc_sql.tab.c"
    break;

  case 119: /* exp: ID DOT ID  */
#line 942 "yacc_sql.y"
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
#line 2556 "yacc_sql.tab.c"
    break;

  case 120: /* exp: NNNUMBER  */
#line 955 "yacc_sql.y"
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
#line 2576 "yacc_sql.tab.c"
    break;

  case 121: /* exp: NNFLOAT  */
#line 970 "yacc_sql.y"
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
#line 2596 "yacc_sql.tab.c"
    break;

  case 122: /* exp: SSS  */
#line 985 "yacc_sql.y"
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
#line 2617 "yacc_sql.tab.c"
    break;

  case 123: /* exp: DATE  */
#line 1001 "yacc_sql.y"
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
#line 2638 "yacc_sql.tab.c"
    break;

  case 124: /* exp: NULL_A  */
#line 1017 "yacc_sql.y"
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
#line 2658 "yacc_sql.tab.c"
    break;

  case 125: /* exp: LBRACE exp_list3 RBRACE  */
#line 1032 "yacc_sql.y"
                            {
    Exp *exp = (Exp *)malloc(sizeof(Exp));
    exp->have_brace = 1;
    exp->have_negative = 0;
    exp->explist = (ExpList *)((yyvsp[-1].exp_list));
    exp->num = exp->explist->num;
    exp->left_exp = NULL;
    (yyval.exp) = (void *)exp;
  }
#line 2672 "yacc_sql.tab.c"
    break;

  case 127: /* rel_list: COMMA ID rel_list  */
#line 1045 "yacc_sql.y"
                        {	
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
        adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-1].string));
		  }
#line 2681 "yacc_sql.tab.c"
    break;

  case 129: /* where: WHERE condition_exps  */
#line 1053 "yacc_sql.y"
                         {
  
  }
#line 2689 "yacc_sql.tab.c"
    break;

  case 131: /* condition_exps: condition_exp  */
#line 1059 "yacc_sql.y"
                  {
  
  }
#line 2697 "yacc_sql.tab.c"
    break;

  case 132: /* condition_exps: condition_exp AND condition_exps  */
#line 1062 "yacc_sql.y"
                                     {
  
  }
#line 2705 "yacc_sql.tab.c"
    break;

  case 133: /* condition_exp: exp_list3 comOp exp_list3  */
#line 1069 "yacc_sql.y"
                            {
    ExpList *explist_left = (ExpList *)((yyvsp[-2].exp_list));
    ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
    ConditionExp cond_exp;
    cond_exp.left = explist_left;
    cond_exp.right = explist_right;
    cond_exp.comp = CONTEXT->comp;
    CONTEXT->condition_exps[CONTEXT->condition_length++] = cond_exp;
  }
#line 2719 "yacc_sql.tab.c"
    break;

  case 134: /* condition_exp: exp_list3 IS_A NULL_A  */
#line 1079 "yacc_sql.y"
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
#line 2736 "yacc_sql.tab.c"
    break;

  case 135: /* condition_exp: exp_list3 IS_A NOT NULL_A  */
#line 1091 "yacc_sql.y"
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
#line 2753 "yacc_sql.tab.c"
    break;

  case 136: /* join: INNER JOIN ID onwhere  */
#line 1107 "yacc_sql.y"
                          {
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
        adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-1].string));
    }
#line 2762 "yacc_sql.tab.c"
    break;

  case 137: /* join: INNER JOIN ID onwhere join  */
#line 1111 "yacc_sql.y"
                                {
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
        adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-2].string));
    }
#line 2771 "yacc_sql.tab.c"
    break;

  case 139: /* onwhere: ON condition_exps  */
#line 1118 "yacc_sql.y"
                        {

    }
#line 2779 "yacc_sql.tab.c"
    break;

  case 140: /* order: ORDER BY ID order2  */
#line 1125 "yacc_sql.y"
                     {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2792 "yacc_sql.tab.c"
    break;

  case 141: /* order: ORDER BY ID DOT ID order2  */
#line 1133 "yacc_sql.y"
                              {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = strdup((yyvsp[-3].string));
    order_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2805 "yacc_sql.tab.c"
    break;

  case 142: /* order: ORDER BY ID ASC order2  */
#line 1141 "yacc_sql.y"
                           {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2818 "yacc_sql.tab.c"
    break;

  case 143: /* order: ORDER BY ID DOT ID ASC order2  */
#line 1149 "yacc_sql.y"
                                  {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = strdup((yyvsp[-4].string));
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2831 "yacc_sql.tab.c"
    break;

  case 144: /* order: ORDER BY ID DESC order2  */
#line 1157 "yacc_sql.y"
                            {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = DESCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2844 "yacc_sql.tab.c"
    break;

  case 145: /* order: ORDER BY ID DOT ID DESC order2  */
#line 1165 "yacc_sql.y"
                                   {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = DESCORDER;
    order_attr.relation_name = strdup((yyvsp[-4].string));
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2857 "yacc_sql.tab.c"
    break;

  case 147: /* order2: COMMA ID order2  */
#line 1177 "yacc_sql.y"
                  {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2870 "yacc_sql.tab.c"
    break;

  case 148: /* order2: COMMA ID DOT ID order2  */
#line 1185 "yacc_sql.y"
                           {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = strdup((yyvsp[-3].string));
    order_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2883 "yacc_sql.tab.c"
    break;

  case 149: /* order2: COMMA ID ASC order2  */
#line 1193 "yacc_sql.y"
                        {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2896 "yacc_sql.tab.c"
    break;

  case 150: /* order2: COMMA ID DOT ID ASC order2  */
#line 1201 "yacc_sql.y"
                               {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = ASCORDER;
    order_attr.relation_name = strdup((yyvsp[-4].string));
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2909 "yacc_sql.tab.c"
    break;

  case 151: /* order2: COMMA ID DESC order2  */
#line 1209 "yacc_sql.y"
                         {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = DESCORDER;
    order_attr.relation_name = NULL;
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2922 "yacc_sql.tab.c"
    break;

  case 152: /* order2: COMMA ID DOT ID DESC order2  */
#line 1217 "yacc_sql.y"
                                {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    OrderAttr order_attr;
    order_attr.order_rule = DESCORDER;
    order_attr.relation_name = strdup((yyvsp[-4].string));
    order_attr.attribute_name = strdup((yyvsp[-2].string));
    adv_selects->order_attrs[adv_selects->order_num++] = order_attr;
  }
#line 2935 "yacc_sql.tab.c"
    break;

  case 153: /* group: GROUP BY ID group2  */
#line 1229 "yacc_sql.y"
                     {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    GroupAttr group_attr;
    group_attr.relation_name = NULL;
    group_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->group_attrs[adv_selects->group_num++] = group_attr;
  }
#line 2947 "yacc_sql.tab.c"
    break;

  case 154: /* group: GROUP BY ID DOT ID group2  */
#line 1236 "yacc_sql.y"
                              {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    GroupAttr group_attr;
    group_attr.relation_name = strdup((yyvsp[-3].string));
    group_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->group_attrs[adv_selects->group_num++] = group_attr;
  }
#line 2959 "yacc_sql.tab.c"
    break;

  case 156: /* group2: COMMA ID group2  */
#line 1246 "yacc_sql.y"
                    {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    GroupAttr group_attr;
    group_attr.relation_name = NULL;
    group_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->group_attrs[adv_selects->group_num++] = group_attr;
  }
#line 2971 "yacc_sql.tab.c"
    break;

  case 157: /* group2: COMMA ID DOT ID group2  */
#line 1253 "yacc_sql.y"
                           {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection;
    GroupAttr group_attr;
    group_attr.relation_name = strdup((yyvsp[-3].string));
    group_attr.attribute_name = strdup((yyvsp[-1].string));
    adv_selects->group_attrs[adv_selects->group_num++] = group_attr;
  }
#line 2983 "yacc_sql.tab.c"
    break;

  case 158: /* comOp: EQ  */
#line 1265 "yacc_sql.y"
             { CONTEXT->comp = EQUAL_TO; }
#line 2989 "yacc_sql.tab.c"
    break;

  case 159: /* comOp: LT  */
#line 1266 "yacc_sql.y"
         { CONTEXT->comp = LESS_THAN; }
#line 2995 "yacc_sql.tab.c"
    break;

  case 160: /* comOp: GT  */
#line 1267 "yacc_sql.y"
         { CONTEXT->comp = GREAT_THAN; }
#line 3001 "yacc_sql.tab.c"
    break;

  case 161: /* comOp: LE  */
#line 1268 "yacc_sql.y"
         { CONTEXT->comp = LESS_EQUAL; }
#line 3007 "yacc_sql.tab.c"
    break;

  case 162: /* comOp: GE  */
#line 1269 "yacc_sql.y"
         { CONTEXT->comp = GREAT_EQUAL; }
#line 3013 "yacc_sql.tab.c"
    break;

  case 163: /* comOp: NE  */
#line 1270 "yacc_sql.y"
         { CONTEXT->comp = NOT_EQUAL; }
#line 3019 "yacc_sql.tab.c"
    break;

  case 164: /* load_data: LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON  */
#line 1275 "yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 3028 "yacc_sql.tab.c"
    break;


#line 3032 "yacc_sql.tab.c"

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

#line 1280 "yacc_sql.y"

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
