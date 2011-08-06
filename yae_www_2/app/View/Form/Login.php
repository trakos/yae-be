<?php
	class View_Form_Login extends Tmvc_View_Form
	{
		public function __construct()
		{
			parent::__construct();
			$this->setCssClass("form");
			$login = new Tmvc_View_Form_Input_Text("login", "login");
			$passwd = new Tmvc_View_Form_Input_Password("password", "password");
			$submit = new Tmvc_View_Form_Button_Submit("login", "login");
			$this->addInputSection("login", "Log in");
			$this->addInput($login);
			$this->addInput($passwd);
			$this->addButton($submit);
		}
	}