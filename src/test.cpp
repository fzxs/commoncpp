
#ifdef TEST

#include <iostream>
#include "commoncpp/gtl_stringbuilder.h"

using namespace std;


void test()
{
	int i = 0;
	gtl::TLStringBuilder<char> builder;

	for (i = 0; i < 10; i++)
	{
		string str("hello");
		builder.AppendLine(str);
	}

	cout << builder.ToString() << endl;
}

int main()
{
	test();
	getchar();
	return 0;
}

#endif



