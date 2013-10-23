


#include "cvmllib.h"

using namespace cvml;

//void TRACK2(char * fmt,...)
//{
//	va_list args;
//	va_start(args,fmt);
//	FILE * fid=fopen("Track2.txt","a");
//	vfprintf(fid,fmt,args);
//	fclose(fid);
//}
//
//#define _TRACK_DEBUG
//
//#ifdef _TRACK_DEBUG
//void TRACK2(char *fmt,...);
//#else
//#define TRACK2 //
//#endif




int svmTest(const char* train_file_name, const char* saved_model_file_name)
{

	cvml::SVMClassifier * pClassifer = new cvml::SVMClassifier;
	
	cvml::ClassTest * p= new cvml::ClassTest;
	//cvml::SVMClassifierBase * pClassifer = new  cvml::SVMClassifier;
	//pClassifer->train(train_file_name, saved_model_file_name );
	
	if(pClassifer) delete pClassifer;

	//double  a = cvml::EPS;
	//printf("a is %f\n",a);
	if(p) delete p;
	return 1;
}



int main(int argc, char **argv)
{
	int dim=3;
	int k=100;
	int numofSamples=40;
	const float * _samples=0;
	float* center =0;

	cvml::doKMeans( dim,  k,  numofSamples,   _samples,  center);

	svmTest(0,0);

	return 1;
}