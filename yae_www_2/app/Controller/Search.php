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
		$limit = 20;
		$page = isset($_GET['page'])?(int)$_GET['page']:1;
		$count = 0;
		if ( $playersForm->validate() )
		{
			$select = Model_Yae::getPlayersSelect();
			$values = $playersForm->getValues();
			foreach ( $values as $key => $value )
			{
				if(!empty($value))
				{
					if ( $key == 'ip' )
					{
						if (preg_match('/^([0-9]*)\.([0-9]*)\.([0-9]*)\.(.*)$/', $value, $matches ))
						{
							$value = "$matches[1].$matches[2].$matches[3].0";
						}
						$select->where("AND (ip & 4294967040) = (INET_ATON(?) & 4294967040)", array($value));
					}
					else if ( $key == 'nick' )
					{
						$select->where("AND (nick LIKE ? OR colorless LIKE ? )", array("%$value%","%$value%"));
					}
					else if( $key == 'slacid' )
					{
						$select->where("AND slacid = ?",array($value));
					}
					else if ( in_array($key,array("pbguid", "etproguid", "slacnick")) )
					{
						$select->where("AND $key LIKE ?",array("%$value%"));
					}
				}
			}
			$select->page($page,$limit);
			$players = $select->getAll();
			$count = Tmvc_Model_Mysql::getConnection()->getCountCalcRows();
		}
		$this->view->assign('limit', $limit);
		$this->view->assign('page', $page);
		$this->view->assign('count', $count);
		$this->view->assign('players', $players);
		$this->view->assign('playersForm', $playersForm);
	}
};