/*
 * YaeMaster.h
 *
 *  Created on: May 18, 2011
 *      Author: trakos
 */

#ifndef YAEMASTER_H_
#define YAEMASTER_H_

#include <ET/Status.h>

class YaeMaster
{
	protected:
		YaeMaster();
		static YaeMaster instance;
	public:
		static YaeMaster& getInstance();
		ET_Status verifyData(ET_Status clientStatus);
};



#endif /* YAEMASTER_H_ */
