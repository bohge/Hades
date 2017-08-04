#ifndef EVENT_IMPLECATION
#error( "Do not include this file in other files" )
#endif


/////////////////////////////////////
//			类成员函数回调		   //
/////////////////////////////////////

#define NAME(a ,b )					NAME_T( a, b ) //这里使用代理的原应是#与##在宏展开的特殊性，当宏展开后如果遇到这两个符号，如果实参是宏，这个宏也不会继续展开了
#define NAME_T( a, b )				a##b
#define IFUNCTION_CLASS				NAME( IDelegate, NAME_FIX )
#define FUNCBIND_CLASS				NAME( func_bind, NAME_FIX )
#define CONST_FUNCBIND_CLASS		NAME( const_func_bind, NAME_FIX )
#define DELEGATE_CLASS				NAME( Delegate, NAME_FIX )
#define SINGLE_DELEGATE_CLASS		NAME( Unidelegate, NAME_FIX )
#define INTERCEPT_DELEGATE_CLASS	NAME( InterceptDelegate, NAME_FIX )


template <typename R HAVE_COMMA EVENT_PARAM_TEMPLATE>
class IFUNCTION_CLASS;
template <typename R, typename T HAVE_COMMA EVENT_PARAM_TEMPLATE >
IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>*  const make_bind(T* const t, R(T::*fn)(EVENT_PARAM_TYPES));
template <typename R, typename T HAVE_COMMA EVENT_PARAM_TEMPLATE >
IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>*  const make_bind(T* const t, R(T::*fn)(EVENT_PARAM_TYPES) const);


template <typename R HAVE_COMMA EVENT_PARAM_TEMPLATE>
class IFUNCTION_CLASS 	//函数指针的接口，提供Function调用的一个入口
{
public:
	virtual ~IFUNCTION_CLASS(){}
	virtual R Invoker(EVENT_PARAM_TYPES_INPUTS) = 0;
	virtual IFUNCTION_CLASS* Duplicate() const = 0;
};

template <typename R,typename T HAVE_COMMA EVENT_PARAM_TEMPLATE >
class FUNCBIND_CLASS  : public IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES> //继承自IFunction借口，提供func调用
{
	typedef R (T::*fn)(EVENT_PARAM_TYPES);//定义一个rta的函数指针
	fn			m_func;//保存函数指针
	T* const	m_class;//保存这个类的指针
public:
	FUNCBIND_CLASS(fn f, T* const t)
		:m_func(f),
		m_class(t)
	{
	}
	FUNCBIND_CLASS(const FUNCBIND_CLASS& input)
		:m_func(input.m_func),
		m_class(input.m_class)
	{
	}
	HADES_FORCEINLINE FUNCBIND_CLASS& operator = (const FUNCBIND_CLASS& rhs)
	{
		m_func = rhs.m_func;
		m_class = rhs.m_class;
	}
	virtual R Invoker( EVENT_PARAM_TYPES_INPUTS ) //实现 IFunction函数接口，调用这个函数
	{
		return (m_class->*m_func)(EVENT_PARAM_INPUTS);
	}
	virtual IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>* Duplicate() const
	{
#ifdef _TRACK_MEMORY
		return make_bind< R, T HAVE_COMMA EVENT_PARAM_TYPES >( m_class, m_func, __FILE__, __LINE__ );
#else
		return make_bind< R, T HAVE_COMMA EVENT_PARAM_TYPES >(m_class, m_func);
#endif
	}
};

template <typename R,typename T HAVE_COMMA EVENT_PARAM_TEMPLATE >
class CONST_FUNCBIND_CLASS  : public IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES> //继承自IFunction借口，提供func调用
{
	typedef R (T::*fn)(EVENT_PARAM_TYPES) const;//定义一个rta的函数指针
	fn			m_func;//保存函数指针
	T* const	m_class;//保存这个类的指针
public:
	CONST_FUNCBIND_CLASS(fn f, T* const t)
		:m_func(f),
		m_class(t)
	{
	}
	CONST_FUNCBIND_CLASS(const CONST_FUNCBIND_CLASS& input)
		:m_func(input.m_func),
		m_class(input.m_class)
	{
	}
	HADES_FORCEINLINE CONST_FUNCBIND_CLASS& operator = (const CONST_FUNCBIND_CLASS& rhs)
	{
		m_func = rhs.m_func;
		m_class = rhs.m_class;
	}
	virtual R Invoker( EVENT_PARAM_TYPES_INPUTS ) //实现 IFunction函数接口，调用这个函数
	{
		return (m_class->*m_func)(EVENT_PARAM_INPUTS);
	}
	virtual IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>* Duplicate() const
	{
#ifdef _TRACK_MEMORY
		return make_bind< R, T HAVE_COMMA EVENT_PARAM_TYPES >( m_class, m_func, __FILE__, __LINE__ );
#else
		return make_bind< R, T HAVE_COMMA EVENT_PARAM_TYPES >(m_class, m_func);
#endif
	}
};

