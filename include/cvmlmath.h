/*=========================================================================

  Program:   Computer Vision and Machine Learning Library
  Module:    $RCSfile: cvmlmath.h $
  Language:  C++
  Date:      $Date: 2013-10-22 00:00:10 $
  Version:   $Revision: 1.00 $

  Copyright (c) Dongfeng Han. All rights reserved.

  If you want to use this code, please request from  Dongfeng Han  a copy

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notices for more information.

  Usage:
  define some commonly used math functions
  NOTE: the current version only support window platform and we tested the code in win7 with VS 2008
=========================================================================*/


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