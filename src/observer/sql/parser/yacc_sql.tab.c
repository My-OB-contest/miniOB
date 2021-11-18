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
  size_t condition_length;
  size_t from_length;
  size_t value_length;
  size_t value_list_length;
  size_t insert_value_length[MAX_NUM];
  Value values[MAX_NUM];
  Value insert_values[MAX_NUM][MAX_NUM];
  ConditionExp condition_exps[MAX_NUM]; // 删掉Condition，增加ConditionExp
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


#line 138 "yacc_sql.tab.c"

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
  YYSYMBOL_IN = 56,                        /* IN  */
  YYSYMBOL_PLUS = 57,                      /* PLUS  */
  YYSYMBOL_MINUS = 58,                     /* MINUS  */
  YYSYMBOL_DIVIDE = 59,                    /* DIVIDE  */
  YYSYMBOL_NNNUMBER = 60,                  /* NNNUMBER  */
  YYSYMBOL_NNFLOAT = 61,                   /* NNFLOAT  */
  YYSYMBOL_ID = 62,                        /* ID  */
  YYSYMBOL_DATE = 63,                      /* DATE  */
  YYSYMBOL_PATH = 64,                      /* PATH  */
  YYSYMBOL_SSS = 65,                       /* SSS  */
  YYSYMBOL_STAR = 66,                      /* STAR  */
  YYSYMBOL_STRING_V = 67,                  /* STRING_V  */
  YYSYMBOL_YYACCEPT = 68,                  /* $accept  */
  YYSYMBOL_commands = 69,                  /* commands  */
  YYSYMBOL_command = 70,                   /* command  */
  YYSYMBOL_exit = 71,                      /* exit  */
  YYSYMBOL_help = 72,                      /* help  */
  YYSYMBOL_sync = 73,                      /* sync  */
  YYSYMBOL_begin = 74,                     /* begin  */
  YYSYMBOL_commit = 75,                    /* commit  */
  YYSYMBOL_rollback = 76,                  /* rollback  */
  YYSYMBOL_drop_table = 77,                /* drop_table  */
  YYSYMBOL_show_tables = 78,               /* show_tables  */
  YYSYMBOL_desc_table = 79,                /* desc_table  */
  YYSYMBOL_create_index = 80,              /* create_index  */
  YYSYMBOL_field_name_list = 81,           /* field_name_list  */
  YYSYMBOL_drop_index = 82,                /* drop_index  */
  YYSYMBOL_create_table = 83,              /* create_table  */
  YYSYMBOL_attr_def_list = 84,             /* attr_def_list  */
  YYSYMBOL_attr_def = 85,                  /* attr_def  */
  YYSYMBOL_number = 86,                    /* number  */
  YYSYMBOL_floatnumber = 87,               /* floatnumber  */
  YYSYMBOL_type = 88,                      /* type  */
  YYSYMBOL_ID_get = 89,                    /* ID_get  */
  YYSYMBOL_insert = 90,                    /* insert  */
  YYSYMBOL_values_lists = 91,              /* values_lists  */
  YYSYMBOL_value_tuple = 92,               /* value_tuple  */
  YYSYMBOL_value_list = 93,                /* value_list  */
  YYSYMBOL_insert_value = 94,              /* insert_value  */
  YYSYMBOL_value = 95,                     /* value  */
  YYSYMBOL_delete = 96,                    /* delete  */
  YYSYMBOL_update = 97,                    /* update  */
  YYSYMBOL_select_clasue = 98,             /* select_clasue  */
  YYSYMBOL_select = 99,                    /* select  */
  YYSYMBOL_select_handle = 100,            /* select_handle  */
  YYSYMBOL_relattrexp = 101,               /* relattrexp  */
  YYSYMBOL_relattrexp2 = 102,              /* relattrexp2  */
  YYSYMBOL_exp_list = 103,                 /* exp_list  */
  YYSYMBOL_exp_list2 = 104,                /* exp_list2  */
  YYSYMBOL_exp_list3 = 105,                /* exp_list3  */
  YYSYMBOL_exps = 106,                     /* exps  */
  YYSYMBOL_exps2 = 107,                    /* exps2  */
  YYSYMBOL_exp = 108,                      /* exp  */
  YYSYMBOL_rel_list = 109,                 /* rel_list  */
  YYSYMBOL_where = 110,                    /* where  */
  YYSYMBOL_condition_exps = 111,           /* condition_exps  */
  YYSYMBOL_condition_exp = 112,            /* condition_exp  */
  YYSYMBOL_join = 113,                     /* join  */
  YYSYMBOL_onwhere = 114,                  /* onwhere  */
  YYSYMBOL_comOp = 115,                    /* comOp  */
  YYSYMBOL_load_data = 116                 /* load_data  */
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
#define YYLAST   302

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  68
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  49
/* YYNRULES -- Number of rules.  */
#define YYNRULES  144
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  304

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   322


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
      65,    66,    67
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   187,   187,   189,   193,   194,   195,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   205,   206,   207,   208,
     209,   213,   218,   223,   229,   235,   241,   247,   253,   259,
     266,   271,   279,   283,   290,   297,   306,   308,   312,   323,
     337,   349,   361,   373,   388,   389,   395,   396,   409,   410,
     411,   412,   415,   425,   446,   449,   454,   456,   461,   463,
     470,   473,   476,   480,   485,   494,   497,   501,   505,   510,
     518,   528,   993,   998,  1011,  1027,  1037,  1045,  1063,  1068,
    1073,  1078,  1083,  1091,  1100,  1105,  1110,  1115,  1123,  1132,
    1137,  1142,  1147,  1155,  1164,  1169,  1174,  1179,  1187,  1198,
    1205,  1214,  1216,  1223,  1246,  1249,  1257,  1268,  1271,  1285,
    1303,  1306,  1314,  1327,  1341,  1355,  1371,  1387,  1404,  1421,
    1437,  1447,  1463,  1465,  1471,  1473,  1478,  1479,  1482,  1489,
    1500,  1512,  1528,  1532,  1537,  1539,  1546,  1547,  1548,  1549,
    1550,  1551,  1552,  1555,  1559
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
  "NULLABLE", "IS_A", "UNIQUE", "IN", "PLUS", "MINUS", "DIVIDE",
  "NNNUMBER", "NNFLOAT", "ID", "DATE", "PATH", "SSS", "STAR", "STRING_V",
  "$accept", "commands", "command", "exit", "help", "sync", "begin",
  "commit", "rollback", "drop_table", "show_tables", "desc_table",
  "create_index", "field_name_list", "drop_index", "create_table",
  "attr_def_list", "attr_def", "number", "floatnumber", "type", "ID_get",
  "insert", "values_lists", "value_tuple", "value_list", "insert_value",
  "value", "delete", "update", "select_clasue", "select", "select_handle",
  "relattrexp", "relattrexp2", "exp_list", "exp_list2", "exp_list3",
  "exps", "exps2", "exp", "rel_list", "where", "condition_exps",
  "condition_exp", "join", "onwhere", "comOp", "load_data", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-235)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -235,   143,  -235,    10,    90,  -235,   -41,    28,    71,    62,
      99,   -12,   129,   139,   141,   148,   156,   109,  -235,  -235,
    -235,  -235,  -235,  -235,  -235,  -235,  -235,  -235,  -235,  -235,
    -235,  -235,  -235,  -235,  -235,   164,    47,  -235,    39,   106,
     163,   116,   118,   178,   179,  -235,   122,   132,   153,  -235,
    -235,  -235,  -235,  -235,   157,  -235,    -1,   185,   186,   192,
     193,  -235,    -7,  -235,  -235,   182,  -235,  -235,  -235,   181,
     195,   -28,    31,   198,   180,   154,   214,   215,  -235,  -235,
     189,   188,   159,   158,    -7,   194,   207,  -235,   208,    79,
     131,   138,   145,   195,   -14,   165,    47,  -235,    -7,    -7,
    -235,    -7,    -7,  -235,   166,   167,   191,  -235,  -235,   216,
      73,   227,   196,   202,  -235,   171,  -235,  -235,    55,  -235,
    -235,     0,   217,   219,   220,     3,   221,   222,   223,    19,
     224,   225,   226,    21,   228,   229,   230,  -235,  -235,   195,
      -8,  -235,  -235,  -235,  -235,  -235,  -235,   231,   104,   232,
     190,    59,   233,   -17,  -235,   211,  -235,   125,   244,  -235,
    -235,   195,    52,   195,   195,   195,   195,   197,   195,   195,
     195,   195,   199,   195,   195,   195,   195,   200,   195,   195,
     195,  -235,   201,   203,   188,   188,   166,   237,  -235,  -235,
    -235,  -235,    -9,   204,   239,  -235,  -235,  -235,  -235,  -235,
     238,   216,   254,  -235,  -235,  -235,  -235,  -235,  -235,   209,
      98,  -235,    73,    73,  -235,  -235,  -235,  -235,  -235,   188,
     205,  -235,   241,   243,  -235,  -235,  -235,  -235,   247,  -235,
    -235,  -235,  -235,   251,  -235,  -235,  -235,  -235,   252,  -235,
    -235,  -235,   253,   210,  -235,  -235,   231,   267,    46,   234,
    -235,   255,   257,   204,    59,   258,   233,  -235,  -235,   235,
    -235,  -235,  -235,   273,   274,   195,   195,   195,   195,   195,
    -235,   245,  -235,  -235,   218,   262,  -235,   204,   278,   265,
     238,  -235,  -235,  -235,  -235,  -235,  -235,  -235,  -235,  -235,
    -235,    73,   236,    70,  -235,  -235,   280,  -235,  -235,  -235,
     240,  -235,  -235,  -235
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,    75,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,    20,
      19,    14,    15,    16,    17,     9,    10,    11,    12,    13,
       8,     5,     7,     6,     4,     0,     0,    18,     0,     0,
       0,     0,     0,     0,     0,    23,     0,     0,     0,    24,
      25,    26,    22,    21,     0,    72,     0,     0,     0,     0,
       0,   119,     0,   115,   116,   113,   118,   117,    99,     0,
     101,   104,   110,     0,     0,     0,     0,     0,    29,    28,
       0,   124,     0,     0,     0,   113,     0,   107,     0,     0,
       0,     0,     0,   101,     0,     0,     0,    76,     0,     0,
     103,     0,     0,   109,     0,     0,     0,    27,    34,    56,
     126,     0,     0,     0,   108,     0,   121,   120,     0,    44,
      46,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    77,   114,   101,
     122,   102,   105,   106,   112,   111,    52,    36,     0,     0,
       0,     0,    54,     0,   125,   127,    70,     0,     0,    45,
      47,   101,     0,   101,   101,   101,   101,     0,   101,   101,
     101,   101,     0,   101,   101,   101,   101,     0,   101,   101,
     101,   100,     0,     0,   124,   124,     0,     0,    48,    49,
      50,    51,    39,     0,     0,    64,    63,    62,    60,    61,
      58,    56,     0,   136,   137,   138,   139,   140,   141,     0,
       0,   142,     0,   126,    69,    68,    67,    65,    66,   124,
       0,    78,     0,     0,    79,    82,    83,    84,     0,    85,
      87,    88,    89,     0,    90,    92,    93,    94,     0,    95,
      97,    98,   122,     0,    73,    74,    36,     0,     0,     0,
      43,    32,     0,     0,     0,     0,    54,    53,   143,     0,
     130,   129,   128,     0,     0,   101,   101,   101,   101,   101,
     123,   134,    37,    35,     0,     0,    42,     0,     0,     0,
      58,    57,    55,   131,    71,   144,    80,    81,    86,    91,
      96,   126,   132,    38,    33,    30,     0,    59,   135,   133,
       0,    41,    31,    40
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -235,  -235,  -235,  -235,  -235,  -235,  -235,  -235,  -235,  -235,
    -235,  -235,  -235,  -234,  -235,  -235,    38,   102,   -88,   -78,
    -235,  -235,  -235,    33,    89,    11,    40,  -235,  -235,  -235,
    -235,   242,  -235,   206,   -93,     4,  -235,   -51,    64,  -235,
    -235,    51,  -152,  -207,  -235,     5,  -235,  -235,  -235
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
       0,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,   252,    29,    30,   187,   147,   198,   199,
     192,   148,    31,   202,   152,   255,   200,   219,    32,    33,
      34,    35,    36,    69,    97,    87,   100,   153,    71,   103,
      72,   184,   111,   154,   155,   185,   292,   212,    37
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     137,   123,   127,   131,   135,    88,   262,   248,     5,    56,
     182,   124,   128,   132,   136,    56,    38,   161,    39,   279,
     166,    43,   203,   204,   205,   206,   207,   208,   162,    98,
      99,   167,   244,   245,   209,    44,   171,   210,   176,   211,
      70,   183,   249,   294,   250,    61,   181,   172,   138,   177,
      48,    61,   139,    63,    64,    85,    66,    84,    67,    63,
      64,    85,    66,    56,    67,    40,    93,   263,   221,   217,
     224,   225,   226,   227,    45,   229,   230,   231,   232,   218,
     234,   235,   236,   237,   298,   239,   240,   241,   114,    56,
     101,    46,    57,    58,    59,    60,    41,   102,    42,    61,
      70,    73,   142,   143,   274,    62,   119,    63,    64,    65,
      66,   195,    67,    68,   222,   159,   160,   118,   223,   119,
     120,   300,   196,   301,   197,    61,   188,   189,   190,   191,
      47,    84,    49,    63,    64,    85,    66,   118,    67,   119,
     120,   121,    50,     2,    51,   122,    54,     3,     4,   259,
     260,    52,     5,     6,     7,     8,     9,    10,    11,    53,
     275,   261,    12,    13,    14,   144,   145,    55,    74,    15,
      16,    75,   286,   287,   288,   289,   290,   214,    76,    17,
      77,    78,    79,   118,    80,   119,   120,    82,   215,   118,
     216,   119,   120,   125,    81,    83,   118,   126,   119,   120,
     129,    89,    90,   118,   130,   119,   120,   133,    91,    92,
      94,   134,    95,    96,   104,   105,   106,   107,   108,   109,
     110,   112,   115,   113,   116,   117,   150,   140,   146,   149,
     156,   158,   151,   138,   163,   157,   164,   165,   168,   169,
     170,   173,   174,   175,   213,   178,   179,   180,   193,   186,
     220,   201,   194,   243,   247,   253,   254,   257,   265,   228,
     266,   233,   238,   242,   267,   258,   251,   264,   268,   269,
     273,   182,   271,   277,   278,   281,   284,   285,   159,   293,
     291,   295,   296,   302,   272,   183,   276,   283,   246,   282,
     256,   297,   303,   270,   280,     0,     0,   299,    86,     0,
       0,     0,   141
};

