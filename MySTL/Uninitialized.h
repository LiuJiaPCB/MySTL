#include<Type_traits.h>
#include<Algorithm>
#include<Iterator.h>

/*********uninitialized_copy()**********/
/*__true_type*/
template<class InputIterator,class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first,InputInterator last,
                                                ForwardIterator result,__true_type)
{
	return copy(first,last,result);
}
/*__false_type*/
template<class InputIterator,class ForwardIterator>
inline ForwardIterator __uninitialized_copy_aux(InputIterator first,InputInterator last,
                                                ForwardIterator result,__false_type)
{
	ForwardIterator cur = result;
	for(;first!=last;++first,++cur)
	{
		construct(&*cur,*first);
	}
	return cur;
}

template<class InputIterator,class ForwardIterator,class T>
inline ForwardIterator __uninitialized_copy(InputIterator first,InputInterator last,
                                            ForwardIterator result)
{
	typedef typename __type_traits<T>::is_POD_type  is_POD;
	return __uninitialized_copy_aux(first,last,result,is_POD());
}

template<class InputIterator,class ForwardIterator>
inline ForwardIterator uninitialized_copy(InputIterator first,InputInterator last,
                                          ForwardIterator result)
{
	return __uninitialized_copy(first,last,result,value_type(result));
}
