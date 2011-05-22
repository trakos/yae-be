/*
 * ETClient/Listener.h
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#ifndef ET_CLIENT_LISTENER_H_
#define ET_CLIENT_LISTENER_H_

#include<string>

class ET_Client_Listener
{
protected:
	ET_Client_Listener();
	static ET_Client_Listener* instance;
public:
	static ET_Client_Listener& getInstance();
	std::wstring getCommandW();
	std::string getCommand();
};

#endif /* ET_CLIENT_LISTENER_H_ */
