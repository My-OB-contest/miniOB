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

typedef struct ParserContext {
  Query * ssql;
  size_t select_length;
  size_t condition_length;
  size_t from_length;
  size_t value_length;
  Value values[MAX_NUM];
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
  context->ssql->sstr.insertion.value_num = 0;
  printf("parse sql failed. error=%s", str);
}


ParserContext *get_context(yyscan_t scanner)
{
  return (ParserContext *)yyget_extra(scanner);
}

#define CONTEXT get_context(scanner)


#line 129 "yacc_sql.tab.c"

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
  YYSYMBOL_UNIQUE = 54,                    /* UNIQUE  */
  YYSYMBOL_NUMBER = 55,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 56,                     /* FLOAT  */
  YYSYMBOL_ID = 57,                        /* ID  */
  YYSYMBOL_DATE = 58,                      /* DATE  */
  YYSYMBOL_PATH = 59,                      /* PATH  */
  YYSYMBOL_SSS = 60,                       /* SSS  */
  YYSYMBOL_STAR = 61,                      /* STAR  */
  YYSYMBOL_STRING_V = 62,                  /* STRING_V  */
  YYSYMBOL_YYACCEPT = 63,                  /* $accept  */
  YYSYMBOL_commands = 64,                  /* commands  */
  YYSYMBOL_command = 65,                   /* command  */
  YYSYMBOL_exit = 66,                      /* exit  */
  YYSYMBOL_help = 67,                      /* help  */
  YYSYMBOL_sync = 68,                      /* sync  */
  YYSYMBOL_begin = 69,                     /* begin  */
  YYSYMBOL_commit = 70,                    /* commit  */
  YYSYMBOL_rollback = 71,                  /* rollback  */
  YYSYMBOL_drop_table = 72,                /* drop_table  */
  YYSYMBOL_show_tables = 73,               /* show_tables  */
  YYSYMBOL_desc_table = 74,                /* desc_table  */
  YYSYMBOL_create_index = 75,              /* create_index  */
  YYSYMBOL_drop_index = 76,                /* drop_index  */
  YYSYMBOL_create_table = 77,              /* create_table  */
  YYSYMBOL_attr_def_list = 78,             /* attr_def_list  */
  YYSYMBOL_attr_def = 79,                  /* attr_def  */
  YYSYMBOL_number = 80,                    /* number  */
  YYSYMBOL_type = 81,                      /* type  */
  YYSYMBOL_ID_get = 82,                    /* ID_get  */
  YYSYMBOL_insert = 83,                    /* insert  */
  YYSYMBOL_value_list = 84,                /* value_list  */
  YYSYMBOL_value = 85,                     /* value  */
  YYSYMBOL_delete = 86,                    /* delete  */
  YYSYMBOL_update = 87,                    /* update  */
  YYSYMBOL_select = 88,                    /* select  */
  YYSYMBOL_select_attr = 89,               /* select_attr  */
  YYSYMBOL_attr_list = 90,                 /* attr_list  */
  YYSYMBOL_rel_list = 91,                  /* rel_list  */
  YYSYMBOL_join = 92,                      /* join  */
  YYSYMBOL_onwhere = 93,                   /* onwhere  */
  YYSYMBOL_where = 94,                     /* where  */
  YYSYMBOL_condition_list = 95,            /* condition_list  */
  YYSYMBOL_condition = 96,                 /* condition  */
  YYSYMBOL_comOp = 97,                     /* comOp  */
  YYSYMBOL_load_data = 98                  /* load_data  */
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
#define YYLAST   285

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  110
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  264

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   317


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
      55,    56,    57,    58,    59,    60,    61,    62
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   158,   158,   160,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   184,   189,   194,   200,   206,   212,   218,   224,   230,
     237,   242,   250,   257,   266,   268,   272,   283,   297,   309,
     321,   333,   348,   351,   352,   353,   354,   357,   366,   382,
     384,   389,   392,   395,   399,   404,   411,   421,   431,   447,
     461,   466,   471,   480,   489,   494,   499,   504,   509,   517,
     526,   531,   536,   541,   549,   558,   563,   568,   573,   581,
     590,   595,   600,   605,   613,   625,   627,   634,   636,   642,
     645,   649,   651,   655,   657,   661,   663,   668,   689,   709,
     729,   751,   772,   793,   815,   816,   817,   818,   819,   820,
     824
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
  "NULLABLE", "UNIQUE", "NUMBER", "FLOAT", "ID", "DATE", "PATH", "SSS",
  "STAR", "STRING_V", "$accept", "commands", "command", "exit", "help",
  "sync", "begin", "commit", "rollback", "drop_table", "show_tables",
  "desc_table", "create_index", "drop_index", "create_table",
  "attr_def_list", "attr_def", "number", "type", "ID_get", "insert",
  "value_list", "value", "delete", "update", "select", "select_attr",
  "attr_list", "rel_list", "join", "onwhere", "where", "condition_list",
  "condition", "comOp", "load_data", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-213)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -213,    75,  -213,     0,    45,    20,   -41,    28,    18,    26,
      42,    25,    73,    80,    95,    96,   110,    63,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,  -213,    57,    64,   120,    78,
      85,   129,   131,   140,   141,    15,  -213,   132,   159,   161,
    -213,   115,   116,   142,  -213,  -213,  -213,  -213,  -213,   136,
     162,   144,   118,   174,   177,    36,    94,    97,   104,    20,
     -21,  -213,   124,  -213,  -213,   152,   151,   128,   126,   130,
     133,   149,  -213,  -213,   171,   172,     9,   175,   176,   178,
      14,   179,   180,   181,    17,   182,   183,   184,    22,   185,
    -213,   173,   173,    -8,   187,    60,   191,   165,   186,  -213,
     188,    47,   189,   150,   173,   173,   173,    -9,   173,   173,
     173,   173,   153,   173,   173,   173,   173,   154,   173,   173,
     173,   173,   155,   173,  -213,  -213,   156,   158,   151,   151,
      88,  -213,  -213,  -213,    19,  -213,  -213,   127,   190,  -213,
      88,   203,   130,   197,  -213,  -213,  -213,  -213,    -7,   160,
     200,  -213,  -213,  -213,   201,   202,  -213,  -213,  -213,  -213,
     204,  -213,  -213,  -213,  -213,   205,  -213,  -213,  -213,  -213,
     207,  -213,   208,   163,   222,   224,   210,   192,  -213,  -213,
    -213,  -213,  -213,  -213,    67,    74,    60,  -213,   151,   193,
     188,   226,   196,   194,  -213,   213,   195,   173,   173,   173,
     173,   173,  -213,   198,  -213,  -213,    88,   214,   127,  -213,
    -213,   206,  -213,   190,   229,   232,  -213,  -213,  -213,   219,
    -213,   234,   221,  -213,  -213,  -213,  -213,  -213,    60,   199,
     210,   236,    81,   209,  -213,  -213,  -213,    52,  -213,   237,
     190,  -213,  -213,  -213,   215,  -213,  -213,   211,  -213,  -213,
    -213,   212,  -213,  -213
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     3,    20,
      19,    14,    15,    16,    17,     9,    10,    11,    12,    13,
       8,     5,     7,     6,     4,    18,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    85,    60,     0,     0,     0,
      23,     0,     0,     0,    24,    25,    26,    22,    21,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    61,     0,    29,    28,     0,    93,     0,     0,     0,
       0,     0,    27,    32,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      86,    85,    85,    87,     0,     0,     0,     0,     0,    47,
      34,     0,     0,     0,    85,    85,    85,     0,    85,    85,
      85,    85,     0,    85,    85,    85,    85,     0,    85,    85,
      85,    85,     0,    85,    62,    63,     0,     0,    93,    93,
       0,    55,    51,    52,     0,    54,    53,     0,    95,    56,
       0,     0,     0,     0,    43,    44,    45,    46,    37,     0,
       0,    68,    69,    64,     0,     0,    65,    73,    74,    70,
       0,    71,    78,    79,    75,     0,    76,    83,    84,    80,
       0,    81,    87,     0,     0,     0,    49,     0,   104,   105,
     106,   107,   108,   109,     0,     0,     0,    94,    93,     0,
      34,     0,     0,     0,    41,     0,     0,    85,    85,    85,
      85,    85,    88,    91,    58,    59,     0,     0,     0,    99,
      97,   100,    98,    95,     0,     0,    35,    33,    42,     0,
      40,     0,     0,    66,    67,    72,    77,    82,     0,    89,
      49,     0,     0,     0,    96,    57,   110,    36,    30,     0,
      95,    90,    50,    48,     0,   101,   102,     0,    39,    31,
      92,     0,    38,   103
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,  -213,
    -213,  -213,  -213,  -213,  -213,    41,    90,  -213,  -213,  -213,
    -213,     4,  -138,  -213,  -213,  -213,   216,  -101,    65,     6,
    -213,  -134,  -212,  -189,  -144,  -213
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     1,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,   153,   110,   229,   158,   111,
      31,   217,   147,    32,    33,    34,    47,    71,   138,   139,
     239,   106,   197,   148,   194,    35
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     134,   135,   186,   195,   184,   185,    36,   223,    37,   202,
     136,   244,   198,   161,   162,   163,    48,   166,   167,   168,
     169,    50,   171,   172,   173,   174,   116,   176,   177,   178,
     179,   121,   181,    69,   126,    49,   101,   117,   260,   131,
     102,   137,   122,    70,   203,   127,   204,   187,   164,   250,
     132,    39,   165,    40,    38,    51,   220,   222,   188,   189,
     190,   191,   192,   193,   224,    41,    42,    43,    44,   154,
     155,   156,   157,    52,   242,     2,    54,    45,   240,     3,
       4,    46,    53,    55,     5,     6,     7,     8,     9,    10,
      11,    84,    85,    86,    12,    13,    14,    87,    56,    57,
      59,    15,    16,   257,   255,   258,   233,   234,   235,   236,
     237,    17,   141,    58,    60,   142,   143,   144,   145,   141,
     146,    61,   142,   143,   219,   145,   141,   146,    62,   142,
     143,   221,   145,   141,   146,    63,   142,   143,   254,   145,
     141,   146,    64,   142,   143,    65,   145,    66,   146,    88,
      89,    90,    92,    93,    94,    91,    67,    68,    95,    96,
      97,    98,    73,    72,    74,    99,   188,   189,   190,   191,
     192,   193,    75,    76,    78,    81,    77,    82,    79,    80,
      83,   103,   104,   105,   113,   107,   108,   109,   114,   115,
     112,    69,   118,   119,   149,   120,   123,   124,   125,   128,
     129,   130,   133,   140,   150,   159,   152,   160,   183,   199,
     170,   175,   180,   182,   201,   151,   206,   205,   207,   208,
     213,   209,   210,   196,   211,   214,   136,   215,   216,   227,
     231,   241,   245,   238,   243,   246,   247,   248,   249,   253,
     259,   226,   200,   261,   252,   251,   230,   212,   137,   218,
     225,   228,   232,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   262,     0,     0,   256,     0,     0,   263,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   100
};

