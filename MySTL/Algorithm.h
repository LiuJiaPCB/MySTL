#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

namespace MySTL
{

template<class OutputIterator, class Size, class T>
	OutputIterator fill_n(OutputIterator first, Size n, const T& value)
	{
		for (; n > 0; --n, ++first)
			*first = value;
		return first;
	}
	
template <class InputIterator,class T>
T accumulate(InputIterator first,InputIterator last,T init)
{
	for(;first!=last;++first)
	{
		init=init + (*first);  //累加到初值init身上 
	}
	return init;
} 

template <class InputIterator,class T,class BinaryOperation>
T accumulate(InputIterator first,InputIterator last,T init,BinaryOperation binary_op)
{
	for(;first!=last;++first)
	{
		init=binary_op(init,*first);
	}
	return init;
}

template<class InputIterator,class OutputIterator,class T,class BinaryOperation>
OutputIterator _adjacent_difference(InputIterator first,InputIterator last,OutputIterator result,T*,
                                   BinaryOperation binary_op)
{
	T value = *first;
	if(++first != last)
	{
		T tmp = *first;
		*++result = binary_op(tmp,value);
		value = tmp;
		
	}				
	return ++result;	
}
template <class InputIterator,class OutputIterator,class BinaryOperation>
OutputIterator adjacent_differance(InputIterator first,InputIterator last,OutputIterator result,
                                   BinaryOperation binary_op)
{
	if(first == last)
	{
		return result;
	}
	*result = *first;
    return 	_adjacent_difference(first,last,result,value_type(first),
                                 binary_op);			   	
} 

template<class InputIterator1,class InputIterator2,class T>
T inner_product(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,T init)
{
	for(;first!=last;++first,++first2)
	{
		init = init + (*first1 * *first2);
	}
	return init;
}

template<class InputIterator1,class InputIterator2,class T,
         class BinaryOperation1,class BinaryOperation2>
T inner_product(InputIterator1 first1,InputIterator1 last1,InputIterator2 first2,T init,
                BinaryOperation1 binary_op1,BinaryOperation2 binary_op2)
{
	for(;first!=last;++first,++first2)
	{
		init = binary_op1(init,binary_op2(*first1,*first2));
	}
	return init;
}

	
}
#endif
