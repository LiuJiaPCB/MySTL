#ifndef _VECTOR_H_
#define _VECTOR_H_

namespace MySTL
{
	template<class T,class Alloc = allocator<T> >
	class Vector
	{
		public:
			typedef   T              value_type;
			typedef   value_type*   pointer;
			typedef   value_type*   iterator;
			typedef   value_type&   reference;
			typedef   size_t         size_type;
			typedef   ptrdiff_t      difference_type;
			
		protected:
		    typedef  Alloc    data_allocator;
			iterator start;
			iterator finish;
			iterator end_of_storage; 
	};
}

#endif