static const yytype_int16 yycheck[] =
{
      93,    89,    90,    91,    92,    56,   213,    16,     9,    16,
      18,    89,    90,    91,    92,    16,     6,    17,     8,   253,
      17,    62,    39,    40,    41,    42,    43,    44,    28,    57,
      58,    28,   184,   185,    51,     7,    17,    54,    17,    56,
      36,    49,    51,   277,    53,    52,   139,    28,    62,    28,
      62,    52,    66,    60,    61,    62,    63,    58,    65,    60,
      61,    62,    63,    16,    65,    55,    62,   219,   161,   157,
     163,   164,   165,   166,     3,   168,   169,   170,   171,   157,
     173,   174,   175,   176,   291,   178,   179,   180,    84,    16,
      59,    29,    45,    46,    47,    48,     6,    66,     8,    52,
      96,    62,    98,    99,    58,    58,    60,    60,    61,    62,
      63,    52,    65,    66,    62,    60,    61,    58,    66,    60,
      61,    51,    63,    53,    65,    52,    22,    23,    24,    25,
      31,    58,     3,    60,    61,    62,    63,    58,    65,    60,
      61,    62,     3,     0,     3,    66,    37,     4,     5,    51,
      52,     3,     9,    10,    11,    12,    13,    14,    15,     3,
     248,   212,    19,    20,    21,   101,   102,     3,    62,    26,
      27,     8,   265,   266,   267,   268,   269,    52,    62,    36,
      62,     3,     3,    58,    62,    60,    61,    34,    63,    58,
      65,    60,    61,    62,    62,    38,    58,    66,    60,    61,
      62,    16,    16,    58,    66,    60,    61,    62,    16,    16,
      28,    66,    31,    18,    16,    35,    62,     3,     3,    30,
      32,    62,    28,    65,    17,    17,    35,    62,    62,    62,
       3,    29,    16,    62,    17,    39,    17,    17,    17,    17,
      17,    17,    17,    17,    33,    17,    17,    17,    16,    18,
       6,    18,    62,    50,    17,    16,    18,     3,    17,    62,
      17,    62,    62,    62,    17,    56,    62,    62,    17,    17,
       3,    18,    62,    18,    17,    17,     3,     3,    60,    17,
      35,     3,    17,     3,   246,    49,    52,    52,   186,   256,
     201,   280,    52,   242,   254,    -1,    -1,   292,    56,    -1,
      -1,    -1,    96
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    69,     0,     4,     5,     9,    10,    11,    12,    13,
      14,    15,    19,    20,    21,    26,    27,    36,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    80,    82,
      83,    90,    96,    97,    98,    99,   100,   116,     6,     8,
      55,     6,     8,    62,     7,     3,    29,    31,    62,     3,
       3,     3,     3,     3,    37,     3,    16,    45,    46,    47,
      48,    52,    58,    60,    61,    62,    63,    65,    66,   101,
     103,   106,   108,    62,    62,     8,    62,    62,     3,     3,
      62,    62,    34,    38,    58,    62,    99,   103,   105,    16,
      16,    16,    16,   103,    28,    31,    18,   102,    57,    58,
     104,    59,    66,   107,    16,    35,    62,     3,     3,    30,
      32,   110,    62,    65,   103,    28,    17,    17,    58,    60,
      61,    62,    66,    86,    87,    62,    66,    86,    87,    62,
      66,    86,    87,    62,    66,    86,    87,   102,    62,    66,
      62,   101,   103,   103,   106,   106,    62,    85,    89,    62,
      35,    16,    92,   105,   111,   112,     3,    39,    29,    60,
      61,    17,    28,    17,    17,    17,    17,    28,    17,    17,
      17,    17,    28,    17,    17,    17,    17,    28,    17,    17,
      17,   102,    18,    49,   109,   113,    18,    84,    22,    23,
      24,    25,    88,    16,    62,    52,    63,    65,    86,    87,
      94,    18,    91,    39,    40,    41,    42,    43,    44,    51,
      54,    56,   115,    33,    52,    63,    65,    86,    87,    95,
       6,   102,    62,    66,   102,   102,   102,   102,    62,   102,
     102,   102,   102,    62,   102,   102,   102,   102,    62,   102,
     102,   102,    62,    50,   110,   110,    85,    17,    16,    51,
      53,    62,    81,    16,    18,    93,    92,     3,    56,    51,
      52,   105,   111,   110,    62,    17,    17,    17,    17,    17,
     109,    62,    84,     3,    58,    86,    52,    18,    17,    81,
      94,    17,    91,    52,     3,     3,   102,   102,   102,   102,
     102,    35,   114,    17,    81,     3,    17,    93,   111,   113,
      51,    53,     3,    52
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    68,    69,    69,    70,    70,    70,    70,    70,    70,
      70,    70,    70,    70,    70,    70,    70,    70,    70,    70,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    80,    81,    81,    82,    83,    84,    84,    85,    85,
      85,    85,    85,    85,    86,    86,    87,    87,    88,    88,
      88,    88,    89,    90,    91,    91,    92,    92,    93,    93,
      94,    94,    94,    94,    94,    95,    95,    95,    95,    95,
      96,    97,    98,    99,    99,   100,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   102,   102,   103,   104,   104,   104,   105,   105,   106,
     107,   107,   107,   108,   108,   108,   108,   108,   108,   108,
     108,   108,   109,   109,   110,   110,   111,   111,   111,   112,
     112,   112,   113,   113,   114,   114,   115,   115,   115,   115,
     115,   115,   115,   115,   116
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
       9,    10,     1,     3,     4,     8,     0,     3,     5,     2,
       7,     6,     4,     3,     1,     2,     1,     2,     1,     1,
       1,     1,     1,     7,     0,     3,     0,     4,     0,     3,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     8,     2,     6,     6,     1,     2,     3,     5,     5,
       7,     7,     5,     5,     5,     5,     7,     5,     5,     5,
       5,     7,     5,     5,     5,     5,     7,     5,     5,     1,
       4,     0,     2,     2,     0,     2,     2,     1,     2,     2,
       0,     2,     2,     1,     3,     1,     1,     1,     1,     1,
       3,     3,     0,     3,     0,     2,     0,     1,     3,     3,
       3,     4,     4,     5,     0,     2,     1,     1,     1,     1,
       1,     1,     1,     2,     8
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
#line 213 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1460 "yacc_sql.tab.c"
    break;

  case 22: /* help: HELP SEMICOLON  */
#line 218 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1468 "yacc_sql.tab.c"
    break;

  case 23: /* sync: SYNC SEMICOLON  */
#line 223 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1476 "yacc_sql.tab.c"
    break;

  case 24: /* begin: TRX_BEGIN SEMICOLON  */
#line 229 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1484 "yacc_sql.tab.c"
    break;

  case 25: /* commit: TRX_COMMIT SEMICOLON  */
#line 235 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1492 "yacc_sql.tab.c"
    break;

  case 26: /* rollback: TRX_ROLLBACK SEMICOLON  */
#line 241 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1500 "yacc_sql.tab.c"
    break;

  case 27: /* drop_table: DROP TABLE ID SEMICOLON  */
#line 247 "yacc_sql.y"
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1509 "yacc_sql.tab.c"
    break;

  case 28: /* show_tables: SHOW TABLES SEMICOLON  */
#line 253 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1517 "yacc_sql.tab.c"
    break;

  case 29: /* desc_table: DESC ID SEMICOLON  */
#line 259 "yacc_sql.y"
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1526 "yacc_sql.tab.c"
    break;

  case 30: /* create_index: CREATE INDEX ID ON ID LBRACE field_name_list RBRACE SEMICOLON  */
#line 267 "yacc_sql.y"
                {
 			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string) ,0);
 		}
