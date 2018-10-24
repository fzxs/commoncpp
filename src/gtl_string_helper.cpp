
#include "commoncpp/gtl_string_helper.h"

#include <string.h>

namespace gtl
{
	/********************************************************
	TStringHelper
	*********************************************************/

	/********************************************************
	   Func Name: split
	Date Created: 2018-10-24
	 Description: 切割字符串
		   Input: 
		  Output: 
		  Return: 
		 Caution: 
	*********************************************************/
	int TStringHelper::split(std::string strSrc, std::string delim, std::vector<std::string> &strWords)
	{
		char *tempBuf = NULL;
		char *pcIndex = NULL;

		if (strSrc.empty() || delim.empty())
		{
			return -1;
		}

		tempBuf = (char *)malloc(strSrc.length() + 1);
		if (NULL == tempBuf)
		{
			return -1;
		}
		memset(tempBuf, 0, strSrc.length() + 1);
		strcpy(tempBuf, strSrc.c_str());

		/*
		strsep()函数比strtok()更安全，所以使用strsep()函数
		*/

		while (pcIndex = strsep(&tempBuf, delim.c_str()),pcIndex)
		{
			strWords.push_back(pcIndex);
		}

		free(tempBuf);
		tempBuf = NULL;

		return 0;
	}


}





