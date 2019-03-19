
#ifndef __REDIS_HELPER_H_
#define __REDIS_HELPER_H_

/*
typedef struct redisReply {
	int type;                       // 返回结果类型
	long long integer;              // 返回类型为整型的时候的返回值
	size_t len;                     // 字符串长度 
	char *str;                      // 返回错误类型或者字符类型的字符串 
	size_t elements;                // 返回数组类型时，元素的数量
	struct redisReply **element;    // 元素结果集合，redisReply对象 
} redisReply;

返回类型有以下几种:
REDIS_REPLY_STRING 1       //字符串
REDIS_REPLY_ARRAY 2        //数组，多个reply，通过element数组以及elements数组大小访问
REDIS_REPLY_INTEGER 3      //整型
REDIS_REPLY_NIL 4          //空，没有数据
REDIS_REPLY_STATUS 5       //状态，str字符串以及len
REDIS_REPLY_ERROR 6        //错误，同STATUS

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
		//获取整型数据
		int getInteger();

		//获取字符型数据
		std::string & getString();

		//获取集合数据
		std::vector<CRedisDataReader> & getList();

		//获取数据类型
		DATA_TYPE getType();

	private:
		DATA_TYPE _dataType;
		int _integerResult;
		std::string _strResult;
		std::vector<CRedisDataReader> _listResult;

	};

	/* redis事务类 */
	/*
	    单个 Redis 命令的执行是原子性的，但Redis没有在事务上增加任何维持原子性的机制，
	所以Redis事务的执行并不是原子性的。
	    事务可以理解为一个打包的批量执行脚本，但批量指令并非原子化的操作，
	中间某条指令的失败不会导致前面已做指令的回滚，也不会造成后续的指令不做。
	*/
	class CRedisTransaction
	{
	public:
		//开启事务
		static CRedisTransaction * begin(CRedisHandle *handle);

		//执行事务命令
		int appendCmd(const char *cmd);

		//关闭事务
		CRedisDataReader end();

	private:
		CRedisTransaction(CRedisHandle *handle) :_handle(handle),_needRollback(false) {}
		CRedisTransaction(CRedisTransaction &r);
		static const char * begin_str;
		static const char * exec_str;
		static const char * state_str;

	private:
		CRedisHandle *_handle;
		bool _needRollback;
		std::vector<CRedisDataReader> _listResult;

	};

	/* redis句柄类 */
	class CRedisHandle
	{
	public:
		static const char * ok_str;
		~CRedisHandle();
	public:
		//连接服务器
		static CRedisHandle * connectTo(const char *ip, uint16_t port);

		//开启事务
		CRedisTransaction *beginTransaction();

		//结束事务
		CRedisDataReader endTransaction(CRedisTransaction *transaction);

		//无查询结果执行redis命令
		int executeNonQuery(const char *cmd);

		//执行redis命令,返回状态
		std::string executeOperation(const char *cmd);

		//执行redis命令，返回查询结果
		CRedisDataReader executeReader(const char *cmd);

		


	private:
		//组装集合数据
		int formatList(redisReply *data, CRedisDataReader &reader);

	private:
		//禁止使用构造函数
		CRedisHandle(redisContext * context) :_context(context) {}
		CRedisHandle(CRedisHandle &r);

	private:
		redisContext *_context;
	};
}


#endif



