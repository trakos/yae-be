/*
 * Exception.h
 *
 *  Created on: 2011-01-25
 *      Author: trakos
 */

#ifndef TNET_TIMEOUTEXCEPTION_H_
#define TNET_TIMEOUTEXCEPTION_H_

#include <string>
#include <Tnet/Exception.h>

class Tnet_TimeoutException : public Tnet_Exception
{
	public:
		Tnet_TimeoutException() : Tnet_Exception("timeout") {};
};

#endif /* TNET_TIMEOUTEXCEPTION_H_ */
