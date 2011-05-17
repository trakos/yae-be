/*
 * utils.h
 *
 *  Created on: 2010-12-03
 *      Author: trakos
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <vector>
#include <iostream>
#include <string>
#include <map>


template<typename T> std::ostream& operator <<(std::ostream& stream, std::map<std::string, T> const &map);
template<typename T> std::ostream& operator <<(std::ostream& stream, std::vector<T> &vector);
template<typename T> std::wostream& operator <<(std::wostream& stream, std::vector<T> &vector);
std::vector<std::wstring> wsplit(std::wstring string, std::wstring character, int limit=0);
std::wstring rtrim(std::wstring string);
std::string wtoa(std::wstring string);


#endif /* UTILS_H_ */
