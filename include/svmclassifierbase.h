
#ifndef __CVML__BASE__CLASSIFIER__H
#define __CVML__BASE__CLASSIFIER__H


#include <vector>
#include "libsvm/svm.h"



namespace cvml{

 class SVMClassifierBase 
 {
  public:

	SVMClassifierBase(){return;};
	virtual int initization(const char* prob_file_name) =0;
	virtual int train(const char* prob_file_name,const char* saved_model_file_name)  = 0;  	
	virtual std::vector<std::vector<float>> predict(const char* prob_file_name,const char* saved_model_file_name)  = 0;

	virtual int saveModel(const char* saved_model_file_name)=0;
	virtual int loadModel(const char* model_file_name)=0;

	virtual int getNumOfSamples()=0;
	virtual ~SVMClassifierBase(){return;};


	// protected:

 };

 }// end of namespace

#endif 