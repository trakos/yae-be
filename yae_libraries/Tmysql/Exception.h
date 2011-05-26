/*
 * Tmysql/Exception.h
 *
 *  Created on: 2011-05-09
 *      Author: trakos
 */

#ifndef TMYSQL_EXCEPTION_H_
#define TMYSQL_EXCEPTION_H_

#include<vector>
#include<string>
#include<stdexcept>
#include<Tmysql/Connection.h>

typedef std::vector<Tmysql_String> Tmysql_Arguments;

class Tmysql_Exception : public std::runtime_error
{
	public:
		Tmysql_Exception(const std::string &what) : std::runtime_error( what ) {};
};
class Tmysql_Exception_LiveFieldUndefined : public Tmysql_Exception
{
	public:
		Tmysql_Exception_LiveFieldUndefined(const std::string &fieldName) : Tmysql_Exception( fieldName+ "undefined in row!" ) {};
};
class Tmysql_Exception_InstanceUndefined : public Tmysql_Exception
{
	public:
		Tmysql_Exception_InstanceUndefined(const std::string &instanceName) : Tmysql_Exception( "instance "+instanceName+" undefined!" ) {};
};
class Tmysql_Exception_Connection : public Tmysql_Exception
{
	public:
		Tmysql_Exception_Connection(const std::string &what) : Tmysql_Exception( what ) {};
};
class Tmysql_Exception_QueryArguments : public Tmysql_Exception
{
	public:
		Tmysql_Exception_QueryArguments(const std::string &argQuery, Tmysql_Arguments &argArguments) : Tmysql_Exception( "query with wrong argument numbers" ), query( argQuery ), arguments( argArguments ) {};
		~Tmysql_Exception_QueryArguments() throw() {};
	protected:
		std::string query;
		Tmysql_Arguments arguments;
};
class Tmysql_Exception_Query : public Tmysql_Exception
{
	public:
		Tmysql_Exception_Query(const std::string &what, std::string queryArgument) : Tmysql_Exception( what ), query(queryArgument) {};
		~Tmysql_Exception_Query() throw() {};
		std::string getQuery() { return this->query; };
	protected:
		std::string query;
};

#endif /* TMYSQL_EXCEPTION_H_ */

