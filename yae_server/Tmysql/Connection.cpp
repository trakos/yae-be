/*
 * Tmysql/Connection.h
 *
 *  Created on: 2011-05-11
 *      Author: trakos
 */

#include <Tmysql/Connection.h>
#include <Logger/Logger.h>

#include <string>
#include <map>
#include <cstring>

std::map< std::string, Tmysql_Connection_InstanceDefinition > Tmysql_Connection::instancesDefinitions;
std::map< std::string, Tmysql_Connection* > Tmysql_Connection::instances;

void Tmysql_Connection::defineInstance( std::string instanceName, std::string host, std::string user, std::string passwd, std::string db, unsigned int port )
{
	Tmysql_Connection_InstanceDefinition instanceDefinition;
	if ( Tmysql_Connection::instancesDefinitions.count(instanceName) > 0 )
	{
		delete[] Tmysql_Connection::instancesDefinitions[instanceName].host;
		delete[] Tmysql_Connection::instancesDefinitions[instanceName].user;
		delete[] Tmysql_Connection::instancesDefinitions[instanceName].passwd;
		delete[] Tmysql_Connection::instancesDefinitions[instanceName].db;
	}
	instanceDefinition.host = new char[host.size()+1];
	strncpy( (char*)instanceDefinition.host, host.c_str(), host.size()+1 );
	instanceDefinition.user = new char[user.size()+1];
	strncpy( (char*)instanceDefinition.user, user.c_str(), user.size()+1 );
	instanceDefinition.passwd = new char[passwd.size()+1];
	strncpy( (char*)instanceDefinition.passwd, passwd.c_str(), passwd.size()+1 );
	instanceDefinition.db = new char[db.size()+1];
	strncpy( (char*)instanceDefinition.db, db.c_str(), db.size()+1 );
	instanceDefinition.port = port;
	Tmysql_Connection::instancesDefinitions[instanceName] = instanceDefinition;
}

void Tmysql_Connection::reconnectInstance( std::string instanceName )
{
	if ( !Tmysql_Connection::instancesDefinitions.count(instanceName) )
	{
		LOG("instance undefined "+instanceName, LERR);
		throw new Tmysql_Exception_InstanceUndefined(instanceName);
	}
	if ( !Tmysql_Connection::instances.count(instanceName) )
	{
		delete Tmysql_Connection::instances[instanceName];
	}
	Tmysql_Connection::instances[instanceName] = new Tmysql_Connection( instanceName, Tmysql_Connection::instancesDefinitions[instanceName] );
}

Tmysql_Connection& Tmysql_Connection::getInstance( std::string instanceName )
{
	if ( !Tmysql_Connection::instancesDefinitions.count(instanceName) )
	{
		LOG("instance undefined "+instanceName, LERR);
		throw new Tmysql_Exception_InstanceUndefined(instanceName);
	}
	if ( !Tmysql_Connection::instances .count(instanceName) )
	{
		Tmysql_Connection::instances[instanceName] = new Tmysql_Connection( instanceName, Tmysql_Connection::instancesDefinitions[instanceName] );
	}
	return *(Tmysql_Connection::instances[instanceName]);
}

Tmysql_Connection::Tmysql_Connection( std::string name, Tmysql_Connection_InstanceDefinition instanceDefinition )
{
	this->connection = mysql_init(NULL);
	this->instanceName = name;
	if(!mysql_real_connect(this->connection,instanceDefinition.host,instanceDefinition.user,instanceDefinition.passwd,instanceDefinition.db,instanceDefinition.port,NULL,0))
	{
		LOG(mysql_error(this->connection), LERR);
		throw Tmysql_Exception_Connection(mysql_error(this->connection));
	}
}

Tmysql_Connection::~Tmysql_Connection( )
{
	mysql_close(this->connection);
}

#include <iostream>
MYSQL_RES* Tmysql_Connection::getQueryResultBase( std::string query )
{
	std::cout << "QUERY :" << query << std::endl;
	if ( mysql_real_query( this->connection, query.c_str(), query.size() ) )
	{
		LOG(mysql_error(this->connection), LERR);
		throw new Tmysql_Exception_Query( mysql_error(this->connection), query );
	}
	return mysql_use_result(this->connection);
}

MYSQL_RES* Tmysql_Connection::getQueryResult( std::string query, std::vector<std::string> &arguments )
{
	return this->getQueryResultBase( this->getQueryString(query, arguments) );
}

std::string Tmysql_Connection::getQueryString( std::string query, std::vector<std::string> &arguments )
{
	int argumentNumber = 0;
	int vectorCount = arguments.size();
	size_t found = query.find_first_of('?');
	while ( found != std::string::npos )
	{
		if ( argumentNumber >= vectorCount )
		{
			LOG("queryArguments number mismatch in "+query, LERR);
			throw new Tmysql_Exception_QueryArguments( query, arguments );
		}
		std::string argument = "'" + this->escape( arguments[argumentNumber] ) + "'";
		query.replace( found, 1, argument );
		found = query.find_first_of('?', found + argument.length() + 1);
		argumentNumber++;
	}
	return query;
}

