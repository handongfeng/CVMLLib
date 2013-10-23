/*=========================================================================

  Program:   Computer Vision and Machine Learning Library
  Module:    $RCSfile: cvmldef.h $
  Language:  C++
  Date:      $Date: 2013-10-22 00:00:10 $
  Version:   $Revision: 1.00 $

  Copyright (c) Dongfeng Han. All rights reserved.


  If you want to use this code, please request from  Dongfeng Han  copy

  This software is distributed WITHOUT ANY WARRANTY; without even 
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
  PURPOSE.  See the above copyright notices for more information.






=========================================================================*/


#ifndef __CVML__DEF__H
#define __CVML__DEF__H




///////// define the dllexport ///////////////
#ifdef CVML_EXPORTS
#define CVML_API __declspec(dllexport)
#else
#define CVML_API __declspec(dllimport)
#endif



/////////////////////////////////////////////////
////////////////////////////////////////////////


#include "limits.h"
#include "float.h"



//////////////// CVML Version Info///////////////

#define CVML_MAJOR_VERSION    1
#define CVML_MINOR_VERSION    0
#define CVML_SUBMINOR_VERSION 0
#define CVMLAUX_STR_EXP(__A)  #__A
#define CVMLAUX_STR(__A)      CVMLAUX_STR_EXP(__A)
#define CVML_VERSION          CVMLAUX_STR(CVML_MAJOR_VERSION) "." CVMLAUX_STR(CVML_MINOR_VERSION) "." CVMLAUX_STR(CVML_SUBMINOR_VERSION)

////////////////////////////////////////////////
////////////////////////////////////////////////





#define EPS_DBL DBL_EPSILON
#define EPS_FLT FLT_EPSILON

#define MAX_INT INT_MAX
#define MIN_INT INT_MIN

#define MAX_DBL DBL_MAX
#define MIN_DBL DBL_MIN






#endif