<?php

	class Model_User_Servers
	{
		static public function getServerData($ip, $port)
		{
			return Tmvc_Model_Mysql::getConnection()->getRow("SELECT id,userid,gameid,lastonline,ip,port,name,passfails,connectionfails,spoofed,UNIX_TIMESTAMP(added_at),added_from_ip FROM servers WHERE ip=INET_ATON(?) AND port=?", $ip, $port);
			return self::_getServersQuery()->where("ip=INET_ATON(?) AND port=?", array($ip,$port) )->getRow();	
		}
		
		static public function getUserServers()
		{
			if( !Model_Auth::getInstance()->isLogged() )
			{
				throw new Tmvc_Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "User not logged in!");
			}
			return self::_getServersQuery()->where("userid=?", array(Model_Auth::getInstance()->getUserId()) )->getRow();
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
			$rconPassword = exec("../resources/enc ".escapeshellarg($rconPassword));
			$q = "INSERT INTO servers (userid,gameid,ip,port,password,added_at,added_from_ip) VALUES ('?','?',INET_ATON('?'),'?','?',NOW(),INET_ATON('?'))";
			$a = array(Model_Auth::getInstance()->getUserIP(),1,$ip,$port,$rconPassword,Model_Auth::getInstance()->getUserIP());
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
			$server = self::_getServersQuery()->where("id=?",$serverId);
			if ( !$server )
			{
				throw new Model_Yae_Exception(E_SERVER_DOESNT_EXISTS, "Server with that id not found!");
			}
			if( !Model_Auth::getInstance()->isLogged() )
			{
				throw new Tmvc_Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "User not logged in!");
			}
			if ( Model_Auth::getInstance()->getUserId() != $server['userid'] && !Model_Auth::getInstance()->isAtLeast(LEVEL_SUPERADMIN) )
			{
				throw new Tmvc_Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Server belongs to someone else!");
			}
			self::_checkServerRcon($ip, $port, $newPassword);
			$newPassword = exec("../resources/enc ".escapeshellarg($newPassword));
			$q = "UPDATE servers SET password=? WHERE id=?";
			$a = array($newPassword, $serverId);
			Tmvc_Model_Mysql::getConnection()->vquery($q,$a);
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
			return Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT id,userid,gameid,lastonline,ip,port,name,passfails,connectionfails,spoofed,UNIX_TIMESTAMP(added_at),added_from_ip FROM servers");
		}
		
	}