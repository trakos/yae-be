<?php
class Controller_Search extends Lib_Controller
{
	protected $_requiredUserLevel = LEVEL_UNLOGGED;
	
	public function playersAction()
	{
		$this->view->assign('siteTitle', "Home");
		
		$playersForm = new View_Form_SearchPlayers();
		$playersForm->setCssClass("search_form");
		$playersForm->setValues($_REQUEST);
		
		$players = array();
		if ( $playersForm->validate() )
		{
			$select = Model_Yae::getPlayersSelect();
			$values = $playersForm->getValues();
			foreach ( $values as $key => $value )
			{
				if ( $value )
				{
					$select->where("AND $key = ?", array($value));
				}
			}
			$select->page(1,5);
			$players = $select->getAll();	
		}
		
		$this->view->assign('players', $players);
		$this->view->assign('playersForm', $playersForm);
	}
};