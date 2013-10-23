
#include "cvmlkmeans.h"
#include "KMlocal.h"
#include "cvmlmath.h"
//#include <ANN/ANN.h>




namespace cvml{

int 
CVML_API 
doKMeans(int dim, int k, int numofSamples, const float * _samples, float*& center)
{

	if(_samples==0)
	{
		printf("_samples ==0 and return\n");
		return 0;
	}
	typedef float T;

	if(center)
	{
		delete [] center;
		center=0;
	}
	center = new T[k*dim];
	memset( center,0,sizeof(T)*k*dim );


	try{
		// execution parameters (see KMterm.h and KMlocal.h)
		KMterm  term(100, 0, 0, 0,              // run for 100 stages
					 0.10, 0.10, 3,             // other typical parameter values 
				     0.50, 10, 0.95);

		//int		dim	= 1;			             // dimension
		int		nPts	= numofSamples;			// number of data points
		KMdata dataPts(dim, nPts);			// allocate data storage
		//KMdata *dataPts = new KMdata(dim, nPts);

		for ( unsigned int i = 0 ; i < numofSamples ; i++ ){
			for(int j=0;i<dim;j++){
				int idx = i * dim + j;
				dataPts[i][j] = _samples[idx];
			}
		}

		dataPts.buildKcTree();			// build filtering structure
		KMfilterCenters ctrs(k, dataPts);		// allocate centers

		// run the algorithm
		//KMlocalLloyds       kmAlg(ctrs, term);	// repeated Lloyd's
		// KMlocalSwap      kmAlg(ctrs, term);	// Swap heuristic
		// KMlocalEZ_Hybrid kmAlg(ctrs, term);	// EZ-Hybrid heuristic
		
		KMlocalHybrid    kmAlg(ctrs, term);	// Hybrid heuristic
		ctrs = kmAlg.execute();			// execute


		for ( unsigned int i = 0 ; i < k ; i++ ){
			for(int j=0;i<dim;j++){
				int idx = i * dim + j;
				center[idx] = static_cast<T>(ctrs[i][j]);
			}
		}

		return 1;
	}
	catch(...)
	{
		cvml::TRACK("doKMeans error \n");
		return 0;
	}

	return 1;
}






int 
CVML_API 
doKMeans(int dim, int k, int numofSamples, const double * _samples, double*& center)
{
	typedef double T;

	if(center)
	{
		delete [] center;
		center=0;
	}
	center = new T[k*dim];
	memset( center,0,sizeof(T)*k*dim );


	try{
		// execution parameters (see KMterm.h and KMlocal.h)
		KMterm  term(100, 0, 0, 0,              // run for 100 stages
			0.10, 0.10, 3,             // other typical parameter values 
			0.50, 10, 0.95);

		//int		dim	= 1;			             // dimension
		int		nPts	= numofSamples;			// number of data points
		KMdata dataPts(dim, nPts);			// allocate data storage
		//KMdata *dataPts = new KMdata(dim, nPts);

		for ( unsigned int i = 0 ; i < numofSamples ; i++ ){
			for(int j=0;i<dim;j++){
				int idx = i * dim + j;
				dataPts[i][j] = _samples[idx];
			}
		}

		dataPts.buildKcTree();			// build filtering structure
		KMfilterCenters ctrs(k, dataPts);		// allocate centers

		// run the algorithm
		//KMlocalLloyds       kmAlg(ctrs, term);	// repeated Lloyd's
		// KMlocalSwap      kmAlg(ctrs, term);	// Swap heuristic
		// KMlocalEZ_Hybrid kmAlg(ctrs, term);	// EZ-Hybrid heuristic
		
		KMlocalHybrid    kmAlg(ctrs, term);	// Hybrid heuristic
		ctrs = kmAlg.execute();			// execute


		for ( unsigned int i = 0 ; i < k ; i++ ){
			for(int j=0;i<dim;j++){
				int idx = i * dim + j;
				center[idx] = static_cast<T>(ctrs[i][j]);
			}
		}

		return 1;
	}
	catch(...)
	{
		//cvml::TRACK("doKMeans error \n");
		return 0;
	}

	return 1;
}







int 
CVML_API 
doKMeans(int dim, int k, int numofSamples, std::vector<std::vector<float>>& _samples,std::vector<std::vector<float>> & center)
{
	typedef float T;
	
	center.clear();
	center.resize(k);

	try{
		// execution parameters (see KMterm.h and KMlocal.h)
		KMterm  term(100, 0, 0, 0,              // run for 100 stages
			0.10, 0.10, 3,             // other typical parameter values 
			0.50, 10, 0.95);

		//int		dim	= 1;			             // dimension
		int		nPts	= numofSamples;			// number of data points
		KMdata dataPts(dim, nPts);			// allocate data storage
		//KMdata *dataPts = new KMdata(dim, nPts);

		for ( unsigned int i = 0 ; i < numofSamples ; i++ ){
			for(int j=0;i<dim;j++){
				dataPts[i][j] = _samples[i][j];
			}
		}

		dataPts.buildKcTree();			// build filtering structure
		KMfilterCenters ctrs(k, dataPts);		// allocate centers

		// run the algorithm
		//KMlocalLloyds       kmAlg(ctrs, term);	// repeated Lloyd's
		// KMlocalSwap      kmAlg(ctrs, term);	// Swap heuristic
		// KMlocalEZ_Hybrid kmAlg(ctrs, term);	// EZ-Hybrid heuristic
		
		KMlocalHybrid    kmAlg(ctrs, term);	// Hybrid heuristic
		ctrs = kmAlg.execute();			// execute


		for ( unsigned int i = 0 ; i < k ; i++ ){
			for(int j=0;i<dim;j++){
				T v =  static_cast<T>(ctrs[i][j]);
				center[i].push_back(v);
			}
		}

		return 1;
	}
	catch(...)
	{
		//cvml::TRACK("doKMeans error \n");
		return 0;
	}

	return 1;
}




int 
CVML_API 
doKMeans(int dim, int k, int numofSamples, std::vector<std::vector<double>>& _samples,std::vector<std::vector<double>> & center)
{
	typedef double T;
	
	center.clear();
	center.resize(k);

	try{
		// execution parameters (see KMterm.h and KMlocal.h)
		KMterm  term(100, 0, 0, 0,              // run for 100 stages
			0.10, 0.10, 3,             // other typical parameter values 
			0.50, 10, 0.95);

		//int		dim	= 1;			             // dimension
		int		nPts	= numofSamples;			// number of data points
		KMdata dataPts(dim, nPts);			// allocate data storage
		//KMdata *dataPts = new KMdata(dim, nPts);

		for ( unsigned int i = 0 ; i < numofSamples ; i++ ){
			for(int j=0;i<dim;j++){
				dataPts[i][j] = _samples[i][j];
			}
		}

		dataPts.buildKcTree();			// build filtering structure
		KMfilterCenters ctrs(k, dataPts);		// allocate centers

		// run the algorithm
		//KMlocalLloyds       kmAlg(ctrs, term);	// repeated Lloyd's
		// KMlocalSwap      kmAlg(ctrs, term);	// Swap heuristic
		// KMlocalEZ_Hybrid kmAlg(ctrs, term);	// EZ-Hybrid heuristic
		
		KMlocalHybrid    kmAlg(ctrs, term);	// Hybrid heuristic
		ctrs = kmAlg.execute();			// execute


		for ( unsigned int i = 0 ; i < k ; i++ ){
			for(int j=0;i<dim;j++){
				T v =  static_cast<T>(ctrs[i][j]);
				center[i].push_back(v);
			}
		}

		return 1;
	}
	catch(...)
	{
		//cvml::TRACK("doKMeans error \n");
		return 0;
	}

	return 1;
}





//
//
//int CVML_API
//bag_of_word(int dim, int cood_book_size, const float* code_book, int nb, float * data, float*& signature)
//{
//
//
//	typedef float T;
//	if (signature)
//	{
//		delete [] signature;
//		signature =0;
//	}
//	signature = new T[cood_book_size];
//	memset(signature,0,sizeof(T)*cood_book_size);
//
//
//	if(cood_book_size<=0 || dim<=0 || nb<=0)
//	{
//		return 0;
//	}
//
//	try
//	{
//		
//		int nPts; // actual number of data points
//		ANNpointArray dataPts; // data points
//		ANNpoint queryPt; // query point
//		ANNidxArray nnIdx; // near neighbor indices
//		ANNdistArray dists; // near neighbor distances
//		ANNkd_tree* kdTree; // search structure
//
//		nPts = cood_book_size;
//		int nearest_number =1;
//		double eps = 0; // error bound
//
//		queryPt = annAllocPt(dim); // allocate query point
//		dataPts = annAllocPts(nPts, dim); // allocate data points
//		nnIdx = new ANNidx[nearest_number]; // allocate near neigh indices
//		dists = new ANNdist[nearest_number];
//
//		for(int i=0;i<cood_book_size;i++)
//		{
//			for(int j=0;j<dim;j++)
//			{
//				int idx = i*dim+j;
//				dataPts[i][j] =  code_book[idx];
//			}
//			
//		}
//
//		kdTree = new ANNkd_tree(dataPts, nPts,dim);
//		
//		for(int i=0;i<nb;i++)
//		{
//			for(int j=0;j<dim;j++)
//			{
//				int idx = i*dim+j;
//				queryPt[j] = data[idx];
//			}
//
//			for(int ii=0;ii<nearest_number;ii++)
//			{
//				nnIdx[ii] =0;
//				dists[ii] =0;
//			}
//
//			kdTree->annkSearch( queryPt, nearest_number, nnIdx, dists, eps);
//
//			int bin = nnIdx[0];
//			if(bin<0) bin =0;
//			if(bin>cood_book_size-1) bin=cood_book_size-1;
//
//			signature[bin]++;
//			
//		}// i
//
//
//		delete [] nnIdx; // clean things up
//		delete [] dists;
//		annDeallocPt(queryPt);
//		annDeallocPts(dataPts);
//		delete kdTree;
//
//
//		int sum =0;
//		for(int i=0;i<cood_book_size;i++)
//		{
//			sum += signature[i] ;
//		}
//		if(sum>0)
//		{
//		for(int i=0;i<cood_book_size;i++)
//		{
//			signature[i] /= sum ;
//		}
//
//		}
//
//
//	}
//	catch(...)
//	{
//		return 0;
//	}
//	return 1;
//}
//
//
//





}// end of namespace cvml