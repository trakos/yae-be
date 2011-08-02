<?php
/**
 * Klasa podstawowa kontrolera
 */
class Lib_Yae_Controller extends Lib_Mvc_Controller_Abstract
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
		$isLogged = Lib_Mvc_Model_Auth::getInstance()->isLogged();
		$userLogin = $isLogged ? Lib_Mvc_Model_Auth::getInstance()->getUserId() : "";
		$userLevel = Lib_Mvc_Model_Auth::getInstance()->getUserLevel();
		$userNameAndSurname = $isLogged ? Lib_Mvc_Model_Auth::getInstance()->get("imie")." ".Lib_Mvc_Model_Auth::getInstance()->get("nazwisko") : "guest";
		$this->view->assign("siteTitle", "TODO" );
		$this->view->assign("isLogged", $isLogged );
		$this->view->assign("userLogin", $userLogin );
		$this->view->assign("userLevel", $userLevel );
	}
};