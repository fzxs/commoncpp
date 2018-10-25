
#ifndef __GTL_SINGLETON_H_
#define __GTL_SINGLETON_H_

/* 单例模式 */

#include <string>
#include "lock.h"

namespace gtl 
{
	/* 单例模式 */
	template<typename T>
	class TLSingleton
	{
	public:
		//创建实例对象
		static T *getInstance();

	private:
		TLSingleton() {};
		TLSingleton(TLSingleton &r);

	private:
		static T * _instance;
		static CMutexLock * _mtx;
	};

	/************************************************************************
	TLSingleton                                                            
	************************************************************************/

	template<typename T>
	T* TLSingleton<T>::_instance = NULL;
	template<typename T>
	CMutexLock * TLSingleton<T>::_mtx = new CMutexLock;

	/********************************************************
	   Func Name: getInstance
	Date Created: 2018-10-12
	 Description: 创建实例对象
		   Input: 
		  Output: 
		  Return: 实例对象
		 Caution: 
	*********************************************************/
	template<typename T>
	T * TLSingleton<T>::getInstance()
	{
		T *tmp = NULL;

		if (NULL == _instance)
		{
			_mtx->lock();
			if (NULL == _instance)
			{
				tmp = new T();
				//如果T有init方法，请在main函数中调用Init方法，当然Init方法也可以随意传参
				_instance = tmp;
			}
			_mtx->unlock();
		}

		return _instance;
	}

}


#endif





