#include <string>
#include <string.h>

using namespace std;

// funkcja pomocnicza do operator+
int pow10(int k)
{
	int ret=1;
	while( k>0 )
	{
		ret*=10;
		k--;
	}
	return ret;
}

wstring operator+(wstring str1, int k)
{
	wstring str2;
	if ( k<0 )
	{
		k=-k;
		str2=(str1+L"-") + k; // rekurencja
	}
	else if( k < 10 )
	{
		wchar_t cstr[2];
		cstr[0]=wchar_t(int('0')+k);
		cstr[1]='\0';
		str2=cstr;
	}
	else
	{
		// pow=(liczba cyfr w k)
		int pow=0;
		for ( int p10=1; p10<=k; p10*=10)
			pow++;
		int cyfr=pow;
		// jako że ostatnie dzielenie ma być przez 1, czyli dla zera to:
		pow--;
		wchar_t* c_str = new wchar_t[cyfr+1];
		while (pow>=0)
		{
			int tmp=int(k/pow10(pow))*pow10(pow);
			k-=tmp;
			int digit=tmp/pow10(pow);
			c_str[cyfr - pow-1]=wchar_t(int('0')+digit);
			pow--;
		}
		c_str[cyfr]='\0';
		str2 = c_str;
		delete[] c_str;
	}
	return (str1 + str2);
}

string operator+(string str1, int k)
{
	string str2;
	if ( k<0 )
	{
		k=-k;
		str2=(str1+"-") + k; // rekurencja
	}
	else if( k < 10 )
	{
		char cstr[2];
		cstr[0]=char(int('0')+k);
		cstr[1]='\0';
		str2=cstr;
	}
	else
	{
		// pow=(liczba cyfr w k)
		int pow=0;
		for ( int p10=1; p10<=k; p10*=10)
			pow++;
		int cyfr=pow;
		// jako że ostatnie dzielenie ma być przez 1, czyli dla zera to:
		pow--;
		char* c_str = new char[cyfr+1];
		while (pow>=0)
		{
			int tmp=int(k/pow10(pow))*pow10(pow);
			k-=tmp;
			int digit=tmp/pow10(pow);
			c_str[cyfr - pow-1]=char(int('0')+digit);
			pow--;
		}
		c_str[cyfr]='\0';
		str2 = c_str;
		delete[] c_str;
	}
	return (str1 + str2);

}
