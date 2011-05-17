/*
 * utils.cpp
 *
 *  Created on: 2010-12-03
 *      Author: trakos
 */

#include <utils.h>
#include <indent.h>
#include <iomanip>

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

std::wstring rtrim(std::wstring string)
{
	boost::algorithm::trim_right(string);
	return string;
}

std::string wtoa(std::wstring string)
{
	const wchar_t* wstr = string.c_str();
	int stringLength = wcslen(wstr);
	char* ascii = new char[stringLength + 1];
	wcstombs( ascii, wstr, stringLength+1 );
	return ascii;
}

std::wstring atow(std::string string)
{
	const char* str = string.c_str();
	wchar_t* wstr  = new wchar_t[strlen(str) + 1];
	mbstowcs( wstr, str, strlen(str) );
	return wstr;
}
