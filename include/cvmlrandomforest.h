#ifndef __CVML__RANDOM__FOREST__H
#define __CVML__RANDOM__FOREST__H


#include <vector>
#include "cvmldef.h"
#include "librf/Sherwood.h"

namespace cvml{



	template <class T>
	class A
	{
	public:
		A<T>();
		int m;
	};

	template<class T>
	A<T>::A()
	{
		printf("A is create\n");
	}

//struct CVML_API CRandomForest: public MicrosoftResearch::Cambridge::Sherwood::Node<int,int>

typedef A<int> INTA;

class CVML_API CRandomForest:std::vector<int>

{
public:
	CRandomForest(int ii);
	int i;
	//A<int> a;
};


CRandomForest::CRandomForest(int ii)
{
	printf("CRandomForest is create\n");
	return ;
}





} // end of namespace cvml

#endif




