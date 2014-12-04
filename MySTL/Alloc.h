#ifndef _ALLOC_H_
#define _ALLOC_H_

#include<cstdlib>

namespace MySTL
{
	class alloc
	{
		private:
			enum {__ALIGN = 8};
			enum {__MAX_BYTES = 128}; 
			enum {__NFREELISTS = __MAX_BYTES/__ALIGN};
		private:
			union obj{
				union obj *free_list_link;
				char client_data[1]; 
			};
		private:
			static size_t ROUND_UP(size_t bytes)
			{
				return (((bytes)+__ALIGN-1)&~(__ALIGN-1));
			}
		private:
		    static obj *free_list[alloc::__NFREELISTS];
		    static size_t FREELIST_INDEX(size_t bytes)
		    {
		    	return (((bytes)+__ALIGN-1)/__ALIGN-1);
		    }
		
		static void *refill(size_t n);
		static char *chunk_alloc(size_t size,int &nobjs);
		
		static char *start_free;
		static char *end_free;
		static size_t  heap_size;
		
		public:
			 static void *allocate(size_t n)
			 {
			 	if(n>__MAX_BYTES)
			 	{
			 		return malloc(n);
			 	}
			 	obj *volatile *my_free_list;
				my_free_list = free_list + FREELIST_INDEX(n);
				obj *result = *my_free_list;
				if(result == 0)
				{
					void *r = refill(ROUND_UP(n));
					return r;
				} 
				*my_free_list = result->free_list_link;
				return(result);
			 };
			 
			 static void deallocate(void *p,size_t n)
			 {
			 	obj *q = (obj*)p;
			 	obj *volatile *my_free_list;
			 	
			 	if(n>__MAX_BYTES)
			 	{
			 		free(p);
			 	}
			 	
			 	my_free_list = free_list + FREELIST_INDEX(n);
				q->free_list_link = *my_free_list;
				*my_free_list = q; 
			 }
			 
			 static void *reallocate(void *p,size_t old_sz,size_t new_sz)
			 {
			 	deallocate(p,old_sz);
			 	p = allocate(new_sz);
			 	return p;
			 }
	};
	
	char *alloc::start_free = 0;
	char *alloc::end_free = 0;
	size_t alloc::heap_size = 0;
	alloc::obj *alloc::free_list[alloc::__NFREELISTS]={
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	};
	
	void *alloc::refill(size_t n)
	{
		int nobjs = 20;
		char *chunk = chunk_alloc(n,nobjs);
		obj *volatile *my_free_list;
		obj *current_obj,*next_obj;
		int i; 
		
		if(1 == nobjs)
		{
			return(chunk);
		}
		
		my_free_list = free_list + FREELIST_INDEX(n);
		
		obj *result = (obj*)chunk;
		*my_free_list = next_obj = (obj*)(chunk+n);
		for(i=1;;i++)
		{
			current_obj = next_obj;
			next_obj = (obj*)((char *)next_obj + n);
			if(nobjs == 1+i)
			{
				current_obj ->free_list_link = 0;
				break;
			} 
			else
			{
				current_obj->free_list_link = next_obj;
			}
		} 
		
		return (result);
	}
	
	char *alloc::chunk_alloc(size_t size,int &nobjs)
	{
		char *result;
		size_t total_bytes = size*nobjs;
		size_t bytes_left = end_free - start_free;
	    
	    if(bytes_left >= total_bytes)
	    {
	    	result = start_free;
	    	start_free = start_free + total_bytes;
	    	return (result);
	    }
	    else if(bytes_left>=size)
	    {
	    	nobjs = bytes_left/size;
	    	total_bytes = size*nobjs;
	    	result = start_free;
	    	start_free = start_free + total_bytes;
			return result; 
	    }
	    else
	    {
	    	
	    	if(bytes_left > 0)
	    	{
	    		obj **my_free_list=free_list + FREELIST_INDEX(bytes_left);
				((obj*)start_free)->free_list_link = *my_free_list;
				*my_free_list = (obj*)start_free;  
	    	}
	    }
	    size_t bytes_to_get = 2*total_bytes + ROUND_UP(heap_size>>4);
	    start_free = (char*)malloc(bytes_to_get);
	    /**/
	    heap_size = heap_size + bytes_to_get;
	    end_free = start_free + bytes_to_get;
	    return chunk_alloc(size,nobjs);
	
	}
}

#endif
