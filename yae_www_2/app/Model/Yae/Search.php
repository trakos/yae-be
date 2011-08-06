<?php
	class Model_Yae_Search
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
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getServersSelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT SQL_CALC_FOUND_ROWS INET_NTOA(ip) AS ip, port, name, UNIX_TIMESTAMP(lastonline) AS lastonline, id FROM servers %s");
			$select->order("lastonline DESC");
			return $select;
		}
		
		/**
		 * @param int $playerId
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayerTimesSelect($playerId)
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT SQL_CALC_FOUND_ROWS UNIX_TIMESTAMP(playedfrom) AS playedfrom, UNIX_TIMESTAMP(playedto) AS playedto, CONCAT(INET_NTOA(servers.ip),':',servers.port) AS server, servers.name AS name, servers.id AS id FROM servers JOIN times on serverid = servers.id %s");
			$select->order("playedto DESC");
			$select->where("AND playerid=?",array($playerId));
			return $select;
		}
		
		/**
		 * @param int $playerId
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayerTimesMonthlySelect($playerId)
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT SQL_CALC_FOUND_ROWS MONTH(playedfrom) AS month, YEAR(playedfrom) AS year, SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) AS minutesplayed, CONCAT(INET_NTOA(servers.ip),':',servers.port) AS server, servers.name AS name, servers.id AS id FROM servers JOIN times on serverid = servers.id %s GROUP BY servers.id,MONTH(playedfrom),YEAR(playedto)");
			$select->order("YEAR(playedfrom) DESC");
			$select->order("MONTH(playedfrom) DESC");
			$select->order("SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) DESC");
			$select->where("AND playerid=?",array($playerId));
			return $select;
		}
		
		/**
		 * @param int $playerId
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayerTimesTotalSelect($playerId)
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT SQL_CALC_FOUND_ROWS SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) AS minutesplayed, CONCAT(INET_NTOA(servers.ip),':',servers.port) AS server, servers.name AS name, servers.id AS id FROM servers JOIN times on serverid = servers.id %s GROUP BY servers.id");
			$select->order("minutesplayed DESC");
			$select->where("AND playerid=?",array($playerId));
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getExactTimesPlayersSelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT SQL_CALC_FOUND_ROWS UNIX_TIMESTAMP(playedfrom) AS playedfrom, UNIX_TIMESTAMP(playedto) AS playedto, nick, SUBSTR(pbguid,32-7,8) AS pbguid, SUBSTR(etproguid,40-9,10) AS etproguid, slacnick, IF(slacid=0,'',slacid) AS slacid, CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(players.ip & 4294967040) ), '*') AS ip, INET_NTOA(players.ip) AS realip, players.id as playerid, serverid from servers JOIN times ON servers.id = serverid JOIN players ON playerid = players.id %s");
			$select->order("playedto DESC");
			return $select;	
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getAllPlayersTimeSelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT SQL_CALC_FOUND_ROWS SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) AS minutesplayed, nick, SUBSTR(pbguid,32-7,8) AS pbguid, SUBSTR(etproguid,40-9,10) AS etproguid, slacnick, IF(slacid=0,'',slacid) AS slacid, CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(players.ip & 4294967040) ), '*') AS ip, INET_NTOA(players.ip) AS realip, players.id as playerid, serverid from servers JOIN times ON servers.id = serverid JOIN players ON playerid = players.id %s GROUP BY playerid");
			$select->order("minutesplayed DESC");
			return $select;	
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getLastMonthPlayersSelect()
		{
			$select = self::getAllPlayersTimeSelect();
			$select->where("AND playedfrom > NOW() - INTERVAL 30 DAY");
			return $select;	
		}
	}	