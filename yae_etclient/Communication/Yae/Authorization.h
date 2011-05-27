/*
 * Communication/Yae/Authorization.h
 *
 *  Created on: 2011-05-08
 *      Author: trakos
 */

#ifndef COMMUNICATION_YAE_AUTHORIZATION_H_
#define COMMUNICATION_YAE_AUTHORIZATION_H_

#include<string>
#include<stdexcept>


enum Communication_Yae_CredentialsCorrectness { NOCREDENTIALS, OFFLINE, NO, YES };
struct Communication_Yae_Credentials
{
	std::string login;
	std::string password;
};

class Communication_Yae_Exception : public std::runtime_error
{
	public:
		Communication_Yae_Exception(const std::string &what) : std::runtime_error( what ) {};
};

class Communication_Yae_Authorization
{
	protected:
		static Communication_Yae_Authorization instance;
		Communication_Yae_Authorization();
		Communication_Yae_Credentials currentCredentials;
		Communication_Yae_CredentialsCorrectness currentState;

	public:
		static Communication_Yae_Authorization& getInstance();
		Communication_Yae_CredentialsCorrectness getCurrentState();
		void setState( Communication_Yae_CredentialsCorrectness state );
		void setCredentials( std::string login, std::string password );
		Communication_Yae_Credentials getCurrentCredentials();
};

#endif /* COMMUNICATION_YAE_AUTHORIZATION_H_ */
