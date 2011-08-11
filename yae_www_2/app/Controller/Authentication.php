<?php
class Controller_Authentication extends Lib_Controller
{
	protected $_requiredUserLevel = LEVEL_UNLOGGED;
	
	protected function _randomizeCaptcha()
	{
		Lib_Captcha::generateCode();
	} 
	
	public function initView()
	{
		parent::initView();
		if ( Model_Auth::getInstance()->isLogged() )
		{
			$this->_redirect("Index", "index");
		}
	}
	
	public function registerAction()
	{
		$registerForm = new View_Form_Registration();
		$registerForm->setValues($_REQUEST);
		
		if ( !empty($_POST) && $registerForm->validate() )
		{
			$values = $registerForm->getValues();
			$login = $values['login'];
			$password = $values['password'];
			$email = $values['email'];
			$userId = Model_User_Authentication::register($login,$password,$email);
			$this->_setSession("registeredLogin", $login);
			$this->_setSession("registeredMail", $email);
			$this->_setSession("justRegistered", 1);
			$this->_redirect("Authentication", "registerSuccess");
		}
		
		$this->_randomizeCaptcha();
		$registerForm->clearCaptcha();
		$this->view->assign("siteTitle", "registration");
		$this->view->assign("registerForm", $registerForm);
	}
	
	public function registerSuccessAction()
	{
		if ( !$this->_getSession("justRegistered") )
		{
			$this->_redirect("Index","index");
		}
		$this->view->assign("registeredEmail", $this->_getSession("registeredMail") );
		$this->view->assign("registeredLogin", $this->_getSession("registeredLogin") );
		$this->view->assign("siteTitle", "registration successful");
	}
	
	public function loginAction()
	{
		$loginForm = new View_Form_Login();
		$loginForm->setValues($_REQUEST);
		
		if( !empty($_POST) && $loginForm->validate() )
		{
			$values = $loginForm->getValues();
			if ( Model_Auth::getInstance()->tryToLogIn($values['login'], $values['password']) )
			{
				$this->_redirect("Index","index");
			}
			$loginForm->getInput("login", "login")->setErrorText("The login and/or password you provided are incorrect.");
		}
		$this->view->assign("siteTitle", "login");
		$this->view->assign("loginForm", $loginForm);
	}
	
	public function logoutAction()
	{
		Model_Auth::getInstance()->logout();
		$this->_redirect("Index","index");
	}
	
	public function forgotPasswordAction()
	{
		$key = $this->_get("key");
		$userId = $this->_get("userId");
		$time = $this->_get("time");
		
		$this->view->assign("noArguments", 0);
		$this->view->assign("keyTooOld", 0);
		$this->view->assign("keyIncorrect", 0);
		$this->view->assign("userNotFound", 0);
		$this->view->assign("notActivated", 0);
		$this->view->assign("keyCorrect", 0);
		$this->view->assign("keyIncorrect", 0);
		$this->view->assign("passwordChanged", 0);
		if ( !$userId || !$key || !$time )
		{
			$this->view->assign("noArguments", 1);
		}
		else if ( time() - $time > (7*24*60*60) )
		{
			$this->view->assign("keyTooOld", 1);
		}
		else
		{
			$user = Tmvc_Model_Mysql::getConnection()->getRow("SELECT * FROM users WHERE userid=?", $userId);
			if ( !$user ) 
			{
				$this->view->assign("userNotFound", 1);
			}
			else if ( !$user['confirmed_mail'] )
			{
				$this->view->assign("notActivated", 1);
			}
			else if ( Model_User_Authentication::validateForgotPasswordKey($key, $userId, $user['username'], $time, $user['password']) )
			{
				$this->view->assign("keyCorrect", 1);
				$changePasswordForm = new View_Form_ForgotChangePassword();
				$changePasswordForm->setValues($_REQUEST);
				$this->view->assign("changePasswordForm", $changePasswordForm);
				
				if( !empty($_POST) && $changePasswordForm->validate() )
				{
					$values = $changePasswordForm->getValues();
					$newPassword = $values['password'];
					
					Model_Auth::getInstance()->setPassword($newPassword, $userId, true);
					$this->view->assign("passwordChanged", 1);
					
				}
			}
			else
			{
				$this->view->assign("keyIncorrect", 1);
			}
		}
		
		
		$this->view->assign("siteTitle", "change forgotten password");
	}
	