#line 1535 "yacc_sql.tab.c"
    break;

  case 31: /* create_index: CREATE UNIQUE INDEX ID ON ID LBRACE field_name_list RBRACE SEMICOLON  */
#line 272 "yacc_sql.y"
            {
 	        CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_unique_index";
            create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string) ,1);
 	    }
#line 1544 "yacc_sql.tab.c"
    break;

  case 32: /* field_name_list: ID  */
#line 280 "yacc_sql.y"
        {
            create_index_append_attribute(&CONTEXT->ssql->sstr.create_index,(yyvsp[0].string));
        }
#line 1552 "yacc_sql.tab.c"
    break;

  case 33: /* field_name_list: ID COMMA field_name_list  */
#line 284 "yacc_sql.y"
        {
            create_index_append_attribute(&CONTEXT->ssql->sstr.create_index,(yyvsp[-2].string));
        }
#line 1560 "yacc_sql.tab.c"
    break;

  case 34: /* drop_index: DROP INDEX ID SEMICOLON  */
#line 291 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1569 "yacc_sql.tab.c"
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
#line 1581 "yacc_sql.tab.c"
    break;

  case 37: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 308 "yacc_sql.y"
                                   {    }
#line 1587 "yacc_sql.tab.c"
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
#line 1602 "yacc_sql.tab.c"
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
#line 1617 "yacc_sql.tab.c"
    break;

  case 40: /* attr_def: ID_get type LBRACE number RBRACE NOT NULL_A  */