#ifdef _TRACK_MEMORY
template <typename R, typename T HAVE_COMMA EVENT_PARAM_TEMPLATE >
IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>*  const make_bind( T* const t, R (T::*fn)(EVENT_PARAM_TYPES), const char *file, int line )	//制造一个函数成员指针类
{
	return dynamic_cast<IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>* const>( new(file,line) FUNCBIND_CLASS<R,T HAVE_COMMA EVENT_PARAM_TYPES>(fn,t));
};

template <typename R, typename T HAVE_COMMA EVENT_PARAM_TEMPLATE >
IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>*  const make_bind( T* const t, R (T::*fn)(EVENT_PARAM_TYPES) const, const char *file, int line )	//制造一个函数成员指针类
{
	return dynamic_cast<IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>* const>( new(file,line) CONST_FUNCBIND_CLASS<R,T HAVE_COMMA EVENT_PARAM_TYPES>(fn,t));
};
#else
template <typename R, typename T HAVE_COMMA EVENT_PARAM_TEMPLATE >
IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>*  const make_bind(T* const t, R(T::*fn)(EVENT_PARAM_TYPES))	//制造一个函数成员指针类
{
	return dynamic_cast<IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>* const>( NEW FUNCBIND_CLASS<R,T HAVE_COMMA EVENT_PARAM_TYPES>(fn,t));
};

template <typename R, typename T HAVE_COMMA EVENT_PARAM_TEMPLATE >
IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>*  const make_bind(T* const t, R(T::*fn)(EVENT_PARAM_TYPES) const)	//制造一个函数成员指针类
{
	return dynamic_cast<IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>* const>( NEW CONST_FUNCBIND_CLASS<R,T HAVE_COMMA EVENT_PARAM_TYPES>(fn,t));
};
#endif

//
//template <typename R, EVENT_PARAM_TEMPLATE >
//class UNORDERED_EVENT_CLASS //无序多播事件
//{
//public:
//	typedef IFUNCTION_CLASS<R, EVENT_PARAM_TYPES> FunctionType;
//private:
//	typedef eastl::map< intptr_t, IFUNCTION_CLASS<R,EVENT_PARAM_TYPES>* >		FunctionPtrMap;
//	typedef typename FunctionPtrMap::iterator										FunctionIterator;
//private:
//	FunctionPtrMap			m_FuncMap;
//private:
//	UNORDERED_EVENT_CLASS( const UNORDERED_EVENT_CLASS<R, EVENT_PARAM_TYPES>& rhs ){}//Event 是不能拷贝的，指针的直接考本会带来问题
//	UNORDERED_EVENT_CLASS<R, EVENT_PARAM_TYPES>& operator = ( const UNORDERED_EVENT_CLASS<R, EVENT_PARAM_TYPES>& rhs ){ return *this; }
//public:
//	UNORDERED_EVENT_CLASS(){}
//	~UNORDERED_EVENT_CLASS()
//	{
//		this->Clear();
//	}
//	HADES_FORCEINLINE CallbackHandle Add( FunctionType* const input )
//	{
//		m_FuncMap.insert( eastl::make_pair( (intptr_t)input, input ) );
//		return input;
//	}
//	HADES_FORCEINLINE void Clear()//删除全部
//	{
//		for(FunctionIterator it = m_FuncMap.begin();
//			it != m_FuncMap.end();
//			++ it)
//		{
//			SAFE_DELETE( it->second );
//		}
//		m_FuncMap.clear();
//	}
//	HADES_FORCEINLINE void Remove(CallbackHandle ptr)//删除部分
//	{
//		FunctionIterator it = m_FuncMap.find( (intptr_t)ptr );
//		if ( m_FuncMap.end() != it )
//		{
//			m_FuncMap.erase( it );
//			SAFE_DELETE(ptr);
//		}
//	}
//	HADES_FORCEINLINE void Multicast( EVENT_PARAM_TYPES_INPUTS ) //多播不支持返回数值
//	{
//		for(FunctionIterator it = m_FuncMap.begin();
//			it != m_FuncMap.end();
//			)
//		{
//			FunctionIterator temp = it;//这么做为了在callback的时候删除自己的callback
//			++ it ;
//			temp->second->Invoker(EVENT_PARAM_INPUTS);
//		}
//	}
//	HADES_FORCEINLINE R Unicast(void* to, EVENT_PARAM_TYPES_INPUTS )//单播
//	{
//		FunctionIterator it = m_FuncMap.find( (intptr_t)to );
//		if ( m_FuncMap.end() != it )
//		{
//			it->second->Invoker(EVENT_PARAM_INPUTS);
//		}
//	}
//	HADES_FORCEINLINE unsigned int GetListenerCount()
//	{
//		return m_FuncMap.size();
//	}
//};



