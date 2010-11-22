<?php
	class Login
	{
		protected $uid = 0;
		protected $username = '';
		protected $isLogged = false;
		protected $sql;
		
		public function __construct($sql)
		{
			$this->sql = $sql;
			if( isset($_SESSION['logged_uid']) && isset($_SESSION['logged_username']) )
			{
				$this->uid = $_SESSION['logged_uid'];
				$this->username = $_SESSION['logged_username'];
				$this->isLogged = true;
			}
			else if(isset($_POST['login']) && isset($_POST['password']))
			{
				$this->tryToLogIn($_POST['login'], $_POST['password']);
			}
			
			if(isset($_GET['action']) && $_GET['action'] == 'logout')
			{
				$this->logout();
			}
		}
		
		public function getUserIP()
		{
			$ip;
			if (getenv("HTTP_CLIENT_IP"))
			{
				$ip = getenv("HTTP_CLIENT_IP");
			}
			else if(getenv("HTTP_X_FORWARDED_FOR"))
			{
				$ip = getenv("HTTP_X_FORWARDED_FOR");
			}
			else if(getenv("REMOTE_ADDR"))
			{
				$ip = getenv("REMOTE_ADDR");
			}
			else
			{
				$ip = "1.1.1.1";
			}
			return $ip;
		}
		
		public function logout()
		{
			unset($_SESSION['logged_uid']);
			unset($_SESSION['logged_username']);
			$this->isLogged = false;
			$this->uid = '';
			$this->username = '';
		}
		
		public function isLogged()
		{
			return $this->isLogged;
		}
		
		public function tryToLogIn($username,$password)
		{
			$password = md5(base64_encode('***REMOVED***'.sha1( strrev($password).'***REMOVED***' ).'***REMOVED***').'***REMOVED***');
			$uid = $this->sql->getOne("SELECT userid FROM users WHERE username='?' AND password='?'", array($username,$password));
			if($uid) 
			{
				$_SESSION['logged_uid'] = $uid;
				$_SESSION['logged_username'] = $username;
				$this->uid =  $uid;
				$this->username = $username;
				$this->isLogged = true;
				return true;
			}
			else
			{
				return false;
			}
		}
		
		public function createUser($username,$password,$cfid)
		{
			$password = md5(base64_encode('***REMOVED***'.sha1( strrev($password).'***REMOVED***' ).'***REMOVED***').'***REMOVED***');
			$this->sql->queryf("INSERT INTO users (crossfireid,username,password,added_from_ip) VALUES ('?','?','?',INET_ATON('?'))", array($cfid,$username,$password,$this->getUserIP()));
		}
		
		public function setPassword($cfid,$password)
		{
			$password = md5(base64_encode('***REMOVED***'.sha1( strrev($password).'***REMOVED***' ).'***REMOVED***').'***REMOVED***');
			$this->sql->queryf("UPDATE users SET password=? WHERE crossfireid=?", array($password,$cfid));
		}
		
		public function cfidExists($cfid)
		{
			$data = $this->sql->getOne("SELECT crossfireid FROM users WHERE crossfireid='?'", $cfid);
			return !(!$data);
		}
		
		public function getUsername()
		{
			return $this->username;
		}
		
		public function getUserID()
		{
			return $this->uid;
		}
	}
?>