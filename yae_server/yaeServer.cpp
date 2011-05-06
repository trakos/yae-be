#include <unistd.h>
#include <pthread.h>
#include <math.h>
#include <iostream>

#include <Tnet/Server.h>
#include <Tnet/Client.h>
#include <Tnet/Message.h>


using std::cout;
using std::endl;

bool running = true;
int currently_checking = 0;
Tnet_Server* yaeServer = NULL;

void* processClient( void *data )
{
	currently_checking++;
	try
	{
		Tnet_Client* yaeClient = (Tnet_Client*)data;
		Tnet_Message message = yaeClient->receive(50);
		yaeClient->send(message);
		cout << message << endl;
	}
	catch (Tnet_Exception exception)
	{
		throw exception;
	}
	currently_checking--;
}

int main()
{
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
