<?php


class Tmvc_Model_Session
{
 	/* Saves the variable in a session with a TMVC_ prefix for the key.
	 * @param string $key
	 * @param unknown_type $value
	 */
	static public function setSession($key, $value)
	{
		$_SESSION["TMVC_$key"] = serialize($value);
	}
	
	/**
	 * Retrieves session variables set with _setSession
	 * @param string $key
	 * @return unknown_type
	 */
	static public function getSession($key)
	{
		if ( !isset($_SESSION["TMVC_$key"]) )
		{
			return NULL;
		}
		return unserialize($_SESSION["TMVC_$key"]);
	}
	
	/**
	 * Unsert session variable set with _setSession
	 * @param unknown_type $key
	 */
	static public function unsetSession($key)
	{
		if ( isset($_SESSION["TMVC_$key"]) )
		{
			unset($_SESSION["TMVC_$key"]);
		}
	}
	 
}
