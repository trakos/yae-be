/*
 * YaeMaster.cpp
 *
 *  Created on: May 18, 2011
 *      Author: trakos
 */

#include <YaeMaster.h>

YaeMaster YaeMaster::instance = YaeMaster();

YaeMaster::YaeMaster() {};

YaeMaster& YaeMaster::getInstance()
{
	return YaeMaster::instance;
}
