/*
 * Tmysql/String.cpp
 *
 *  Created on: May 10, 2011
 *      Author: trakos
 */

#include<Tmysql/String.h>

std::ostream& operator <<(std::ostream& stream, Tmysql_String const &b)
{
	if( b.isNull() )
	{
		return stream << "NULL";
	}
	return stream << (std::string)b;
}

bool Tmysql_String::operator ==(const Tmysql_String& b) const
{
	if ( this->isNull() && b.isNull() )
	{
		return true;
	}
	else if ( this->isNull() != b.isNull() )
	{
		return false;
	}
	else
	{
		return (std::string)(*this) == (std::string)b;
	}
}
