/*
 * ETClient/Input.h
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#ifndef ET_CLIENT_INPUT_H_
#define ET_CLIENT_INPUT_H_

#include<string>

class ETClientInput
{
protected:
	ETClientInput();
	static ETClientInput instance;
public:
	static ETClientInput& getInstance();
	bool shortMessage(std::wstring message,std::wstring nocolor=L"",std::wstring color=L"");
	//bool multiLineMessage(std::wstring message);
};

#endif /* ET_CLIENT_INPUT_H_ */
