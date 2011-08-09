<?php
	class View_Form_Login extends Tmvc_View_Form
	{
		public function __construct()
		{
			parent::__construct();
			$this->setCssClass("yae form");
			$this->setMethod("post");
			$login = new Tmvc_View_Form_Input_Text("login", "login");
			$login->addValidator( new Tmvc_View_Form_Validator_StringLength(3,16) );
			$password = new Tmvc_View_Form_Input_Password("password", "password");
			$password->addValidator( new Tmvc_View_Form_Validator_StringLength(6,500) );
			$submit = new Tmvc_View_Form_Button_Submit("submitLogin", "login");
			$this->addInputSection("login", "Log in");
			$this->addInput($login);
			$this->addInput($password);
			$this->addButton($submit);
		}
	}