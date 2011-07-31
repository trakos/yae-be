<?php
/**
 * Klasa podstawowa kontrolera
 */
class Lib_Controller
{
	/**
	 * Widok
	 * @var Lib_View
	 */
	public $view;
	
	/**
	 * Jaki poziom użytkownika jest wymagany, aby miał dostęp do tego kontrolera (najbezpieczniej najwyższy jako wartość domyślny).
	 */
	protected $_requiredUserLevel = LEVEL_SUPERADMIN;
	
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
		$this->view = Lib_View::getInstance();
		$this->init();
	}
	
	/**
	 * Dodanie pliku javascript do sekcji head layoutu.
	 */
	public function addJavascript($path)
	{
		Lib_Includer_Javascript::getInstance()->addFile($path);
	}
	
	/**
	 * Dodanie pliku css do sekcji head layoutu.
	 */
	public function addStyle($path)
	{
		Lib_Includer_Css::getInstance()->addFile($path);
	}
	
	/**
	 * Wypisuje zmienną z php w formie przypisania wartości javascriptu
	 * @param string $name nazwa zmiennej, której chcemy nadać wartość
	 * @param string $value wartość, którą ma przyjąć
	 */
	public function addJavascriptVariable($name,$value)
	{
		Lib_Includer_Javascript::getInstance()->addScriptText("var ".$name." = ".json_encode($value).";\n");
	}
	
	/**
	 * Metoda służąca do przekierowania na inną stronę wewnątrz systemu.
	 * @param string $controller
	 * @param string $action
	 * @param array $arguments
	 */
	protected function _redirect($controller,$action='Index',$arguments=array())
	{
		$url = Lib_View::getInstance()->link($controller,$action,$arguments,false);
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
	public function init()
	{
		$this->addStyle("style.css");
		$this->addStyle("1/layout.css");
		$this->addStyle("style.css");
	}
	
	/**
	 * Inicjalizacja obiektu zachodząca tuż przed wywołaniem widoku, już po wywołaniu akcji
	 */
	public function initView()
	{
		$isLogged = Model_Auth::getInstance()->isLogged();
		$userNameAndSurname = $isLogged ? Model_Auth::getInstance()->get("imie")." ".Model_Auth::getInstance()->get("nazwisko") : "";
		$userLogin = $isLogged ? Model_Auth::getInstance()->getUserId() : "";
		$userLevel = Model_Auth::getInstance()->getUserLevel();
		$userLevelName = $isLogged ? Model_Mysql::getConnection()->getOne("SELECT nazwa FROM nazwy_poziomow WHERE poziom=?", $userLevel ) : "niezalogowany";
		$this->view->assign("isLogged", $isLogged );
		$this->view->assign("userNameAndSurname", $userNameAndSurname );
		$this->view->assign("userLogin", $userLogin );
		$this->view->assign("userLevel", $userLevel );
		$this->view->assign("userLevelName", $userLevelName );
	}
};