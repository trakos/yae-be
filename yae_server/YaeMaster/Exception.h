/*
 * YaeMaster/Exception.h
 *
 *  Created on: May 18, 2011
 *      Author: trakos
 */

#ifndef YAEMASTER_EXCEPTION_H_
#define YAEMASTER_EXCEPTION_H_

#include <stdexcept>

class YaeMaster_Exception : public std::runtime_error
{
	public:
		YaeMaster_Exception(std::string& what) : std::runtime_error(what) {};
};

#endif /* YAEMASTER_EXCEPTION_H_ */
