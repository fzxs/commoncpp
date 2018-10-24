
#ifndef __GTL_SINGLETON_H_
#define __GTL_SINGLETON_H_

#include <string>
#include "lock.h"

namespace gtl 
{
	template<typename T>
	class TLSingleton
	{
	public:
		virtual ~TLSingleton() {}
	public:
		//����ʵ������
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
	 Description: ����ʵ������
		   Input: 
		  Output: 
		  Return: ʵ������
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
	/* �������ĵ��� */
	template<typename T>
	class TLSingletonParam
	{
	public:
		virtual ~TLSingletonParam() {}
	public:
		//����ʵ������
		static T *getInstance();

		//����ʵ������
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
	 Description: ����ʵ������
		   Input: 
		  Output: 
		  Return: ʵ������
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
	 Description: ����ʵ������
		   Input: 
		  Output: 
		  Return: ʵ������
		 Caution: 
	*********************************************************/
	template<typename T>
	T * TLSingletonParam<T>::getInstance()
	{
		return _instance;
	}


}


#endif





