#ifndef _DLIST_H_
#define _DLIST_H_
#include"Iterator.h"
#include"Allocator.h"
#include"Alloc.h"
#include"Construct.h"
#include"Type_traits.h"
#include"Uninitialized.h"

namespace MySTL
{
	template <class T>
	struct __list_node
	{
		__list_node<T> *prev;
		__list_node<T> *next;
		T data;
	};
	/******************begin of iterator******************/ 
	template<class T,class Ref,class Ptr>
	struct __list_iterator
	{
		typedef __list_iterator<T,Ref,Ptr> iterator;
		typedef __list_iterator<T,Ref,Ptr> self;
		typedef __list_node<T>* link_type;
		
		typedef bidirectional_iterator_tag iterator_category;
		typedef T value_type;
		typedef Ptr pointer;
		typedef Ref reference;		
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		
		link_type node;
		
		__list_iterator(link_type x):node(x){}
		__list_iterator(){} 
		__list_iterator(const iterator&x):node(x.node){}
		
		bool operator==(const self& x)const
		{
			return node == x.node;
		}
		bool operator!=(const self& x)const
		{
			return node != x.node;
		}
		
		reference operator*() const
		{
			return (*node).data;
		}
		
		self& operator++()
		{
			node = (link_type)((*node).next);
			return *this;
		}
		
		self operator++(int)
		{
			self tmp = *this;
			++*this;
			return tmp;
		}
		
		self& operator--()
		{
			node = (link_type)((*node).prev);
			return *this;
		}
		
		self operator--(int)
		{
			self tmp = *this;
			--*this;
			return tmp;
		}	
		
	};
	
	/******************end of iterator******************/
	
	
	template<class T,class Alloc = allocator<T> >
	class Dlist
	{
	public:
		    typedef T                 value_type;
			typedef value_type*       pointer;
			typedef const value_type* const_pointer;
			typedef value_type&       reference;
			typedef const value_type& const_reference;
			typedef size_t            size_type;
			typedef ptrdiff_t         difference_type;
	protected:
			typedef __list_node<T> list_node;
			typedef allocator<list_node> list_node_allocator;
	public:
			typedef list_node* link_type;
			typedef __list_iterator<T,T&,T*> iterator;
	protected:
			link_type node;
	protected:
			link_type get_node()
			{
				return list_node_allocator::allocate();
			}
			void put_node(link_type p)
			{
				list_node_allocator::deallocate(p);
			}
			
			link_type create_node(const T&x)
			{
				link_type p = get_node();
				construct(&p->data,x);
				return p;
			}
			
			void destory_node(link_type p)
			{
				destroy(&p->data);
				put_node(p);
			}
			
			void empty_initialize()
			{
				node = get_node();
				node->next = node;
				node->prev = node;
			}
	public:
			Dlist(){empty_initialize();}
			iterator begin(){return (link_type)((*node).next);}	
		    iterator end(){return node;}
			bool     empty() const{return node->next == node;}
			size_type size() const
			{
				size_type result = 0;
				link_type tmp = node;
				while(tmp->next!=node)
				{
					result++;
					tmp = tmp->next;
				}
				return result;
			}
		
			reference front(){return *begin();}
			reference back(){return *(--end());}
		
			iterator insert(iterator position,const T&x)
			{
				link_type tmp = create_node(x);
				tmp->next = position.node;
				tmp->prev = position.node->prev;
				(link_type(position.node->prev))->next = tmp;
				position.node->prev = tmp;
				return tmp; 
			}
		
			void push_front(const T&x){insert(begin(),x);}
			void push_back(const T&x){insert(end(),x);}
			iterator erase(iterator position)
			{
				link_type next_node = link_type(position.node->next);
				link_type prev_node = link_type(position.node->prev);
				prev_node->next = next_node;
				next_node->prev = prev_node;
				destroy_node(position.node);
				return iterator(next_node);
			}
		
			void clear()
			{
				link_type cur = (link_type)node->next;
				while(cur!=node)
				{
					link_type tmp = cur;
					cur = (link_type)cur->next;
					destroy_node(tmp); 
				}
				node->next = node;
				node->prev = node; 
			} 
			void remove(const T& value)
			{
				iterator first = begin();
				iterator last = end();
				while(first!=last)
				{
					iterator next = first;
					++next;
					if(*first == value)
					{
						erase(first);
					}
					first = next;
				}
			}
	
			void unique()
			{
				iterator first = begin();
				iterator last = end();
				if(first == last)
				{
					return ;
				}
				iterator next = first;
				while(++next!=last)
				{
					if(*first == *next)
					{
						erase(next);
					}
					else
					{
						first = next;
					}
					next = first;
				}	
			} 
		
			void reverse();
		
	protected:
		void transfer(iterator position,iterator first,iterator last)
		{
			if(position != last)
			{
				(*(link_type((*last.node).prev))).next = position.node;
				(*(link_type((*first.node).prev))).next = last.node;
				(*(link_type((*position.node).prev))).next = first.node;
				link_type tmp = link_type((*position.node).prev);
				(*position.node).prev = (*last.node).prev;	
				(*last.node).prev = (*first.node).prev;	
				(*first.node).prev = tmp;
			}
		}
	public:
		void splice(iterator position,Dlist&x)
		{
			if(!x.empty())
			{
				transfer(position,x.begin(),x.end());
			}
		}
	    
		void splice(iterator position,Dlist&,iterator i)
		{
			iterator j = i;
			++j;
			if(position == i||position == j){return;}
			transfer(position,i,j);
		} 
		void splice(iterator position,Dlist&,iterator first,iterator last)
		{
			if(first!=last)
			{
				transfer(position,first,last);
			}
		}	
		
		void merge(Dlist<T,Alloc>&x);	
	};
	
	template<class T,class Alloc>
	void Dlist<T,Alloc>::merge(Dlist<T,Alloc> &x)
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();
		
		while(first1!=last1&&first2!=last2)
		{
			if(*first2<*first1)
			{
				iterator next = first2;
				tranfer(first1,first2,++next);
				first2 = next;
			}
			else
			{
				++first1;
			} 
		}
		if(first2!=last2)
		{
			transfer(last1,first2,last2);
		}
	}
	
	template<class T,class Alloc>
	void Dlist<T,Alloc>::reverse()
	{
		if(empty()||link_type(node->next)->next == node){return;}
		iterator first = begin();
		++first;
		while(first!=end())
		{
			iterator old = first;
			first++;
			transfer(begin(),old,first);
	    }
	}
/*	
	template<class T,class Alloc>
	void Dlist<T,Alloc>::sort()
	{
		if(empty()==TRUE||link_type(node->next)->next == node){return;}
		
	}
*/	
}

#endif
