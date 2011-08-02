<?php

/**
 * Class to control authorization.
 */
class Lib_Yae_Model_Auth extends Lib_Mvc_Model_Auth
{
	/**
	 * Sets password for an user given by $login or otherwise currently logged in.
	 * 
	 * @param string $password
	 * @param string $login
	 * @throws Lib_Mvc_Model_Auth_Exception
	 */
	public function setPassword($password, $login=false)
	{
		if ( !$login && $this->_isLogged )
		{
			Model_Mysql::getConnection()->query("UPDATE uzytkownicy SET haslo=? WHERE login=?", $password, $this->uid);
		}
		else if ( $this->_userLevel >= LEVEL_SUPERADMIN )
		{
			Model_Mysql::getConnection()->query("UPDATE uzytkownicy SET haslo=? WHERE login=?", $password, $login);
		}
		else
		{
			throw new Lib_Mvc_Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Insufficient user level to perform this operation!");
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
				login, imie, nazwisko, poziom
			FROM 
				uzytkownicy
				JOIN grupy_uzytk USING(id_grupy)
			WHERE 
				login=? 
				AND haslo=?
		";
		$user = Model_Mysql::getConnection()->getRow($query, $username,$password);
		if($user)
		{
			$this->_uid = $user['login'];
			$this->_data = $user;
			$this->_userLevel = $user['poziom'];
			$this->_isLogged = true;
		}
	}
}
