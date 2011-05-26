/*
 * Message.c
 *
 *  Created on: 2011-01-25
 *      Author: trakos
 */


#include <utils.h>
#include <Tnet/Message.h>

void Tnet_Message::clear()
{
	this->ints.clear();
	this->strings.clear();
};

std::ostream& operator <<(std::ostream& stream, Tnet_Message const &message)
{
	stream << "Tnet_Message()" << std::endl;
	stream << "{" << std::endl;
	stream << "\t" << "[ints   ]" << " " << "=>" << "\t" << indent << message.ints << std::endl << unindent;
	stream << "\t" << "[strings]" << " " << "=>" << "\t" << indent << message.strings << std::endl << unindent;
	stream << "}";
	return stream;
}
