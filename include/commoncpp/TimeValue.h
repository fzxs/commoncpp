
#ifndef __GTL_TIME_H_
#define __GTL_TIME_H_

#include <stdint.h>
#include <time.h>
#include <sys/time.h>
#include <string>

/* 时间类 */

/*
	struct timeval {
		time_t      tv_sec;     // seconds
		suseconds_t tv_usec;    // microseconds
	};

	struct timespec {
		time_t   tv_sec;        // seconds
        long     tv_nsec;       // nanoseconds 
	};

*/

namespace gtl
{
	class TimeValue
	{
	public:
		//构造函数
		TimeValue(void) :_sec(0), _usec(0) 
		{
		}

		//构造函数
		TimeValue(uint64_t sec, uint64_t usec = 0) :_sec(0), _usec(usec) 
		{
		}

		//构造函数
		explicit TimeValue(const struct timeval &tv) :_sec(tv.tv_sec), _usec(tv.tv_usec) 
		{
		}

		//构造函数
		explicit TimeValue(const struct timespec &ts) :_sec(ts.tv_sec), _usec(ts.tv_nsec / 1000) 
		{
		}

	public:
		//类型强转struct timeval
		operator struct timeval() const;

		//类型强转struct timeval
		operator struct timespec() const;

		//返回秒
		time_t second() const
		{
			return this->_sec;
		}

		//返回微妙
		long microsecond() const
		{
			return this->_usec;
		}

		//set
		void set(uint64_t sec, uint64_t usec = 0)
		{
			this->_sec = sec;
			this->_usec = usec;
		}

		//set
		void set(const struct timeval &tv)
		{
			this->_sec = tv.tv_sec;
			this->_usec = tv.tv_usec;
		}

		//set
		void set(const struct timespec &ts)
		{
			this->_sec = ts.tv_sec;
			this->_usec = ts.tv_nsec / 1000;
		}

		//加法运算
		TimeValue operator+(const TimeValue &r);

		//减法运算
		TimeValue operator-(const TimeValue &r);

		//小于运算
		bool operator<(const TimeValue &r);

		//小于运算
		bool operator<(time_t sec);

		//大于运算
		bool operator>(const TimeValue &r);

		//大于运算
		bool operator>(time_t sec);

		//等于运算
		bool operator==(const TimeValue &r);

		//等于运算重载
		bool operator==(time_t sec);

		//当前时间
		static TimeValue now();

		//获取日期
		std::string toDate(void);

		//获取时间
		std::string toTime(void);

	protected:
		time_t _sec;    //秒
		long _usec;     //微妙
	};
}

#endif
