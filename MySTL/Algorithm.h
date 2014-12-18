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
	
}
#endif