	public function requestForgotPasswordSentAction()
	{
		if ( !$this->_getSession("justRequestedPassword") )
		{
			$this->_redirect("Index","index");
		}
		$this->view->assign("requestedMail", $this->_getSession("requestedMail") );
		$this->view->assign("siteTitle", "e-mail message sent");
	}
	
	public function requestForgotPasswordAction()
	{
		$forgotForm = new View_Form_ForgotPassword();
		$forgotForm->setValues($_REQUEST);
		
		if( !empty($_POST) && $forgotForm->validate() )
		{
			$values = $forgotForm->getValues();
			$user = Tmvc_Model_Mysql::getConnection()->getRow("SELECT * FROM users WHERE email=?", $values['email']);
			if ( !$user )
			{
				$forgotForm->getInput("forgotpassword", "email")->setErrorText("There is no account connected with the email address you've provided.");
			}
			else
			{
				Model_User_Authentication::forgotPasswordMail($user);
				$this->_setSession("justRequestedPassword", 1);
				$this->_setSession("requestedMail", $values['email']);
				$this->_redirect("Authentication", "requestForgotPasswordSent");
			}
		}
		
		$this->_randomizeCaptcha();
		$forgotForm->clearCaptcha();
		$this->view->assign("siteTitle", "forgotten password");
		$this->view->assign("forgotForm", $forgotForm);
	}
	
	public function resendActivationAction()
	{
		$resendForm = new View_Form_ResendActivation();
		$resendForm->setValues($_REQUEST);
		
		if( !empty($_POST) && $resendForm->validate() )
		{
			$values = $resendForm->getValues();
			$user = Tmvc_Model_Mysql::getConnection()->getRow("SELECT * FROM users WHERE email=?", $values['email']);
			if ( !$user )
			{
				$resendForm->getInput("resendactivation", "email")->setErrorText("There is no account connected with the email address you've provided.");
			}
			else if ( $user['confirmed_mail'] )
			{
				$resendForm->getInput("resendactivation", "email")->setErrorText("This account is already active.");
			}
			else
			{
				Model_User_Authentication::sendActivationMail($user['userid'], $user['username'], $user['email']);
				$this->_setSession("justResentActivation", 1);
				$this->_setSession("resentActivationMail", $values['email']);
				$this->_redirect("Authentication", "resentActivationSuccess");
			}
		}
		
		$this->_randomizeCaptcha();
		$resendForm->clearCaptcha();
		$this->view->assign("siteTitle", "resend activation link");
		$this->view->assign("resendForm", $resendForm);
	}
	
	public function resentActivationSuccessAction()
	{
		if ( !$this->_getSession("justResentActivation") )
		{
			$this->_redirect("Index","index");
		}
		$this->view->assign("resentActivationMail", $this->_getSession("resentActivationMail") );
		$this->view->assign("siteTitle", "activation mail resent successful");
	}
	
	public function activationAction()
	{
		$time = $this->_get('time');
		$userId = $this->_get("userId");
		$key = $this->_get("key");
		
		$this->view->assign("noArguments", 0);
		$this->view->assign("keyTooOld", 0);
		$this->view->assign("keyIncorrect", 0);
		$this->view->assign("userNotFound", 0);
		$this->view->assign("alreadyActivated", 0);
		$this->view->assign("activated", 0);
		$this->view->assign("keyIncorrect", 0);
		$this->view->assign("siteTitle", "account activation");
		if ( !$userId || !$key || !$time )
		{
			$this->view->assign("noArguments", 1);
		}
		else if ( time() - $time > (7*24*60*60) )
		{
			$this->view->assign("keyTooOld", 1);
		}
		else
		{
			$user = Tmvc_Model_Mysql::getConnection()->getRow("SELECT * FROM users WHERE userid=?", $userId);
			if ( !$user ) 
			{
				$this->view->assign("userNotFound", 1);
			}
			else if ( $user['confirmed_mail'] )
			{
				$this->view->assign("alreadyActivated", 1);
			}
			else if ( Model_User_Authentication::activate($key, $userId, $user['username'], $time) )
			{
				$this->view->assign("activated", 1);
			}
			else
			{
				$this->view->assign("keyIncorrect", 1);
			}
		}
	}
	
	public function captchaAction()
	{
		Lib_Captcha::renderCaptcha();
		die();
	}
	
};