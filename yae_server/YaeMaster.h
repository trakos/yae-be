/*
 * YaeMaster.h
 *
 *  Created on: May 18, 2011
 *      Author: trakos
 */

#ifndef YAEMASTER_H_
#define YAEMASTER_H_

class YaeMaster
{
	protected:
		YaeMaster();
		static YaeMaster instance;
	public:
		static YaeMaster& getInstance();
};



#endif /* YAEMASTER_H_ */
