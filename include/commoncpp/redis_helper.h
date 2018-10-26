
#ifndef __REDIS_HELPER_H_
#define __REDIS_HELPER_H_

/*
typedef struct redisReply {
	int type;                       // ���ؽ������
	long long integer;              // ��������Ϊ���͵�ʱ��ķ���ֵ
	size_t len;                     // �ַ������� 
	char *str;                      // ���ش������ͻ����ַ����͵��ַ��� 
	size_t elements;                // ������������ʱ��Ԫ�ص�����
	struct redisReply **element;    // Ԫ�ؽ�����ϣ�redisReply���� 
} redisReply;

�������������¼���:
REDIS_REPLY_STRING 1       //�ַ���
REDIS_REPLY_ARRAY 2        //���飬���reply��ͨ��element�����Լ�elements�����С����
REDIS_REPLY_INTEGER 3      //����
REDIS_REPLY_NIL 4          //�գ�û������
REDIS_REPLY_STATUS 5       //״̬��str�ַ����Լ�len
REDIS_REPLY_ERROR 6        //����ͬSTATUS

*/
#include "commoncpp/hiredis/hiredis.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <map>

namespace gtl
{
	class CRedisHandle;

	class CRedisDataReader
	{	
	public:
		friend class CRedisHandle;
		CRedisDataReader() :_dataType(UNKNOW),_integerResult(0) {}
		enum DATA_TYPE
		{
			STRING,
			ARRAY,
			INTEGER,
			UNKNOW,
		};

	public:
		//��ȡ��������
		int getInteger();

		//��ȡ�ַ�������
		std::string & getString();

		//��ȡ��������
		std::vector<CRedisDataReader> & getList();

		//��ȡ��������
		DATA_TYPE getType();

	private:
		DATA_TYPE _dataType;
		int _integerResult;
		std::string _strResult;
		std::vector<CRedisDataReader> _listResult;

	};

	/* redis����� */
	class CRedisHandle
	{
	public:
	public:
		static const char * ok_str;
		~CRedisHandle();
	public:
		//���ӷ�����
		static CRedisHandle * connectTo(const char *ip, uint16_t port);

		//�޲�ѯ���ִ��redis����
		int executeNonQuery(const char *cmd);

		//ִ��redis������ز�ѯ���
		CRedisDataReader executeReader(const char *cmd);

	private:
		//��װ��������
		int formatList(redisReply *data, CRedisDataReader &reader);

	private:
		//��ֹʹ�ù��캯��
		CRedisHandle(redisContext * context) :_context(context) {}
		CRedisHandle(CRedisHandle &r);

	private:
		redisContext *_context;
	};
}


#endif


