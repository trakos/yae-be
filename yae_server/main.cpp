#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <iostream>

#include <Tnet/Server.h>
#include <Tnet/Client.h>
#include <Tnet/Message.h>
#include <Tlogger/Front.h>
#include <ET/Client/Status.h>
#include <indent.h>
#include <YaeMaster/Database.h>
#include <YaeMaster/Protocol.h>
#include <Tmysql/Connection.h>

#define YAETIMEOUT 50

using std::cout;
using std::endl;

bool running = true;
int currently_checking = 0;
Tnet_Server* yaeServer = NULL;

void* processClient( void *data )
{
	currently_checking++;
	cout << "checking out new client..." << endl;
	Tnet_Client* yaeClient = (Tnet_Client*)data;
	try
	{
		Tnet_Message message = yaeClient->receive(YAETIMEOUT);
		cout << message << endl;
		YaeMaster_Protocol_Action action = YaeMaster_Protocol::getInstance().getAction(message.strings["action"]);
		Tmysql_LiveRow user = YaeMaster_Database::getInstance().authenticate(message.strings["login"], message.strings["password"]);
		if ( !user.isRowNull() )
		{
			message.clear();
			message.strings["status"] = "ok";
			message.strings["authStatus"] = "ok";
			switch(action)
			{
				case ONLINE_WITHOUT_ET:
				{
					YaeMaster_Database::getInstance().userOnlineWithoutET(user);
					yaeClient->send(message);
					break;
				}
				case ONLINE_WITH_ET:
				case YAE_SEARCH:
				{
					yaeClient->send(message);
					ETClientStatus status = YaeMaster_Protocol::getInstance().receiveETClientStatus(yaeClient);
					switch(action)
					{
						case ONLINE_WITH_ET:
						{
							YaeMaster_Database::getInstance().userOnlineWithET(user,status);
							YaeMaster_Protocol::getInstance().sendSimpleStatus(yaeClient, "ok");
							break;
						}
						case YAE_SEARCH:
						{
							message.clear();
							message = YaeMaster_Database::getInstance().performYaeSearch(status);
							message.strings["status"] = "ok";
							yaeClient->send(message);
							break;
						}
					}
					break;
				}
				case CHECK_AUTH:
				{
					yaeClient->send(message);
					break;
				}
				default:
				{
					message.clear();
					message.strings["status"] = "unkown_action";
					yaeClient->send(message);
					break;
				}
			}
		}
		else
		{
			message.clear();
			message.strings["status"] = "ok";
			message.strings["authStatus"] = "bad";
		}
		cout << message << endl;
		yaeClient->send(message);
	}
	catch (YaeMaster_Exception exception)
	{
		Tnet_Message message;
		message.strings["status"] = "error";
		yaeClient->send(message);
		cout << message << endl;
		LOG(exception.what(), LERR);
	}
	catch (Tnet_Exception exception)
	{
		LOG(exception.what(), LERR);
	}
	delete yaeClient;
	cout << "client finished." << endl;
	currently_checking--;
}

int main()
{
	IndentFacet::initialize();
	Tmysql_Connection::defineInstance("dawn", "localhost", "dawn", "***REMOVED***", "dawn", 0);
	Tmysql_Connection::getInstance("dawn").setAutoReconnect(true);
	LOG.couting = LSDBG;
	// we are not joining pthreads (i.e. waiting for them)
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	yaeServer = new Tnet_Server(1743);
	// main loop
	while(running)
	{
		if ( yaeServer->isNewConnectionAvailable(10) )
		{
			Tnet_Connection* connection = yaeServer->getNewConnection();
			pthread_t* id;
			processClient((void*)connection);
			//pthread_create( id, &attr, processClient, (void*)connection);
		}
	}
	while(currently_checking)
	{
		sleep(1);
	}
	pthread_attr_destroy(&attr);
	pthread_exit(0);
}
