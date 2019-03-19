
#include "commoncpp/redis_helper.h"

#include <string.h>

namespace gtl
{
	/********************************************************
	CRedisDataReader
	*********************************************************/

	/********************************************************
	   Func Name: getInteger
	Date Created: 2018-10-26
	 Description: 获取整型数据
		   Input: 
		  Output: 
		  Return: 结果
		 Caution: 
	*********************************************************/
	int CRedisDataReader::getInteger()
	{
		return _integerResult;
	}

	/********************************************************
	   Func Name: getString
	Date Created: 2018-10-26
	 Description: 获取字符型数据
		   Input: 
		  Output: 
		  Return: 结果
		 Caution: 
	*********************************************************/
	std::string & CRedisDataReader::getString()
	{
		return _strResult;
	}

	/********************************************************
	   Func Name: getList
	Date Created: 2018-10-26
	 Description: 获取集合数据
		   Input: 
		  Output: 
		  Return: 结果
		 Caution: 
	*********************************************************/
	std::vector<CRedisDataReader> & CRedisDataReader::getList()
	{
		return _listResult;
	}

	/********************************************************
	   Func Name: getList
	Date Created: 2018-10-26
	 Description: 获取集合数据
		   Input: 
		  Output: 
		  Return: 结果
		 Caution: 
	*********************************************************/
	CRedisDataReader::DATA_TYPE CRedisDataReader::getType()
	{
		return _dataType;
	}

	/********************************************************
	CRedisTransaction
	*********************************************************/
	const char * CRedisTransaction::begin_str = "MULTI";
	const char * CRedisTransaction::exec_str = "EXEC";
	const char * CRedisTransaction::state_str = "QUEUED";

	/********************************************************
	   Func Name: begin
	Date Created: 2018-10-26
	 Description: 开始事务
		   Input: 
		  Output: 
		  Return: 事务实例
		 Caution: 
	*********************************************************/
	CRedisTransaction * CRedisTransaction::begin(CRedisHandle *handle)
	{
		CRedisTransaction * result = NULL;

		result = new CRedisTransaction(handle);
		if (NULL == result)
		{
			return NULL;
		}
		
		//开启事务
		if ("OK" != handle->executeOperation(begin_str))
		{
			return NULL;
		}

		return result;

	}

	/********************************************************
	   Func Name: appendCmd
	Date Created: 2018-10-26
	 Description: 执行事务命令
		   Input: 
		  Output: 
		  Return: error code
		 Caution: 
	*********************************************************/
	int CRedisTransaction::appendCmd(const char *cmd)
	{
		std::string strRes;

		if (NULL == cmd)
		{
			return -1;
		}

		if (state_str == _handle->executeOperation(cmd))
		{
			return 0;
		}
		_needRollback = true;

		return -1;

	}

	/********************************************************
	   Func Name: end
	Date Created: 2018-10-26
	 Description: 结束事务
		   Input: 
		  Output: 
		  Return: 结果集
		 Caution: 
	*********************************************************/
	CRedisDataReader CRedisTransaction::end()
	{
		CRedisDataReader reader;

		if (_needRollback)
		{
			return reader;
		}

		return _handle->executeReader(exec_str);

	}

	/********************************************************
	CRedisHandle
	*********************************************************/
	//const char * CRedisHandle::error_str;
	const char * CRedisHandle::ok_str = "OK";

	/********************************************************
	   Func Name: CRedisHandle
	Date Created: 2018-10-26
	 Description: 构造函数
		   Input: 
		  Output: 
		  Return: 
		 Caution: 
	*********************************************************/
	CRedisHandle::~CRedisHandle()
	{
		if (_context)
		{
			redisFree(_context);
			_context = NULL;
		}
	}

	/********************************************************
	   Func Name: connectTo
	Date Created: 2018-10-26
	 Description: 连接服务器
		   Input: 
		  Output: 
		  Return: error code
		 Caution: 
	*********************************************************/
	CRedisHandle * CRedisHandle::connectTo(const char *ip, uint16_t port)
	{
		/*
		函数原型：redisContext *redisConnect(const char *ip, int port);
		说明：该函数用来连接redis数据库，参数为数据库的ip地址和端口，通常默认端口为6379。
		该函数返回一个redisContext对象。
		*/
		CRedisHandle *handle = NULL;
		redisContext *context = NULL;
		if (NULL == ip)
		{
			return NULL;
		}
		context = redisConnect(ip, port);
		if (NULL == context)
		{
			return NULL;
		}
		handle = new CRedisHandle(context);

		return handle;

	}

