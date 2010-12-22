/*
 * Exception.h
 *
 *  Created on: 2011-01-25
 *      Author: trakos
 */

#ifndef TNET_MESSAGE_BEING_RECEIVED_EXCEPTION_H_
#define TNET_MESSAGE_BEING_RECEIVED_EXCEPTION_H_

#include <string>
#include <Tnet/ProtocolException.h>

class Tnet_MessageBeingReceivedException : public Tnet_ProtocolException
{
	public:
		Tnet_MessageBeingReceivedException() : Tnet_ProtocolException("Tnet_Message being currently receiving, broke by usage of other receiving methods in between") {};
};

#endif /* TNET_MESSAGE_BEING_RECEIVED_EXCEPTION_H_ */
