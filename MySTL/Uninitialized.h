#ifndef _UNINITIALIZED_H_
#define _UNINITIALIZED_H_

#include"Type_traits.h"
#include"Construct.h"
#include"Algorithm.h"
#include<Algorithm>
#include"Iterator.h"

namespace MySTL{
	
/*********uninitialized_copy()**********/
/*__true_type*/
template<class InputIterator,class ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last,
		                                ForwardIterator result, _true_type)
{
	return std::copy(first,last,result);
}
/*__false_type*/
template<class InputIterator,class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first,InputIterator last,
                                                ForwardIterator result,_false_type)
{
	ForwardIterator cur = result;
	for(;first!=last;++first,++cur)
	{
		construct(&*cur,*first);
	}
	return cur;
}

template<class InputIterator,class ForwardIterator,class T>
inline ForwardIterator __uninitialized_copy(InputIterator first,InputIterator last,
                                            ForwardIterator result,T*)
{
	typedef  typename _type_traits<T>::is_POD_type  is_POD;
	return __uninitialized_copy_aux(first,last,result,is_POD());
}

template<class InputIterator,class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first,InputIterator last,
                                          ForwardIterator result)
{
	return __uninitialized_copy(first,last,result,value_type(result));
}

/*********uninitialized_fill()**********/
/*__true_type*/
template<class ForwardIterator,class T>
inline void __uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,
                                                const T& x,_true_type)
{
	return fill(first,last,x);
}
/*__false_type*/
template<class ForwardIterator,class T>
inline void __uninitialized_fill_aux(ForwardIterator first,ForwardIterator last,
                                                const T& x,_false_type)
{
	ForwardIterator cur = first;
	for(;cur!=last;++cur)
	{
		construct(&*cur,x);
	}
}

template<class ForwardIterator,class T,class T1>
inline void __uninitialized_fill(ForwardIterator first,ForwardIterator last,
                                            const T& x,T1*)
{
	typedef  typename _type_traits<T1>::is_POD_type  is_POD;
	return __uninitialized_fill_aux(first,last,x,is_POD());
}

template<class ForwardIterator,class T>
inline void uninitialized_fill(ForwardIterator first,ForwardIterator last,
                               const T& x)
{
	__uninitialized_fill(first,last,x,value_type(first));
}

/*********uninitialized_fill_n()**********/
/*__true_type*/
template<class ForwardIterator,class Size,class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,Size n,
                                                const T& x,_true_type)
{
	return fill_n(first,n,x);
}
/*__false_type*/
template<class ForwardIterator,class Size,class T>
inline ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first,Size n,
                                                const T& x,_false_type)
{
	ForwardIterator cur = first;
	for(;n>0;--n,++cur)
	{
		construct(&*cur,x);
	}
	return cur; 
}

template<class ForwardIterator,class Size,class T,class T1>
inline ForwardIterator __uninitialized_fill_n(ForwardIterator first,Size n,
                                              const T& x,T1*)
{
	typedef  typename _type_traits<T1>::is_POD_type  is_POD;
	return __uninitialized_fill_n_aux(first,n,x,is_POD());
}

template<class ForwardIterator,class Size,class T>
inline ForwardIterator uninitialized_fill_n(ForwardIterator first,Size n,
                                              const T& x)
{
	return __uninitialized_fill_n(first,n,x,value_type(first));
}

}
#endif
