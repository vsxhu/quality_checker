#ifndef __PARSE_H__
#define __PARSE_H__

#include <stdio.h>

#include "parse_types.h"
 
/* macros */

#define catVariable (category)(catAlpha | catDigit | catUnderscore)
#define catKeyword  (catAlpha | catDigit | catMinus)

#define parseLexThisVariable(p,s) parseLexThisCategory(p,catVariable,s)
#define parseLexVariable(p,s)     parseLexCategory(p,catVariable,s)

#define parseLexThisKeyword(p,s)  parseLexThisCategory(p,(category)catKeyword,s)
#define parseLexKeyword(p,s)      parseLexCategory(p,catKeyword,s)

#define parseLexThisOper(p,s)     parseLexThisCategory(p,catOper,s)
#define parseLexOper(p,s)         parseLexCategory(p,catOper,s)

#define assign(ass)               (ass,1)
#define done                      (1)
#define fail                      (0)




#ifdef __GNUC__

#define many(p)                   ({while((p));1;})

#define maybe(p)                  (p,1)

#endif

/* types */

typedef enum
{
  catAlpha      = 1,
  catDigit      = 2,
  catOper       = 4,
  catBracket    = 8,
  catQuote      = 16,
  
  catMinus      = 32,
  catUnderscore = 64
} category;

typedef
  category
  *judgement;

struct _parse;

typedef
  mybool
  (parseGetCharFunction)( struct _parse *, char * );

typedef
  mybool
  (parseUnGetCharFunction)( struct _parse *, char );

typedef
  void
  (parseDisposeFunction)( struct _parse * );

typedef struct _parse
{
  char      *buf;
  int       bufptr;
  judgement judge;
  int       line;

  /* for FILE implementations */
  FILE      *file;

  /* for string implementations */
  char      *contents;
  int       ptr;
  
  parseGetCharFunction   *getChar;
  parseUnGetCharFunction *unGetChar;
  parseDisposeFunction   *dispose;
} parse;

/* functions */

parse *parseCreateFile( FILE *file );
parse *parseCreateFileName( char *name );
parse *parseCreateString( char *s );
void parseDispose( parse *p );

void parseSetJudgement( parse *p, char c, category );
void parseAddJudgement( parse *p, char c, category );
category parseGetJudgement( parse *p, char c );

mybool parseError( parse *p, char *s );
mybool parseEnd( parse *p );

mybool parseAChar( parse *p, char *c );
mybool parseAnyChar( parse *p );
mybool parseThisChar( parse *p, char c );
mybool parseUnParseChar( parse *p, char c );

mybool parseThisString( parse *p, char *s );
mybool parseUnParseString( parse *p, char *s );

mybool parseDigit( parse *p, char *c );
mybool parseNumber( parse *p, int *n );
mybool parseSignedNumber( parse *p, int *n );

mybool parseCharCategory( parse *p, category cat, char *c );

mybool parseSpaceAndComments( parse *p );

mybool parseLexThisCategory( parse *p, category cat, char *s );
mybool parseLexCategory( parse *p, category cat, char *s );
mybool parseLexNumber( parse *p, int *n );
mybool parseLexSignedNumber( parse *p, int *n );
mybool parseLexThisString( parse *p, char *s );
mybool parseLexThisChar( parse *p, char c );
mybool parseLexEnd( parse *p );

mybool parseLexIfKeyword( parse *p, char *s, mybool *b );
mybool parseLexIfNumber( parse *p, int *n );
mybool parseLexIfSignedNumber( parse *p, int *n );
mybool parseLexKeywordNumber( parse *p, char *s, int *n );
mybool parseLexKeywordSignedNumber( parse *p, char *s, int *n );

mybool parseQuoted( parse *p, char *s );
mybool parseSingleQuoted( parse *p, char *s );
mybool parseLexQuoted( parse *p, char *s );

#endif /* PARSE_H */
