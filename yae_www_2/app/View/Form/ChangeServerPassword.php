<?php
	class View_Form_ChangeServerPassword extends Tmvc_View_Form
	{
		protected $_captchaInput;
		
		public function __construct()
		{
			parent::__construct();
			$this->setCssClass("yae form");
			$this->setMethod("post");
			$this->addInputSection("changeServerPassword", "Change server password");
			$this->addInput(new Tmvc_View_Form_Input_Password("rcon", "New rcon password"))
				->addValidator( new Tmvc_View_Form_Validator_StringLength(1,500) );
			$this->_captchaInput = new View_Form_Input_Captcha("captcha", "Captcha");
			$this->_captchaInput->addValidator( new View_Form_Validator_Captcha() );
			$this->addInput($this->_captchaInput);
			$submit = new Tmvc_View_Form_Button_Submit("change", "change");
			$this->addButton($submit);
		}
		
		public function clearCaptcha()
		{
			$this->_captchaInput->setValue("");
		}
	}