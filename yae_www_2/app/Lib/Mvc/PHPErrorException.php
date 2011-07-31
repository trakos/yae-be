<?php
/**
 * Plik z klasą PHPErrorHandler
 * @package Lib
 * @category Lib
 * @version 1.0
 */

/**
 * Klasa wyjątku wyrzucanego, gdy wystąpi standardowy phpowy error/warning/notice, 
 * poprzez przeciążenie error handlera w indexie.
 * 
 * @version 1.0
 * 
 */
class Lib_Mvc_PHPErrorException extends Exception
{
	/**
	 * Rodzaj błędu (patrz też: stałe zdefiniowane przez php zacztynające się na E_)
	 * @var int
	 */
    protected $severity;
    
    /**
     * Zachowany prawdziwy tekst błędu zwrócony przez PHP.
     * @var string
     */
    protected $realMessage;
   
    /**
     * Konstruktor błędu wyrzucanego gdy wystąpi phpowy błąd.
     * @param string $message
     * @param integer $code
     * @param integer $severity
     * @param string $filename
     * @param integer $lineno
     */
    public function __construct($message, $code, $severity, $filename, $lineno)
    {
        $this->realMessage = $message;
        $this->message = "Wystąpił błąd wewnętrzny systemu.";
        $this->code = $code;
        $this->severity = $severity;
        $this->file = $filename;
        $this->line = $lineno;
    }
   
    /**
     * Pobiera rodzaj błędu.
     */
    public function getSeverity()
    {
        return $this->severity;
    }
    
    /**
     * Pobiera prawdziwą wiadomość błędu zwróconą przez PHP.
     */
    public function getRealMessage()
    {
    	return $this->realMessage;
    }
}