std::string Tmysql_Connection::escape( std::string value )
{
	char* buffer = new char[ value.length() * 2 + 1 ];
	mysql_real_escape_string( this->connection, buffer, value.c_str(), value.length() );
	return (std::string)buffer;
}

Tmysql_Row Tmysql_Connection::fetchRow( std::string query )
{
	std::vector< std::string > emptyArguments;
	return this->fetchRow(query, emptyArguments);
}

Tmysql_Row Tmysql_Connection::fetchRow( std::string query, std::vector< std::string > &arguments )
{
	MYSQL_RES* result = this->getQueryResult(query,arguments);
	mysql_use_result(this->connection);
	unsigned int num_fields = mysql_num_fields(result);
	MYSQL_FIELD* fields = mysql_fetch_fields(result);
	MYSQL_ROW row;
	Tmysql_Row trow;
	if ((row = mysql_fetch_row(result)))
	{
		unsigned long* lengths = mysql_fetch_lengths(result);
		for( unsigned int i = 0; i < num_fields; i++)
		{
			if (!row[i])
			{
				trow[ fields[i].name ] = Tmysql_String();
			}
			else
			{
				std::string value(row[i], lengths[i]);
				trow[ fields[i].name ] = value;
			}
	   }
	}
	mysql_free_result(result);
	return trow;
}

Tmysql_RowSet Tmysql_Connection::fetchRowSet( std::string query )
{
	std::vector< std::string > emptyArguments;
	return this->fetchRowSet(query, emptyArguments);
}

Tmysql_RowSet Tmysql_Connection::fetchRowSet( std::string query, std::vector< std::string > &arguments )
{
	MYSQL_RES* result = this->getQueryResult(query,arguments);
	mysql_use_result(this->connection);
	unsigned int num_fields = mysql_num_fields(result);
	MYSQL_FIELD* fields = mysql_fetch_fields(result);
	MYSQL_ROW row;
	Tmysql_RowSet trowset;
	while ((row = mysql_fetch_row(result)))
	{
		Tmysql_Row trow;
		unsigned long* lengths = mysql_fetch_lengths(result);
		for (unsigned int i = 0; i < num_fields; i++)
		{
			if (!row[i])
			{
				trow[ fields[i].name ] = Tmysql_String();
			}
			else
			{
				std::string value(row[i], lengths[i]);
				trow[ fields[i].name ] = value;
			}
		}
		trowset.push_back(trow);
	}
	mysql_free_result(result);
	return trowset;
}

Tmysql_LiveRow Tmysql_Connection::fetchLiveRow(  std::string tableName, std::string where )
{
	std::vector< std::string > emptyArguments;
	return this->fetchLiveRow(tableName, where, emptyArguments);
}

Tmysql_LiveRow Tmysql_Connection::fetchLiveRow( std::string tableName, std::string where, std::vector< std::string > &arguments )
{
	std::string query = "SELECT * FROM `"+tableName+"` " + this->getQueryString(where,arguments);
	std::vector< std::string > emptyArgs;
	return Tmysql_LiveRow( this->instanceName, tableName, this->fetchRow( query, emptyArgs ) );
}

Tmysql_LiveRowSet Tmysql_Connection::fetchLiveRowSet( std::string tableName, std::string where )
{
	std::vector< std::string > emptyArguments;
	return this->fetchLiveRowSet(tableName, where, emptyArguments);
}

Tmysql_LiveRowSet Tmysql_Connection::fetchLiveRowSet( std::string tableName, std::string where, std::vector< std::string > &arguments )
{
	std::string query = "SELECT * FROM `"+tableName+"` " + this->getQueryString(where,arguments);
	std::vector< std::string > emptyArgs;
	Tmysql_RowSet trowset = this->fetchRowSet( query, emptyArgs );
	Tmysql_LiveRowSet lrowset;
	for ( unsigned long long i=0; i<trowset.size(); i++ )
	{
		Tmysql_LiveRow lrow = Tmysql_LiveRow(this->instanceName, tableName, trowset[i]);
		lrowset.push_back(lrow);
	}
	return lrowset;
}

void Tmysql_Connection::ping()
{
	if(mysql_ping(this->connection))
	{
		LOG(mysql_error(this->connection), LERR);
		throw new Tmysql_Exception_Connection( mysql_error(this->connection) );
	}
	return;
}

void Tmysql_Connection::setAutoReconnect( bool useAutoReconnect )
{
	if(mysql_options(this->connection, MYSQL_OPT_RECONNECT, &useAutoReconnect))
	{
		throw new Tmysql_Exception_Connection( mysql_error(this->connection) );
	}
	return;
}

unsigned long long Tmysql_Connection::getLastInsertId()
{
	return mysql_insert_id(this->connection);
}

unsigned long long Tmysql_Connection::query( std::string query, std::vector< std::string > &arguments )
{
	MYSQL_RES* result = this->getQueryResult(query,arguments);
	unsigned long long affected = mysql_affected_rows(this->connection);
	mysql_free_result(result);
	return affected;
}
