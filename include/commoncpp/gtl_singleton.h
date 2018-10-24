
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
		virtual ~TLSingleton() {}
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

	/************************************************************************/
	/* TLSingleton                                                            
	/************************************************************************/

	template<typename T>
	TLSingleton<T> * TLSingleton<T>::_instance = NULL;
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
		int result = 0;

		if (NULL == _instance)
		{
			_mtx->lock();
			if (NULL == _instance)
			{
				tmp = new T();
				result = tmp->init();
				if (result)
				{
					_mtx->unlock();
					return NULL;
				}
				_instance = tmp;
			}
			_mtx->unlock();
		}

		return _instance;
	}

	/*-----------------------------------------------------------*/
	/* 带参数的单例 */
	template<typename T>
	class TLSingletonParam
	{
	public:
		virtual ~TLSingletonParam() {}
	public:
		//创建实例对象
		static T *getInstance();

		//创建实例对象
		static T *getInstance(std::string strTensorName, std::string strFile, bool bSeqlen);

	private:
		TLSingletonParam() {};
		TLSingletonParam(TLSingletonParam &r);

	private:
		static T * _instance;
		static CMutexLock * _mtx;
	};

	/************************************************************************/
	/* TLSingletonParam                                                            
	/************************************************************************/

	template<typename T>
	TLSingletonParam<T> * TLSingletonParam<T>::_instance = NULL;
	template<typename T>
	CMutexLock * TLSingletonParam<T>::_mtx = new CMutexLock;

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
	T * TLSingletonParam<T>::getInstance(std::string strTensorName, std::string strFile, bool bSeqlen)
	{
		T *tmp = NULL;
		int result = 0;

		if (NULL == _instance)
		{
			_mtx->lock();
			if (NULL == _instance)
			{
				tmp = new T();
				result = tmp->init(strTensorName, strFile, bSeqlen);
				if (result)
				{
					_mtx->unlock();
					return NULL;
				}
				_instance = tmp;
			}
			_mtx->unlock();
		}

		return _instance;
	}

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
	T * TLSingletonParam<T>::getInstance()
	{
		return _instance;
	}


}


#endif





