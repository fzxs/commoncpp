
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
	 Description: ��ȡ��������
		   Input: 
		  Output: 
		  Return: ���
		 Caution: 
	*********************************************************/
	int CRedisDataReader::getInteger()
	{
		return _integerResult;
	}

	/********************************************************
	   Func Name: getString
	Date Created: 2018-10-26
	 Description: ��ȡ�ַ�������
		   Input: 
		  Output: 
		  Return: ���
		 Caution: 
	*********************************************************/
	std::string & CRedisDataReader::getString()
	{
		return _strResult;
	}

	/********************************************************
	   Func Name: getList
	Date Created: 2018-10-26
	 Description: ��ȡ��������
		   Input: 
		  Output: 
		  Return: ���
		 Caution: 
	*********************************************************/
	std::vector<CRedisDataReader> & CRedisDataReader::getList()
	{
		return _listResult;
	}

	/********************************************************
	   Func Name: getList
	Date Created: 2018-10-26
	 Description: ��ȡ��������
		   Input: 
		  Output: 
		  Return: ���
		 Caution: 
	*********************************************************/
	CRedisDataReader::DATA_TYPE CRedisDataReader::getType()
	{
		return _dataType;
	}

	/********************************************************
	CRedisHandle
	*********************************************************/
	//const char * CRedisHandle::error_str;
	const char * CRedisHandle::ok_str = "OK";

	/********************************************************
	   Func Name: CRedisHandle
	Date Created: 2018-10-26
	 Description: ���캯��
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
	 Description: ���ӷ�����
		   Input: 
		  Output: 
		  Return: error code
		 Caution: 
	*********************************************************/
	CRedisHandle * CRedisHandle::connectTo(const char *ip, uint16_t port)
	{
		/*
		����ԭ�ͣ�redisContext *redisConnect(const char *ip, int port);
		˵�����ú�����������redis���ݿ⣬����Ϊ���ݿ��ip��ַ�Ͷ˿ڣ�ͨ��Ĭ�϶˿�Ϊ6379��
		�ú�������һ��redisContext����
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
	   Func Name: executeNonQuery
	Date Created: 2018-10-26
	 Description: �޲�ѯ���ִ��redis����
		   Input: 
		  Output: 
		  Return: �����Ϣ
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
	   Func Name: executeReader
	Date Created: 2018-10-26
	 Description: ִ��redis������ز�ѯ���
		   Input: 
		  Output: 
		  Return: �����Ϣ
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
	 Description: ��ʽ����������
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




