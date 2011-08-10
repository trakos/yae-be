<?php

	class Model_User_Servers
	{
		static public function getServerData($ip, $port)
		{
			return self::_getServersQuery()->where("AND ip=INET_ATON(?) AND port=?", array($ip,$port) )->getRow();	
		}
		
		static public function getUserServer( $serverId )
		{
			$server = self::_getServersQuery()->where("AND id=?", array($serverId))->getRow();
			if ( !$server )
			{
				throw new Model_Yae_Exception(E_SERVER_DOESNT_EXISTS, "Server with that id not found!");
			}
			else if( !Model_Auth::getInstance()->isLogged() )
			{
				throw new Tmvc_Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "User not logged in!");
			}
			else if ( Model_Auth::getInstance()->getUserId() != $server['userid'] && !Model_Auth::getInstance()->isAtLeast(LEVEL_SUPERADMIN) )
			{
				throw new Tmvc_Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Server belongs to someone else!");
			}
			return $server;
		}
		
		static public function getUserServers( $userId=NULL )
		{
			if( !Model_Auth::getInstance()->isLogged() )
			{
				throw new Tmvc_Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "User not logged in!");
			}
			if ( $userId && !Model_Auth::getInstance()->isAtLeast(LEVEL_SUPERADMIN) )
			{
				throw new Tmvc_Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Not enough privileges to do it!");
			}
			else if ( !$userId )
			{
				$userId = Model_Auth::getInstance()->getUserId();
			}
			return self::_getServersQuery()->where("AND userid=?", array($userId) )->getAll();
		}
		
		/**
		 * @param string $ip
		 * @param int $port
		 * @param string $rconPassword
		 * @throws Model_Yae_Exception
		 */
		static public function addServer($ip, $port, $rconPassword)
		{
			if ( self::getServerData($ip,$port) )
			{
				throw new Model_Yae_Exception(E_SERVER_EXISTS, "Server is already added by other user!");
			}
			self::_checkServerRcon($ip, $port, $rconPassword);
			$rconPassword = self::_encodePassword($rconPassword);
			$q = "INSERT INTO servers (userid,gameid,ip,port,password,added_at,added_from_ip) VALUES (?,?,INET_ATON(?),?,?,NOW(),INET_ATON(?))";
			$a = array(Model_Auth::getInstance()->getUserId(),1,$ip,$port,$rconPassword,Model_Auth::getInstance()->getUserIP());
			Tmvc_Model_Mysql::getConnection()->vquery($q,$a);
		}
		
		/**
		 * 
		 * @param unknown_type $serverId
		 * @param unknown_type $newPassword
		 * @throws Model_Yae_Exception
		 * @throws Tmvc_Model_Auth_Exception
		 */
		static public function changeServerPassword($serverId, $newPassword)
		{
			$server = self::getUserServer($serverId);
			$ip = $server['ip'];
			$port = $server['port'];
			self::_checkServerRcon($ip, $port, $newPassword);
			$newPassword = self::_encodePassword($newPassword);
			$q = "UPDATE servers SET password=?,nextcheck=NOW() WHERE id=?";
			$a = array($newPassword, $serverId);
			Tmvc_Model_Mysql::getConnection()->vquery($q,$a);
		}
		
		/**
		 * Disable server checking, i.e. set nextcheck to "infinity" (latest date possible)
		 */
		static public function disableServer($serverId)
		{
			$server = self::getUserServer($serverId);
			$q = "UPDATE servers SET nextcheck=? WHERE id=?";
			$a = array(YAE_SERVER_OFF_DATE, $serverId);
			Tmvc_Model_Mysql::getConnection()->vquery($q,$a);
		}
		
		/**
		 * Force server checking, i.e. set nextcheck to NOW()
		 */
		static public function enableServer($serverId)
		{
			$server = self::getUserServer($serverId);
			$q = "UPDATE servers SET nextcheck=NOW() WHERE id=?";
			$a = array($serverId);
			Tmvc_Model_Mysql::getConnection()->vquery($q,$a);
		}
		
		/**
		 * Change server owner. Must be either admin or current owner.
		 * @param int $serverId
		 * @param int $newUserId
		 */
		static public function changeOwner($serverId, $newUserId)
		{
			$server = self::getUserServer($serverId);
			$q = "UPDATE servers SET userid=? WHERE id=?";
			$a = array($userId, $serverId);
			Tmvc_Model_Mysql::getConnection()->vquery($q,$a);
		}
		
		static protected function _encodePassword($password)
		{
			$password = exec(SRC_PATH_RESOURCES . "/enc ".escapeshellarg($password));
			if ( empty($password) )
			{
				throw new Model_Yae_Exception(E_SERVER_RCON_ENC_EMPTY, "Encoding utitility returned empty string!");
			}
			return $password;
		}
		
		/**
		 * @param string $ip
		 * @param int $port
		 * @param string $rconPassword
		 * @throws Model_Yae_Exception
		 */
		static protected function _checkServerRcon($ip, $port, $rconPassword)
		{
			try
			{
				$cmd = 'status';
				$fp = fsockopen("udp:/"."/$ip", "$port");
				stream_set_blocking($fp, FALSE);
				$cmd = "\xff\xff\xff\xffrcon \"$rconPassword\" $cmd";
				fputs($fp, $cmd, strlen($cmd));
				usleep(0.4*1000000);
				$res="";
				do
				{
				        $buf = fread($fp, 2048);
			        	$res .= substr($buf, 4);
				} while( !empty($buf) );
				usleep(0.4*1000000);
				if ( trim($res) == "print\nBad rconpassword." )
				{
					throw new Model_Yae_Exception(E_SERVER_WRONG_PASSWORD, "Wrong rcon password for this server!");
				}
				$res = explode("\n", trim($res));
				if(count($res) < 2)
				{
					throw new Model_Yae_Exception(E_SERVER_UNAVAILABLE, "Couldn't reach server.");
				}
			}
			catch ( Tmvc_Controller_PHPErrorException $e )
			{
				throw new Model_Yae_Exception(E_SERVER_UNAVAILABLE, $e->getRealMessage());
			}
			return true;
		}
		
		
		
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		static protected function _getServersQuery()
		{
			return Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT id,userid,gameid,UNIX_TIMESTAMP(lastonline) AS lastonline,INET_NTOA(ip) AS ip,port,name,passfails,connectionfails,spoofed,UNIX_TIMESTAMP(added_at) AS added_at,INET_NTOA(added_from_ip) AS added_from_ip,UNIX_TIMESTAMP(nextcheck) AS nextcheck,UNIX_TIMESTAMP(lastonline) AS lastonline FROM servers");
		}
		
	}