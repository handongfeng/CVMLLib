


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <math.h>
#include "svmhelper.h"


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




int free_problem(svm_problem* prob, svm_node *x_space)
{

	/*
	 if prob is not 0, just delete it
	*/


	if (prob->y)  
	{	
		//delete [] (prob->y);
		free(prob->y);
		prob->y =0;
	}

	if (prob->x)
	{
		/*for(int i =0;i<prob->l;i++)
		{
			delete [] (prob->x[i]);
			prob->x[i] = 0;
		}
		delete [] (prob->x);*/

		free(prob->x);
		prob->x =0;
	}

	free(x_space);
	x_space =0;
	return 1;
}




void set_parm(svm_parameter* param,double C, double gamma)
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




void read_problem(const char *filename,  svm_problem*& prob, svm_node *&x_space,  svm_parameter*& param)

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
		TRACK(DEBUG_TRACK, "can't open input file %s\n",filename);
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











double do_cross_validation(svm_problem* prob, svm_parameter* param, int nr_fold)
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




double get_best_param(svm_problem* prob, svm_parameter* param, int nr_fold, int lowerC, int upperC, int lowergamma, int uppergamma)
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





int svm_do_train(svm_problem* prob, svm_parameter* param, svm_model *& model, int cross_validation)
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



int do_train(const char *filename,const char *saved_model_filename)
{

	svm_node *x_space =0;
	svm_problem* prob=0;
	svm_parameter* param=new svm_parameter;

	set_parm(param);
	printf("C and gamma are %f %f\n",param->C, param->gamma);
	read_problem(filename,prob,x_space,param);
	
	int cross_validation =1;
	svm_model *model =0;
	svm_do_train(prob, param, model,  cross_validation);

	printf("model out %f\n",(*model).param.C);

	printf("finished svm_do_train\n");
	if(svm_save_model(saved_model_filename,model))
	{
		fprintf(stderr, "can't save model to file %s\n", saved_model_filename);
	}


	svm_free_and_destroy_model(&model);

	svm_destroy_param(param);

	free_problem( prob,x_space);

	return 1;
}








double do_predict(svm_problem* prob,  svm_model *model, int idx)
{
	double label = svm_predict(model, prob->x[idx]);

	return label;
}







int test_do_predict(const char * prob_filename, const char *model_filename,double& accuracy)
{
	svm_node *x_space =0;
	svm_problem* prob=0;
	svm_parameter* param=new svm_parameter;
	set_parm(param);
	read_problem(prob_filename,prob,x_space,param);

	accuracy =0;
	svm_model *model = svm_load_model(model_filename);

	for(int i=0;i<prob->l;i++)
	{
		double v = svm_predict(model, prob->x[i]);
		if( fabs(v-prob->y[i])<0.1 )
		{
			accuracy = accuracy+1;
		}
	}

	accuracy =  accuracy/prob->l;

	printf("predict accuracy is %f\n",accuracy); 

	svm_free_and_destroy_model(&model);
	svm_destroy_param(param);
	free_problem( prob,x_space);
	return 1;
}