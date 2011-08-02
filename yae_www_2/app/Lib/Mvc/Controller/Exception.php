<?php 
/**
 * @package Lib
 * @category Lib
 * @version 1.0
 */

/**
 * 
 * Exception thrown by Lib_Mvc_Controller_Front and other controllers.
 *
 */
class Lib_Mvc_Controller_Exception extends Exception
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
