<?php
	class View_Form_SearchPlayers extends Tmvc_View_Form
	{
		public function __construct()
		{
			parent::__construct();
			$this->setMethod("get");
			$this->setCssClass("simpleForm");
			$this->addInputSection("search", "Search for players");
			$this->addInput(new Tmvc_View_Form_Input_Text("nick", "Nick"));
			$this->addInput(new Tmvc_View_Form_Input_Text("ip", "IP"));
			$this->addInput(new Tmvc_View_Form_Input_Text("pbguid", "PunkBuster guid"));
			$this->addInput(new Tmvc_View_Form_Input_Text("etproguid", "ET Pro guid"));
			$this->addInput(new Tmvc_View_Form_Input_Text("slacid", "SLAC id"));
			$this->addInput(new Tmvc_View_Form_Input_Text("slacnick", "SLAC nick"));
			$this->addInput(new Tmvc_View_Form_Input_Hidden("c", "controller"))->setValue("Search");
			$this->addInput(new Tmvc_View_Form_Input_Hidden("a", "action"))->setValue("players");
			$submit = new Tmvc_View_Form_Button_Submit("search", "search");
			$this->addButton($submit);
			$this->setAction( Tmvc_View::getInstance()->link("Search", "players") );
		}
	}