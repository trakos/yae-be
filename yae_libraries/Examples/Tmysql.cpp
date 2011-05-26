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
#include <utils.h>

int main()
{
	IndentFacet::initialize();
	Tmysql_Connection::defineInstance("trakos", "localhost", "root", "***REMOVED***", "dawn", 0);

	std::string someString = "\"\"hehe\"\"@!:D";
	std::cout << someString << std::endl;
	std::cout << Tmysql_Connection::getInstance("trakos").escape(someString) << std::endl;

	Tmysql_Arguments arguments = { "arg1", "arg2" };
	std::string query = "SELECT * FROM tableName WHERE a1=? OR a2=?";
	std::cout << Tmysql_Connection::getInstance("trakos").getQueryString(query, arguments) << std::endl;

	Tmysql_Connection::getInstance("trakos").setAutoReconnect(true);
	Tmysql_Arguments args = { (Tmysql_Expression)"NOW()", "x\\'" };
	Tmysql_LiveRow server = Tmysql_Connection::getInstance("trakos").fetchLiveRow("servers", "WHERE id=? AND id=?", args);
	//std::cout << server << std::endl;
	//server.set("connectionfails", "30");
	std::cout << server << std::endl;
	//server.save();
}
