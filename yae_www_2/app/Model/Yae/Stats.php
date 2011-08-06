<?php
	class Model_Yae_Stats
	{
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getMostActiveBySlac()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
				SELECT SQL_CALC_FOUND_ROWS
					SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) AS minutesplayed,
					nick,
					SUBSTR(pbguid,32-7,8) AS pbguid,
					SUBSTR(etproguid,40-9,10) AS etproguid,
					slacnick,
					IF(slacid=0,'',slacid) AS slacid,
					CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(players.ip & 4294967040) ), '*') AS ip,
					INET_NTOA(players.ip) AS realip,
					players.id as playerid
				FROM times 
					JOIN players ON playerid=players.id 
				%s
				GROUP BY slacid
EOF
			);
			$select->order("minutesplayed DESC");
			$select->where("AND slacid IS NOT NULL");
			$select->where("AND slacid <> ''");
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getMostActiveBySlacLastMonth()
		{
			$select = self::getMostActiveBySlac();
			$select->where("AND playedfrom > NOW() - INTERVAL 30 DAY");
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getMostActiveByPb()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
				SELECT SQL_CALC_FOUND_ROWS
					SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) AS minutesplayed,
					nick,
					SUBSTR(pbguid,32-7,8) AS pbguid,
					SUBSTR(etproguid,40-9,10) AS etproguid,
					slacnick,
					IF(slacid=0,'',slacid) AS slacid,
					CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(players.ip & 4294967040) ), '*') AS ip,
					INET_NTOA(players.ip) AS realip,
					players.id as playerid
				FROM times 
					JOIN players ON playerid=players.id 
				%s
				GROUP BY pbguid
EOF
			);
			$select->order("minutesplayed DESC");
			$select->where("AND pbguid IS NOT NULL");
			$select->where("AND pbguid <> ''");
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getMostActiveByPbLastMonth()
		{
			$select = self::getMostActiveByPb();
			$select->where("AND playedfrom > NOW() - INTERVAL 30 DAY");
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getMostActive()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
				SELECT SQL_CALC_FOUND_ROWS
					SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) AS minutesplayed,
					nick,
					SUBSTR(pbguid,32-7,8) AS pbguid,
					SUBSTR(etproguid,40-9,10) AS etproguid,
					slacnick,
					IF(slacid=0,'',slacid) AS slacid,
					CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(players.ip & 4294967040) ), '*') AS ip,
					INET_NTOA(players.ip) AS realip,
					players.id as playerid
				FROM times 
					JOIN players ON playerid=players.id 
				%s
				GROUP BY players.id
EOF
			);
			$select->order("minutesplayed DESC");
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getMostActiveLastMonth()
		{
			$select = self::getMostActive();
			$select->where("AND playedfrom > NOW() - INTERVAL 30 DAY");
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getMostActiveServers()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
				SELECT SQL_CALC_FOUND_ROWS 
					SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) AS minutesplayed,
					INET_NTOA(ip) AS ip,
					port,
					CONCAT(INET_NTOA(servers.ip),':',servers.port) AS server,
					name,
					UNIX_TIMESTAMP(lastonline) AS lastonline,
					servers.id AS id
				FROM times
					JOIN servers ON serverid=servers.id
				%s
				GROUP BY servers.id

EOF
			);
			$select->order("minutesplayed DESC");
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getMostActiveServersLastMonth()
		{
			$select = self::getMostActiveServers();
			$select->where("AND playedfrom > NOW() - INTERVAL 30 DAY");
			return $select;
		}
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////						CACHE				//////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getMostActiveCacheBySlac()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
				SELECT SQL_CALC_FOUND_ROWS
					minutesplayed,
					nick,
					SUBSTR(pbguid,32-7,8) AS pbguid,
					SUBSTR(etproguid,40-9,10) AS etproguid,
					slacnick,
					IF(slacid=0,'',slacid) AS slacid,
					CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(players.ip & 4294967040) ), '*') AS ip,
					INET_NTOA(players.ip) AS realip,
					players.id as playerid
				FROM stats_players_alltime_byslac 
					JOIN players ON playerid=players.id 
				%s
EOF
			);
			$select->order("minutesplayed DESC");
			$select->where("AND slacid IS NOT NULL");
			$select->where("AND slacid <> ''");
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getMostActiveCacheByPb()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
				SELECT SQL_CALC_FOUND_ROWS
					minutesplayed,
					nick,
					SUBSTR(pbguid,32-7,8) AS pbguid,
					SUBSTR(etproguid,40-9,10) AS etproguid,
					slacnick,
					IF(slacid=0,'',slacid) AS slacid,
					CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(players.ip & 4294967040) ), '*') AS ip,
					INET_NTOA(players.ip) AS realip,
					players.id as playerid
				FROM stats_players_alltime_bypb 
					JOIN players ON playerid=players.id 
				%s
EOF
			);
			$select->order("minutesplayed DESC");
			$select->where("AND pbguid IS NOT NULL");
			$select->where("AND pbguid <> ''");
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getMostActiveCache()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
				SELECT SQL_CALC_FOUND_ROWS
					minutesplayed,
					nick,
					SUBSTR(pbguid,32-7,8) AS pbguid,
					SUBSTR(etproguid,40-9,10) AS etproguid,
					slacnick,
					IF(slacid=0,'',slacid) AS slacid,
					CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(players.ip & 4294967040) ), '*') AS ip,
					INET_NTOA(players.ip) AS realip,
					players.id as playerid
				FROM stats_players_alltime_byid 
					JOIN players ON playerid=players.id 
				%s
EOF
			);
			$select->order("minutesplayed DESC");
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getMostActiveServersCache()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
				SELECT SQL_CALC_FOUND_ROWS
					minutesplayed,
					INET_NTOA(ip) AS ip,
					port,
					CONCAT(INET_NTOA(servers.ip),':',servers.port) AS server,
					name,
					UNIX_TIMESTAMP(lastonline) AS lastonline,
					servers.id AS id
				FROM stats_servers_alltime 
					JOIN servers ON serverid=servers.id 
				%s
EOF
			);
			$select->order("minutesplayed DESC");
			return $select;
		}
		
		public static function refreshStatsCache()
		{
			$sql = Tmvc_Model_Mysql::getConnection();
			$sql->query("DELETE FROM stats_players_alltime_byslac");
			$sql->query("DELETE FROM stats_players_alltime_bypb");
			$sql->query("DELETE FROM stats_players_alltime_byid");
			$sql->query("DELETE FROM stats_servers_alltime");
			$sql->query(<<<EOF
			
				INSERT INTO stats_players_alltime_byslac SELECT
					playerid,
					SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) AS minutesplayed
				FROM times 
					JOIN players ON playerid=players.id 
				GROUP BY slacid
EOF
			);
			$sql->query(<<<EOF
			
				INSERT INTO stats_players_alltime_bypb SELECT
					playerid,
					SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) AS minutesplayed
				FROM times 
					JOIN players ON playerid=players.id 
				GROUP BY pbguid
EOF
			);
			$sql->query(<<<EOF
			
				INSERT INTO stats_players_alltime_byid SELECT
					playerid,
					SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) AS minutesplayed
				FROM times
				GROUP BY playerid
EOF
			);
			$sql->query(<<<EOF
			
				INSERT INTO stats_servers_alltime SELECT
					serverid,
					SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) AS minutesplayed
				FROM times
				GROUP BY serverid
EOF
			);
			
		}
	}	