
#ifdef TEST

#include <iostream>

#include "commoncpp/gtl_stringbuilder.h"
#include "commoncpp/gtl_string_helper.h"
#include "commoncpp/redis_helper.h"

using namespace std;

void test()
{
	//int result = 0;
	gtl::CRedisHandle *handle = gtl::CRedisHandle::connectTo("127.0.0.1", 6379);
	if (NULL == handle)
	{
		printf("connectTo() error !\n");
		return;
	}

	gtl::CRedisTransaction * transaction = handle->beginTransaction();
	if (NULL == transaction)
	{
		return;
	}
	transaction->appendCmd("rpush mylist 2");
	transaction->appendCmd("rpush mylist 3");
	transaction->appendCmd("rpush mylist 4");
	transaction->appendCmd("lrange mylist 0 10");
	gtl::CRedisDataReader reader = handle->endTransaction(transaction);


	//result = handle->executeNonQuery("MULTI");
	//if (result)
	//{
	//	printf("executeNonQuery()111 error !\n");
	//	return;
	//}
	//result = handle->executeNonQuery("lpush mylist 2");
	//if (result)
	//{
	//	printf("executeNonQuery() 2222 error !\n");
	//	return;
	//}
	//result = handle->executeNonQuery("lpush mylist 3");
	//if (result)
	//{
	//	printf("executeNonQuery() 333 error !\n");
	//	return;
	//}
	//gtl::CRedisDataReader reader = handle->executeReader("lrange mylist 0 10");

	std::vector<gtl::CRedisDataReader>::iterator it;
	for (it = reader.getList().begin(); it != reader.getList().end(); ++it)
	{
		if (gtl::CRedisDataReader::STRING == it->getType())
		{
			cout << it->getString() << endl;
		}

	}

}

//void simple(redisContext * context, const char *cmd)
//{
//	redisReply *data = NULL;
//
//	data = (redisReply *)redisCommand(context, cmd);
//	if (NULL == data)
//	{
//		printf("redisCommand() failed .\n");
//		return;
//	}
//
//	switch (data->type)
//	{
//	case REDIS_REPLY_STRING:
//		printf("cmd result [%s]\n", data->str);
//		break;
//	case REDIS_REPLY_ARRAY:
//		printf("cmd result array \n");
//		break;
//	case REDIS_REPLY_INTEGER:
//		printf("cmd result data[%lld] \n", data->integer);
//		break;
//	case REDIS_REPLY_NIL:
//		printf("result is null .\n");
//		break;
//	case REDIS_REPLY_STATUS:
//		printf("cmd staus[%s]\n", data->str);
//		break;
//	case REDIS_REPLY_ERROR:
//		printf("cmd error[%s]\n", data->str);
//		break;
//	default:
//		printf("what ? \n");
//		break;
//	}
//	freeReplyObject(data);
//}
//
//void test()
//{
//	redisContext * context = NULL;
//	
//	context = redisConnect("127.0.0.1", 6379);
//	if (NULL == context)
//	{
//		printf("redisConnect() failed .\n");
//		return;
//	}
//
//	simple(context, "MULTI");
//	simple(context, "set book maomao");
//	simple(context, "set music xiexie");
//	simple(context, "get music1");
//	simple(context, "EXEC");
//
//	redisFree(context);
//
//}

int main()
{
	test();
	getchar();
	return 0;
}

#endif



