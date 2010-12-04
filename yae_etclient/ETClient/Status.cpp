/*
 * ETClient/Status.cpp
 *
 *  Created on: 2010-12-02
 *      Author: trakos
 */

#include <ETClient/Status.h>
#include <ETClient/Console.h>
#include <string>

ETClientStatus ETClientStatus::instance = ETClientStatus();

ETClientStatus::ETClientStatus(){}

ETClientStatus& ETClientStatus::getInstance()
{
	return ETClientStatus::instance;
}