static const yytype_int16 yycheck[] =
{
     101,   102,   140,   147,   138,   139,     6,   196,     8,    16,
      18,   223,   150,   114,   115,   116,    57,   118,   119,   120,
     121,     3,   123,   124,   125,   126,    17,   128,   129,   130,
     131,    17,   133,    18,    17,     7,    57,    28,   250,    17,
      61,    49,    28,    28,    51,    28,    53,    28,    57,   238,
      28,     6,    61,     8,    54,    29,   194,   195,    39,    40,
      41,    42,    43,    44,   198,    45,    46,    47,    48,    22,
      23,    24,    25,    31,   218,     0,     3,    57,   216,     4,
       5,    61,    57,     3,     9,    10,    11,    12,    13,    14,
      15,    55,    56,    57,    19,    20,    21,    61,     3,     3,
      37,    26,    27,    51,   242,    53,   207,   208,   209,   210,
     211,    36,    52,     3,    57,    55,    56,    57,    58,    52,
      60,    57,    55,    56,    57,    58,    52,    60,     8,    55,
      56,    57,    58,    52,    60,    57,    55,    56,    57,    58,
      52,    60,    57,    55,    56,    16,    58,    16,    60,    55,
      56,    57,    55,    56,    57,    61,    16,    16,    61,    55,
      56,    57,     3,    31,     3,    61,    39,    40,    41,    42,
      43,    44,    57,    57,    38,    57,    34,     3,    16,    35,
       3,    57,    30,    32,    35,    57,    60,    57,    17,    17,
      57,    18,    17,    17,     3,    17,    17,    17,    17,    17,
      17,    17,    17,    16,    39,    16,    18,    57,    50,     6,
      57,    57,    57,    57,    17,    29,    16,    57,    17,    17,
      57,    17,    17,    33,    17,     3,    18,     3,    18,     3,
      17,    17,     3,    35,    28,     3,    17,     3,    17,     3,
       3,   200,   152,    28,   240,   239,    52,   182,    49,    57,
      57,    55,    57,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    57,    -1,    -1,    57,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    69
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    64,     0,     4,     5,     9,    10,    11,    12,    13,
      14,    15,    19,    20,    21,    26,    27,    36,    65,    66,
      67,    68,    69,    70,    71,    72,    73,    74,    75,    76,
      77,    83,    86,    87,    88,    98,     6,     8,    54,     6,
       8,    45,    46,    47,    48,    57,    61,    89,    57,     7,
       3,    29,    31,    57,     3,     3,     3,     3,     3,    37,
      57,    57,     8,    57,    57,    16,    16,    16,    16,    18,
      28,    90,    31,     3,     3,    57,    57,    34,    38,    16,
      35,    57,     3,     3,    55,    56,    57,    61,    55,    56,
      57,    61,    55,    56,    57,    61,    55,    56,    57,    61,
      89,    57,    61,    57,    30,    32,    94,    57,    60,    57,
      79,    82,    57,    35,    17,    17,    17,    28,    17,    17,
      17,    17,    28,    17,    17,    17,    17,    28,    17,    17,
      17,    17,    28,    17,    90,    90,    18,    49,    91,    92,
      16,    52,    55,    56,    57,    58,    60,    85,    96,     3,
      39,    29,    18,    78,    22,    23,    24,    25,    81,    16,
      57,    90,    90,    90,    57,    61,    90,    90,    90,    90,
      57,    90,    90,    90,    90,    57,    90,    90,    90,    90,
      57,    90,    57,    50,    94,    94,    85,    28,    39,    40,
      41,    42,    43,    44,    97,    97,    33,    95,    85,     6,
      79,    17,    16,    51,    53,    57,    16,    17,    17,    17,
      17,    17,    91,    57,     3,     3,    18,    84,    57,    57,
      85,    57,    85,    96,    94,    57,    78,     3,    55,    80,
      52,    17,    57,    90,    90,    90,    90,    90,    35,    93,
      85,    17,    97,    28,    95,     3,     3,    17,     3,    17,
      96,    92,    84,     3,    57,    85,    57,    51,    53,     3,
      95,    28,    52,    57
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    63,    64,    64,    65,    65,    65,    65,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    65,    65,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    75,    76,    77,    78,    78,    79,    79,    79,    79,
      79,    79,    80,    81,    81,    81,    81,    82,    83,    84,
      84,    85,    85,    85,    85,    85,    86,    87,    88,    88,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    90,    90,    91,    91,    92,
      92,    93,    93,    94,    94,    95,    95,    96,    96,    96,
      96,    96,    96,    96,    97,    97,    97,    97,    97,    97,
      98
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     2,     2,     2,     2,     4,     3,     3,
       9,    10,     4,     8,     0,     3,     5,     2,     7,     6,
       4,     3,     1,     1,     1,     1,     1,     1,     9,     0,
       3,     1,     1,     1,     1,     1,     5,     8,     7,     7,
       1,     2,     4,     4,     5,     5,     7,     7,     5,     5,
       5,     5,     7,     5,     5,     5,     5,     7,     5,     5,
       5,     5,     7,     5,     5,     0,     2,     0,     3,     4,
       5,     0,     3,     0,     3,     0,     3,     3,     3,     3,
       3,     5,     5,     7,     1,     1,     1,     1,     1,     1,
       8
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
#line 184 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_EXIT;//"exit";
    }
