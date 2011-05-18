/*
 * Message.h
 *
 *  Created on: 2011-01-25
 *      Author: trakos
 */

#ifndef TNET_MESSAGE_H_
#define TNET_MESSAGE_H_

#include <string>
#include <map>
#include <utils.h>

typedef std::map<std::string, int>::iterator intMapIterator;
typedef std::map<std::string, std::string>::iterator stringMapIterator;

class Tnet_Message
{
	public:
		std::map<std::string, int> ints;
		std::map<std::string, std::string> strings;
		void clear();
};

std::ostream& operator <<(std::ostream& stream, Tnet_Message const &message);

#endif /* TNET_MESSAGE_H_ */
