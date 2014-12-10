#ifndef _VECTOR_H_
#define _VECTOR_H_
//#include"Iterator.h"
#include"Allocator.h"
#include"Alloc.h"
#include"Construct.h"
#include"Type_traits.h"
#include"Uninitialized.h"

namespace MySTL
{
	template< class T,class Alloc = allocator<T> >
	class Vector
	{		
		public:
			typedef   T                   value_type;
			typedef   value_type*         pointer;
			typedef   value_type*         iterator;
			typedef   const value_type*   const_iterator;
			typedef   value_type&         reference;
			typedef   const T&            const_reference; 
			typedef   size_t              size_type;
			typedef   ptrdiff_t           difference_type;
			
		protected:
		    typedef  Alloc   data_allocator;
			iterator start;
			iterator finish;
			iterator end_of_storage; 
			
		public:
			/*构造，析构，拷贝构造函数*/ 
			Vector():start(0),finish(0),end_of_storage(0){}  //构造函数
			explicit Vector(const size_type n);
			Vector(const size_type n,const value_type& value);
			Vector(iterator first,iterator last);
			Vector(const Vector& v);
            Vector& operator=(const Vector& v);
			~Vector()
			{
				destroyAndDeallocateAll();  //member function
			}			
			/*迭代器函数*/
			iterator begin(){return (start);}
			const_iterator begin() const{return (start);}
			const_iterator cbegin() const{return (start);}
			iterator end() {return (end);}
			const_iterator end()const {return (end);}
			const_iterator cend()const {return (end);}
			/*比较*/ 
			bool operator ==(const Vector& v);
			bool operator !=(const Vector& v);
			
			
			/*容量函数*/
			difference_type size() const{return finish - start;}
			difference_type capacity() const{return end_of_storage-start;}
			bool empty()const {return start == finish;}
			void resize(size_type n,value_type val = value_type());
			void reserve(size_type n);
			void shrink_to_fit()
			{
				data_allocator::deallocate(finish,end_of_storage);
				end_of_storage = finish;
			} 
			/*访问函数*/
			value_type & operator[](const difference_type i){return  *(begin()+i);} 
			value_type& front(){return *(begin());}
			value_type& back(){return *(end()-1);}
			pointer data() {return start;}
			
			/*修改容器的函数*/ 
			void clear()
			{
				data_allocator::desroy(start,finish);
				finish = start;
			}
			void swap(Vector &v)
			{
				if(this != &v)
				{
					swap(start,v.start);
					swap(finish,v.finish);
					swap(end_of_storage,v.end_of_storage); 
				}
			}
			void push_back(const value_type& value);
			void pop_back()
			{
				--finish;
				data_allocator::destroy(finish);
			}
			iterator instert(iterator position,const value_type& val);
			void insert(iterator position,const size_type &n,const value_type &val);
			void insert(iterator position,iterator first,iterator last);
			iterator erase(iterator position);
			iterator erase(iterator first,iterator last);
		private:
			void destroyAndDeallocateAll()
			{
				if(capacity()!=0)
				{
					data_allocator::destroy(start,finish);
					data_allocator::deallocate(start,capacity()); 
				}
			}
			
			void allocateAndFillN(const size_type n,const value_type &value)
			{
				start = data_allocator::allocate(n);
				__uninitialized_fill_n(start,n,value);
				finish = end_of_storage = start+n;
			}
			void allocateAndCopy(iterator first,iterator last)
			{
				start = data_allocator::allocate(last-first);
				finish = MySTL::uninitialized_copy(first,last,start);
				end_of_storage = finish; 
			}
			void vector_aux(iterator first,iterator last)
			{
				allocateAndCopy(first,last);
			}
			void vector_aux(iterator n,const value_type& value)
			{
				allocateAndFillN(n,value);
			}
			template<class Integer>
			void insert_aux(Integer n,const value_type &value,_true_type)
			{
				allocateAndFillN(n,value);
			}
			void insert_aux(iterator position,iterator first,iterator last,_false_type);
			template<class Integer>
			void insert_aux(iterator position,Integer n,const value_type &value,_true_type);
			void reallocateAndCopy(iterator position,iterator first,iterator last);
			void reallocateAndFillN(iterator position,const size_type &n,const value_type &val);
			size_type getNewCapacity(size_type n)const
			{
				size_type oldCapacity = end_of_storage-start;
				size_type res = oldCapacity>n?oldCapacity:n;
				size_type newCapacity = (oldCapacity!=0?(oldCapacity+res):n);
				return newCapacity;
			}

   };/*end of class Vector*
			/*构造，析构，复制函数*/
			template<class T,class Alloc>
		    Vector<T,Alloc>::Vector(const size_type n)
			{
                allocateAndFillN(n,value_type());				
			} 
			
			template<class T,class Alloc>
			Vector<T,Alloc>::Vector(const size_type n,const value_type &value)
			{
				allocateAndFillN(n,value);
			}
			
			template<class T,class Alloc>
			Vector<T,Alloc>::Vector(iterator first,iterator last)
			{
				vector_aux(first,last); 
			}
			
			template<class T,class Alloc>
			Vector<T,Alloc>::Vector(const Vector &v)
			{
				allocateAndCopy(v.start,v.finish);
			}
			
