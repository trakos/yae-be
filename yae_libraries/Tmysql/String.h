/*
 * Tmysql/String.h
 *
 *  Created on: 2011-05-09
 *      Author: trakos
 */

#ifndef TMYSQL_STRING_H_
#define TMYSQL_STRING_H_

#include <string>

class Tmysql_String
{
	public:
		bool isNull() const { return this->nullIndicator; };
		Tmysql_String(): nullIndicator(true), string("") {};
		Tmysql_String(std::string b): nullIndicator(false), string(b) {};
		Tmysql_String(const char* b): nullIndicator(false), string(b) {};
		operator std::string() const { return this->string; };
		//Tmysql_String& operator=(const std::string& b) { this->string = b; this->nullIndicator=false; }
		//Tmysql_String& operator=(const char*& b) { this->string = b; this->nullIndicator=false; }
		void nullify() { this->nullIndicator = true; this->string = ""; };
		bool operator ==(const Tmysql_String& b) const;
	protected:
		std::string string;
		bool nullIndicator;
};

extern std::ostream& operator <<(std::ostream& stream, Tmysql_String const &b);

#endif /* TMYSQL_STRING_H_ */
