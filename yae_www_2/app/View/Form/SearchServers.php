<?php
	class View_Form_SearchServers extends Tmvc_View_Form
	{
		public function __construct()
		{
			parent::__construct();
			$this->addInputSection("search", "Search for servers");
			$this->addInput(new Tmvc_View_Form_Input_Text("ip", "IP"));
			$this->addInput(new Tmvc_View_Form_Input_Text("port", "Port"));
			$submit = new Tmvc_View_Form_Button_Submit("search", "search");
			$this->addButton($submit);
			$this->setAction( Tmvc_View::getInstance()->link("Search", "servers") );
		}
	}