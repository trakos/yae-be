/*
 * ETClient/Info.h
 *
 *  Created on: 2010-11-26
 *      Author: trakos
 */



#ifndef ET_CLIENT_INFO_H_
#define ET_CLIENT_INFO_H_

#include <string>

#include <ET/Client/Status.h>

class ET_Client_Info
{
	protected:
		ET_Client_Info();
		static ET_Client_Info instance;
		std::wstring localPlayerName();
		ET_Client_Status_Server_W serverInfo();
		ET_Client_Status_Players_W playersInfo(bool& success,bool& online,bool& slac,bool& etpro,bool& pb);
		ET_Client_Status_Player ETPlayerWToETPlayer(ET_Client_Status_Player_W etplayerw);
	public:
		static ET_Client_Info& getInstance();
		int waitingForETCommandTime;
		int pauseAfterCommand;
		ET_Client_Status getStatus( bool echoProgress=false );
		ET_Client_Status_W getStatusW( bool echoProgress=false );
		bool isPlaying();
		std::wstring getVariableValue(std::wstring name, bool& success);
};

#endif /* ET_CLIENT_INFO_H_ */
