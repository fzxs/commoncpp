
#include "commoncpp/convert.h"

#include <stdlib.h>
#include <string.h>
#include <locale.h>   /* 宽字符转换 */
#include <wchar.h>
#include <assert.h>

using namespace gtl;
/********************************************************
SymbolConvert
*********************************************************/

/********************************************************
   Func Name: convert2Wchar
Date Created: 2019-2-12
 Description: 字符串转宽字符
	   Input: 
	  Output: 
	  Return: 宽字符
	 Caution: 
*********************************************************/
std::wstring SymbolConvert::convert2Wchar(const std::string &str)
{
	assert(!str.empty());

	int len = 0;
	char *old_local = NULL;
	wchar_t *targetStr = NULL;
	std::wstring wStr;

	old_local = setlocale(LC_ALL, NULL);
	if (NULL == old_local)
	{
		return wStr;
	}
	setlocale(LC_ALL, "zh_CN.UTF-8");

	//如果第一个参数为NULL,则可以用来返回宽字符的长度
	len = mbstowcs(NULL, str.c_str(), str.length() * 2);
	if (len <= 0)
	{
		return wStr;
	}

	len += 1;

	targetStr = (wchar_t *)malloc(len * sizeof(wchar_t));
	if (NULL == targetStr)
	{
		return wStr;
	}
	memset(targetStr, 0, len * sizeof(wchar_t));

	if (mbstowcs(targetStr, str.c_str(), len) <= 0)
	{
		return wStr;
	}

	wStr = targetStr;

	setlocale(LC_ALL, old_local);

	free(targetStr);
	targetStr = NULL;

	return wStr;
}

/********************************************************
   Func Name: convert2Char
Date Created: 2019-2-12
 Description: 宽字符串转字符串
	   Input: 
	  Output: 
	  Return: 字符串
	 Caution: 
*********************************************************/
std::string SymbolConvert::convert2Char(const std::wstring &wStr)
{
	assert(!wStr.empty());

	char *old_local = NULL;
	char *targetStr = NULL;
	std::string cStr;
	int strLen = 0;

	//设置本地变量
	old_local = setlocale(LC_ALL, NULL);
	if (NULL == old_local)
	{
		return cStr;
	}
	setlocale(LC_ALL, "zh_CN.UTF-8");

	//如果第一个参数为NULL,则可以用来返回字符串的长度
	strLen = wcstombs(NULL, wStr.c_str(), 0);
	if (strLen <= 0)
	{
		return cStr;
	}

	//字符串结尾以'\0'
	strLen += 1;

	targetStr = (char *)malloc(strLen * sizeof(char));
	if (NULL == targetStr)
	{
		return cStr;
	}
	memset(targetStr, 0, strLen * sizeof(char));

	if (wcstombs(targetStr, wStr.c_str(), strLen) <= 0)
	{
		return cStr;
	}

	cStr = targetStr;

	setlocale(LC_ALL, old_local);

	free(targetStr);
	targetStr = NULL;

	return cStr;
}
