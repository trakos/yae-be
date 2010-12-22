/*
 * Exception.h
 *
 *  Created on: 2011-01-25
 *      Author: trakos
 */

#ifndef TNET_EXCEPTION_H_
#define TNET_EXCEPTION_H_

#include <string>
#include <stdexcept>

class Tnet_Exception : public std::runtime_error
{
	public:
		Tnet_Exception(std::string message) : std::runtime_error(message) {};
};

#endif /* TNET_EXCEPTION_H_ */
