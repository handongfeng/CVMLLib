#ifndef __CVML__KMEANS__H
#define __CVML__KMEANS__H


#include <vector>
#include "cvmldef.h"


namespace cvml{

// kmeans interface

/*
dim feature dimension
k : kmeans number
numof Sample: nuumber of data point
_samples: data points. size is numofSamples*dim
center: retured center size is k x dim
*/
int CVML_API doKMeans(int dim, int k, int numofSamples, const float * _samples, float*& center);
int CVML_API doKMeans(int dim, int k, int numofSamples, const double * _samples, double*& center);
int CVML_API doKMeans(int dim, int k, int numofSamples, std::vector<std::vector<float>>&  _samples,std::vector<std::vector<float>> & center);
int CVML_API doKMeans(int dim, int k, int numofSamples, std::vector<std::vector<double>>& _samples,std::vector<std::vector<double>> & center);

/*
dim feature dimension
k : kmeans number

code_book:  size is cood_book_size x dim
data : size is nb x dim
signature: returned bag-of-feature feature. size is  cood_book_size
*/
//int CVML_API bag_of_word(int dim, int cood_book_size, const float* code_book, int nb, float * data, float*& signature);
/////////////////////////////////////////////////////////////////////////////////////////





} // end of namespace
#endif




