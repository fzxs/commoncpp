
#ifndef __GTL_CONVERT_H_
#define __GTL_CONVERT_H_

#include <string>

namespace gtl
{
	class SymbolConvert
	{
		//字符串转宽字符串
		static std::wstring convert2Wchar(const std::string &str);

		//宽字符串转字符串
		static std::string convert2Char(const std::wstring &wStr);

	};
}

#endif


