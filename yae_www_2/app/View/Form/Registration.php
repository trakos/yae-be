<?php
	class View_Form_Registration extends Tmvc_View_Form
	{
		protected $_captchaInput;
		
		public function __construct()
		{
			parent::__construct();
			$this->setCssClass("yae form");
			$login = new Tmvc_View_Form_Input_Text("login", "Login");
			$login->addValidator( new Tmvc_View_Form_Validator_StringLength(3,16) );
			$login->addValidator( new View_Form_Validator_Login() );
			$password = new Tmvc_View_Form_Input_Password("password", "Password");
			$password->addValidator( new Tmvc_View_Form_Validator_StringLength(6,500) );
			$confirmPassword = new Tmvc_View_Form_Input_Password("password_confirm", "Repeat password");
			$confirmPassword->addValidator( new Tmvc_View_Form_Validator_Confirm("password", "Password") );
			$mail = new Tmvc_View_Form_Input_Text("email", "Your e-mail address");
			$mail->addValidator( new Tmvc_View_Form_Validator_StringLength(3,500) );
			$mail->addValidator( new Tmvc_View_Form_Validator_Email() );
			$mail->addValidator( new View_Form_Validator_UniqueEmail() );
			$captcha = new View_Form_Input_Captcha("captcha", "Captcha");
			$captcha->addValidator( new View_Form_Validator_Captcha() );
			$submit = new Tmvc_View_Form_Button_Submit("register", "create account");
			$this->addInputSection("registration", "Registration");
			$this->addInput($login);
			$this->addInput($mail);
			$this->addInput($password);
			$this->addInput($confirmPassword);
			$this->addInput($captcha);
			$this->addButton($submit);
			$this->_captchaInput = $captcha;
		}
		
		public function clearCaptcha()
		{
			$this->_captchaInput->setValue("");
		}
	}