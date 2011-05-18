/*
 * Communication/Yae/Server.h
 *
 *  Created on: 2011-05-08
 *      Author: trakos
 */

#ifndef COMMUNICATION_YAE_SERVER_H_
#define COMMUNICATION_YAE_SERVER_H_

#include<string>
#include<Tnet/Message.h>
#include<Tnet/Connection.h>
#include<Communication/Yae/Authorization.h>
#include<ETClient/Info.h>

class Communication_Yae_Server
{
	protected:
		static std::string masterIP;
		static int masterPort;
		static unsigned int timeout;
		static Communication_Yae_Server instance;
		static int version;
		Tnet_Connection* connection;
		Communication_Yae_Server();
		void connect();
		void reconnect();
		void disconnect();
		Tnet_Message receiveSimpleStatusPacket();
		Tnet_Message sendAuthMessage(Tnet_Message message);
		Tnet_Message sendMessage(Tnet_Message message);
		Tnet_Message sendYaeClientData(ETClientStatus data);

	public:
		static Communication_Yae_Server& getInstance();
		~Communication_Yae_Server();
		Communication_Yae_CredentialsCorrectness areCredentialsCorrect(std::string login, std::string password);
		void onlineWithET(ETClientStatus data);
		void onlineWithoutET();
		Tnet_Message performYaeSearch(ETClientStatus data);
};

#endif /* COMMUNICATION_YAE_SERVER_H_ */