#line 1402 "yacc_sql.tab.c"
    break;

  case 22: /* help: HELP SEMICOLON  */
#line 189 "yacc_sql.y"
                   {
        CONTEXT->ssql->flag=SCF_HELP;//"help";
    }
#line 1410 "yacc_sql.tab.c"
    break;

  case 23: /* sync: SYNC SEMICOLON  */
#line 194 "yacc_sql.y"
                   {
      CONTEXT->ssql->flag = SCF_SYNC;
    }
#line 1418 "yacc_sql.tab.c"
    break;

  case 24: /* begin: TRX_BEGIN SEMICOLON  */
#line 200 "yacc_sql.y"
                        {
      CONTEXT->ssql->flag = SCF_BEGIN;
    }
#line 1426 "yacc_sql.tab.c"
    break;

  case 25: /* commit: TRX_COMMIT SEMICOLON  */
#line 206 "yacc_sql.y"
                         {
      CONTEXT->ssql->flag = SCF_COMMIT;
    }
#line 1434 "yacc_sql.tab.c"
    break;

  case 26: /* rollback: TRX_ROLLBACK SEMICOLON  */
#line 212 "yacc_sql.y"
                           {
      CONTEXT->ssql->flag = SCF_ROLLBACK;
    }
#line 1442 "yacc_sql.tab.c"
    break;

  case 27: /* drop_table: DROP TABLE ID SEMICOLON  */
