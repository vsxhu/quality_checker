/* error.c */

#include "parse_types.h"
#include "parse_print.h"
#include "parse_error.h"

/* global variables */

print *errorGlobalPrint = NULL;

/* functions */

void errorInitPrint( void )
{
  if ( !errorGlobalPrint )
    errorGlobalPrint = printCreateFile( stderr );
}

void errorSetPrint( print *p, mybool dispose )
{
  if ( dispose && errorGlobalPrint )
    printDispose( errorGlobalPrint );
  errorGlobalPrint = p;
}

print *errorGetPrint( void )
{
  return errorGlobalPrint;
}

/* end error.c */
