<?php

/**
 * Class to control authorization.
 */
abstract class Tmvc_Model_Auth
{
	/**
	 * Data saved in session.
	 * @var array
	 */
	protected $_data = array();
	/**
	 * User id.
	 * @var string
	 */
	protected $_uid = "";
	/**
	 * Currently logged user access level.
	 * @var int
	 */
	protected $_userLevel = LEVEL_UNLOGGED;
	/**
	 * Is user logged in.
	 * 
	 * @var boolean
	 */
	protected $_isLogged = false;

	/**
	 * Object instance
	 * @var Tmvc_Model_Auth
	 */
	static protected $_instance=NULL;

	/**
	 * Return instance of an object.
	 * @return Tmvc_Model_Auth
	 */
	static public function getInstance()
	{
		if(!self::$_instance)
		{
			self::$_instance = new static();
		}
		return self::$_instance;
	}

	protected function __construct()
	{
		if( isset($_SESSION['loggedData']) && isset($_SESSION['loggedUid'])  && isset($_SESSION['loggedUserLevel']) )
		{
			$this->_data = $_SESSION['loggedData'];
			$this->_uid = $_SESSION['loggedUid'];
			$this->_userLevel = $_SESSION['loggedUserLevel'];
			$this->_isLogged = true;
		}
	}

	/**
	 * Logs out of the system.
	 */
	public function logout()
	{
		unset($_SESSION['loggedData']);
		unset($_SESSION['loggedUid']);
		unset($_SESSION['loggedUserLevel']);
		$this->_data = array();
		$this->_uid = 0;
		$this->_userLevel = LEVEL_UNLOGGED;
		$this->_isLogged = false;
	}

	/**
	 * Returns whether user is logged in.
	 * @return boolean
	 */
	public function isLogged()
	{
		return $this->_isLogged;
	}


	/**
	 * Returns IP of a connection, it returns "1.1.1.1" in case of a failure.
	 * @return string
	 */
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

	/**
	 * Returns values stored in a session for currently logged in user.
	 * @param string $name
	 * @throws Tmvc_Model_Auth_Exception
	 * @return string
	 */
	public function get($name)
	{
		if ( !$this->_isLogged )
		{
			throw new Tmvc_Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "You must be logged in in order to use session data!");
		}
		else if ( !isset($this->_data[$name]))
		{
			throw new Tmvc_Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Session data with given name does not exist!");
		}
		return $this->_data[$name];
	}

	/**
	 * Returns id of a currently logged user.
	 */
	public function getUserId()
	{
		return $this->_uid;
	}

	/**
	 * Returns level of a currently logged in user.
	 */
	public function getUserLevel()
	{
		return $this->_userLevel;
	}

	/**
	 * Checks if a user level is on a $status or higher level.
	 * @param int $status
	 * @return boolean
	 */
	public function isAtLeast($status)
	{
		return $this->_userLevel >= $status;
	}

	/**
	 * Tries to log in, returns true when succesfull and false when the data is incorrect.
	 * @param string $username
	 * @param string $password
	 * @return boolean
	 */
	public function tryToLogIn($username,$password)
	{
		$this->_tryToLogIn($username,$password);
		if($this->_isLogged)
		{
			$_SESSION['loggedUid'] = $this->_uid;
			$_SESSION['loggedData'] = $this->_data;
			$_SESSION['loggedUserLevel'] = $this->_userLevel;
			return true;
		}
		else
		{
			return false;
		}
	}
	
	/**
	 * Internal method to log in, it should set _uid, _data, _userLevel and _isLogged upon success.
	 * @param string $username
	 * @param string $password
	 */
	abstract protected function _tryToLogIn($username, $password);

	/**
	 * Sets password for an user given by $login or otherwise currently logged in.
	 * 
	 * @param string $password
	 * @param string $userId
	 * @param bool $forcePrivileges whether to ignore privileges check and allow to set password of other users or while being unlogged
	 * @throws Tmvc_Model_Auth_Exception
	 */
	abstract public function setPassword($password, $userId=false, $forcePrivileges=false);
}
