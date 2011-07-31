<?php 
/**
 * @package Lib
 * @category Lib
 * @version 1.0
 */

/**
 * 
 * Wyjątek wyrzucany przez kontrolery i FrontController
 *
 */
class Lib_ControllerException extends Exception
{
	/**
	 * Konstruktor wyjątku 
	 * 
	 * @param integer $code numer błędu
	 * @param string $message dodatkowy argument ułatwiający identyfikację błędu 
	 */
	public function __construct($code,$message='')
	{
		parent::__construct($message, $code);
	}	
};