	/********************************************************
	   Func Name: beginTransaction
	Date Created: 2018-10-26
	 Description: 开启事务
		   Input: 
		  Output: 
		  Return: 事务实例对象
		 Caution: 
	*********************************************************/
	CRedisTransaction *CRedisHandle::beginTransaction()
	{
		return CRedisTransaction::begin(this);
	}

	/********************************************************
	   Func Name: beginTransaction
	Date Created: 2018-10-26
	 Description: 结束事务
		   Input: 
		  Output: 
		  Return: 结果集
		 Caution: 
	*********************************************************/
	CRedisDataReader CRedisHandle::endTransaction(CRedisTransaction *transaction)
	{
		CRedisDataReader reader;

		if (NULL == transaction)
		{
			return reader;
		}
		reader = transaction->end();
		delete transaction;
		transaction = NULL;

		return reader;

	}

	/********************************************************
	   Func Name: executeNonQuery
	Date Created: 2018-10-26
	 Description: 无查询结果执行redis命令
		   Input: 
		  Output: 
		  Return: 结果信息
		 Caution: 
	*********************************************************/
	int CRedisHandle::executeNonQuery(const char *cmd)
	{
		int result = 0;
		redisReply *data = NULL;

		if (NULL == cmd)
		{
			return -1;
		}
		data = (redisReply *)redisCommand(_context, cmd);
		if (NULL == data)
		{
			return -1;
		}

		if (REDIS_REPLY_STATUS == data->type)
		{
			result = (0 == strcmp(ok_str, data->str) ? 0 : -1);
		}

		freeReplyObject(data);

		return result;
	}

	/********************************************************
	   Func Name: executeOperation
	Date Created: 2018-10-26
	 Description: 执行redis命令,返回状态
		   Input: 
		  Output: 
		  Return: 结果信息
		 Caution: 
	*********************************************************/
	std::string CRedisHandle::executeOperation(const char *cmd)
	{
		std::string strRes;
		redisReply *data = NULL;

		if (NULL == cmd)
		{
			return "";
		}
		data = (redisReply *)redisCommand(_context, cmd);
		if (NULL == data)
		{
			return "";
		}

		if (REDIS_REPLY_STATUS == data->type)
		{
			strRes = data->str;
		}

		freeReplyObject(data);

		return strRes;
	}

	/********************************************************
	   Func Name: executeReader
	Date Created: 2018-10-26
	 Description: 执行redis命令，返回查询结果
		   Input: 
		  Output: 
		  Return: 结果信息
		 Caution: 
	*********************************************************/
	CRedisDataReader CRedisHandle::executeReader(const char *cmd)
	{
		CRedisDataReader reader;
		redisReply *data = NULL;

		if (NULL == cmd)
		{
			return reader;
		}

		data = (redisReply *)redisCommand(_context, cmd);
		if (NULL == data)
		{
			return reader;
		}

		switch (data->type)
		{
		case REDIS_REPLY_STRING:
			reader._strResult = data->str;
			break;
		case REDIS_REPLY_ARRAY:
			reader._dataType = CRedisDataReader::ARRAY;
			formatList(data, reader);
			break;
		case REDIS_REPLY_INTEGER:
			reader._integerResult = data->integer;
			break;
		case REDIS_REPLY_NIL:
		case REDIS_REPLY_STATUS:
		case REDIS_REPLY_ERROR:
		default:
			reader._dataType = CRedisDataReader::UNKNOW;
			break;
		}

		freeReplyObject(data);

		return reader;
	}

	/********************************************************
	   Func Name: formatList
	Date Created: 2018-10-26
	 Description: 格式化数组数据
		   Input: 
		  Output: 
		  Return: error code
		 Caution: 
	*********************************************************/
	int CRedisHandle::formatList(redisReply *data, CRedisDataReader &reader)
	{
		//int result = 0;
		uint32_t i = 0;
		redisReply *dataNode = NULL;

		for (i = 0; i < data->elements; i++)
		{
			CRedisDataReader nodeReader;
			dataNode = data->element[i];
			switch (dataNode->type)
			{
			case REDIS_REPLY_STRING:
				nodeReader._dataType = CRedisDataReader::STRING;
				nodeReader._strResult = dataNode->str;
				break;
			case REDIS_REPLY_ARRAY:
				nodeReader._dataType = CRedisDataReader::ARRAY;
				formatList(dataNode, nodeReader);
				break;
			case REDIS_REPLY_INTEGER:
				nodeReader._dataType = CRedisDataReader::INTEGER;
				nodeReader._integerResult = data->integer;
				break;
			case REDIS_REPLY_NIL:
			case REDIS_REPLY_STATUS:
			case REDIS_REPLY_ERROR:
			default:
				continue;
				break;
			}
			reader._listResult.push_back(nodeReader);
		}

		return -1;
	}



}