#line 338 "yacc_sql.y"
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
#line 1633 "yacc_sql.tab.c"
    break;

  case 41: /* attr_def: ID_get type LBRACE number RBRACE NULLABLE  */
#line 350 "yacc_sql.y"
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
#line 1649 "yacc_sql.tab.c"
    break;

  case 42: /* attr_def: ID_get type NOT NULL_A  */
#line 362 "yacc_sql.y"
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
#line 1665 "yacc_sql.tab.c"
    break;

  case 43: /* attr_def: ID_get type NULLABLE  */
#line 374 "yacc_sql.y"
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
#line 1681 "yacc_sql.tab.c"
    break;

  case 44: /* number: NNNUMBER  */
#line 388 "yacc_sql.y"
             {(yyval.number) = (yyvsp[0].number);}
#line 1687 "yacc_sql.tab.c"
    break;

  case 45: /* number: MINUS NNNUMBER  */
#line 389 "yacc_sql.y"
                     {
      (yyval.number) = -((yyvsp[0].number)); 
    }
#line 1695 "yacc_sql.tab.c"
    break;

  case 46: /* floatnumber: NNFLOAT  */
#line 395 "yacc_sql.y"
          {(yyval.floatsAndStr) = (yyvsp[0].floatsAndStr); }
#line 1701 "yacc_sql.tab.c"
    break;

  case 47: /* floatnumber: MINUS NNFLOAT  */
