/*
 * Tmysql/Row.h
 *
 *  Created on: 2011-05-12
 *      Author: trakos
 */

#ifndef TMYSQL_LIVEROW_H_
#define TMYSQL_LIVEROW_H_

#include <map>
#include <vector>
#include <string>

#include <Tmysql/Exception.h>
#include <Tmysql/String.h>

typedef std::map<std::string, Tmysql_String> Tmysql_Row;
typedef std::vector< Tmysql_Row > Tmysql_RowSet;

class Tmysql_LiveRow
{
	public:
		Tmysql_LiveRow( std::string instanceName, std::string tableName, Tmysql_Row data ): mysqlInstanceName(instanceName), mysqlTableName(tableName), rowData(data), originalData(data) {};
		void set( std::string fieldName, Tmysql_String value );
		void setNull( std::string fieldName );
		Tmysql_String get( std::string fieldName );
		bool isFieldNull( std::string fieldName );
		bool isRowNull() const;
		void save();
		Tmysql_Row getRowData() const { return this->rowData; };
		std::string getTableName() const { return this->mysqlTableName; };
		std::string getInstanceName() const { return this->mysqlInstanceName; };
	protected:
		std::string mysqlInstanceName;
		std::string mysqlTableName;
		Tmysql_Row rowData;
		Tmysql_Row originalData;

};
typedef std::vector< Tmysql_LiveRow > Tmysql_LiveRowSet;

std::ostream& operator <<(std::ostream& stream, Tmysql_LiveRow const &b);

#endif /* TMYSQL_LIVEROW_H_ */
