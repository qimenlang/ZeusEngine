#pragma once

#include <type_traits>
/*
*	单例模式：
*		构造函数
*	    拷贝构造、赋值操作符、移动构造、移动赋值操作符都应该delete
*		
*/

namespace zeus {
	//template<typename T>
	//class singleton
	//{
	//public:
	//	virtual ~singleton() noexcept = default;
	//	singleton(const singleton&) = delete;//这里为什么没有T
	//	singleton& operator =(const singleton&) = delete;

	//	T instance() noexcept(std::is_nothrow_constructible<T>::value) {
	//		static T instance;
	//		return instance;
	//	}
	//private:
	//	singleton() = default;

	//};
	template<typename T>
	class singleton
	{
	protected:
		singleton() = default;
	public:
		virtual ~singleton() noexcept = default;
		singleton(const singleton&) = delete;
		singleton& operator =(const singleton&) = delete;
		singleton(singleton&&) = delete;
		singleton& operator =(singleton&&) = delete;
		static T& instance()
		{	
			static T instance;//magic static 保证了线程安全，没有data-race
			return instance;
		}
	};
}