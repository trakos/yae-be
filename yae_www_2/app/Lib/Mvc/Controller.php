<?php
/**
 * Klasa podstawowa kontrolera
 */
abstract class Lib_Mvc_Controller
{
	/**
	 * Widok
	 * @var Lib_Mvc_View
	 */
	public $view;
	
	/**
	 * Jaki poziom użytkownika jest wymagany, aby miał dostęp do tego kontrolera (najbezpieczniej najwyższy jako wartość domyślny).
	 */
	protected $_requiredUserLevel = 99999;
	
	/**
	 * Sprawdzenie, czy obecnie zalogowany użytkownik ma wystarczający poziom, żeby mieć dostęp do tego kontrolera.
	 * @return bool
	 */
	public function canCurrentUserAccess()
	{
		return Model_Auth::getInstance()->isAtLeast($this->_requiredUserLevel);
	} 
	
	/**
	 * Konstruktor, tworzy widok
	 */
	public function __construct()
	{
		$this->view = Lib_Mvc_View::getInstance();
		$this->init();
	}
	
	/**
	 * Dodanie pliku javascript do sekcji head layoutu.
	 */
	public function addJavascript($path)
	{
		Lib_Mvc_Includer_Javascript::getInstance()->addFile($path);
	}
	
	/**
	 * Dodanie pliku css do sekcji head layoutu.
	 */
	public function addStyle($path)
	{
		Lib_Mvc_Includer_Css::getInstance()->addFile($path);
	}
	
	/**
	 * Wypisuje zmienną z php w formie przypisania wartości javascriptu
	 * @param string $name nazwa zmiennej, której chcemy nadać wartość
	 * @param string $value wartość, którą ma przyjąć
	 */
	public function addJavascriptVariable($name,$value)
	{
		Lib_Mvc_Includer_Javascript::getInstance()->addScriptText("var ".$name." = ".json_encode($value).";\n");
	}
	
	/**
	 * Metoda służąca do przekierowania na inną stronę wewnątrz systemu.
	 * @param string $controller
	 * @param string $action
	 * @param array $arguments
	 */
	protected function _redirect($controller,$action='Index',$arguments=array())
	{
		$url = Lib_Mvc_View::getInstance()->link($controller,$action,$arguments,false);
		header( "Location: ".$url );
		die();
	}
	
	/**
	 * Pobranie zmiennej z danych przesłanych przez użytkownika razem z zapytaniem. NULL gdy nie przesłano danej o nazwie $key.
	 * @param string $key
	 * @return string
	 */
	protected function _get( $key )
	{
		if( isset($_REQUEST[$key]) ) {
			return $_REQUEST[$key];
		} else {
			return NULL;
		}
	}
	
	/**
	 * Inicjalizacja obiektu
	 */
	abstract public function init();
	/**
	 * Inicjalizacja obiektu zachodząca tuż przed wywołaniem widoku, już po wywołaniu akcji
	 */
	abstract public function initView();
};