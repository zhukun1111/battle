#include <string>
#include <iostream>
#include <sstream>
#include <memory.h>

using namespace std;
void ConvertOnce(const string& strSrc, string& strDst);
void convert(char* src, char* dst)
{
	//assert(src);
	//assert(dst);

	std::string strSrc(src);
	
	while(1)
	{
		std::string strDst;
		ConvertOnce(strSrc, strDst);
		if(strSrc == strDst) break;

		strSrc = strDst;
	}

	strcpy(dst, strSrc.c_str());
}
string ConvertIntToString(int i)
{
	stringstream ss;
	ss << i;
	return ss.str();
}
void ConvertOnce(const string& strSrc, string& strDst)
{
	int count = 1;
	for(size_t i = 0; i < strSrc.size(); i++)
	{
		if(i != strSrc.size() - 1 && strSrc[i] == strSrc[i+1])
		{
			count++;
		}
		else
		{
				if( count != 1) strDst += ConvertIntToString(count);
				strDst.push_back(strSrc[i]);
				count = 1;

			
		}
	}
}

class A
{
public:
	~A()
	{
		cout << "~A" << endl;
	}
};

class B : public A
{
public:
	virtual ~B()
	{
		cout << "~B" << endl;
	}
};

class C : public B
{
public:
	~C()
	{
		cout << "~C" << endl;
	}
};


int main()
{
	/*char strSrc[] = "422223888622";
	char *strDst = (char *)malloc(100);
	convert(strSrc, strDst);
	int i = 0;*/
		A* a = new A();
		B* b = new B();
		C* c = new C();
		A* d = new B();
		//A* e = new C();
		//B* f = new C();

		delete a;
		delete b;
		delete c;
		delete d;
		//delete e;
		//delete f;
		

		system("pause");
		return 0;
}