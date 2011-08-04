<?php
class Controller_Index extends Lib_Controller
{
	protected $_requiredUserLevel = LEVEL_UNLOGGED;
	
	public function indexAction()
	{
		$this->view->assign('siteTitle', "Home");
		
		$this->view->assign('playersForm', new View_Form_SearchPlayers());
		$this->view->assign('serversForm', new View_Form_SearchServers());
		
		$this->view->assign("loginForm", $form);
		
		$this->view->assign("failureMsg", $this->_get("failureMsg"));
 		$this->view->assign("successMsg", $this->_get("successMsg"));
 		if ( isset($_POST['login']) && isset($_POST['password']) )
 		{
 			$login = $_POST['login'];
 			$password = $_POST['password'];
 			if ( Model_Auth::getInstance()->isLogged() )
 			{
 				$this->view->assign("failureMsg", "Użytkownik jest już zalogowany!");
 			}
 			else if ( !Model_Auth::getInstance()->tryToLogIn($login, $password) )
 			{
 				$this->view->assign("failureMsg", "Dane niepoprawne.");
 			}
 			else
 			{
 				$this->view->assign("successMsg", "Użytkownik zalogowany poprawnie!");
 			}
 		}
	}
	
 	public function logoutAction()
 	{
 		$this->view->assign('wasLogged', Model_Auth::getInstance()->isLogged() );
 		Model_Auth::getInstance()->logout();
 	}
	
 	public function changepasswordAction()
 	{
 		if ( !Model_Auth::getInstance()->isLogged() )
 		{
 			throw new Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Musisz być zalogowany aby móc zmienić swoje hasło!");
 		}
 		$this->view->assign("failureMsg", "");
 		$this->view->assign("successMsg", "");
 		$this->view->assign("passwordChanged", false);
 		if ( isset($_POST['password1']) && isset($_POST['password2']) )
 		{
 			$password1 = $_POST['password1'];
 			$password2 = $_POST['password2'];
 			if ( $password1 != $password2 )
 			{
 				$this->view->assign("failureMsg", "Hasła różnią się!");
 			}
 			else if ( mb_strlen($password1,"UTF-8") < 6 )
 			{
 				$this->view->assign("failureMsg", "Hasło musi mieć przynajmniej 6 znaków!");
 			}
 			else if ( mb_strlen($password1,"UTF-8") > 20 )
 			{
 				$this->view->assign("failureMsg", "Hasło nie może przekraczać 20 znaków!");
 			}
 			else
 			{
 				Model_Auth::getInstance()->setPassword($password1);
 				$this->view->assign("successMsg", "Hasło zmienione!");
 				$this->view->assign("passwordChanged", true);
 			}
 		}
 	}
};