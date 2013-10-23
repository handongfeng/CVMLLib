
#ifndef __CVML__SVM__HELPER__H
#define __CVML__SVM__HELPER__H


#include "svm.h"


//int scaleData(const char* original_file_name, 
//			  const char* scaled_file_name,
//			  double lower,
//			  double upper,)

void set_parm(svm_parameter* param,double C=1, double gamma=0);
void read_problem(const char *filename,  svm_problem*& prob,  svm_node *&x_space, svm_parameter*& param);
double do_cross_validation(svm_problem* prob, svm_parameter* param, int nr_fold);
//double get_best_param(svm_problem* prob, svm_parameter* param, int nr_fold);
double get_best_param(svm_problem* prob, svm_parameter* param, int nr_fold, int lowerC=-4, int upperC=4, int lowergamma=-8, int uppergamma=8);
int svm_do_train(svm_problem* prob, svm_parameter* param, svm_model *&model, int cross_validation=0);
int do_train(const char *filename,const char *saved_model_filename);

//predict part
double do_predict(svm_problem* prob,  svm_model *model, int idx);
int test_do_predict(const char * prob_filename, const char *model_filename,double& accuracy);


#endif