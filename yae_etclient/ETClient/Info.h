/*
 * ETClient/Info.h
 *
 *  Created on: 2010-11-26
 *      Author: trakos
 */



#ifndef ET_CLIENT_INFO_H_
#define ET_CLIENT_INFO_H_

#include <string>

#include <ETClient/Status.h>

class ETClientInfo
{
	protected:
		ETClientInfo();
		static ETClientInfo instance;
		std::wstring localPlayerName();
		ETServerW serverInfo();
		ETPlayersW playersInfo(bool& success,bool& online,bool& slac,bool& etpro,bool& pb);
		ETPlayer ETPlayerWToETPlayer(ETPlayerW etplayerw);
	public:
		static ETClientInfo& getInstance();
		int waitingForETCommandTime;
		int pauseAfterCommand;
		ETClientStatus getStatus( bool echoProgress=false );
		ETClientStatusW getStatusW( bool echoProgress=false );
		bool isPlaying();
		std::wstring getVariableValue(std::wstring name, bool& success);
};

#endif /* ET_CLIENT_INFO_H_ */
