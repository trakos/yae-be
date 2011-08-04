<?php
/**
 * @package Lib
 * @category Lib
 */

/**
 * Class of an exception, which can be thrown when standard php error (or warning/notice etc.) occurs. *
 */
class Tmvc_Controller_PHPErrorException extends Exception
{
	/**
	 * Error type (value of one of the php constatns E_*)
	 * @var int
	 */
    protected $severity;
    
    /**
     * Text error given by php.
     * @var string
     */
    protected $realMessage;
   
    /**
     * Exception constructor.
     * @param string $message
     * @param integer $code
     * @param integer $severity
     * @param string $filename
     * @param integer $lineno
     */
    public function __construct($message, $code, $severity, $filename, $lineno)
    {
        $this->realMessage = $message;
        $this->message = "Internal system error occurs.";
        $this->code = $code;
        $this->severity = $severity;
        $this->file = $filename;
        $this->line = $lineno;
    }
   
    /**
     * Gets severity of an error (value of one of the php constatns E_*).
     */
    public function getSeverity()
    {
        return $this->severity;
    }
    
    /**
     * Returns original php error text.
     */
    public function getRealMessage()
    {
    	return $this->realMessage;
    }
}