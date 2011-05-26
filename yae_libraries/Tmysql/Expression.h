/*
 * Tmysql/String.h
 *
 *  Created on: 2011-05-26
 *      Author: trakos
 */

#ifndef TMYSQL_EXPRESSION_H_
#define TMYSQL_EXPRESSION_H_

#include <string>
#include <utils.h>

class Tmysql_String;

class Tmysql_Expression
{
	public:
		Tmysql_Expression(): text("") {};
		Tmysql_Expression(std::string b): text(b) {};
		Tmysql_Expression(const char* b): text(b) {};
		const std::string& getString() const { return this->text; }
	protected:
		std::string text;
};

extern std::ostream& operator <<(std::ostream& stream, Tmysql_Expression const &b);

#endif /* TMYSQL_STRING_H_ */
