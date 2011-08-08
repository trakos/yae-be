<?php
	class View_Form_ForgotPassword extends Tmvc_View_Form
	{
		public function __construct()
		{
			parent::__construct();
			$this->setCssClass("yae form");
			$mail = new Tmvc_View_Form_Input_Text("email", "Account e-mail address");
			$mail->addValidator( new Tmvc_View_Form_Validator_StringLength(3,500) );
			$mail->addValidator( new Tmvc_View_Form_Validator_Email() );
			$submit = new Tmvc_View_Form_Button_Submit("forgotpassword", "Send change password link");
			$this->addInputSection("forgotpassword", "Forgotten password");
			$this->addInput($mail);
			$this->addButton($submit);
		}
	}