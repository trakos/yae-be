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

#include <Tmysql/Exception.h>
#include <Tmysql/LiveRow.h>

struct Tmysql_Connection_InstanceDefinition
{
		const char* host;
		const char* user;
		const char* passwd;
		const char* db;
		unsigned int port;
};
typedef std::vector<std::string> Tmysql_Arguments;

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
		MYSQL_RES* getQueryResult( std::string query, std::vector<std::string> &arguments );
	public:
		static void defineInstance( std::string instanceName, std::string host, std::string user, std::string passwd, std::string db, unsigned int port=0 );
		static void reconnectInstance( std::string instanceName );
		static Tmysql_Connection& getInstance( std::string instanceName );
		Tmysql_LiveRow fetchLiveRow( std::string tableName, std::string where="" );
		Tmysql_LiveRow fetchLiveRow( std::string tableName, std::string where, std::vector< std::string > &arguments );
		Tmysql_LiveRowSet fetchLiveRowSet( std::string tableName, std::string where="" );
		Tmysql_LiveRowSet fetchLiveRowSet( std::string tableName, std::string where, std::vector< std::string > &arguments );
		Tmysql_Row fetchRow( std::string query );
		Tmysql_Row fetchRow( std::string query, std::vector< std::string > &arguments );
		Tmysql_RowSet fetchRowSet( std::string query );
		Tmysql_RowSet fetchRowSet( std::string query, std::vector< std::string > &arguments );
		unsigned long long query( std::string query, std::vector< std::string > &arguments );
		unsigned long long getLastInsertId( );
		std::string escape( std::string value );
		void ping();
		void setAutoReconnect( bool useAutoReconnect );
		std::string getQueryString( std::string query, std::vector<std::string> &arguments );
};

#endif /* TMYSQL_CONNECTION_H_ */
