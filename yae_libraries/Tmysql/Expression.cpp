/*
 * Tmysql/Expression.cpp
 *
 *  Created on: May 26, 2011
 *      Author: trakos
 */

#include<Tmysql/Expression.h>

std::ostream& operator <<(std::ostream& stream, Tmysql_Expression const &b)
{
	return stream << b.getString();
}
