
#ifndef __GTC_LOCK_H_
#define __GTC_LOCK_H_

/*锁模块*/

#include<pthread.h>

namespace gtc 
{
	//互斥锁
	class Cond_Lock;
	class Mutex_Lock
	{
	public:
		friend class Cond_Lock;
	public:
		Mutex_Lock();
		~Mutex_Lock();

	public:
		void lock();
		void unlock();

	protected:
		pthread_mutex_t _mtx;

	};

	//条件锁
	class Cond_Lock
	{
	public:
		Cond_Lock(Mutex_Lock mutex);
		virtual ~Cond_Lock();

	public:
		//条件等待
		int wait();

		//条件等待
		int timedwait(long sec, long nsec = 0);

		//发送信号
		int signal();

		//广播信号
		int broadcast();

	private:
		pthread_cond_t _cond;
		Mutex_Lock _mutex;
		//bool m_signalWait;
	};

	//读写锁
	class RW_Lock
	{
	public:
		RW_Lock();
		~RW_Lock();

	public:
		//获取读锁
		void readLock();

		//获取写锁
		void writeLock();

		//释放锁
		void unLock();

	private:
		pthread_rwlock_t _rwlock;
	};

	//范围读锁
	class Guard_Mutex_Lock
	{
	public:
		Guard_Mutex_Lock(Mutex_Lock &mutex) :_mtx(mutex)
		{
			this->_mtx.lock();
		}
		~Guard_Mutex_Lock()
		{
			this->_mtx.unlock();
		}
	private:
		Mutex_Lock &_mtx;
	};
}




#endif