#line 218 "yacc_sql.y"
                            {
        CONTEXT->ssql->flag = SCF_DROP_TABLE;//"drop_table";
        drop_table_init(&CONTEXT->ssql->sstr.drop_table, (yyvsp[-1].string));
    }
#line 1451 "yacc_sql.tab.c"
    break;

  case 28: /* show_tables: SHOW TABLES SEMICOLON  */
#line 224 "yacc_sql.y"
                          {
      CONTEXT->ssql->flag = SCF_SHOW_TABLES;
    }
#line 1459 "yacc_sql.tab.c"
    break;

  case 29: /* desc_table: DESC ID SEMICOLON  */
#line 230 "yacc_sql.y"
                      {
      CONTEXT->ssql->flag = SCF_DESC_TABLE;
      desc_table_init(&CONTEXT->ssql->sstr.desc_table, (yyvsp[-1].string));
    }
#line 1468 "yacc_sql.tab.c"
    break;

  case 30: /* create_index: CREATE INDEX ID ON ID LBRACE ID RBRACE SEMICOLON  */
#line 238 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_index";
			create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string) ,0);
		}
#line 1477 "yacc_sql.tab.c"
    break;

  case 31: /* create_index: CREATE UNIQUE INDEX ID ON ID LBRACE ID RBRACE SEMICOLON  */
#line 243 "yacc_sql.y"
            {
	        CONTEXT->ssql->flag = SCF_CREATE_INDEX;//"create_unique_index";
            create_index_init(&CONTEXT->ssql->sstr.create_index, (yyvsp[-6].string), (yyvsp[-4].string), (yyvsp[-2].string) ,1);
	    }
#line 1486 "yacc_sql.tab.c"
    break;

  case 32: /* drop_index: DROP INDEX ID SEMICOLON  */
#line 251 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_DROP_INDEX;//"drop_index";
			drop_index_init(&CONTEXT->ssql->sstr.drop_index, (yyvsp[-1].string));
		}
#line 1495 "yacc_sql.tab.c"
    break;

  case 33: /* create_table: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE SEMICOLON  */
#line 258 "yacc_sql.y"
                {
			CONTEXT->ssql->flag=SCF_CREATE_TABLE;//"create_table";
			// CONTEXT->ssql->sstr.create_table.attribute_count = CONTEXT->value_length;
			create_table_init_name(&CONTEXT->ssql->sstr.create_table, (yyvsp[-5].string));
			//临时变量清零	
			CONTEXT->value_length = 0;
		}
#line 1507 "yacc_sql.tab.c"
    break;

  case 35: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 268 "yacc_sql.y"
                                   {    }
#line 1513 "yacc_sql.tab.c"
    break;

  case 36: /* attr_def: ID_get type LBRACE number RBRACE  */
#line 273 "yacc_sql.y"
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
#line 1528 "yacc_sql.tab.c"
    break;

  case 37: /* attr_def: ID_get type  */
#line 284 "yacc_sql.y"
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
#line 1543 "yacc_sql.tab.c"
    break;

  case 38: /* attr_def: ID_get type LBRACE number RBRACE NOT NULL_A  */
#line 298 "yacc_sql.y"
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
#line 1559 "yacc_sql.tab.c"
    break;

  case 39: /* attr_def: ID_get type LBRACE number RBRACE NULLABLE  */
