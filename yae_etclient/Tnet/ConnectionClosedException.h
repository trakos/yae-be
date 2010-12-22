/*
 * Exception.h
 *
 *  Created on: 2011-01-25
 *      Author: trakos
 */

#ifndef TNET_CONNECTIONCLOSEDEXCEPTION_H_
#define TNET_CONNECTIONCLOSEDEXCEPTION_H_

#include <string>
#include <Tnet/Exception.h>

class Tnet_ConnectionClosedException : public Tnet_Exception
{
	public:
		Tnet_ConnectionClosedException() : Tnet_Exception("connection closed") {};
};

#endif /* TNET_CONNECTIONCLOSEDEXCEPTION_H_ */
