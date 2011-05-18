/*
 * YaeMaster/Protocol.h
 *
 *  Created on: May 18, 2011
 *      Author: trakos
 */

#ifndef YAEMASTER_PROTOCOL_H
#define YAEMASTER_PROTOCOL_H

#include <string>

#include <ETClient/Status.h>
#include <Tnet/Connection.h>
#include <YaeMaster/Exception.h>

enum YaeMaster_Protocol_Action { CHECK_AUTH, ONLINE_WITHOUT_ET, ONLINE_WITH_ET, YAE_SEARCH };

class YaeMaster_Protocol
{
	protected:
		YaeMaster_Protocol();
		static YaeMaster_Protocol instance;
	public:
		static YaeMaster_Protocol& getInstance();
		ETClientStatus receiveETClientStatus( Tnet_Connection* connection );
		YaeMaster_Protocol_Action getAction(std::string actionName);
		void sendSimpleStatus(Tnet_Connection* connection,std::string status="ok");
};

#endif /* YAEMASTER_PROTOCOL_H */
