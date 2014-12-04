#ifndef _CONSTRUCT_H
#define _CONSTRUCT_H

#include<new>
#include"Type_traits.h"

namespace MySTL
{
/*construct*/	
	template<class T1,class T2>
	inline void construct(T1 *p,const T2& value)
	{
		new(p) T1(value);
	}
/*destory*/
	template <class T>
	inline void destory(T *pointer)
	{
		pointer->~T();
	} 

	template<class ForwardIterator>
	inline void __destory_aux(ForwardIterator first,ForwardIterator last,_false_type)
	{
		for(;first<last;++first)
		{
			destory(&*first);
		}
	}
	
	template<class ForwardIterator>
	inline void __destory_aux(ForwardIterator first,ForwardIterator last,_true_type)
	{
	}
	
	template<class ForwardIterator,class T>
	inline void __destory(ForwardIterator first,ForwardIterator last,T*)
	{
		typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor;
		__destroy_aux(first,last,trivial_destructor());
	}
	
	template<class ForwardIterator>
	inline void destory(ForwardIterator first,ForwardIterator last)
	{
		__destroy(first,last,value_type(first));
	}	
}

#endif	
	
