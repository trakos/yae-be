<?php
/**
 * Klasa podstawowa kontrolera
 */
class Lib_Controller extends Tmvc_Controller_Abstract
{
	/**
	 * Jaki poziom użytkownika jest wymagany, aby miał dostęp do tego kontrolera (najbezpieczniej najwyższy jako wartość domyślny).
	 */
	protected $_requiredUserLevel = LEVEL_SUPERADMIN;
	
	/**
	 * Inicjalizacja obiektu
	 */
	public function init()
	{
		$this->addStyle("style.css");
		$this->addStyle("2/layout.css");
	}
	
	/**
	 * Inicjalizacja obiektu zachodząca tuż przed wywołaniem widoku, już po wywołaniu akcji
	 */
	public function initView()
	{
		$isLogged = Tmvc_Model_Auth::getInstance()->isLogged();
		$userLogin = $isLogged ? Tmvc_Model_Auth::getInstance()->getUserId() : "";
		$userLevel = Tmvc_Model_Auth::getInstance()->getUserLevel();
		$userNameAndSurname = $isLogged ? Tmvc_Model_Auth::getInstance()->get("imie")." ".Tmvc_Model_Auth::getInstance()->get("nazwisko") : "guest";
		$this->view->assign("isLogged", $isLogged );
		$this->view->assign("userLogin", $userLogin );
		$this->view->assign("userLevel", $userLevel );
	}
};