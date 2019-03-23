
#include "TimeValue.h"

#include <string.h>

using namespace gtl;

/************************************************************************
 TimeValue                                                              
************************************************************************/

/********************************************************
   Func Name: struct timeval
Date Created: 2018-12-26
 Description: 类型强转为struct timeval
	   Input:
	  Output:
	  Return: struct timeval
	 Caution:
*********************************************************/
TimeValue::operator struct timeval() const
{
	struct timeval tv;

	tv.tv_sec = this->_sec;
	tv.tv_usec = this->_usec;

	return tv;
}

/********************************************************
   Func Name: struct timespec
Date Created: 2018-12-26
 Description: 类型强转为struct timespec
	   Input:
	  Output:
	  Return: struct timespec
	 Caution:
*********************************************************/
TimeValue::operator struct timespec() const
{
	struct timespec ts;

	ts.tv_sec = _sec;
	ts.tv_nsec = this->_usec * 1000;

	return ts;
}

/********************************************************
   Func Name: set
Date Created: 2018-12-26
 Description: set
	   Input:
	  Output:
	  Return: 
	 Caution:
*********************************************************/
TimeValue TimeValue::operator+(const TimeValue &r)
{
	return TimeValue(this->_sec + r._sec, this->_usec + r._usec);
}

/********************************************************
   Func Name: operator-
Date Created: 2018-12-27
 Description: 运算符重载-
	   Input:
	  Output:
	  Return: TimeValue
	 Caution:
*********************************************************/
TimeValue TimeValue::operator-(const TimeValue &r)
{
	return TimeValue(this->_sec - r._sec, this->_usec - r._usec);
}

/********************************************************
   Func Name: operator<
Date Created: 2018-12-27
 Description: 运算符重载<
	   Input:
	  Output:
	  Return: 布尔值
	 Caution:
*********************************************************/
bool TimeValue::operator<(time_t sec)
{
	return this->_sec < sec;
}

/********************************************************
   Func Name: operator<
Date Created: 2018-12-27
 Description: 运算符重载<
	   Input:
	  Output:
	  Return: 布尔值
	 Caution:
*********************************************************/
bool TimeValue::operator<(const TimeValue &r)
{
	if (this->_sec < r._sec)
	{
		return true;
	}else if (this->_sec > r._sec)
	{
		return false;
	}
	else
	{
		if (this->_usec < r._usec)
		{
			return true;
		}
	}

	return false;

}

/********************************************************
   Func Name: operator>
Date Created: 2018-12-27
 Description: 运算符重载>
	   Input:
	  Output:
	  Return: 布尔值
	 Caution:
*********************************************************/
bool TimeValue::operator>(time_t sec)
{
	return this->_sec > sec;
}

/********************************************************
   Func Name: operator>
Date Created: 2018-12-27
 Description: 运算符重载>
	   Input:
	  Output:
	  Return: 布尔值
	 Caution:
*********************************************************/
bool TimeValue::operator>(const TimeValue &r)
{
	if (this->_sec > r._sec)
	{
		return true;
	}
	else if (this->_sec < r._sec)
	{
		return false;
	}
	else
	{
		if (this->_usec > r._usec)
		{
			return true;
		}
	}

	return false;
}

/********************************************************
   Func Name: operator==
Date Created: 2018-12-27
 Description: 运算符重载==
	   Input:
	  Output:
	  Return: 布尔值
	 Caution:
*********************************************************/
bool TimeValue::operator==(const TimeValue &r)
{
	if (_sec == r._sec && _usec == r._usec)
	{
		return true;
	}

	return false;
}

/********************************************************
   Func Name: operator==
Date Created: 2018-12-27
 Description: 运算符重载==
	   Input:
	  Output:
	  Return: 布尔值
	 Caution:
*********************************************************/
bool TimeValue::operator==(time_t sec)
{
	return this->_sec == sec ? true : false;
}

/********************************************************
   Func Name: now
Date Created: 2018-12-27
 Description: 当前时间
	   Input:
	  Output:
	  Return: 当前时间
	 Caution:
*********************************************************/
TimeValue TimeValue::now()
{
	struct timeval tv;

	if (::gettimeofday(&tv, NULL))
	{
		return TimeValue(tv);
	}

	return TimeValue();
}

/********************************************************
   Func Name: toDate
Date Created: 2018-12-27
 Description: 获取日期
	   Input:
	  Output:
	  Return: 日期
	 Caution:
*********************************************************/
std::string TimeValue::toDate(void)
{
	char date[32] = { 0 };
	struct tm localTime;

	::localtime_r(&this->_sec, &localTime);
	strftime(date, 32, "%Y-%m-%d", &localTime);

	return date;
}

/********************************************************
   Func Name: toTime
Date Created: 2018-12-27
 Description: 获取时间
	   Input:
	  Output:
	  Return: 时间
	 Caution:
*********************************************************/
std::string TimeValue::toTime(void)
{
	char timeVal[32] = { 0 };
	struct tm localTime;

	::localtime_r(&this->_sec, &localTime);
	strftime(timeVal, 32, "%Y-%m-%d", &localTime);

	return timeVal;
}