template <typename R HAVE_COMMA EVENT_PARAM_TEMPLATE >
class DELEGATE_CLASS //有序多播事件
{
public:
	typedef IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>* Callback;
private:
	typedef eastl::list< eastl::pair< intptr_t, IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>* > >	FunctionPtrListPair;
	typedef typename FunctionPtrListPair::iterator									FunctionIterator;
	typedef typename FunctionPtrListPair::reverse_iterator							FunctionRIterator;
private:
	FunctionPtrListPair			m_FuncListPair;
private:
	void _DoCopy( const DELEGATE_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>& rhs )
	{
		FOR_EACH_CONST(typename FunctionPtrListPair, pair, rhs.m_FuncListPair)
		{
			Add( pair->second->Duplicate() );
		}
	}
public:
	DELEGATE_CLASS(){}
	~DELEGATE_CLASS()
	{
		this->Clear();
	}
	DELEGATE_CLASS( const DELEGATE_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>& rhs )//Event 是不能拷贝的，指针的直接考本会带来问题
	{
		_DoCopy( rhs );
	}
	DELEGATE_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>& operator = ( const DELEGATE_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>& rhs )
	{
		_DoCopy( rhs );
		return *this;
	}
public:
	HADES_FORCEINLINE CallbackHandle Add( Callback const input )
	{
		m_FuncListPair.push_back( eastl::make_pair( (intptr_t)input, input ) );
		return input;
	}
	HADES_FORCEINLINE void Clear()//删除全部
	{
		for(FunctionIterator it = m_FuncListPair.begin();
			it != m_FuncListPair.end();
			++ it)
		{
			SAFE_DELETE( it->second );
		}
		m_FuncListPair.clear();
	}
	HADES_FORCEINLINE void Remove(CallbackHandle ptr)//删除部分
	{
		for(FunctionIterator it = m_FuncListPair.begin();
			it != m_FuncListPair.end();
			++ it)
		{
			if ( it->first == (intptr_t)ptr )
			{
				SAFE_DELETE( it->second );
				m_FuncListPair.erase( it );
				break;
			}
		}
	}
	HADES_FORCEINLINE void Multicast( EVENT_PARAM_TYPES_INPUTS ) //多播不支持返回数值
	{
		for(FunctionIterator it = m_FuncListPair.begin();
			it != m_FuncListPair.end();
			)
		{
			FunctionIterator temp = it;//这么做为了在callback的时候删除自己的callback
			++ it ;
			temp->second->Invoker(EVENT_PARAM_INPUTS);
		}
	}
	HADES_FORCEINLINE void RMulticast( EVENT_PARAM_TYPES_INPUTS ) //反向多播不支持返回数值（从末尾到开头）
	{
		for(FunctionRIterator it = m_FuncListPair.rbegin();
			it != m_FuncListPair.rend();
			)
		{
			FunctionRIterator temp = it;//这么做为了在callback的时候删除自己的callback
			++ it ;
			temp->second->Invoker(EVENT_PARAM_INPUTS);
		}
	}
	HADES_FORCEINLINE R Unicast(void* to HAVE_COMMA EVENT_PARAM_TYPES_INPUTS )//单播
	{
		for(FunctionIterator it = m_FuncListPair.begin();
			it != m_FuncListPair.end();
			++ it)
		{
			if ( it->first == (intptr_t)to )
			{
				it->second->Invoker(EVENT_PARAM_INPUTS);
				break;
			}
		}
	}
	HADES_FORCEINLINE unsigned int GetListenerCount()
	{
		return m_FuncListPair.size();
	}
};



template <typename R HAVE_COMMA EVENT_PARAM_TEMPLATE >
class SINGLE_DELEGATE_CLASS
{
public:
	typedef IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>* Callback;
private:
	Callback			m_FuncPtr;
private:
	void _DoCopy( const SINGLE_DELEGATE_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>& rhs )
	{
		m_FuncPtr = rhs.m_FuncPtr->Duplicate();
	}
public:
	SINGLE_DELEGATE_CLASS():m_FuncPtr(NULL){}
	~SINGLE_DELEGATE_CLASS()
	{
		this->Clear();
	}
	SINGLE_DELEGATE_CLASS( const SINGLE_DELEGATE_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>& rhs )
	{
		_DoCopy( rhs );
	}
	SINGLE_DELEGATE_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>& operator = ( const SINGLE_DELEGATE_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>& rhs )
	{
		_DoCopy( rhs );
		return *this;
	}
public:
	HADES_FORCEINLINE bool Empty()
	{
		return NULL == m_FuncPtr;
	}
	HADES_FORCEINLINE CallbackHandle Add( Callback const input )
	{
		SAFE_DELETE( m_FuncPtr );
		m_FuncPtr = input;
		return input;
	}
	HADES_FORCEINLINE void Clear()//删除全部
	{
		SAFE_DELETE( m_FuncPtr );
		m_FuncPtr = NULL;
	}
	HADES_FORCEINLINE R Unicast( EVENT_PARAM_TYPES_INPUTS )//单播
	{
		return m_FuncPtr->Invoker(EVENT_PARAM_INPUTS);
	}
	HADES_FORCEINLINE unsigned int GetListenerCount()
	{
		return NULL == m_FuncPtr ? 0 : 1;
	}
};