#line 310 "yacc_sql.y"
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
#line 1575 "yacc_sql.tab.c"
    break;

  case 40: /* attr_def: ID_get type NOT NULL_A  */
#line 322 "yacc_sql.y"
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
#line 1591 "yacc_sql.tab.c"
    break;

  case 41: /* attr_def: ID_get type NULLABLE  */
#line 334 "yacc_sql.y"
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
#line 1607 "yacc_sql.tab.c"
    break;

  case 42: /* number: NUMBER  */
#line 348 "yacc_sql.y"
                       {(yyval.number) = (yyvsp[0].number);}
#line 1613 "yacc_sql.tab.c"
    break;

  case 43: /* type: INT_T  */
#line 351 "yacc_sql.y"
              { (yyval.number)=INTS; }
#line 1619 "yacc_sql.tab.c"
    break;

  case 44: /* type: STRING_T  */
#line 352 "yacc_sql.y"
                  { (yyval.number)=CHARS; }
#line 1625 "yacc_sql.tab.c"
    break;

  case 45: /* type: FLOAT_T  */
#line 353 "yacc_sql.y"
                 { (yyval.number)=FLOATS; }
#line 1631 "yacc_sql.tab.c"
    break;

  case 46: /* type: DATE_T  */
#line 354 "yacc_sql.y"
                    { (yyval.number)=DATES; }
#line 1637 "yacc_sql.tab.c"
    break;

  case 47: /* ID_get: ID  */
#line 358 "yacc_sql.y"
        {
		char *temp=(yyvsp[0].string); 
		snprintf(CONTEXT->id, sizeof(CONTEXT->id), "%s", temp);
	}
#line 1646 "yacc_sql.tab.c"
    break;

  case 48: /* insert: INSERT INTO ID VALUES LBRACE value value_list RBRACE SEMICOLON  */
#line 367 "yacc_sql.y"
                {
			// CONTEXT->values[CONTEXT->value_length++] = *$6;

			CONTEXT->ssql->flag=SCF_INSERT;//"insert";
			// CONTEXT->ssql->sstr.insertion.relation_name = $3;
			// CONTEXT->ssql->sstr.insertion.value_num = CONTEXT->value_length;
			// for(i = 0; i < CONTEXT->value_length; i++){
			// 	CONTEXT->ssql->sstr.insertion.values[i] = CONTEXT->values[i];
      // }
			inserts_init(&CONTEXT->ssql->sstr.insertion, (yyvsp[-6].string), CONTEXT->values, CONTEXT->value_length);

      //临时变量清零
      CONTEXT->value_length=0;
    }
#line 1665 "yacc_sql.tab.c"
    break;

  case 50: /* value_list: COMMA value value_list  */
#line 384 "yacc_sql.y"
                              { 
  		// CONTEXT->values[CONTEXT->value_length++] = *$2;
	  }
#line 1673 "yacc_sql.tab.c"
    break;

  case 51: /* value: NUMBER  */
#line 389 "yacc_sql.y"
          {	
  		value_init_integer(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].number));
		}
#line 1681 "yacc_sql.tab.c"
    break;

  case 52: /* value: FLOAT  */
#line 392 "yacc_sql.y"
          {
  		value_init_float(&CONTEXT->values[CONTEXT->value_length++], ((yyvsp[0].floatsAndStr)).floats);
		}
#line 1689 "yacc_sql.tab.c"
    break;

  case 53: /* value: SSS  */
#line 395 "yacc_sql.y"
         {
			(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_string(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
		}
#line 1698 "yacc_sql.tab.c"
    break;

  case 54: /* value: DATE  */
#line 399 "yacc_sql.y"
              {
		(yyvsp[0].string) = substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
  		value_init_date(&CONTEXT->values[CONTEXT->value_length++], (yyvsp[0].string));
	}
#line 1707 "yacc_sql.tab.c"
    break;

  case 55: /* value: NULL_A  */
#line 404 "yacc_sql.y"
                {
		value_init_null(&CONTEXT->values[CONTEXT->value_length++]);
	}
#line 1715 "yacc_sql.tab.c"
    break;

  case 56: /* delete: DELETE FROM ID where SEMICOLON  */
#line 412 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_DELETE;//"delete";
			deletes_init_relation(&CONTEXT->ssql->sstr.deletion, (yyvsp[-2].string));
			deletes_set_conditions(&CONTEXT->ssql->sstr.deletion, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;	
    }
#line 1727 "yacc_sql.tab.c"
    break;

  case 57: /* update: UPDATE ID SET ID EQ value where SEMICOLON  */
#line 422 "yacc_sql.y"
                {
			CONTEXT->ssql->flag = SCF_UPDATE;//"update";
			Value *value = &CONTEXT->values[0];
			updates_init(&CONTEXT->ssql->sstr.update, (yyvsp[-6].string), (yyvsp[-4].string), value, 
					CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->condition_length = 0;
		}
#line 1739 "yacc_sql.tab.c"
    break;

  case 58: /* select: SELECT select_attr FROM ID rel_list where SEMICOLON  */
#line 432 "yacc_sql.y"
                {
			// CONTEXT->ssql->sstr.selection.relations[CONTEXT->from_length++]=$4;
			selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-3].string));

			selects_append_conditions(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);

			CONTEXT->ssql->flag=SCF_SELECT;//"select";
			// CONTEXT->ssql->sstr.selection.attr_num = CONTEXT->select_length;

			//临时变量清零
			CONTEXT->condition_length=0;
			CONTEXT->from_length=0;
			CONTEXT->select_length=0;
			CONTEXT->value_length = 0;
	}
