<?php

/**
 * Class to control authorization.
 */
class Model_Auth extends Tmvc_Model_Auth
{
	/**
	 * Sets password for an user given by $login or otherwise currently logged in.
	 * 
	 * @param string $password
	 * @param string $login
	 * @param bool $force skips privileges checks
	 * @throws Tmvc_Model_Auth_Exception
	 */
	public function setPassword($password, $userId=false, $force=false)
	{
		$password = md5(base64_encode('***REMOVED***'.sha1( strrev($password).'***REMOVED***' ).'***REMOVED***').'***REMOVED***');
		if ( !$userId && $this->_isLogged )
		{
			Tmvc_Model_Mysql::getConnection()->query("UPDATE users SET password=? WHERE userid=?", $password, $this->uid);
		}
		else if ( $this->_userLevel >= LEVEL_SUPERADMIN || $force)
		{
			Tmvc_Model_Mysql::getConnection()->query("UPDATE users SET password=? WHERE userid=?", $password, $userId);
		}
		else
		{
			throw new Tmvc_Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Insufficient user level to perform this operation!");
		}
	}
	
	/** 
	 * Internal method to log in, it should set _uid, _data, _userLevel and _isLogged upon success.
	 * @param string $username
	 * @param string $password
	 */
	public function _tryToLogIn($username,$password)
	{
		$query = 
		"
			SELECT 
				userid, crossfireid, username, role, unix_timestamp(date_registered)
			FROM 
				users
			WHERE 
				username=? 
				AND password=?
		";
		$password = md5(base64_encode('***REMOVED***'.sha1( strrev($password).'***REMOVED***' ).'***REMOVED***').'***REMOVED***');
		$user = Tmvc_Model_Mysql::getConnection()->getRow($query, $username, $password);
		if($user)
		{
			$this->_uid = $user['login'];
			$this->_data = $user;
			$this->_userLevel = $user['poziom'];
			$this->_isLogged = true;
		}
	}
}
