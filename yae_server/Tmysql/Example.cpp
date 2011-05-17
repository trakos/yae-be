/*
 * Tmysql/Example.cpp
 *
 *  Created on: May 10, 2011
 *      Author: trakos
 */

#include <iostream>
#include <initializer_list>

#include <Tmysql/String.h>
#include <Tmysql/Connection.h>
#include <indent.h>

int main()
{
	IndentFacet::initialize();
	Tmysql_Connection::defineInstance("trakos", "localhost", "root", "***REMOVED***", "dawn", 0);

	Tmysql_String myString = "czesc";
	myString = "czesc";
	std::cout << "p" << myString << "e" << std::endl;
	std::string someString = "\"\"hehe\"\"@!:D";
	std::cout << "p" << someString << "e" << std::endl;
	std::cout << someString << std::endl;
	std::cout << Tmysql_Connection::getInstance("trakos").escape(someString) << std::endl;
	std::vector<std::string> arguments = { "arg1", "arg2" };
	std::string query = "SELECT * FROM tableName WHERE a1=? OR a2=?";
	std::cout << Tmysql_Connection::getInstance("trakos").getQueryString(query, arguments) << std::endl;

	Tmysql_Connection::getInstance("trakos").setAutoReconnect(true);
	Tmysql_Arguments args = {"1"};
	Tmysql_LiveRow server = Tmysql_Connection::getInstance("trakos").fetchLiveRow("servers", "WHERE id=?", args);
	//std::cout << server << std::endl;
	//server.set("connectionfails", "30");
	std::cout << server << std::endl;
	//server.save();
}
