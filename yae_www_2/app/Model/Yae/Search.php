<?php
	class Model_Yae_Search
	{
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////					players					//////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayersSelect()
		{
			// dość ciekawe jest przycięcie IP: binary anduje z inet_aton(255.255.255.0) dzięki czemu ostatnie dwa znaki to zawsze ".0", więc mogę usunąć trailing 0 i dodać * na końcu, co da mi zamaskowanie ostatniego oktetu w adresie.
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT SQL_CALC_FOUND_ROWS nick, CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(ip & 4294967040) ), '*') AS ip, INET_NTOA(ip) AS realip, SUBSTR(pbguid,32-7,8) AS pbguid, SUBSTR(etproguid,40-9,10) AS etproguid, slacnick, IF(slacid=0,'',slacid) AS slacid, id FROM players");
			$select->order('id DESC');
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayersSlacSelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
			SELECT SQL_CALC_FOUND_ROWS
				nick,
				CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(ip & 4294967040) ), '*') AS ip,
				INET_NTOA(ip) AS realip,
				SUBSTR(pbguid,32-7,8) AS pbguid,
				SUBSTR(etproguid,40-9,10) AS etproguid,
				slacnick,
				IF(slacid=0,'',slacid) AS slacid,
				COUNT(DISTINCT pbguid) AS pbguid_count,
				COUNT(DISTINCT ip) AS ip_count,
				COUNT(DISTINCT nick) AS nick_count,
				COUNT(DISTINCT etproguid) AS etproguid_count
			FROM players
				
EOF
			);
			$select->group("slacid");
			$select->order('id DESC');
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayersPbSelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
			SELECT SQL_CALC_FOUND_ROWS
				nick,
				CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(ip & 4294967040) ), '*') AS ip,
				INET_NTOA(ip) AS realip,
				SUBSTR(pbguid,32-7,8) AS pbguid,
				SUBSTR(etproguid,40-9,10) AS etproguid,
				slacnick,
				IF(slacid=0,'',slacid) AS slacid,
				COUNT(DISTINCT slacid) AS slacid_count,
				COUNT(DISTINCT ip) AS ip_count,
				COUNT(DISTINCT nick) AS nick_count,
				COUNT(DISTINCT etproguid) AS etproguid_count
			FROM players
				
EOF
			);
			$select->group("pbguid");
			$select->order('id DESC');
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayersIpSelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
			SELECT SQL_CALC_FOUND_ROWS
				nick,
				CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(ip & 4294967040) ), '*') AS ip,
				INET_NTOA(ip) AS realip,
				SUBSTR(pbguid,32-7,8) AS pbguid,
				SUBSTR(etproguid,40-9,10) AS etproguid,
				slacnick,
				IF(slacid=0,'',slacid) AS slacid,
				COUNT(DISTINCT slacid) AS slacid_count,
				COUNT(DISTINCT pbguid) AS pbguid_count,
				COUNT(DISTINCT nick) AS nick_count,
				COUNT(DISTINCT etproguid) AS etproguid_count
			FROM players
				
EOF
			);
			$select->group("ip");
			$select->order('id DESC');
			return $select;
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayersEtproSelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
			SELECT SQL_CALC_FOUND_ROWS
				nick,
				CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(ip & 4294967040) ), '*') AS ip,
				INET_NTOA(ip) AS realip,
				SUBSTR(pbguid,32-7,8) AS pbguid,
				SUBSTR(etproguid,40-9,10) AS etproguid,
				slacnick,
				IF(slacid=0,'',slacid) AS slacid,
				COUNT(DISTINCT slacid) AS slacid_count,
				COUNT(DISTINCT pbguid) AS pbguid_count,
				COUNT(DISTINCT ip) AS ip_count,
				COUNT(DISTINCT nick) AS nick_count
			FROM players
				
