/*
 * utils.cpp
 *
 *  Created on: 2010-12-03
 *      Author: trakos
 */

#include <iomanip>

#ifdef __unix__
	#include <Tmysql/LiveRow.h>
	#include <Tmysql/String.h>
#endif
#include <utils.h>
#include <indent.h>
#include <sstream>

template<typename T> std::ostream& operator <<(std::ostream& stream, std::map<std::string, T> const &map)
{
	typedef typename std::map<std::string, T>::const_iterator mapIterator;
	stream << "Map()" << std::endl;
	stream << "{" << std::endl;
	mapIterator end = map.end();
	for ( mapIterator it = map.begin(); it != end; ++it)
	{
		stream << "\t" << std::setw(20) << std::left << "["+it->first+"]" << " " << "=>" << "\t" << indent
				<< it->second << std::endl << unindent;
	}
	stream << "}";
	return stream;
}
#ifdef __unix__
	template std::ostream& operator <<(std::ostream& stream, std::map<std::string, Tmysql_Row> const &map);
#endif

template<typename T> std::ostream& operator <<(std::ostream& stream, std::vector<T> &vector)
{
	stream << "Vector()" << std::endl;
	stream << "{" << std::endl;
	size_t size = vector.size();
	for (size_t i = 0; i < size; ++i)
	{
		stream << "\t" << "[" << i << "]" << " " << "=>" << "\t" << indent
				<< vector.at(i) << std::endl << unindent;
	}
	stream << "}";
	return stream;
}

template<typename T> std::wostream& operator <<(std::wostream& stream, std::vector<T> &vector)
{
	stream << L"Vector()" << std::endl;
	stream <<  L"{" << std::endl;
	size_t size = vector.size();
	for (size_t i = 0; i < size; ++i)
	{
		stream <<  L"\t" <<  L"[" << i <<  L"]" <<  L" " <<  L"=>" <<  L"\t" << vector.at(i) << std::endl;
	}
	stream << L"}";
	return stream;
}

template std::ostream& operator <<(std::ostream& stream, std::map<std::string, int> const &map);
template std::ostream& operator <<(std::ostream& stream, std::map<std::string, std::string> const &map);
template std::ostream& operator <<(std::ostream& stream, std::vector<int> &vector);
template std::ostream& operator <<(std::ostream& stream, std::vector<std::string> &vector);
template std::wostream& operator <<(std::wostream& stream, std::vector<std::wstring> &vector);

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/lambda/lambda.hpp>
#include <vector>
#include <string>

std::vector<std::wstring> wsplit(std::wstring string, std::wstring character, int limit)
{
	std::vector<std::wstring> array;
	int found;
	int k = 0;
	while ( (found=string.find_first_of(character)) != string.npos)
	{
		if(found!=0)
		{
			array.push_back(string.substr(0, found));
			k++;
		}
		string = string.substr(found+1);
		if(limit && k==limit)
		{
			break;
		}
	}
	if (string.length() > 0)
	{
		array.push_back(string);
	}
	return array;
}

std::vector<std::string> split(std::string string, std::string character, int limit)
{
	std::vector<std::string> array;
	int found;
	int k = 0;
	while ( (found=string.find_first_of(character)) != string.npos)
	{
		if(found!=0)
		{
			array.push_back(string.substr(0, found));
			k++;
		}
		string = string.substr(found+1);
		if(limit && k==limit)
		{
			break;
		}
	}
	if (string.length() > 0)
	{
		array.push_back(string);
	}
	return array;
}

std::wstring rtrim(std::wstring string)
{
	boost::algorithm::trim_right(string);
	return string;
}

std::string wtoa(std::wstring string)
{
	const wchar_t* wstr = string.c_str();
	char* ascii = new char[wcslen(wstr) + 1];
	wcstombs( ascii, wstr, wcslen(wstr) );
	ascii[wcslen(wstr)] = '\0';
	return ascii;
}

std::wstring atow(std::string string)
{
	const char* str = string.c_str();
	wchar_t* wstr  = new wchar_t[strlen(str) + 1];
	mbstowcs( wstr, str, strlen(str) );
	return wstr;
}

std::string itos(int number)
{
	std::stringstream ss;
	std::string str;
	ss << number;
	ss >> str;
	return str;
}

std::string itos(unsigned int number)
{
	std::stringstream ss;
	std::string str;
	ss << number;
	ss >> str;
	return str;
}

std::string itos(long long number)
{
	std::stringstream ss;
	std::string str;
	ss << number;
	ss >> str;
	return str;
}

std::string itos(unsigned long long number)
{
	std::stringstream ss;
	std::string str;
	ss << number;
	ss >> str;
	return str;
}

int stoi(std::string string)
{
	std::stringstream ss;
	ss << string;
	int k;
	ss >> k;
	return k;
}

// funkcja pomocnicza do operator+
int pow10(int k)
{
	int ret = 1;
	while (k > 0)
	{
		ret *= 10;
		k--;
	}
	return ret;
}

std::wstring operator+(std::wstring aString, int k)
{
	if (k < 0)
	{
		k = -k;
		aString += L"-" + k; // rekurencja
	}
	else if (k < 10)
	{
		wchar_t cstr[2];
		cstr[0] = wchar_t(int('0') + k);
		cstr[1] = '\0';
		aString += cstr;
	}
	else
	{
		// pow=(liczba cyfr w k)
		int pow = 0;
		for (int p10 = 1; p10 <= k; p10 *= 10)
			pow++;
		int cyfr = pow;
		// jako że ostatnie dzielenie ma być przez 1, czyli dla zera to:
		pow--;
		wchar_t* c_str = new wchar_t[cyfr + 1];
		while (pow >= 0)
		{
			int tmp = int(k / pow10(pow)) * pow10(pow);
			k -= tmp;
			int digit = tmp / pow10(pow);
			c_str[cyfr - pow - 1] = wchar_t(int('0') + digit);
			pow--;
		}
		c_str[cyfr] = '\0';
		aString += c_str;
		delete[] c_str;
	}
	return aString;
}

std::string operator+(std::string aString, int k)
{
	if (k < 0)
	{
		k = -k;
		aString += "-" + k;
	}
	else if (k < 10)
	{
		char cstr[2];
		cstr[0] = char(int('0') + k);
		cstr[1] = '\0';
		aString += cstr;
	}
	else
	{
		// pow=(liczba cyfr w k)
		int pow = 0;
		for (int p10 = 1; p10 <= k; p10 *= 10)
			pow++;
		int cyfr = pow;
		// jako że ostatnie dzielenie ma być przez 1, czyli dla zera to:
		pow--;
		char* c_str = new char[cyfr + 1];
		while (pow >= 0)
		{
			int tmp = int(k / pow10(pow)) * pow10(pow);
			k -= tmp;
			int digit = tmp / pow10(pow);
			c_str[cyfr - pow - 1] = char(int('0') + digit);
			pow--;
		}
		c_str[cyfr] = '\0';
		aString += c_str;
		delete[] c_str;
	}
	return aString;
}
