/*
 * YaeMaster/Database.cpp
 *
 *  Created on: May 18, 2011
 *      Author: trakos
 */

#include <YaeMaster/Database.h>

YaeMaster_Database YaeMaster_Database::instance = YaeMaster_Database();

YaeMaster_Database::YaeMaster_Database() {};

YaeMaster_Database& YaeMaster_Database::getInstance()
{
	return YaeMaster_Database::instance;
}

