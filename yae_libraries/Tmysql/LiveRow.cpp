/*
 * Tmysql/LiveRow.cpp
 *
 *  Created on: May 10, 2011
 *      Author: trakos
 */

#include <vector>
#include <map>
#include <string>

#include <Tmysql/LiveRow.h>
#include <Tmysql/Connection.h>
#include <Tmysql/Exception.h>
#include <utils.h>


void Tmysql_LiveRow::set( std::string fieldName, Tmysql_String value )
{
	if ( !this->rowData.count(fieldName) )
	{
		throw new Tmysql_Exception_LiveFieldUndefined(fieldName);
	}
	this->rowData[fieldName] = value;
	return;
}

void Tmysql_LiveRow::setNull( std::string fieldName )
{
	if ( !this->rowData.count(fieldName) )
	{
		throw new Tmysql_Exception_LiveFieldUndefined(fieldName);
	}
	this->rowData[fieldName].nullify();
	return;
}

Tmysql_String Tmysql_LiveRow::get( std::string fieldName )
{
	if ( !this->rowData.count(fieldName) )
	{
		throw new Tmysql_Exception_LiveFieldUndefined(fieldName);
	}
	return this->rowData[fieldName];
}

bool Tmysql_LiveRow::isFieldNull( std::string fieldName )
{
	if ( !this->rowData.count(fieldName) )
	{
		throw new Tmysql_Exception_LiveFieldUndefined(fieldName);
	}
	return this->rowData[fieldName].isNull();
}

bool Tmysql_LiveRow::isRowNull( ) const
{
	return this->rowData.empty();
}

void Tmysql_LiveRow::save()
{
	if ( this->isRowNull() )
	{
		return;
	}
	std::string query = "UPDATE " + this->mysqlTableName + " SET ";
	bool first = true;
	Tmysql_Arguments arguments;
	for ( Tmysql_Row::const_iterator it = this->rowData.begin(); it != this->rowData.end(); ++it )
	{
		std::string key = it->first;
		Tmysql_String value = it->second;
		if ( this->rowData[key] == this->originalData[key] )
		{
			continue;
		}
		if(first)
		{
			first = false;
		}
		else
		{
			query += ", ";
		}
		if ( value.isNull() )
		{
			query += "" + key + " = NULL";
		}
		else
		{
			query += "" + key + " = ?";
			arguments.push_back(value);
		}
	}
	if ( first )
	{
		return;
	}
	query += " WHERE 1";
	for ( Tmysql_Row::const_iterator it = this->originalData.begin(); it != this->originalData.end(); ++it )
	{
		std::string key = it->first;
		Tmysql_String value = it->second;
		if ( value.isNull() )
		{
			query += " AND " + key + " IS NULL";
		}
		else
		{
			query += " AND " + key + " = ?";
			arguments.push_back(value);
		}
	}
	Tmysql_Connection::getInstance(this->mysqlInstanceName).query(query,arguments);
}

std::ostream& operator <<(std::ostream& stream, Tmysql_LiveRow const &b)
{
	return stream << b.getRowData();
}
