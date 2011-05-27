#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <iostream>

#include <Tnet/Server.h>
#include <Tnet/Client.h>
#include <Tnet/Message.h>
#include <Tlogger/Front.h>
#include <ET/Status.h>
#include <ET/Server.h>
#include <utils.h>
#include <YaeMaster/Database.h>
#include <YaeMaster/Protocol.h>
#include <YaeMaster.h>
#include <Tmysql/Connection.h>

#define YAETIMEOUT 5000

using std::cout;
using std::endl;

bool running = true;
int currently_checking = 0;
Tnet_Server* yaeServer = NULL;

void* processClient( void *data )
{
	currently_checking++;
	Tnet_Connection* yaeClient = (Tnet_Connection*)data;
	cout << "checking out new client, "+yaeClient->getIp()+":"+yaeClient->getPort()+"..." << endl;
	try
	{
		Tnet_Message message = yaeClient->receive(YAETIMEOUT);
		YaeMaster_Protocol_Action action = YaeMaster_Protocol::getInstance().getAction(message.strings["action"]);
		Tmysql_LiveRow user = YaeMaster_Database::getInstance().authenticate(message.strings["login"], message.strings["password"]);
		if ( !user.isRowNull() && action!=CHECK_AUTH && ::stoi(user.get("last_query"))+10 >= time(0) )
		{
			user.set("last_query", (unsigned int)time(0));
			message.clear();
			message.strings["status"] = "toofast";
		}
		else if ( !user.isRowNull() )
		{
			user.set("last_query", (unsigned int)time(0));
			message.clear();
			message.strings["status"] = "ok";
			message.strings["authStatus"] = "ok";
			switch(action)
			{
				case ONLINE_WITHOUT_ET:
				case ONLINE_WITH_ET_NOT_ON_SERVER:
				{
					YaeMaster_Database::getInstance().userOnlineWithoutET(user);
					yaeClient->send(message);
					break;
				}
				case ONLINE_WITH_ET:
				case YAE_SEARCH:
				{
					yaeClient->send(message);
					ET_Status status = YaeMaster_Protocol::getInstance().receiveETClientStatus(yaeClient);
					std::cout << status << std::endl;
					try
					{
						status = YaeMaster::getInstance().verifyData(status);
					}
					catch (ET_Server_Exception exception)
					{
						YaeMaster_Protocol::getInstance().sendSimpleStatus(yaeClient, "bad");
						throw exception;
					}
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
			sleep(1);
		}
		yaeClient->send(message);
	}
	catch (YaeMaster_Exception exception)
	{
		Tnet_Message message;
		message.strings["status"] = "error";
		yaeClient->send(message);
		LOG(exception.what(), LERR);
	}
	catch (Tnet_Exception exception)
	{
		LOG(exception.what(), LERR);
	}
	catch (ET_Server_Exception exception)
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
	LOG.couting = LSDBG;

	Tmysql_Connection::defineInstance("dawn", "localhost", "dawn", "***REMOVED***", "dawn", 0);
	Tmysql_Connection::getInstance("dawn").setAutoReconnect(true);
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

int _main()
{
	LOG.couting = LSDBG;
	IndentFacet::initialize();
	//ET_Server server("213.108.31.35", 27960);
	ET_Server server("localhost", 27960);
	server.getPublicStatus();
	return 0;
}
