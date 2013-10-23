

#ifndef __CVML__SVM__CLASSIFIER__H
#define __CVML__SVM__CLASSIFIER__H


#include "cvmldef.h"
#include "svmclassifierbase.h"
#include "libsvm/svm.h"

#include <vector>

namespace cvml{


class CVML_API SVMClassifier: public SVMClassifierBase 
//class CVML_API SVMClassifier
{

public:



	typedef svm_problem ProblemType;
	typedef svm_node NodeType;


	SVMClassifier();
	~SVMClassifier();

	
	void set_parm(svm_parameter* param,double C=1, double gamma=0);
	void read_problem(const char *filename,  ProblemType*& prob, NodeType *&x_space,  svm_parameter*& param);
	double do_cross_validation(ProblemType* prob, svm_parameter* param, int nr_fold);
	double get_best_param(ProblemType* prob, svm_parameter* param, int nr_fold, int lowerC=-4, int upperC=4, int lowergamma=-8, int uppergamma=8);
	int svm_do_train(ProblemType* prob, svm_parameter* param, svm_model *&model, int cross_validation=0);
	int do_train(const char *filename,const char *saved_model_filename);

	int saveModel(const char* saved_model_file_name);
	int loadModel(const char* model_file_name);

	int getNumOfSamples(){return m_prob->l;}
private:
	/* create m_prob m_x_space, m_para and free model and set m_model==0*/
	int initization(const char* prob_file_name);
	int train(const char* prob_file_name,const char* saved_model_file_name);
	std::vector<std::vector<float>> predict(const char* prob_file_name,const char* saved_model_file_name) ;
	
	int free_problem();
	



private:

	ProblemType * m_prob;
	NodeType * m_x_space;

	svm_parameter* m_param;
	svm_model * m_model;

};

} // end of namespace cvml

#endif