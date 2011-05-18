/*
 * Communication/Yae/Master.h
 *
 *  Created on: 2011-05-08
 *      Author: trakos
 */

#ifndef COMMUNICATION_YAE_MASTER_H_
#define COMMUNICATION_YAE_MASTER_H_

#include<string>
#include<Tnet/Message.h>
#include<Tnet/Connection.h>
#include<Communication/Yae/Authorization.h>
#include<ET/Client/Info.h>

class Communication_Yae_Master
{
	protected:
		static std::string masterIP;
		static int masterPort;
		static unsigned int timeout;
		static Communication_Yae_Master instance;
		static int version;
		Tnet_Connection* connection;
		Communication_Yae_Master();
		void connect();
		void reconnect();
		void disconnect();
		Tnet_Message receiveSimpleStatusPacket();
		Tnet_Message sendAuthMessage(Tnet_Message message);
		Tnet_Message sendMessage(Tnet_Message message);
		Tnet_Message sendYaeClientData(ET_Client_Status data);

	public:
		static Communication_Yae_Master& getInstance();
		~Communication_Yae_Master();
		Communication_Yae_CredentialsCorrectness areCredentialsCorrect(std::string login, std::string password);
		void onlineWithET(ET_Client_Status data);
		void onlineWithoutET();
		Tnet_Message performYaeSearch(ET_Client_Status data);
};

#endif /* COMMUNICATION_YAE_MASTER_H_ */
