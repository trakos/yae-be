/*
 * main.cpp
 *
 *  Created on: 2010-11-23
 *      Author: trakos
 */
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <initializer_list>
#include <utils.h>
#include <ETClient/Console.h>
#include <ETClient/Input.h>
#include <ETClient/Listener.h>
#include <ETClient/Info.h>
#include <Window/AuthDialog.h>
#include <Logger/Logger.h>
#include <Tnet/Server.h>
#include <Tnet/Client.h>
#include <indent.h>

using std::wstring;
using std::string;
using std::wcout;
using std::cout;
using std::endl;

typedef std::vector< string > array;
typedef std::vector< std::vector < string > > array2D;

int main()
{
	//
	IndentFacet::initialize();
	Tnet_Server* server = new Tnet_Server(27960);
	Tnet_Client* client = new Tnet_Client("127.0.0.1", 27960);
	Tnet_Connection* conn = server->getNewConnection();
	Tnet_Message message;
	message.ints["test"] = 1907;
	message.strings["lorememem ipsumeme"] = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Donec ut velit nec purus faucibus pretium ornare at lorem. Aliquam erat volutpat. Sed posuere accumsan eros, ut bibendum massa vestibulum ut. Maecenas non diam sed massa consectetur sodales. Nam purus est, lacinia in venenatis in, ultrices vitae ligula. Quisque sagittis feugiat dolor a dapibus. Integer sed urna a nulla tincidunt viverra at eu nibh. Nam non dui est, in hendrerit purus. Mauris at bibendum dui. Integer vitae ligula turpis, vulputate malesuada justo. Aliquam nec ipsum tempus velit condimentum pharetra sit amet vel augue. In faucibus risus vel massa feugiat dapibus a non urna. Fusce pellentesque posuere enim, a porttitor neque viverra ac. Mauris interdum purus quis lectus facilisis placerat. Aliquam quis risus quis dui adipiscing ornare. Nunc congue, est at mattis iaculis, lectus nisl commodo nibh, sit amet adipiscing risus nunc at eros. \
		Nullam viverra eros id eros pharetra ac placerat tellus malesuada. Aliquam dictum ligula ut nunc tempor eget pretium metus sodales. Nulla facilisi. Aenean a elit non justo aliquam viverra. Nunc semper semper mauris, nec sodales justo pharetra vitae. Pellentesque dictum malesuada viverra. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Mauris ornare diam felis, at eleifend sem. Aenean lobortis urna vel metus viverra commodo. Vivamus eget ullamcorper nulla. Ut feugiat scelerisque risus id vulputate. Vivamus lobortis est vitae diam elementum pellentesque. \
		Cras lobortis turpis et tortor tristique vel viverra magna condimentum. Morbi consequat lacus in lectus auctor blandit. Cras auctor mattis nibh. Nunc urna tellus, pellentesque vitae iaculis ac, elementum quis elit. Morbi interdum dictum dictum. Integer ac massa libero, eu pretium orci. Vestibulum bibendum, lorem varius faucibus consectetur, quam est faucibus erat, vel euismod felis quam vel felis. Nulla in odio lorem. Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Vivamus ullamcorper, velit laoreet ultricies consequat, ligula dui vulputate arcu, ut venenatis quam libero in elit. Mauris a risus erat. Phasellus non urna eget lectus fringilla tempor id in orci. \
		Nulla sit amet nulla nec nunc volutpat pharetra feugiat nec nibh. Maecenas bibendum venenatis lorem, non tristique neque tincidunt in. Maecenas non mauris lectus, sit amet blandit tellus. Aliquam ipsum est, bibendum eget pharetra adipiscing, semper eu libero. In varius, magna eget placerat porta, nulla arcu faucibus odio, quis pellentesque turpis massa sed erat. Quisque in nunc eget tellus viverra porta. Phasellus sed odio ante. Etiam et velit leo, suscipit hendrerit nunc. Duis mollis magna mollis magna molestie vel gravida dui facilisis. Donec id leo velit. In hac habitasse platea dictumst. Ut sit amet lectus nec ipsum tempor sodales quis in nulla. Nunc massa justo, luctus sed laoreet rutrum, tempus et mauris. In vitae augue et eros sollicitudin aliquet. Integer non nunc purus. Nullam pellentesque sem a est fermentum gravida. Aliquam erat volutpat. Mauris fermentum, enim vel bibendum molestie, neque metus scelerisque felis, quis bibendum justo leo ac nisi. Curabitur sagittis gravida leo, a rutrum nisl elementum non. Pellentesque egestas semper ligula, sed euismod est commodo vitae. \
		Etiam vitae leo sit amet nisl viverra elementum. Maecenas pretium sollicitudin purus vel ultricies. Maecenas vel velit velit, eget vehicula elit. Ut eu lacinia lacus. Praesent est elit, tempus eu adipiscing sodales, sodales a risus. Etiam at lorem lorem. Mauris imperdiet pharetra viverra. Ut posuere arcu diam, nec blandit augue. Duis enim odio, eleifend eget bibendum sit amet, commodo in lorem. Duis ultrices rhoncus magna quis tincidunt. Sed lobortis vehicula sem nec elementum. Maecenas accumsan iaculis orci, sed faucibus ipsum laoreet at. Etiam lectus metus, mattis eget varius pellentesque, blandit eget metus. Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nunc fringilla porttitor eros sed porttitor. Duis dignissim dictum sagittis. Donec molestie molestie ornare. Aliquam tristique elementum massa eget tempus. Phasellus molestie aliquet sapien in rhoncus. ";
	client->send(message);
	array test = { "hehe", "hehe2", "lolol" };
	cout << message << endl;
	Tnet_Message reply = conn->receive();
	cout << reply.strings["test"] << endl;
	return 0;
	conn->sendString("hello world!");
	std::string text = client->receiveString(1);
	std::string text2 = client->receiveString(3);
	cout <<  text.size() << ":" << text << endl;
	return 0;
	WindowAuthDialogReturn data = WindowAuthDialog::getInstance().ask();
	wcout << data.success << "\t" << data.login << " " << data.password << endl;
	//showWindow();
	//MessageBox(NULL, "Goodbye, cruel world!", "Note", MB_OK);
	return 0;
	LOG.couting = LSDBG;
	LOG("",LSDBG);
	int k = MessageBox(NULL, "Za mało pamięci RAM", "My application", MB_OKCANCEL | MB_DEFBUTTON2 | MB_ICONWARNING | MB_SYSTEMMODAL);
	wcout << k << endl;
	return 0;
	ETClientConsole::getInstance().toggleWindow(true);
	ETClientConsole::getInstance().moveToTheEnd();
	ETClientStatus status = ETClientInfo::getInstance().getStatus();
	return 0;
	while(1)
	{
		wstring str = ETClientListener::getInstance().getCommand();
		if(str!=L"")
		{
			wcout << 'p' << str << 'e' << endl;
		}
		Sleep(100);
	}


	ETClientConsole etconsole = ETClientConsole::getInstance();
	etconsole.sendMessage(L"echo lol");
	while(!etconsole.isConsoleAttached())
	{
		etconsole.attachConsole();
		fflush(stdout);
		Sleep(1000);
	}
	while(1)
	{
		wstring lastLine = etconsole.getLastLine();
		if(!lastLine.empty())
		{
			wcout << lastLine << std::flush;
		}
	}
}
