/*
 * Tmysql/Connection.h
 *
 *  Created on: 2011-05-09
 *      Author: trakos
 */

#ifndef TMYSQL_CONNECTION_H_
#define TMYSQL_CONNECTION_H_

#include <string>
#include <map>
#include <mysql/mysql.h>

#include <Tmysql/LiveRow.h>
#include <Tmysql/String.h>

typedef std::vector<Tmysql_String> Tmysql_Arguments;

struct Tmysql_Connection_InstanceDefinition
{
		const char* host;
		const char* user;
		const char* passwd;
		const char* db;
		unsigned int port;
};

class Tmysql_Connection
{
	protected:
		static std::map< std::string, Tmysql_Connection_InstanceDefinition > instancesDefinitions;
		static std::map< std::string, Tmysql_Connection* > instances;
		std::string instanceName;
		MYSQL* connection;
		Tmysql_Connection( std::string name, Tmysql_Connection_InstanceDefinition instanceDefinition );
		~Tmysql_Connection();
		MYSQL_RES* getQueryResultBase( std::string query );
		MYSQL_RES* getQueryResult( std::string query, Tmysql_Arguments &arguments );
	public:
		static void defineInstance( std::string instanceName, std::string host, std::string user, std::string passwd, std::string db, unsigned int port=0 );
		static void reconnectInstance( std::string instanceName );
		static Tmysql_Connection& getInstance( std::string instanceName );
		Tmysql_LiveRow fetchLiveRow( std::string tableName, std::string where="" );
		Tmysql_LiveRow fetchLiveRow( std::string tableName, std::string where, Tmysql_Arguments &arguments );
		Tmysql_LiveRowSet fetchLiveRowSet( std::string tableName, std::string where="" );
		Tmysql_LiveRowSet fetchLiveRowSet( std::string tableName, std::string where, Tmysql_Arguments &arguments );
		Tmysql_Row fetchRow( std::string query );
		Tmysql_Row fetchRow( std::string query, Tmysql_Arguments &arguments );
		Tmysql_RowSet fetchRowSet( std::string query );
		Tmysql_RowSet fetchRowSet( std::string query, Tmysql_Arguments &arguments );
		unsigned long long query( std::string query, Tmysql_Arguments &arguments );
		unsigned long long getLastInsertId( );
		std::string escape( const Tmysql_String& );
		void ping();
		void setAutoReconnect( bool useAutoReconnect );
		std::string getQueryString( std::string query, Tmysql_Arguments &arguments );
};

#endif /* TMYSQL_CONNECTION_H_ */