template <typename R HAVE_COMMA EVENT_PARAM_TEMPLATE >
class INTERCEPT_DELEGATE_CLASS //截断事件，返回值必须是bool
{
public:
	typedef IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>* Callback;
private:
	typedef eastl::list< eastl::pair< intptr_t, IFUNCTION_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>* > >	FunctionPtrListPair;
	typedef typename FunctionPtrListPair::iterator									FunctionIterator;
	typedef typename FunctionPtrListPair::reverse_iterator							FunctionRIterator;
private:
	FunctionPtrListPair			m_FuncListPair;
private:
	void _DoCopy( const INTERCEPT_DELEGATE_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>& rhs )
	{
		FOR_EACH_CONST(typename FunctionPtrListPair, pair, rhs.m_FuncListPair)
		{
			Add( pair->second->Duplicate() );
		}
	}
public:
	INTERCEPT_DELEGATE_CLASS(){}
	~INTERCEPT_DELEGATE_CLASS()
	{
		this->Clear();
	}
	INTERCEPT_DELEGATE_CLASS( const INTERCEPT_DELEGATE_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>& rhs )//Event 是不能拷贝的，指针的直接考本会带来问题
	{
		_DoCopy( rhs );
	}
	INTERCEPT_DELEGATE_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>& operator = ( const INTERCEPT_DELEGATE_CLASS<R HAVE_COMMA EVENT_PARAM_TYPES>& rhs )
	{
		_DoCopy( rhs );
		return *this;
	}
public:
	HADES_FORCEINLINE CallbackHandle Add( Callback const input )
	{
		m_FuncListPair.push_back( eastl::make_pair( (intptr_t)input, input ) );
		return input;
	}
	HADES_FORCEINLINE void Clear()//删除全部
	{
		for(FunctionIterator it = m_FuncListPair.begin();
			it != m_FuncListPair.end();
			++ it)
		{
			SAFE_DELETE( it->second );
		}
		m_FuncListPair.clear();
	}
	HADES_FORCEINLINE void Remove(CallbackHandle ptr)//删除部分
	{
		for(FunctionIterator it = m_FuncListPair.begin();
			it != m_FuncListPair.end();
			++ it)
		{
			if ( it->first == (intptr_t)ptr )
			{
				SAFE_DELETE( it->second );
				m_FuncListPair.erase( it );
				break;
			}
		}
	}
	HADES_FORCEINLINE bool Multicast( EVENT_PARAM_TYPES_INPUTS ) //多播
	{
		for(FunctionIterator it = m_FuncListPair.begin();
			it != m_FuncListPair.end();
			)
		{
			FunctionIterator temp = it;//这么做为了在callback的时候删除自己的callback
			++ it ;
			if (temp->second->Invoker(EVENT_PARAM_INPUTS))
			{
				return true;
			}
		}
		return false;
	}
	HADES_FORCEINLINE void RMulticast( EVENT_PARAM_TYPES_INPUTS ) //反向多播
	{
		for(FunctionRIterator it = m_FuncListPair.rbegin();
			it != m_FuncListPair.rend();
			)
		{
			FunctionRIterator temp = it;//这么做为了在callback的时候删除自己的callback
			++ it ;
			if( temp->second->Invoker(EVENT_PARAM_INPUTS) )
			{
				return true;
			}
		}
		return false;
	}
	HADES_FORCEINLINE unsigned int GetListenerCount()
	{
		return m_FuncListPair.size();
	}
};


#undef NAME
#undef NAME_T
#undef IFUNCTION_CLASS
#undef FUNCBIND_CLASS
#undef DELEGATE_CLASS
#undef SINGLE_DELEGATE_CLASS
#undef NAME_FIX
#undef HAVE_COMMA
#undef EVENT_PARAM_TEMPLATE
#undef EVENT_PARAM_TYPES_INPUTS
#undef EVENT_PARAM_TYPES
#undef EVENT_PARAM_INPUTS
