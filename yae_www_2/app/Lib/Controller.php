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
		$this->addStyle("icons.css");
	}
	
	public function useSessionMessages()
	{
		$this->view->assign("errorMessage", $this->_getSession("errorMessage") );
		$this->view->assign("successMessage", $this->_getSession("successMessage") );
		$this->view->assign("infoMessage", $this->_getSession("infoMessage") );
		$this->_unsetSession("errorMessage");
		$this->_unsetSession("successMessage");
		$this->_unsetSession("infoMessage");
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
		$this->view->assign("currentController", Tmvc_View::$controllerName );
		$this->view->assign("currentAction", Tmvc_View::$actionName );
		$this->addJavascript("jquery.js");
		$this->addJavascript("jquery.query.js");
		$this->addJavascript("jquery.slideOut.js");
		$this->addJavascript("yae.js");
		$jsFile = Tmvc_View::$controllerName.'/'.Tmvc_View::$actionName.'.js';
		if ( file_exists(SRC_PATH_JS.'/'.$jsFile) )
		{
			$this->addJavascript($jsFile);
		} 
	}
};