			/*容量相关*/
			template<class T,class Alloc>
			void Vector<T,Alloc>::resize(size_type n,value_type val)
			{
				if(n<size())
				{
					data_allocator::destroy(start+n,finish);
					finish = start + n;
				}
				else if(n>size()&&n<=capacity())
				{
					size_type lengthOfInsert = n-size();
					finish = __uninitialized_fill_n(finish,lengthOfInsert,val);
				}
				else if(n>capacity())
				{
					size_type lengthOfInsert = n-size();
					T *newStart = data_allocator::allocate(getNewCapacity(lengthOfInsert));
					T *newFinish = MySTL::uninitialized_copy(begin(),end(),newStart);
					newFinish = __uninitialized_fill_n(newFinish,lengthOfInsert,val);
					
					destroyAndDeallocateAll();
					start = newStart;
					finish = newFinish;
					end_of_storage = start + n;
				}
			} 
			
			template<class T,class Alloc>
			void Vector<T,Alloc>::reserve(size_type n)
			{
				if(n<=capacity())
				return;
				T *newStart = data_allocator::allocate(n);
				T *newFinish = MySTL::uninitialized_copy(begin(),end(),newStart);
				destroyAndDeallocateAll();
				
				start = newStart;
				finish = newFinish;
				end_of_storage = start + n;
			}
			/*修改容器相关函数*/
			template<class T,class Alloc>
			typename Vector<T,Alloc>::iterator Vector<T,Alloc>::erase(iterator position)
			{
				return erase(position,position+1);
			} 
			
			
			template<class T,class Alloc>
			typename Vector<T,Alloc>::iterator Vector<T,Alloc>::erase(iterator first,iterator last)
			{
				difference_type lenOfTail = end()-last;
				difference_type lenOfRemoved = last-first;
				finish = finish - lenOfRemoved;
				for(;lenOfTail!=0;--lenOfTail)
				{
					*(first++) = *(last++); 
				}
				return (first);
			} 
			template<class T,class Alloc>
			void Vector<T,Alloc>::reallocateAndCopy(iterator position,iterator first,iterator last)
			{
				difference_type newCapacity = getNewCapacity(last-first);
				T *newStart = data_allocator::allocate(newCapacity); 
				T *newEndOfStorage = newStart + newCapacity;
				T *newFinish = MySTL::uninitialized_copy(begin(),position,newStart);
				newFinish = MySTL::uninitialized_copy(first,last,newFinish);
				newFinish = MySTL::uninitialized_copy(position,end(),newFinish);
				
				destroyAndDeallocateAll();
				start = newStart;
				finish = newFinish;
				end_of_storage = newEndOfStorage; 
			}
			template<class T,class Alloc>
			void Vector<T,Alloc>::reallocateAndFillN(iterator position,const size_type &n,const value_type &val)
			{
				difference_type newCapacity = getNewCapacity(n);
				T *newStart = data_allocator::allocate(newCapacity); 
				T *newEndOfStorage = newStart + newCapacity;
				T *newFinish = MySTL::uninitialized_copy(begin(),position,newStart);
				newFinish = __uninitialized_fill_n(newFinish,n,val);
				newFinish = MySTL::uninitialized_copy(position,end(),newFinish);
				
				destroyAndDeallocateAll();
				start = newStart;
				finish = newFinish;
				end_of_storage = newEndOfStorage; 
			}
			
			template<class T,class Alloc>
			void Vector<T,Alloc>::insert_aux(iterator position,iterator first,iterator last,_false_type)
			{
				difference_type locationLeft = end_of_storage - finish;
				difference_type locationNeed = last - first;
				
				if(locationLeft >= locationNeed)
				{
					iterator tempPtr = end()-1;
					for(;tempPtr - position >= 0;--tempPtr)
					{
						*(tempPtr+locationNeed) = *tempPtr;
					}
					MySTL::uninitialized_copy(first,last,position);
					finish = finish + locationNeed;
				}
				else
				{
					reallocateAndCopy(position,first,last);
				}
			}
			template<class T,class Alloc>
            template<class Integer>
			void Vector<T,Alloc>::insert_aux(iterator position,Integer n,const value_type &value,_true_type)
			{
				difference_type locationLeft = end_of_storage - finish;
				difference_type locationNeed = n;
				
				if(locationLeft >= locationNeed)
				{
					iterator tempPtr = end()-1;
					for(;tempPtr - position >= 0;--tempPtr)
					{
						*(tempPtr+locationNeed) = *tempPtr;
					}
					uninitialized_fill_n(position,n,value);
					finish = finish + locationNeed;
				}
				else
				{
					reallocateAndFillN(position,n,value);
				}
			}	
			
			template<class T,class Alloc>
			bool Vector<T,Alloc>::operator ==(const Vector& v)
			{
				if(size()!=v.size())
				{
					return false;
				}
				else
				{
					iterator ptr1 = start;
					iterator ptr2 = v.start;
					for(;ptr1!=finish&&ptr2!=v.finish;++ptr1,++ptr2)
					{
						if(*ptr1!=*ptr2)
						{
							return false;
						}
					}
					return true;
				}
			}
			
			template<class T,class Alloc>
			bool Vector<T,Alloc>::operator !=(const Vector& v)
			{
				return !(*this==v);
			}

}
           
#endif
