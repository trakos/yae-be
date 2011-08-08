<?php 

class Model_Yae_Exception extends Exception
{
	/**
	 * Exception constructor
	 * 
	 * @param integer $code error code
	 * @param string $message additional text helping debugging the error. 
	 */
	public function __construct($code,$message='')
	{
		parent::__construct($message, $code);
	}	
};
