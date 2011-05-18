/*
 * Exception.h
 *
 *  Created on: 2011-01-25
 *      Author: trakos
 */

#ifndef TNET_PROTOCOLEXCEPTION_H_
#define TNET_PROTOCOLEXCEPTION_H_

#include <string>
#include <Tnet/Exception.h>

class Tnet_ProtocolException : public Tnet_Exception
{
	public:
		Tnet_ProtocolException() : Tnet_Exception("protocol error") {};
	protected:
		Tnet_ProtocolException(std::string message) : Tnet_Exception( message ) {};
};

#endif /* TNET_PROTOCOLEXCEPTION_H_ */
