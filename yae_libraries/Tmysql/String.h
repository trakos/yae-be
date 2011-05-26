/*
 * Tmysql/String.h
 *
 *  Created on: 2011-05-09
 *      Author: trakos
 */

#ifndef TMYSQL_STRING_H_
#define TMYSQL_STRING_H_

#include <string>
#include <utils.h>
#include <Tmysql/Expression.h>

class Tmysql_String
{
	public:
		bool isNull() const { return this->nullIndicator; };
		bool doNotEscape() const { return this->dontEscape; };
		Tmysql_String(): nullIndicator(true), dontEscape(false), text("") {};
		Tmysql_String(std::string b): nullIndicator(false), dontEscape(false), text(b) {};
		Tmysql_String(const char* b): nullIndicator(false), dontEscape(false), text(b) {};
		Tmysql_String(unsigned long long b): nullIndicator(false), dontEscape(false), text(::itos(b)) {};
		Tmysql_String(long long b): nullIndicator(false), dontEscape(false), text(::itos(b)) {};
		Tmysql_String(int b): nullIndicator(false), dontEscape(false), text(::itos(b)) {};
		Tmysql_String(unsigned int b): nullIndicator(false), dontEscape(false), text(::itos(b)) {};
		Tmysql_String(const Tmysql_Expression& b): nullIndicator(false), dontEscape(true), text(b.getString()) {};
		operator std::string() const { return this->text; };
		//Tmysql_String& operator=(const std::string& b) { this->string = b; this->nullIndicator=false; }
		//Tmysql_String& operator=(const char*& b) { this->string = b; this->nullIndicator=false; }
		void nullify() { this->nullIndicator = true; this->text = ""; };
		bool operator ==(const Tmysql_String& b) const;
	protected:
		std::string text;
		bool dontEscape;
		bool nullIndicator;
};

extern std::ostream& operator <<(std::ostream& stream, Tmysql_String const &b);

#endif /* TMYSQL_STRING_H_ */