EOF
			);
			$select->group("etproguid");
			$select->order('id DESC');
			return $select;
		}
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////					servers					//////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getServersSelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT SQL_CALC_FOUND_ROWS INET_NTOA(ip) AS ip, port, name, UNIX_TIMESTAMP(lastonline) AS lastonline, id FROM servers");
			$select->order("lastonline DESC");
			return $select;
		}
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////					showPlayer				//////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/**
		 * @param int $playerId
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayerTimesSelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
				SELECT SQL_CALC_FOUND_ROWS
				
					playerid,
					nick,
					CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(players.ip & 4294967040) ), '*') AS ip,
					INET_NTOA(players.ip) AS realip,
					SUBSTR(pbguid,32-7,8) AS pbguid,
					SUBSTR(etproguid,40-9,10) AS etproguid,
					slacnick,
					IF(slacid=0,'',slacid) AS slacid,
					
					UNIX_TIMESTAMP(playedfrom) AS playedfrom,
					UNIX_TIMESTAMP(playedto) AS playedto,
					
					CONCAT(INET_NTOA(servers.ip),':',servers.port) AS server,
					name,
					serverid
					
				FROM players
					JOIN times ON playerid=players.id
					JOIN servers ON serverid = servers.id
EOF
			);
			$select->order("playedto DESC");
			return $select;
		}
		
		/**
		 * @param int $playerId
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayerTimesMonthlySelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
				SELECT SQL_CALC_FOUND_ROWS
				
					month,
					year,
					minutesplayed,
					
					CONCAT(INET_NTOA(servers.ip),':',servers.port) AS server,
					name,
					serverid
					
				FROM times_monthly
					JOIN servers ON serverid = servers.id
					JOIN players ON playerid=players.id
EOF
			);
			$select->group("serverid");
			$select->group("month");
			$select->group("year");
			$select->order("year DESC");
			$select->order("month DESC");
			$select->order("minutesplayed DESC");
			return $select;
		}
		
		/**
		 * @param int $playerId
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayerTimesAllTimeSelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect(<<<EOF
			
				SELECT SQL_CALC_FOUND_ROWS
					
					SUM(minutesplayed) AS minutesplayed,
					
					CONCAT(INET_NTOA(servers.ip),':',servers.port) AS server,
					name,
					serverid
					
				FROM times_monthly
					JOIN servers ON serverid = servers.id
					JOIN players ON playerid=players.id
EOF
			);
			$select->group("serverid");
			$select->order("minutesplayed DESC");
			return $select;
		}
		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////
		/////////////////////					showServer				//////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getServerPlayersExactTimes()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT SQL_CALC_FOUND_ROWS UNIX_TIMESTAMP(playedfrom) AS playedfrom, UNIX_TIMESTAMP(playedto) AS playedto, nick, SUBSTR(pbguid,32-7,8) AS pbguid, SUBSTR(etproguid,40-9,10) AS etproguid, slacnick, IF(slacid=0,'',slacid) AS slacid, CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(players.ip & 4294967040) ), '*') AS ip, INET_NTOA(players.ip) AS realip, players.id as playerid, serverid from servers JOIN times ON servers.id = serverid JOIN players ON playerid = players.id");
			$select->order("playedto DESC");
			return $select;	
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getServerPlayersAllTimeSelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect( <<<EOF
			
				SELECT SQL_CALC_FOUND_ROWS
					SUM((UNIX_TIMESTAMP(playedto)-UNIX_TIMESTAMP(playedfrom))/60) AS minutesplayed,
					nick,
					SUBSTR(pbguid,32-7,8) AS pbguid,
					SUBSTR(etproguid,40-9,10) AS etproguid,
					slacnick, IF(slacid=0,'',slacid) AS slacid,
					CONCAT(TRIM( TRAILING '0' FROM INET_NTOA(players.ip & 4294967040) ), '*') AS ip,
					INET_NTOA(players.ip) AS realip,
					players.id as playerid,
					serverid
				FROM servers
					JOIN times ON servers.id = serverid
					JOIN players ON playerid = players.id
EOF
			);
			$select->order("minutesplayed DESC");
			return $select;	
		}
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getServerPlayersLastMonthSelect()
		{
			$select = self::getServerPlayersAllTimeSelect();
			$select->where("AND playedfrom > NOW() - INTERVAL 30 DAY");
			return $select;	
		}
	}	