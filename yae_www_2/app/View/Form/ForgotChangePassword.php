<?php
	class View_Form_ForgotChangePassword extends Tmvc_View_Form
	{
		protected $_captchaInput;
		
		public function __construct()
		{
			parent::__construct();
			$this->setCssClass("yae form");
			$password = new Tmvc_View_Form_Input_Password("password", "Password");
			$password->addValidator( new Tmvc_View_Form_Validator_StringLength(6,500) );
			$confirmPassword = new Tmvc_View_Form_Input_Password("password_confirm", "Repeat password");
			$confirmPassword->addValidator( new Tmvc_View_Form_Validator_Confirm("password", "Password") );
			/*$keyInput = new Tmvc_View_Form_Input_Hidden("forgot_key", "forgot_key");
			$useridInput = new Tmvc_View_Form_Input_Hidden("forgot_userid", "forgot_userid");
			$timeInput = new Tmvc_View_Form_Input_Hidden("forgott_time", "forgot_time");*/
			$submit = new Tmvc_View_Form_Button_Submit("forgotpassword", "Change password");
			$this->addInputSection("forgotpassword", "Change password");
			$this->addInput($password);
			$this->addInput($confirmPassword);
			/*$this->addInput($keyInput);
			$this->addInput($useridInput);
			$this->addInput($timeInput);*/
			$this->addButton($submit);
		}
	}