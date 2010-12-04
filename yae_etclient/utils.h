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

template<typename T> std::ostream& operator <<(std::ostream& stream, std::vector<T> &vector);
template<typename T> std::wostream& operator <<(std::wostream& stream, std::vector<T> &vector);
std::vector<std::wstring> wsplit(std::wstring string, std::wstring character);

#endif /* UTILS_H_ */