#line 396 "yacc_sql.y"
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
#line 1716 "yacc_sql.tab.c"
    break;

  case 48: /* type: INT_T  */
#line 409 "yacc_sql.y"
              { (yyval.number)=INTS; }
#line 1722 "yacc_sql.tab.c"
    break;

  case 49: /* type: STRING_T  */
#line 410 "yacc_sql.y"
                  { (yyval.number)=CHARS; }
#line 1728 "yacc_sql.tab.c"
    break;

  case 50: /* type: FLOAT_T  */
#line 411 "yacc_sql.y"
                 { (yyval.number)=FLOATS; }
#line 1734 "yacc_sql.tab.c"
    break;

  case 51: /* type: DATE_T  */
#line 412 "yacc_sql.y"
                    { (yyval.number)=DATES; }
#line 1740 "yacc_sql.tab.c"
    break;

  case 52: /* ID_get: ID  */
#line 416 "yacc_sql.y"
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1749 "yacc_sql.tab.c"
    break;

  case 53: /* insert: INSERT INTO ID VALUES value_tuple values_lists SEMICOLON  */
#line 426 "yacc_sql.y"
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
#line 1772 "yacc_sql.tab.c"
    break;

  case 54: /* values_lists: %empty  */
#line 446 "yacc_sql.y"
               {
        //CONTEXT->value_list_length++;
    }
#line 1780 "yacc_sql.tab.c"
    break;

  case 55: /* values_lists: COMMA value_tuple values_lists  */
#line 449 "yacc_sql.y"
                                    {
  	     //CONTEXT->value_list_length++;
	  }
#line 1788 "yacc_sql.tab.c"
    break;

  case 57: /* value_tuple: LBRACE insert_value value_list RBRACE  */
#line 456 "yacc_sql.y"
                                             {
        CONTEXT->value_list_length++;
	  }
#line 1796 "yacc_sql.tab.c"
    break;

  case 59: /* value_list: COMMA insert_value value_list  */
#line 463 "yacc_sql.y"
                                   {
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1804 "yacc_sql.tab.c"
    break;

  case 60: /* insert_value: number  */
#line 470 "yacc_sql.y"
           {
  		value_init_integer(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], (yyvsp[0].number));
		}
#line 1812 "yacc_sql.tab.c"
    break;

  case 61: /* insert_value: floatnumber  */
#line 473 "yacc_sql.y"
                 {
  		value_init_float(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], ((yyvsp[0].floatsAndStr)).floats);
		}
#line 1820 "yacc_sql.tab.c"
    break;

  case 62: /* insert_value: SSS  */
#line 476 "yacc_sql.y"
         {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], (yyvsp[0].string));
		}
#line 1829 "yacc_sql.tab.c"
    break;

  case 63: /* insert_value: DATE  */
#line 480 "yacc_sql.y"
              {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_date(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++], (yyvsp[0].string));
	}
#line 1838 "yacc_sql.tab.c"
    break;

  case 64: /* insert_value: NULL_A  */
#line 485 "yacc_sql.y"
                {
		// value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
		value_init_null(&CONTEXT->insert_values[CONTEXT->value_list_length][CONTEXT->insert_value_length[CONTEXT->value_list_length]++]); // 多条插入
	}
#line 1847 "yacc_sql.tab.c"
    break;

  case 65: /* value: number  */
#line 494 "yacc_sql.y"
           {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number));
		}
#line 1855 "yacc_sql.tab.c"
    break;

  case 66: /* value: floatnumber  */
#line 497 "yacc_sql.y"
                {
  		// value_init_float(&CONTEXT->values[CONTEXT->value_length++], ($1).floats);
		value_init_float2(&CONTEXT->values[CONTEXT->value_length++], ((yyvsp[0].floatsAndStr)).floats, ((yyvsp[0].floatsAndStr)).str);
	}
#line 1864 "yacc_sql.tab.c"
    break;

  case 67: /* value: SSS  */
