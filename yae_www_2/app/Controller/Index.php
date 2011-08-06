<?php
class Controller_Index extends Lib_Controller
{
	protected $_requiredUserLevel = LEVEL_UNLOGGED;
	
	public function indexAction()
	{
		$this->view->assign('siteTitle', "Home");
		
		$playersForm = new View_Form_SearchPlayers();
		$playersForm->setCssClass("yae");
		$serversForm = new View_Form_SearchServers();
		$serversForm->setCssClass("yae");
		$this->view->assign('playersForm', $playersForm);
		$this->view->assign('serversForm', $serversForm);
	}
};