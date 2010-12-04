/*
 * utils.cpp
 *
 *  Created on: 2010-12-03
 *      Author: trakos
 */

#include <utils.h>
/*
#include <indent.h>
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
}*/

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

template std::wostream& operator <<(std::wostream& stream, std::vector<std::wstring> &vector);

#include <boost/algorithm/string.hpp>
#include <boost/lambda/lambda.hpp>
#include <vector>
#include <string>

std::vector<std::wstring> wsplit(std::wstring string, std::wstring character)
{
	std::vector<std::wstring> array;
	boost::split(array, string, boost::is_any_of(character), boost::token_compress_on);
	return array;
}
