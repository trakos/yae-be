/*
 * ETClient/Listener.h
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#ifndef ET_CLIENT_LISTENER_H_
#define ET_CLIENT_LISTENER_H_

#include<string>

class ETClientListener
{
protected:
	ETClientListener();
	static ETClientListener instance;
public:
	static ETClientListener& getInstance();
	std::wstring getCommandW();
	std::string getCommand();
};

#endif /* ET_CLIENT_LISTENER_H_ */
