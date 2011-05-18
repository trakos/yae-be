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
#include <ETClient/Status.h>

class YaeMaster_Database
{
	protected:
		YaeMaster_Database();
		static YaeMaster_Database instance;
	public:
		static YaeMaster_Database& getInstance();
		Tmysql_LiveRow authenticate(std::string login, std::string password);
		void userOnlineWithoutET(Tmysql_LiveRow& user);
		void userOnlineWithET(Tmysql_LiveRow& user, ETClientStatus& status);
		Tnet_Message performYaeSearch(ETClientStatus& status);
};



#endif /* YAEMASTER_DATABASE_H_ */
