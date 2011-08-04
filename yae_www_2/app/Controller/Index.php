<?php
class Controller_Index extends Lib_Controller
{
	protected $_requiredUserLevel = LEVEL_UNLOGGED;
	
	public function indexAction()
	{
		$this->view->assign('siteTitle', "Home");
		
		$this->view->assign('playersForm', new View_Form_SearchPlayers());
		$this->view->assign('serversForm', new View_Form_SearchServers());
	}
};