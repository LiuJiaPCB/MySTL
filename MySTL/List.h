#ifndef _LIST_H_
#define _LIST_H_
#include"Iterator.h"
#include"Allocator.h"
#include"Alloc.h"
#include"Construct.h"
#include"Type_traits.h"
#include"Uninitialized.h"

namespace MySTL
{
	struct slist_node_base
	{
		slist_node_base * next;
	}
	
	template<class T>
	struct slist_node:public slist_node_base
	{
		T data;
	};
	
	//在某一节点后插入一个新节点 
	inline slist_node_base* slist_make_link(slist_node_base *pre_node,slist_node_base *new_node)
	{
		new_node->next = prev_node->next;
		prev_node->next = new_node;
		return new_node;
	}
	//单项链表的长度 
	inline size_t slist_size(slist_node_base *node)
	{
		size_t result = 0;
		for(;node!=NULL;node=node->next)
		{
			++result;
		}
		return result;
	} 
	//单向链表的迭代器的基本结构
	class slist_iterator_base
	{
      public:
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef forward_iterator_tag iterator_category;
		
		slist_node_base *node;
		
		slist_iterator_base(slist_node_base *x):node(x)
		{
		}
		
		void incr()
		{
			node = node->next;
		}
		
		bool operator==(const slist_iterstor_base& x)const
		{
			return node == x.node;
		}
		bool operator!=(const slist_iterator_base &x)const
		{
			return node != x.node;
		}
		
	};
	 
	template<class T,class Ref,class Ptr>
	struct slist_iterator:public slist_iterator_base
	{
		typedef slist_iterator<T,&T,*T>    iterator;
		typedef slist_iterator<T,Ref,Ptr>  self; 
		typedef T      value_type;
		typedef Ptr    pointer;
		typedef Ref    reference;
		typedef slist_node<T> list_node;
		
		//迭代器构造函数
		slist_iterator():slist_iterator_base(0){}
		slist_iterator(list_node *x):slist_iterator_base(x){}
		slist_iterator(const iterator &x):slist_iterator_base(x.node){}
		
		reference operator*()const
		{
			return ((list_node*)node)->data;
		} 
		
		pointer operator->() const 
		{
			return &(operator*());
		}
		
		self& operator++()
		{
			incr();
			return *this;
		}
		
		self operator++(int)
		{
			self tmp = *this;
			incr();
			return tmp;
		}		
		 
	};
	
	template< class T,class Alloc = allocator<T> >
	class List
	{
		public:
			typedef T                 value_type;
			typedef value_type*       pointer;
			typedef const value_type* const_pointer;
			typedef value_type&       reference;
			typedef const value_type& const_reference;
			typedef size_t            size_type;
			typedef ptrdiff_t         difference_type;
			
			typedef slist_iterator<T,T&,T*> iterator;
			typedef slist_iterator<T,const T&,const T*> const_iterator;	
			
		private:
		    list_node_base  head; //链表头部
		    
		    typedef slist_node<T> list_node;
		    typedef slist_node_base list_node_base;
		    typedef slist_iterator_base iterator_base;
		    typedef allocator<list_node> list_node_allocator;
		    
		    static list_node *create_node(const value_type &x)
		    {
		    	list_node *node = list_node_allocator::allocate();
		    	construct(&node->data,x);
		    	node->next = 0;
		    	return node;
		    }
		    
			
	    public:
		    slist(){head.next = 0;}
			~slist(){clear();}	
		public:
			iterator begin()
			{
				return iterator((list_node*)head.next);
			}
			iterator end()
			{
				return iterator(0);
			}
			size_type size()const
			{
				return slist_size(head.next); 
			}
			bool empty()const
			{
				return head.next == 0;
			}
			
			void swap(slist& L)
			{
				list_node_base *tmp = head.next;
				head.next = L.head.next;
				L.head.next = tmp;
			}
		public:
			reference front()
			{
				return ((list_node*)head.next)->data;
			}
			
			void push_front(const value_type & x)
			{
				slist_make_link(&head,create_node(x));
			} 
			
			void pop_front()
			{
				list_node* node = (list_node*)head.next;
				head.next = node->next;
				destroy_node(node); 
			}
	};
	
}

#endif
