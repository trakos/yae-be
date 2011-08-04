<?php
	class Model_Yae
	{
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayersSelect()
		{
			// dość ciekawe jest przycięcie IP: binary anduje z inet_aton(255.255.255.0) dzięki czemu ostatnie dwa znaki to zawsze ".0", więc mogę usunąć trailing 0 i dodać * na końcu, co da mi zamaskowanie ostatniego oktetu w adresie.
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT SQL_CALC_FOUND_ROWS nick, CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(ip & 4294967040) ), '*') AS ip, INET_NTOA(ip) AS realip, SUBSTR(pbguid,32-7,8) AS pbguid, SUBSTR(etproguid,40-9,10) AS etproguid, slacnick, IF(slacid=0,'',slacid) AS slacid, id FROM players %s");
			$select->order('id DESC');
			return $select;
		}
	}	