#line 501 "yacc_sql.y"
         {
			(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
		}
#line 1873 "yacc_sql.tab.c"
    break;

  case 68: /* value: DATE  */
#line 505 "yacc_sql.y"
              {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_date(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
	}
#line 1882 "yacc_sql.tab.c"
    break;

  case 69: /* value: NULL_A  */
#line 510 "yacc_sql.y"
                {
		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
	}
#line 1890 "yacc_sql.tab.c"
    break;

  case 70: /* delete: DELETE FROM ID where SEMICOLON  */
#line 519 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->condition_exps, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;	
    }
#line 1902 "yacc_sql.tab.c"
    break;

  case 71: /* update: UPDATE ID SET ID EQ value where SEMICOLON  */
#line 529 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value, 
					CONTEXT->condition_exps, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
		}
#line 1914 "yacc_sql.tab.c"
    break;

  case 72: /* select_clasue: select SEMICOLON  */
#line 993 "yacc_sql.y"
                    {
        CONTEXT->ssql->flag=SCF_SELECT;
    }
#line 1922 "yacc_sql.tab.c"
    break;

  case 73: /* select: select_handle relattrexp FROM ID rel_list where  */
#line 998 "yacc_sql.y"
                                                   {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
    adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-2].string));
    advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps, CONTEXT->condition_length);
    //临时变量清零
    CONTEXT->condition_length=0;
    CONTEXT->from_length=0;
    CONTEXT->value_length = 0;
    int tmp = CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1];
    CONTEXT->deep_stack.top--;
    CONTEXT->comp_deep--;
    (yyval.number)=tmp;
  }
#line 1940 "yacc_sql.tab.c"
    break;

  case 74: /* select: select_handle relattrexp FROM ID join where  */
#line 1012 "yacc_sql.y"
  {
    AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
     adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-2].string));
     advselects_append_conditionexps(adv_selects, CONTEXT->condition_exps, CONTEXT->condition_length);
     //临时变量清零
     CONTEXT->condition_length=0;
     CONTEXT->from_length=0;
     int tmp = CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1];
     CONTEXT->deep_stack.top--;
     CONTEXT->comp_deep--;
     (yyval.number)=tmp;
  }
#line 1957 "yacc_sql.tab.c"
    break;

  case 75: /* select_handle: SELECT  */
#line 1028 "yacc_sql.y"
  {
     CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top] = CONTEXT->select_length;
     CONTEXT->deep_stack.top++;
     CONTEXT->comp_deep++;
     CONTEXT->select_length++;
     CONTEXT->ssql->sstr.adv_selection[0].select_num=CONTEXT->select_length;
  }
#line 1969 "yacc_sql.tab.c"
    break;

  case 76: /* relattrexp: exp_list relattrexp2  */
#line 1037 "yacc_sql.y"
                       {
    RelAttrExp exp;
    exp.agg_type = NOTAGG;
    exp.is_star = 0;
    exp.explist = (ExpList *)((yyvsp[-1].exp_list));
    exp.num = exp.explist->num;
    advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
  }
#line 1982 "yacc_sql.tab.c"
    break;

  case 77: /* relattrexp: MINUS exp_list relattrexp2  */
#line 1045 "yacc_sql.y"
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
#line 2001 "yacc_sql.tab.c"
    break;

  case 78: /* relattrexp: COUNT LBRACE ID RBRACE relattrexp2  */
