#ifndef __CVML__MATH__H
#define __CVML__MATH__H



#include "stdarg.h"
#include "stdio.h"
#include "math.h"
#include "cvmldef.h"


namespace cvml{


  void TRACK(char *fmt,...)
  {
	va_list args;
	va_start(args,fmt);
	FILE * fid=fopen("Track.txt","a");
	vfprintf(fid,fmt,args);
	fclose(fid);
   }


  template<class T> 
  T max(const T& a, const T& b)
  {
	  T c = (a>=b) ? a : b;
	  return c;
  }

  template<class T> 
  T min(const T& a, const T& b)
  {
	  T c = (a<=b) ? a : b;
	  return c;
  }




} //end of namespace cvml{



#endif