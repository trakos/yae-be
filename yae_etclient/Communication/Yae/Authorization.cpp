/*
 * ETClient/Input.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <stdlib.h>

#include <Communication/Yae/Authorization.h>
#include <Communication/Yae/Master.h>
#include <Tlogger/Front.h>
#include <Window/AuthDialog.h>

Communication_Yae_Authorization Communication_Yae_Authorization::instance = Communication_Yae_Authorization();

Communication_Yae_Authorization::Communication_Yae_Authorization()
{
	this->currentState = NOCREDENTIALS;
	this->currentCredentials.login = "";
	this->currentCredentials.password = "";
}

Communication_Yae_Authorization& Communication_Yae_Authorization::getInstance()
{
	return Communication_Yae_Authorization::instance;
}

void Communication_Yae_Authorization::setState( Communication_Yae_CredentialsCorrectness newState )
{
	if ( newState == NO )
	{
		LOG("User cancelled credentials window, exiting...", LWARN);
		exit(1743);
	}
	else
	{
		LOG((std::string)"Current authorization status is: " + ( newState==YES ? "authorized" : ( newState==NOCREDENTIALS ? "no credentials given" : "offline" ) ), LNOTE );
	}
	this->currentState = newState;
}

Communication_Yae_CredentialsCorrectness Communication_Yae_Authorization::getCurrentState()
{
	return this->currentState;
}

Communication_Yae_Credentials Communication_Yae_Authorization::getCurrentCredentials()
{
	if ( this->currentState == NOCREDENTIALS || this->currentState == OFFLINE )
	{
		bool first = true;
		while ( 1 )
		{
			if ( this->currentState == NOCREDENTIALS )
			{
				Window_AuthDialogReturn authData = Window_AuthDialog::getInstance().ask(first ? "" : "login or password incorrect", this->currentCredentials.login, this->currentCredentials.password);
				first = false;
				if ( authData.success == false )
				{
					this->setState(NO);
					return this->currentCredentials;
				}
				this->currentCredentials.login = wtoa(authData.login);
				this->currentCredentials.password = wtoa(authData.password);
			}
			Communication_Yae_CredentialsCorrectness state = Communication_Yae_Master::getInstance().areCredentialsCorrect(this->currentCredentials.login, this->currentCredentials.password);
			if ( state == YES || state == OFFLINE )
			{
				// OFFLINE or YES (i.e. good credentials) - nothing more to do.
				this->setState(state);
				return this->currentCredentials;
			}
			else if ( state == NO && this->currentState == OFFLINE )
			{
				this->setState(NOCREDENTIALS);
			}
			// NO - try again! here we go loop!
		}
	}
	// YES - good credentials.
	// NO - it' not possible ATM, but anyway, there's nothing to do for this state.
	return this->currentCredentials;
}

void Communication_Yae_Authorization::setCredentials( std::string login, std::string password )
{
	this->currentCredentials.login = login;
	this->currentCredentials.password = password;
}

