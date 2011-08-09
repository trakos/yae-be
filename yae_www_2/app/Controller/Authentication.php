<?php
class Controller_Authentication extends Lib_Controller
{
	protected $_requiredUserLevel = LEVEL_UNLOGGED;
	
	protected function _randomizeCaptcha()
	{
		$possible = '23456789bcdfghjkmnpqrstvwxyz';
		$code = '';
		$i = 0;
		while ($i < 6) {
			$code .= substr($possible, mt_rand(0, strlen($possible)-1), 1);
			$i++;
		}
		$this->_setSession("captcha", $code);
		return $code;
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
		$captchaText = $this->_randomizeCaptcha();
		
		//determine width and height for our image and create it
		$imgW = 200;
		$imgH = 50;
		$image = imagecreatetruecolor($imgW, $imgH);
		 
		//setup background color and border color
		$backgr_col = imagecolorallocate($image, 238,239,239);
		$border_col = imagecolorallocate($image, 208,208,208);
		 
		//let's choose color in range of purple color
		$text_col = imagecolorallocate($image, rand(80,100),rand(50,60),rand(70,80));
		 
		//now fill rectangle and draw border
		//imagefilledrectangle($image, 0, 0, $imgW, $imgH, $backgr_col);
		//imagerectangle($image, 0, 0, $imgW-1, $imgH-1, $border_col);
		
		$bgImage = imagecreatefromjpeg(SRC_PATH_RESOURCES . "/captchabg.jpg");
		imagecopy($image, $bgImage, 0, 0, 0, 0, 200, 50);
		imagedestroy($bgImage);
		
		// add some random lines
		$noise_color = imagecolorallocate($image, 130, 120, 120);
		for( $i=0; $i<10; $i++ ) {
			imageline($image, mt_rand(0,$imgW), mt_rand(0,$imgH), mt_rand(0,$imgW), mt_rand(0,$imgH), $noise_color);
		}
		
		//save fonts in same folder where you PHP captcha script is
		//name these fonts by numbers from 1 to 3
		//we shall choose different font each time
		$fonts = array();
		$iterator = new DirectoryIterator(SRC_PATH_RESOURCES . "/fonts/");
		foreach ( $iterator as $font )
		{
			if ( $font->isFile() )
			{
				$fonts[] = $font->getFilename();
			}
		}
		$fn = rand(1,count($fonts)) - 1;
		$font = SRC_PATH_RESOURCES . "/fonts/" . $fonts[$fn];
		 
		//setup captcha letter size and angle of captcha letters
		$font_size = $imgH / 2.2;
		$box = imagettfbbox($font_size, 0, $font, $captchaText);
		$startX = (int)($imgW - $box[4]-30) / 2;
		$y = (int)($imgH - $box[5]) / 2;
		for ( $i=0;$i<6;$i++ )
		{
			$angle = rand(-15,15);
			$x = $startX + (($box[4]/6) + 5)*$i;
			imagettftext($image, $font_size, $angle, $x, $y, $text_col, $font, $captchaText[$i]);
		}
		 
		//now we should output captcha image
		header("Content-type: image/png");
		imagepng($image);
		imagedestroy ($image);
		die();
	}
	
};