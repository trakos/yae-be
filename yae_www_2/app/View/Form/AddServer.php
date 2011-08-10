<?php
	class View_Form_AddServer extends Tmvc_View_Form
	{
		protected $_captchaInput;
		
		public function __construct()
		{
			parent::__construct();
			$this->setCssClass("yae form");
			$this->setMethod("post");
			$this->addInputSection("addServer", "Add server");
			$this->addInput(new Tmvc_View_Form_Input_Text("ip", "Server IP"))
				->addValidator( new Tmvc_View_Form_Validator_Ip() );
			$this->addInput(new Tmvc_View_Form_Input_Text("port", "Server port"))
				->addValidator( new Tmvc_View_Form_Validator_Port() );
			$this->addInput(new Tmvc_View_Form_Input_Password("rcon", "Server rcon password"))
				->addValidator( new Tmvc_View_Form_Validator_StringLength(1,500) );
			$this->_captchaInput = new View_Form_Input_Captcha("captcha", "Captcha");
			$this->_captchaInput->addValidator( new View_Form_Validator_Captcha() );
			$this->addInput($this->_captchaInput);
			$submit = new Tmvc_View_Form_Button_Submit("addServer", "add server");
			$this->addButton($submit);
		}
		
		public function clearCaptcha()
		{
			$this->_captchaInput->setValue("");
		}
	}