#line 1759 "yacc_sql.tab.c"
    break;

  case 59: /* select: SELECT select_attr FROM ID join where SEMICOLON  */
#line 448 "yacc_sql.y"
            {
			selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-3].string));
			selects_append_conditions(&CONTEXT->ssql->sstr.selection, CONTEXT->conditions, CONTEXT->condition_length);
			CONTEXT->ssql->flag=SCF_SELECT;//"select";
			CONTEXT->condition_length=0;
			CONTEXT->from_length=0;
			CONTEXT->select_length=0;
			CONTEXT->value_length = 0;

	    }
#line 1774 "yacc_sql.tab.c"
    break;

  case 60: /* select_attr: STAR  */
#line 461 "yacc_sql.y"
         {  
			RelAttr attr;
			relation_attr_init(&attr, NULL, "*");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1784 "yacc_sql.tab.c"
    break;

  case 61: /* select_attr: ID attr_list  */
#line 466 "yacc_sql.y"
                   {
			RelAttr attr;
			relation_attr_init(&attr, NULL, (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1794 "yacc_sql.tab.c"
    break;

  case 62: /* select_attr: ID DOT ID attr_list  */
#line 471 "yacc_sql.y"
                              {
			RelAttr attr;
			relation_attr_init(&attr, (yyvsp[-3].string), (yyvsp[-1].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1804 "yacc_sql.tab.c"
    break;

  case 63: /* select_attr: ID DOT STAR attr_list  */
#line 480 "yacc_sql.y"
                            {
            RelAttr attr;
            relation_attr_init(&attr, (yyvsp[-3].string), "*");
            selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
        }
#line 1814 "yacc_sql.tab.c"
    break;

  case 64: /* select_attr: COUNT LBRACE ID RBRACE attr_list  */
#line 489 "yacc_sql.y"
                                           {
            RelAttr attr;
			relation_agg_attr_init(&attr, AGGCOUNT, NULL, (yyvsp[-2].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	    }
#line 1824 "yacc_sql.tab.c"
    break;

  case 65: /* select_attr: COUNT LBRACE STAR RBRACE attr_list  */
#line 494 "yacc_sql.y"
                                                 {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGCOUNT, NULL, "*");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1834 "yacc_sql.tab.c"
    break;

  case 66: /* select_attr: COUNT LBRACE ID DOT ID RBRACE attr_list  */
#line 499 "yacc_sql.y"
                                                          {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGCOUNT, (yyvsp[-4].string), (yyvsp[-2].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1844 "yacc_sql.tab.c"
    break;

  case 67: /* select_attr: COUNT LBRACE ID DOT STAR RBRACE attr_list  */
#line 504 "yacc_sql.y"
                                                            {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGCOUNT, (yyvsp[-4].string), "*");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1854 "yacc_sql.tab.c"
    break;

  case 68: /* select_attr: COUNT LBRACE NUMBER RBRACE attr_list  */
#line 509 "yacc_sql.y"
                                                       {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGCOUNT, NULL, "*");
			attr.is_attr = 0;
			attr.agg_val_type = AGGNUMBER;
			attr.agg_val.intv = (yyvsp[-2].number);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1867 "yacc_sql.tab.c"
    break;

  case 69: /* select_attr: COUNT LBRACE FLOAT RBRACE attr_list  */
#line 517 "yacc_sql.y"
                                                      {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGCOUNT, NULL, "*");
			attr.is_attr = 0;
			attr.agg_val_type = AGGFLOAT;
			attr.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
			attr.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1881 "yacc_sql.tab.c"
    break;

  case 70: /* select_attr: MAX LBRACE ID RBRACE attr_list  */
#line 526 "yacc_sql.y"
                                         {
            RelAttr attr;
			relation_agg_attr_init(&attr, AGGMAX, NULL, (yyvsp[-2].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	    }
#line 1891 "yacc_sql.tab.c"
    break;

  case 71: /* select_attr: MAX LBRACE STAR RBRACE attr_list  */
#line 531 "yacc_sql.y"
                                               {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGMAX, NULL, "*");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1901 "yacc_sql.tab.c"
    break;

  case 72: /* select_attr: MAX LBRACE ID DOT ID RBRACE attr_list  */
#line 536 "yacc_sql.y"
                                                        {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGMAX, (yyvsp[-4].string), (yyvsp[-2].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1911 "yacc_sql.tab.c"
    break;

  case 73: /* select_attr: MAX LBRACE NUMBER RBRACE attr_list  */
#line 541 "yacc_sql.y"
                                                     {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGMAX, NULL, "*");
			attr.is_attr = 0;
			attr.agg_val_type = AGGNUMBER;
			attr.agg_val.intv = (yyvsp[-2].number);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1924 "yacc_sql.tab.c"
    break;

  case 74: /* select_attr: MAX LBRACE FLOAT RBRACE attr_list  */
#line 549 "yacc_sql.y"
                                                    {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGMAX, NULL, "*");
			attr.is_attr = 0;
			attr.agg_val_type = AGGFLOAT;
			attr.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
			attr.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1938 "yacc_sql.tab.c"
    break;

  case 75: /* select_attr: MIN LBRACE ID RBRACE attr_list  */
#line 558 "yacc_sql.y"
                                         {
            RelAttr attr;
			relation_agg_attr_init(&attr, AGGMIN, NULL, (yyvsp[-2].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	    }
#line 1948 "yacc_sql.tab.c"
    break;

  case 76: /* select_attr: MIN LBRACE STAR RBRACE attr_list  */
#line 563 "yacc_sql.y"
                                               {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGMIN, NULL, "*");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1958 "yacc_sql.tab.c"
    break;

  case 77: /* select_attr: MIN LBRACE ID DOT ID RBRACE attr_list  */
#line 568 "yacc_sql.y"
                                                        {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGMIN, (yyvsp[-4].string), (yyvsp[-2].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1968 "yacc_sql.tab.c"
    break;

  case 78: /* select_attr: MIN LBRACE NUMBER RBRACE attr_list  */
#line 573 "yacc_sql.y"
                                                     {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGMIN, NULL, "*");
			attr.is_attr = 0;
			attr.agg_val_type = AGGNUMBER;
			attr.agg_val.intv = (yyvsp[-2].number);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1981 "yacc_sql.tab.c"
    break;

  case 79: /* select_attr: MIN LBRACE FLOAT RBRACE attr_list  */
#line 581 "yacc_sql.y"
                                                    {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGMIN, NULL, "*");
			attr.is_attr = 0;
			attr.agg_val_type = AGGFLOAT;
			attr.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
			attr.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 1995 "yacc_sql.tab.c"
    break;

  case 80: /* select_attr: AVG LBRACE ID RBRACE attr_list  */
#line 590 "yacc_sql.y"
                                         {
            RelAttr attr;
			relation_agg_attr_init(&attr, AGGAVG, NULL, (yyvsp[-2].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
	    }
#line 2005 "yacc_sql.tab.c"
    break;

  case 81: /* select_attr: AVG LBRACE STAR RBRACE attr_list  */
#line 595 "yacc_sql.y"
                                               {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGAVG, NULL, "*");
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 2015 "yacc_sql.tab.c"
    break;

  case 82: /* select_attr: AVG LBRACE ID DOT ID RBRACE attr_list  */
#line 600 "yacc_sql.y"
                                                        {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGAVG, (yyvsp[-4].string), (yyvsp[-2].string));
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 2025 "yacc_sql.tab.c"
    break;

  case 83: /* select_attr: AVG LBRACE NUMBER RBRACE attr_list  */
#line 605 "yacc_sql.y"
                                                     {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGAVG, NULL, "*");
			attr.is_attr = 0;
			attr.agg_val_type = AGGNUMBER;
			attr.agg_val.intv = (yyvsp[-2].number);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 2038 "yacc_sql.tab.c"
    break;

  case 84: /* select_attr: AVG LBRACE FLOAT RBRACE attr_list  */
#line 613 "yacc_sql.y"
                                                    {
			RelAttr attr;
			relation_agg_attr_init(&attr, AGGAVG, NULL, "*");
			attr.is_attr = 0;
			attr.agg_val_type = AGGFLOAT;
			attr.agg_val.floatv = ((yyvsp[-2].floatsAndStr)).floats;
			attr.agg_val.str = strdup(((yyvsp[-2].floatsAndStr)).str);
			selects_append_attribute(&CONTEXT->ssql->sstr.selection, &attr);
		}
#line 2052 "yacc_sql.tab.c"
    break;

  case 86: /* attr_list: COMMA select_attr  */
#line 627 "yacc_sql.y"
                        {
		
	}
#line 2060 "yacc_sql.tab.c"
    break;

  case 88: /* rel_list: COMMA ID rel_list  */
#line 636 "yacc_sql.y"
                        {	
				selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-1].string));
		  }
#line 2068 "yacc_sql.tab.c"
    break;

  case 89: /* join: INNER JOIN ID onwhere  */
#line 642 "yacc_sql.y"
                          {
        selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-1].string));
    }
#line 2076 "yacc_sql.tab.c"
    break;

  case 90: /* join: INNER JOIN ID onwhere join  */
#line 645 "yacc_sql.y"
                                {
        selects_append_relation(&CONTEXT->ssql->sstr.selection, (yyvsp[-2].string));
    }
#line 2084 "yacc_sql.tab.c"
    break;

  case 92: /* onwhere: ON condition condition_list  */
#line 651 "yacc_sql.y"
                                 {

    }
#line 2092 "yacc_sql.tab.c"
    break;

  case 94: /* where: WHERE condition condition_list  */
#line 657 "yacc_sql.y"
                                     {	
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 2100 "yacc_sql.tab.c"
    break;

  case 96: /* condition_list: AND condition condition_list  */
#line 663 "yacc_sql.y"
                                   {
				// CONTEXT->conditions[CONTEXT->condition_length++]=*$2;
			}
#line 2108 "yacc_sql.tab.c"
    break;

  case 97: /* condition: ID comOp value  */
#line 669 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));

			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$ = ( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name = NULL;
			// $$->left_attr.attribute_name= $1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 0;
			// $$->right_attr.relation_name = NULL;
			// $$->right_attr.attribute_name = NULL;
			// $$->right_value = *$3;

		}
#line 2133 "yacc_sql.tab.c"
    break;

  case 98: /* condition: value comOp value  */
#line 690 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 2];
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$ = ( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 0;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=NULL;
			// $$->left_value = *$1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 0;
			// $$->right_attr.relation_name = NULL;
			// $$->right_attr.attribute_name = NULL;
			// $$->right_value = *$3;

		}
#line 2157 "yacc_sql.tab.c"
    break;

  case 99: /* condition: ID comOp ID  */
#line 710 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, NULL, (yyvsp[-2].string));
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=$1;
			// $$->comp = CONTEXT->comp;
			// $$->right_is_attr = 1;
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=$3;

		}
#line 2181 "yacc_sql.tab.c"
    break;

  case 100: /* condition: value comOp ID  */
#line 730 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];
			RelAttr right_attr;
			relation_attr_init(&right_attr, NULL, (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 0;
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=NULL;
			// $$->left_value = *$1;
			// $$->comp=CONTEXT->comp;
			
			// $$->right_is_attr = 1;
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=$3;
		
		}
#line 2207 "yacc_sql.tab.c"
    break;

  case 101: /* condition: ID DOT ID comOp value  */
#line 752 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-4].string), (yyvsp[-2].string));
			Value *right_value = &CONTEXT->values[CONTEXT->value_length - 1];

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 0, NULL, right_value);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;

			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;
			// $$->left_attr.relation_name=$1;
			// $$->left_attr.attribute_name=$3;
			// $$->comp=CONTEXT->comp;
			// $$->right_is_attr = 0;   //属性值
			// $$->right_attr.relation_name=NULL;
			// $$->right_attr.attribute_name=NULL;
			// $$->right_value =*$5;			
							
    }
#line 2232 "yacc_sql.tab.c"
    break;

  case 102: /* condition: value comOp ID DOT ID  */
#line 773 "yacc_sql.y"
                {
			Value *left_value = &CONTEXT->values[CONTEXT->value_length - 1];

			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 0, NULL, left_value, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 0;//属性值
			// $$->left_attr.relation_name=NULL;
			// $$->left_attr.attribute_name=NULL;
			// $$->left_value = *$1;
			// $$->comp =CONTEXT->comp;
			// $$->right_is_attr = 1;//属性
			// $$->right_attr.relation_name = $3;
			// $$->right_attr.attribute_name = $5;
									
    }
#line 2257 "yacc_sql.tab.c"
    break;

  case 103: /* condition: ID DOT ID comOp ID DOT ID  */
#line 794 "yacc_sql.y"
                {
			RelAttr left_attr;
			relation_attr_init(&left_attr, (yyvsp[-6].string), (yyvsp[-4].string));
			RelAttr right_attr;
			relation_attr_init(&right_attr, (yyvsp[-2].string), (yyvsp[0].string));

			Condition condition;
			condition_init(&condition, CONTEXT->comp, 1, &left_attr, NULL, 1, &right_attr, NULL);
			CONTEXT->conditions[CONTEXT->condition_length++] = condition;
			// $$=( Condition *)malloc(sizeof( Condition));
			// $$->left_is_attr = 1;		//属性
			// $$->left_attr.relation_name=$1;
			// $$->left_attr.attribute_name=$3;
			// $$->comp =CONTEXT->comp;
			// $$->right_is_attr = 1;		//属性
			// $$->right_attr.relation_name=$5;
			// $$->right_attr.attribute_name=$7;
    }
#line 2280 "yacc_sql.tab.c"
    break;

  case 104: /* comOp: EQ  */
#line 815 "yacc_sql.y"
             { CONTEXT->comp = EQUAL_TO; }
#line 2286 "yacc_sql.tab.c"
    break;

  case 105: /* comOp: LT  */
#line 816 "yacc_sql.y"
         { CONTEXT->comp = LESS_THAN; }
#line 2292 "yacc_sql.tab.c"
    break;

  case 106: /* comOp: GT  */
#line 817 "yacc_sql.y"
         { CONTEXT->comp = GREAT_THAN; }
#line 2298 "yacc_sql.tab.c"
    break;

  case 107: /* comOp: LE  */
#line 818 "yacc_sql.y"
         { CONTEXT->comp = LESS_EQUAL; }
#line 2304 "yacc_sql.tab.c"
    break;

  case 108: /* comOp: GE  */
#line 819 "yacc_sql.y"
         { CONTEXT->comp = GREAT_EQUAL; }
#line 2310 "yacc_sql.tab.c"
    break;

  case 109: /* comOp: NE  */
#line 820 "yacc_sql.y"
         { CONTEXT->comp = NOT_EQUAL; }
#line 2316 "yacc_sql.tab.c"
    break;

  case 110: /* load_data: LOAD DATA INFILE SSS INTO TABLE ID SEMICOLON  */
#line 825 "yacc_sql.y"
                {
		  CONTEXT->ssql->flag = SCF_LOAD_DATA;
			load_data_init(&CONTEXT->ssql->sstr.load_data, (yyvsp[-1].string), (yyvsp[-4].string));
		}
#line 2325 "yacc_sql.tab.c"
    break;


#line 2329 "yacc_sql.tab.c"

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

#line 830 "yacc_sql.y"

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
