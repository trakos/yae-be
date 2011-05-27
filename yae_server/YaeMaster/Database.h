/*
 * YaeMaster/Database.h
 *
 *  Created on: May 18, 2011
 *      Author: trakos
 */

#ifndef YAEMASTER_DATABASE_H_
#define YAEMASTER_DATABASE_H_

#include <string>
#include <YaeMaster/Exception.h>
#include <ET/Status.h>
#include <Tmysql/LiveRow.h>
#include <Tnet/Message.h>

class YaeMaster_Database
{
	protected:
		YaeMaster_Database();
		static YaeMaster_Database instance;
		Tmysql_LiveRow getYaetUser(Tmysql_LiveRow& user);
	public:
		static YaeMaster_Database& getInstance();
		Tmysql_LiveRow authenticate(std::string login, std::string password);
		void userOnlineWithoutET(Tmysql_LiveRow& user);
		void userOnlineWithET(Tmysql_LiveRow& user, ET_Status& status);
		Tnet_Message performYaeSearch(ET_Status& status);
};



#endif /* YAEMASTER_DATABASE_H_ */
