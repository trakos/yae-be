<?php
class Controller_Stats extends Lib_Controller
{
	protected $_requiredUserLevel = LEVEL_UNLOGGED;

	public function statsAction()
	{
		$this->view->assign('siteTitle', "statistics");

		$history = array();
		$limit = 20;
		$page = isset($_GET['page'])?(int)$_GET['page']:1;
		$types = array
		(
			"slacmonth"	=> "Most active players (by SLAC ids) this month",
			"pbmonth"	=> "Most active players (by PunkBuster guids) this month",
			"plrsmonth"	=> "Most active players (by nick/guid combinations) this month",
			"srvmonth"	=> "Most active servers this month",
			"slacall"	=> "Most active players (by SLAC ids) all time",
			"pball"		=> "Most active players (by PunkBuster guids) all time",
			"plrsall"	=> "Most active players (by nick/guid combinations) all time",
			"srvall"	=> "Most active servers all time",
		);
		$type = ( isset($_GET['type']) && in_array( $_GET['type'], array_keys($types) ) ) ? $_GET['type'] : "slacmonth";
		$count = 0;
		if ( $type == "slacall" )
		{
			$select = Model_Yae_Stats::getMostActiveCacheBySlac();
		}
		else if ( $type == "slacmonth" )
		{
			$select = Model_Yae_Stats::getMostActiveBySlacLastMonth();
		}
		else if ( $type == "pball" )
		{
			$select = Model_Yae_Stats::getMostActiveCacheByPb();
		}
		else if ( $type == "pbmonth" )
		{
			$select = Model_Yae_Stats::getMostActiveByPbLastMonth();
		}
		else if ( $type == "plrsall" )
		{
			$select = Model_Yae_Stats::getMostActiveCache();
		}
		else if ( $type == "plrsmonth" )
		{
			$select = Model_Yae_Stats::getMostActiveLastMonth();
		}
		else if ( $type == "srvall" )
		{
			$select = Model_Yae_Stats::getMostActiveServersCache();
		}
		else if ( $type == "srvmonth" )
		{
			$select = Model_Yae_Stats::getMostActiveServersLastMonth();
		}
		$select->page($page,$limit);
		$history = $select->getAll();
		$count = Tmvc_Model_Mysql::getConnection()->getCountCalcRows();
		$this->view->assign('history', $history);
		$this->view->assign('limit', $limit);
		$this->view->assign('page', $page);
		$this->view->assign('count', $count);
		$this->view->assign('currentType', $type);
		$this->view->assign('types', $types);
	}
	
	public function cacheAction()
	{
		Model_Yae_Stats::refreshStatsCache();
		die();
	}
};