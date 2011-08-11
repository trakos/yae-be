<?php
	class View_Form_ContactForm extends Tmvc_View_Form
	{
		protected $_captchaInput;
		
		public function __construct()
		{
			parent::__construct();
			$this->setCssClass("yae form");
			$message = new Tmvc_View_Form_Input_TextArea("message", "Message being sent");
			$message->addValidator( new Tmvc_View_Form_Validator_StringLength(3,2000) );
			$this->_captchaInput = new View_Form_Input_Captcha("captcha", "Captcha");
			$this->_captchaInput->addValidator( new View_Form_Validator_Captcha() );
			$submit = new Tmvc_View_Form_Button_Submit("sendMessage", "send message");
			$this->addInputSection("sendMessage", "Contact");
			$this->addInput($message);
			$this->addInput($this->_captchaInput);
			$this->addButton($submit);
		}
		
		public function clearCaptcha()
		{
			$this->_captchaInput->setValue("");
		}
	}