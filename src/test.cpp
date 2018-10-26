
#ifdef TEST

#include <iostream>

#include "commoncpp/gtl_stringbuilder.h"
#include "commoncpp/gtl_string_helper.h"
#include "commoncpp/redis_helper.h"

using namespace std;

void test()
{
	int result = 0;
	gtl::CRedisHandle *handle = gtl::CRedisHandle::connectTo("127.0.0.1", 6379);
	if (NULL == handle)
	{
		printf("connectTo() error !\n");
		return;
	}
	result = handle->executeNonQuery("lpush mylist 1");
	if (result)
	{
		printf("executeNonQuery() error !\n");
		return;
	}
	result = handle->executeNonQuery("lpush mylist 2");
	if (result)
	{
		printf("executeNonQuery() error !\n");
		return;
	}
	result = handle->executeNonQuery("lpush mylist 3");
	if (result)
	{
		printf("executeNonQuery() error !\n");
		return;
	}
	gtl::CRedisDataReader reader = handle->executeReader("lrange mylist 0 10");

	std::vector<gtl::CRedisDataReader>::iterator it;
	for (it = reader.getList().begin(); it != reader.getList().end(); ++it)
	{
		if (gtl::CRedisDataReader::STRING == it->getType())
		{
			cout << it->getString() << endl;
		}
		
	}

}

int main()
{
	test();
	getchar();
	return 0;
}

#endif



