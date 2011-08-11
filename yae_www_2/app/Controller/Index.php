<?php
class Controller_Index extends Lib_Controller
{
	protected $_requiredUserLevel = LEVEL_UNLOGGED;
	
	public function indexAction()
	{
		$this->view->assign('siteTitle', "Home");
		
		$playersForm = new View_Form_SearchPlayers();
		$playersForm->setCssClass("yae");
		$serversForm = new View_Form_SearchServers();
		$serversForm->setCssClass("yae");
		$this->view->assign('playersForm', $playersForm);
		$this->view->assign('serversForm', $serversForm);
	}
	
	public function contactAction()
	{
		$contactForm = new View_Form_ContactForm();
		$contactForm->setValues($_REQUEST);
		
		if ( isset($_POST['captcha']) && $contactForm->validate() )
		{
			$values = $contactForm->getValues();
			
			$message = $values['message'];
			
			$subject = "YAE CONTACT FORM";
			$text = $message;
			$text .= "\n\n\n\nXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX\n\n\n\n";
			$text .= "logged: ".( Model_Auth::getInstance()->isLogged() ? Model_Auth::getInstance()->getUserId() : "no" );
			$text .= "\n\n";
			$text .= "ip: ".Model_Auth::getInstance()->getUserIP();
			$text .= "\n\n";
			$text .= print_r($_SERVER,true);
			$text .= "\n\n";
			
			$headers = 'From: '.(Model_Auth::getInstance()->isLogged() ? Model_Auth::getInstance()->get("username") : "YAEFORM").' <'.(Model_Auth::getInstance()->isLogged() ? Model_Auth::getInstance()->get("email") : "yae@trakos.pl").'>' . "\r\n";
			$headers .= 'Content-Type: text/plain; charset="utf-8"' . "\r\n";
			mail("yae@trakos.pl", $subject, $text, $headers);
			
			$this->_redirect("Index", "contactSuccess");
		}
		
		Lib_Captcha::generateCode();
		$contactForm->clearCaptcha();
		$this->view->assign("siteTitle", "contact");
		$this->view->assign("contactForm", $contactForm);
	}
	
	public function contactSuccessAction()
	{
		$this->view->assign("siteTitle", "message send");
	}
	
};