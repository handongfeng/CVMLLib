/*************************************************************************



CVML lib def files



*************************************************************************/


#ifndef __CVML__DEF__H
#define __CVML__DEF__H



#ifdef CVML_EXPORTS
#define CVML_API __declspec(dllexport)
#else
#define CVML_API __declspec(dllimport)
#endif



#include "limits.h"
#include "float.h"

#define EPS_DBL DBL_EPSILON
#define EPS_FLT FLT_EPSILON

#define MAX_INT INT_MAX
#define MIN_INT INT_MIN

#define MAX_DBL DBL_MAX
#define MIN_DBL DBL_MIN






#endif