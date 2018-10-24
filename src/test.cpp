
#ifdef TEST

#include <iostream>

#include "commoncpp/gtl_stringbuilder.h"
#include "commoncpp/gtl_string_helper.h"

using namespace std;

void test()
{
	string str = "1,2,3,4,5";
	vector<string> list;
	vector<string>::iterator it;
	int result = 0;

	result = gtl::TStringHelper::split(str, ";", list);

	if (result)
	{
		cout << "error ." << endl;
		return;
	}

	for (it = list.begin();it != list.end();++it)
	{
		cout << *it << endl;
	}
}

int main()
{
	test();
	getchar();
	return 0;
}

#endif