#line 1063 "yacc_sql.y"
                                         {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2011 "yacc_sql.tab.c"
    break;

  case 79: /* relattrexp: COUNT LBRACE STAR RBRACE relattrexp2  */
#line 1068 "yacc_sql.y"
                                               {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2021 "yacc_sql.tab.c"
    break;

  case 80: /* relattrexp: COUNT LBRACE ID DOT ID RBRACE relattrexp2  */
#line 1073 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2031 "yacc_sql.tab.c"
    break;

  case 81: /* relattrexp: COUNT LBRACE ID DOT STAR RBRACE relattrexp2  */
#line 1078 "yacc_sql.y"
                                                      {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, (yyvsp[-4].string), "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2041 "yacc_sql.tab.c"
    break;

  case 82: /* relattrexp: COUNT LBRACE number RBRACE relattrexp2  */
#line 1083 "yacc_sql.y"
                                                 {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2054 "yacc_sql.tab.c"
    break;

  case 83: /* relattrexp: COUNT LBRACE floatnumber RBRACE relattrexp2  */
#line 1091 "yacc_sql.y"
                                                      {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGCOUNT, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2068 "yacc_sql.tab.c"
    break;

  case 84: /* relattrexp: MAX LBRACE ID RBRACE relattrexp2  */
#line 1100 "yacc_sql.y"
                                       {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2078 "yacc_sql.tab.c"
    break;

  case 85: /* relattrexp: MAX LBRACE STAR RBRACE relattrexp2  */
#line 1105 "yacc_sql.y"
                                             {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2088 "yacc_sql.tab.c"
    break;

  case 86: /* relattrexp: MAX LBRACE ID DOT ID RBRACE relattrexp2  */
#line 1110 "yacc_sql.y"
                                                  {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2098 "yacc_sql.tab.c"
    break;

  case 87: /* relattrexp: MAX LBRACE number RBRACE relattrexp2  */
#line 1115 "yacc_sql.y"
                                               {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2111 "yacc_sql.tab.c"
    break;

  case 88: /* relattrexp: MAX LBRACE floatnumber RBRACE relattrexp2  */
#line 1123 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMAX, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2125 "yacc_sql.tab.c"
    break;

  case 89: /* relattrexp: MIN LBRACE ID RBRACE relattrexp2  */
#line 1132 "yacc_sql.y"
                                       {  
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2135 "yacc_sql.tab.c"
    break;

  case 90: /* relattrexp: MIN LBRACE STAR RBRACE relattrexp2  */
#line 1137 "yacc_sql.y"
                                             {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2145 "yacc_sql.tab.c"
    break;

  case 91: /* relattrexp: MIN LBRACE ID DOT ID RBRACE relattrexp2  */
#line 1142 "yacc_sql.y"
                                                  {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2155 "yacc_sql.tab.c"
    break;

  case 92: /* relattrexp: MIN LBRACE number RBRACE relattrexp2  */
#line 1147 "yacc_sql.y"
                                               { 
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2168 "yacc_sql.tab.c"
    break;

  case 93: /* relattrexp: MIN LBRACE floatnumber RBRACE relattrexp2  */
#line 1155 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGMIN, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2182 "yacc_sql.tab.c"
    break;

  case 94: /* relattrexp: AVG LBRACE ID RBRACE relattrexp2  */
#line 1164 "yacc_sql.y"
                                       {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2192 "yacc_sql.tab.c"
    break;

  case 95: /* relattrexp: AVG LBRACE STAR RBRACE relattrexp2  */
#line 1169 "yacc_sql.y"
                                             {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2202 "yacc_sql.tab.c"
    break;

  case 96: /* relattrexp: AVG LBRACE ID DOT ID RBRACE relattrexp2  */
#line 1174 "yacc_sql.y"
                                                  {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, (yyvsp[-4].string), (yyvsp[-2].string));
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2212 "yacc_sql.tab.c"
    break;

  case 97: /* relattrexp: AVG LBRACE number RBRACE relattrexp2  */
#line 1179 "yacc_sql.y"
                                               {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGNUMBER;
            exp.agg_val.intv = (yyvsp[-2].number);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2225 "yacc_sql.tab.c"
    break;

  case 98: /* relattrexp: AVG LBRACE floatnumber RBRACE relattrexp2  */
#line 1187 "yacc_sql.y"
                                                    {
            RelAttrExp exp;
            relation_agg_relattrexp_init(&exp, AGGAVG, NULL, "*");
            exp.is_attr = 0;
            exp.agg_val_type = AGGFLOAT;
            exp.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
            exp.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
            advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
        }
#line 2239 "yacc_sql.tab.c"
    break;

  case 99: /* relattrexp: STAR  */
#line 1198 "yacc_sql.y"
           {
      RelAttrExp exp;
      exp.agg_type = NOTAGG;
      exp.is_star = 1;
      exp.relation_name = NULL;
      advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
    }
#line 2251 "yacc_sql.tab.c"
    break;

  case 100: /* relattrexp: ID DOT STAR relattrexp2  */
#line 1205 "yacc_sql.y"
                              {
      RelAttrExp exp;
      exp.agg_type = NOTAGG;
      exp.is_star = 1;
      exp.relation_name = strdup((yyvsp[-3].string));
      advselects_append_relattrexp(&CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]], &exp);
    }
#line 2263 "yacc_sql.tab.c"
    break;

  case 103: /* exp_list: exps exp_list2  */
#line 1223 "yacc_sql.y"
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
#line 2288 "yacc_sql.tab.c"
    break;

  case 104: /* exp_list2: %empty  */
#line 1246 "yacc_sql.y"
             {
    (yyval.exp_list) = NULL;
  }
#line 2296 "yacc_sql.tab.c"
    break;

  case 105: /* exp_list2: PLUS exp_list  */
#line 1249 "yacc_sql.y"
                  {
    ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
    while(explist_right->left_explist) {
      explist_right = explist_right->left_explist;
    }
    explist_right->calop = PLUS_OP;
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2309 "yacc_sql.tab.c"
    break;

  case 106: /* exp_list2: MINUS exp_list  */
#line 1257 "yacc_sql.y"
                   {
    ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
    while(explist_right->left_explist) {
      explist_right = explist_right->left_explist;
    }
    explist_right->calop = MINUS_OP;
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2322 "yacc_sql.tab.c"
    break;

  case 107: /* exp_list3: exp_list  */
#line 1268 "yacc_sql.y"
           {
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2330 "yacc_sql.tab.c"
    break;

  case 108: /* exp_list3: MINUS exp_list  */
#line 1271 "yacc_sql.y"
                   {
    ExpList *explist = (ExpList *)((yyvsp[0].exp_list));
    while(explist->left_explist) {
      explist = explist->left_explist;
    }
    explist->exp->have_negative = 1;
    (yyval.exp_list) = (yyvsp[0].exp_list);
  }
#line 2343 "yacc_sql.tab.c"
    break;

  case 109: /* exps: exp exps2  */
#line 1285 "yacc_sql.y"
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
#line 2363 "yacc_sql.tab.c"
    break;

  case 110: /* exps2: %empty  */
#line 1303 "yacc_sql.y"
             {
    (yyval.exp) = NULL;
  }
#line 2371 "yacc_sql.tab.c"
    break;

  case 111: /* exps2: STAR exps  */
#line 1306 "yacc_sql.y"
              {
    Exp *exp_right = (Exp *)((yyvsp[0].exp));
    while(exp_right->left_exp) {
      exp_right = exp_right->left_exp;
    }
    exp_right->calop = TIME_OP;
    (yyval.exp) = (yyvsp[0].exp);
  }
#line 2384 "yacc_sql.tab.c"
    break;

  case 112: /* exps2: DIVIDE exps  */
#line 1314 "yacc_sql.y"
                {
    Exp *exp_right = (Exp *)((yyvsp[0].exp));
    while(exp_right->left_exp) {
      exp_right = exp_right->left_exp;
    }
    exp_right->calop = DIVIDE_OP;
    (yyval.exp) = (yyvsp[0].exp);
  }
#line 2397 "yacc_sql.tab.c"
    break;

  case 113: /* exp: ID  */
#line 1327 "yacc_sql.y"
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
#line 2416 "yacc_sql.tab.c"
    break;

  case 114: /* exp: ID DOT ID  */
#line 1341 "yacc_sql.y"
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
#line 2435 "yacc_sql.tab.c"
    break;

  case 115: /* exp: NNNUMBER  */
#line 1355 "yacc_sql.y"
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
#line 2456 "yacc_sql.tab.c"
    break;

  case 116: /* exp: NNFLOAT  */
#line 1371 "yacc_sql.y"
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
#line 2477 "yacc_sql.tab.c"
    break;

  case 117: /* exp: SSS  */
#line 1387 "yacc_sql.y"
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
#line 2499 "yacc_sql.tab.c"
    break;

  case 118: /* exp: DATE  */
#line 1404 "yacc_sql.y"
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
#line 2521 "yacc_sql.tab.c"
    break;

  case 119: /* exp: NULL_A  */
#line 1421 "yacc_sql.y"
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
#line 2542 "yacc_sql.tab.c"
    break;

  case 120: /* exp: LBRACE exp_list3 RBRACE  */
#line 1437 "yacc_sql.y"
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
#line 2557 "yacc_sql.tab.c"
    break;

  case 121: /* exp: LBRACE select RBRACE  */
#line 1447 "yacc_sql.y"
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
#line 2576 "yacc_sql.tab.c"
    break;

  case 123: /* rel_list: COMMA ID rel_list  */
#line 1465 "yacc_sql.y"
                        {	
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
        adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-1].string));
		  }
#line 2585 "yacc_sql.tab.c"
    break;

  case 125: /* where: WHERE condition_exps  */
#line 1473 "yacc_sql.y"
                         {
  
  }
#line 2593 "yacc_sql.tab.c"
    break;

  case 127: /* condition_exps: condition_exp  */
#line 1479 "yacc_sql.y"
                  {
  
  }
#line 2601 "yacc_sql.tab.c"
    break;

  case 128: /* condition_exps: condition_exp AND condition_exps  */
#line 1482 "yacc_sql.y"
                                     {
  
  }
#line 2609 "yacc_sql.tab.c"
    break;

  case 129: /* condition_exp: exp_list3 comOp exp_list3  */
#line 1489 "yacc_sql.y"
                            {
    ExpList *explist_left = (ExpList *)((yyvsp[-2].exp_list));
    ExpList *explist_right = (ExpList *)((yyvsp[0].exp_list));
    ConditionExp cond_exp;
    cond_exp.left = explist_left;
    cond_exp.right = explist_right;
    cond_exp.comp = CONTEXT->comp[CONTEXT->comp_deep-1];
    printf("%d\n\n",CONTEXT->comp[CONTEXT->comp_deep-1]);
    CONTEXT->condition_exps[CONTEXT->condition_length++] = cond_exp;
  }
#line 2624 "yacc_sql.tab.c"
    break;

  case 130: /* condition_exp: exp_list3 IS_A NULL_A  */
#line 1500 "yacc_sql.y"
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
#line 2641 "yacc_sql.tab.c"
    break;

  case 131: /* condition_exp: exp_list3 IS_A NOT NULL_A  */
#line 1512 "yacc_sql.y"
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
#line 2658 "yacc_sql.tab.c"
    break;

  case 132: /* join: INNER JOIN ID onwhere  */
#line 1528 "yacc_sql.y"
                          {
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
        adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-1].string));
    }
#line 2667 "yacc_sql.tab.c"
    break;

  case 133: /* join: INNER JOIN ID onwhere join  */
#line 1532 "yacc_sql.y"
                                {
        AdvSelects *adv_selects = &CONTEXT->ssql->sstr.adv_selection[CONTEXT->deep_stack.sel_lengh[CONTEXT->deep_stack.top-1]];
        adv_selects->relations[adv_selects->relation_num++] = strdup((yyvsp[-2].string));
    }
#line 2676 "yacc_sql.tab.c"
    break;

  case 135: /* onwhere: ON condition_exps  */
#line 1539 "yacc_sql.y"
                        {

    }
#line 2684 "yacc_sql.tab.c"
    break;

  case 136: /* comOp: EQ  */
#line 1546 "yacc_sql.y"
             { CONTEXT->comp[CONTEXT->comp_deep-1] = EQUAL_TO; }
#line 2690 "yacc_sql.tab.c"
    break;

  case 137: /* comOp: LT  */
#line 1547 "yacc_sql.y"
         { CONTEXT->comp[CONTEXT->comp_deep-1] = LESS_THAN; }
#line 2696 "yacc_sql.tab.c"
    break;

  case 138: /* comOp: GT  */
#line 1548 "yacc_sql.y"
         { CONTEXT->comp[CONTEXT->comp_deep-1] = GREAT_THAN; }
#line 2702 "yacc_sql.tab.c"
    break;

  case 139: /* comOp: LE  */
#line 1549 "yacc_sql.y"
         { CONTEXT->comp[CONTEXT->comp_deep-1] = LESS_EQUAL; }
#line 2708 "yacc_sql.tab.c"
    break;

  case 140: /* comOp: GE  */
#line 1550 "yacc_sql.y"
         { CONTEXT->comp[CONTEXT->comp_deep-1] = GREAT_EQUAL; }
#line 2714 "yacc_sql.tab.c"
    break;

  case 141: /* comOp: NE  */
#line 1551 "yacc_sql.y"
         { CONTEXT->comp[CONTEXT->comp_deep-1] = NOT_EQUAL; }
#line 2720 "yacc_sql.tab.c"
    break;

  case 142: /* comOp: IN  */
#line 1552 "yacc_sql.y"
         { CONTEXT->comp[CONTEXT->comp_deep-1] = IN_SUB;
            printf("%d\n\n",CONTEXT->comp[CONTEXT->comp_deep-1]);
            }
#line 2728 "yacc_sql.tab.c"
    break;

  case 143: /* comOp: NOT IN  */
#line 1555 "yacc_sql.y"
             { CONTEXT->comp[CONTEXT->comp_deep-1] = NOT_IN_SUB;}
#line 2734 "yacc_sql.tab.c"
    break;

  case 144: /* load_data: LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON  */
#line 1560 "yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 2743 "yacc_sql.tab.c"
    break;


#line 2747 "yacc_sql.tab.c"

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

#line 1565 "yacc_sql.y"

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
