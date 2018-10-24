
#ifndef __GTL_STRING_HELPER_H_
#define __GTL_STRING_HELPER_H_

#include <string>
#include <vector>

namespace gtl
{
	class TStringHelper
	{
	public:
		//切割字符串
		static int split(std::string strSrc, std::string delim, std::vector<std::string> &strWords);
	};
}


#endif




