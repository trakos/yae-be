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
		$userId = $isLogged ? Tmvc_Model_Auth::getInstance()->getUserId() : 0;
		$userLevel = Tmvc_Model_Auth::getInstance()->getUserLevel();
		$username = $isLogged ? Tmvc_Model_Auth::getInstance()->get("username") : "guest";
		$this->view->assign("isLogged", $isLogged );
		$this->view->assign("userName", $username );
		$this->view->assign("userId", $userId );
		$this->view->assign("userLevel", $userLevel );
		$this->addJavascript("jquery.js");
		$this->addJavascript("query.js");
		$jsFile = Tmvc_View::$controllerName.'/'.Tmvc_View::$actionName.'.js';
		if ( file_exists(SRC_PATH_JS.'/'.$jsFile) )
		{
			$this->addJavascript($jsFile);
		} 
	}
};