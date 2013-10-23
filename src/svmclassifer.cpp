

#include "svmclassifier.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>






namespace cvml{


#define DEBUG_TRACK 1

#define Malloc(type,n) (type *)malloc((n)*sizeof(type))


void exit_input_error(int line_num)
{
	fprintf(stderr,"Wrong input format at line %d\n", line_num);
	exit(1);
}


static char* readline(FILE *input, char*&line, int& max_line_len)
{
	int len;
	
	if(fgets(line,max_line_len,input) == NULL)
		return NULL;

	while(strrchr(line,'\n') == NULL)
	{
		max_line_len *= 2;
		line = (char *) realloc(line,max_line_len);
		len = (int) strlen(line);
		if(fgets(line+len,max_line_len-len,input) == NULL)
			break;
	}
	return line;
}







SVMClassifier::SVMClassifier()
{
	m_param =0;
	m_model =0;
	m_prob =0;
	m_x_space =0;


	return ;
}

SVMClassifier::~SVMClassifier()
{

	free_problem();

	if(m_model)
	{

		svm_free_and_destroy_model(&m_model);
		m_model =0;

	}


	if(m_param)
	{
		svm_destroy_param(m_param);
		m_param=0;
	}

	return ;

}





int  SVMClassifier::
free_problem()
{

	printf("11\n");
	if(m_prob){
		if (m_prob->y)  
		{	
			free(m_prob->y);
			m_prob->y =0;
		}


		printf("22\n");

		if (m_prob->x)
		{
			free(m_prob->x);
			m_prob->x =0;
		}

		m_prob =0;
	}

	printf("33\n");

	if(m_x_space)
	{
		free(m_x_space);
		m_x_space =0;
	}
	return 1;
}











void SVMClassifier::
set_parm(svm_parameter* param,double C, double gamma)
{
	if(param==0) param= new svm_parameter;


	// default values
	param->svm_type = C_SVC;
	param->kernel_type = RBF;
	param->degree = 3;
	param->gamma =gamma;	// 1/num_features
	param->coef0 = 0;
	param->nu = 0.5;
	param->cache_size = 100;
	param->C = C;
	param->eps = 1e-3;
	param->p = 0.1;
	param->shrinking = 1;
	param->probability = 0;
	param->nr_weight = 0;
	param->weight_label = NULL;
	param->weight = NULL;
	//cross_validation = 0;
}




void SVMClassifier::
read_problem(const char *filename,  ProblemType*& prob, NodeType *&x_space,  svm_parameter*& param)

{
	if(prob==0){
		//free_problem(prob);
		prob = new svm_problem;
	}

	if(x_space){
		
		free(x_space);
		x_space =0;
	}
	//////////////////////////////////////

	int elements, max_index,max_line_len, inst_max_index, i, j;

	FILE *fp = fopen(filename,"r");
	
	char *endptr;
	char *idx, *val, *label;
	char *line = NULL;

	if(fp == NULL)
	{
		fprintf(stderr,"can't open input file %s\n",filename);
		//TRACK(DEBUG_TRACK, "can't open input file %s\n",filename);
		exit(1);
	}

	prob->l = 0;
	elements = 0;

	max_line_len = 1024;
	line = Malloc(char,max_line_len);
	while(readline(fp,line,max_line_len)!=NULL)
	{
		char *p = strtok(line," \t"); // label

		// features
		while(1)
		{
			p = strtok(NULL," \t");
			if(p == NULL || *p == '\n') // check '\n' as ' ' may be after the last feature
				break;
			++elements;
		}
		++elements;
		++ (prob->l);
	}
	rewind(fp);


	prob->y = Malloc(double,prob->l);
	prob->x = Malloc(struct svm_node *,prob->l);
	
	x_space = Malloc(struct svm_node,elements);

	max_index = 0;
	j=0;
	for(i=0;i<prob->l;i++)
	{
		inst_max_index = -1; // strtol gives 0 if wrong format, and precomputed kernel has <index> start from 0
		readline(fp,line,max_line_len);
		prob->x[i] = &x_space[j];
		label = strtok(line," \t\n");
		if(label == NULL) // empty line
			exit_input_error(i+1);

		prob->y[i] = strtod(label,&endptr);
		if(endptr == label || *endptr != '\0')
			exit_input_error(i+1);

		while(1)
		{
			idx = strtok(NULL,":");
			val = strtok(NULL," \t");

			if(val == NULL)
				break;

			errno = 0;
			x_space[j].index = (int) strtol(idx,&endptr,10);
			if(endptr == idx || errno != 0 || *endptr != '\0' || x_space[j].index <= inst_max_index)
				exit_input_error(i+1);
			else
				inst_max_index = x_space[j].index;

			errno = 0;
			x_space[j].value = strtod(val,&endptr);
			if(endptr == val || errno != 0 || (*endptr != '\0' && !isspace(*endptr)))
				exit_input_error(i+1);

			++j;
		}

		if(inst_max_index > max_index)
			max_index = inst_max_index;
		x_space[j++].index = -1;
	}

	if(param->gamma == 0 && max_index > 0)
		param->gamma = 1.0/max_index;

	if(param->kernel_type == PRECOMPUTED)
		for(i=0;i<prob->l;i++)
		{
			if (prob->x[i][0].index != 0)
			{
				fprintf(stderr,"Wrong input format: first column must be 0:sample_serial_number\n");
				exit(1);
			}
			if ((int)prob->x[i][0].value <= 0 || (int)prob->x[i][0].value > max_index)
			{
				fprintf(stderr,"Wrong input format: sample_serial_number out of range\n");
				exit(1);
			}
		}

	fclose(fp);

	free(line);

}










double SVMClassifier::
do_cross_validation(ProblemType* prob, svm_parameter* param, int nr_fold)
{
	int i;
	int total_correct = 0;
	double total_error = 0;
	double sumv = 0, sumy = 0, sumvv = 0, sumyy = 0, sumvy = 0;
	double *target = Malloc(double,prob->l);

	double accuracy =0;
	double errors =0;

	svm_cross_validation(prob,param,nr_fold,target);
	if(param->svm_type == EPSILON_SVR ||
	   param->svm_type == NU_SVR)
	{
		for(i=0;i<prob->l;i++)
		{
			double y = prob->y[i];
			double v = target[i];
			total_error += (v-y)*(v-y);
			sumv += v;
			sumy += y;
			sumvv += v*v;
			sumyy += y*y;
			sumvy += v*y;
		}

		errors = total_error/prob->l;
		accuracy = 1- errors;
		printf("Cross Validation Mean squared error = %g\n",total_error/prob->l);
		printf("Cross Validation Squared correlation coefficient = %g\n",
			((prob->l*sumvy-sumv*sumy)*(prob->l*sumvy-sumv*sumy))/
			((prob->l*sumvv-sumv*sumv)*(prob->l*sumyy-sumy*sumy))
			);
	}
	else
	{
		for(i=0;i<prob->l;i++)
			if(target[i] == prob->y[i])
				++total_correct;

		accuracy = 1.0*total_correct/prob->l;
		printf("Cross Validation Accuracy = %f, %f %f\n",100.0*total_correct/prob->l, param->C, param->gamma);
	}
	free(target);


	return accuracy;
}













double SVMClassifier::
get_best_param(ProblemType* prob, svm_parameter* param, int nr_fold, int lowerC, int upperC, int lowergamma, int uppergamma)
{
	if(lowerC>upperC || lowergamma>uppergamma)
	{
		printf("error in get_best_param  ___ for __  lowerC>upperC || lowergamma>uppergamma ");
		return -1;
	}

	printf("test %d %d %d %d\n", lowerC,upperC, lowergamma, uppergamma);

	double C , gamma;
	double bestC , bestGamma;
	double best_accuracy = 0;

	//for(int c = -5; c<=15;c++)
	for(int c = lowerC; c<=upperC;c++)
	{
		C = pow(2.0,c);
		//for(int g = -15; g<=5;g++)
		for(int g = lowergamma; g<=uppergamma;g++)
		{
			gamma = pow(2.0,g);
			param->C = C;
			param->gamma = gamma;
			double accuracy = do_cross_validation(prob, param,  nr_fold);
			if(accuracy>best_accuracy)
			{
				best_accuracy = accuracy;
				bestC = C;
				bestGamma = gamma;
			}
		}
	}


	param->C = bestC;
	param->gamma = bestGamma;
	return best_accuracy;

}








int SVMClassifier::
svm_do_train(ProblemType* prob, svm_parameter* param, svm_model *&model, int cross_validation)
{
	const char* error_msg = svm_check_parameter(prob,param);
	
	if(error_msg)
	{
		printf("ERROR: %s\n",error_msg);
		exit(1);
	}



	if(cross_validation)
	{
		int nr_fold =5;
		double accuracy = get_best_param( prob,  param, nr_fold);
		printf("CV accuracy is %f\n", accuracy );
		printf("CV C and gamma is %f %f\n", param->C,param->gamma );
	}
		
	model = svm_train(prob,param);


	//if(svm_save_model(model_file_name,model))
	//{
	//	fprintf(stderr, "can't save model to file %s\n", model_file_name);
	//	exit(1);
	//}



	return 1;
}





int SVMClassifier::
initization(const char* prob_file_name)
{


try{
	free_problem();

	if(m_param)
	{
		svm_destroy_param(m_param);
		m_param =0;
	}
    
	m_prob =0;
	m_x_space =0;
	
	m_param=new svm_parameter;
	set_parm(m_param);

	read_problem(prob_file_name,  m_prob, m_x_space,  m_param);



	if(m_model)
	{
		svm_free_and_destroy_model(&m_model);
		m_model=0;
	}


	return 1;
}
catch(...)
{
	printf("error in svm classifer initization\n");
	return 0;
}

}

int
SVMClassifier::
train(const char* prob_file_name, const char* saved_model_file_name)
{

	initization(prob_file_name);
	
	int cross_validation =1;

	svm_do_train(m_prob, m_param, m_model,  cross_validation);

	printf("model out %f\n",(*m_model).param.C);


	saveModel(saved_model_file_name);

	printf("finished svm_do_train\n");
	

	return 1;

}


std::vector<std::vector<float>>
SVMClassifier::
predict(const char* prob_file_name, const char* saved_model_file_name)
{
	
	free_problem();

	if(m_param)
	{
		svm_destroy_param(m_param);
		m_param =0;
	}
    
	m_prob =0;
	m_x_space =0;
	
	m_param=new svm_parameter;
	set_parm(m_param);

	read_problem(prob_file_name,  m_prob, m_x_space,  m_param);


	loadModel( saved_model_file_name);

	
	std::vector<std::vector<float>> vecResult;
	vecResult.resize(m_prob->l);

	for(int i=0;i<m_prob->l;i++)
	{
		double v = svm_predict(m_model, m_prob->x[i]);
		//if( fabs(v-m_prob->y[i])<0.1 )
		//{
		//	accuracy = accuracy+1;
		//}
		float vGround = static_cast<float>(m_prob->y[i]);
		
		float vPredict= static_cast<float>(v);

		std::vector<float> vec;
		vec.push_back(vGround);
		vec.push_back(vPredict);
		vecResult.push_back(vec);
	}

	return vecResult;

}




int
SVMClassifier::
saveModel(const char* saved_model_file_name)
{
	if(svm_save_model(saved_model_file_name,m_model))
	{
		fprintf(stderr, "can't save model to file %s\n", saved_model_file_name);
		return 0;
	}

	return 1;
}




int
SVMClassifier::
loadModel(const char* model_file_name)
{

	if(m_model)
	{
		svm_free_and_destroy_model(&m_model);
		m_model=0;
	}

	m_model = svm_load_model(model_file_name);

	return 1;

}






} //end of namespace cvml


