<?php
class Controller_Search extends Lib_Controller
{
	protected $_requiredUserLevel = LEVEL_UNLOGGED;

	public function playersAction()
	{
		$this->view->assign('siteTitle', "players search");

		$playersForm = new View_Form_SearchPlayers();
		$playersForm->setValues($_REQUEST);

		$players = array();
		$limit = 20;
		$page = isset($_GET['page'])?(int)$_GET['page']:1;
		$count = 0;
		if ( $playersForm->validate() )
		{
			$select = Model_Yae_Search::getPlayersSelect();
			$values = $playersForm->getValues();
			foreach ( $values as $key => $value )
			{
				if(!empty($value))
				{
					if ( $key == 'ip' )
					{
						$start = $end = $value;
						if( preg_match('/^([0-9]*)\.?([0-9*%]*)\.?([0-9*%]*)\.?([0-9*%]*)$/', trim($value), $matches ) )
						{
							$d = $matches[1];
							$c = $matches[2];
							$b = $matches[3];
							$a = $matches[4];
							if( empty($a) || !is_numeric($a) )
							{
								if( empty($b) || !is_numeric($b) )
								{
									if( empty($c) || !is_numeric($c) )
									{
										$start = "$d.0.0.0";
										$end = "$d.255.255.255";
									}
									else
									{
										$start = "$d.$c.0.0";
										$end = "$d.$c.255.255";
									}
								}
								else
								{
									$start = "$d.$c.$b.0";
									$end = "$d.$c.$b.255";	
								}
							}
							else
							{
								$start = "$d.$c.$b.$a";
								$end = "$d.$c.$b.$a";
							}
						}
						$select->where("AND ip >= INET_ATON(?) AND ip <= INET_ATON(?)", array($start, $end));
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

	public function serversAction()
	{
		$this->view->assign('siteTitle', "servers search");

		$serversForm = new View_Form_SearchServers();
		$serversForm->setValues($_REQUEST);

		$servers = array();
		$limit = 20;
		$page = isset($_GET['page'])?(int)$_GET['page']:1;
		$count = 0;
		if ( $serversForm->validate() )
		{
			$select = Model_Yae_Search::getServersSelect();
			$values = $serversForm->getValues();
			foreach ( $values as $key => $value )
			{
				if(!empty($value))
				{
					if ( $key == 'ip' )
					{
						$select->where("AND ip = INET_ATON(?)", array($value));
					}
					else
					{
						if ( in_array($key, array('port')) )
						{
							$select->where("AND $key = ?",array($value));
						}
					}
				}
			}
			$select->page($page,$limit);
			$servers = $select->getAll();
			$count = Tmvc_Model_Mysql::getConnection()->getCountCalcRows();
		}
		$this->view->assign('limit', $limit);
		$this->view->assign('page', $page);
		$this->view->assign('count', $count);
		$this->view->assign('servers', $servers);
		$this->view->assign('serversForm', $serversForm);
	}

	public function showPlayerAction()
	{
		$this->view->assign('siteTitle', "player details");

		$history = array();
		$limit = 20;
		$page = isset($_GET['page'])?(int)$_GET['page']:1;
		$types = array
		(
			"monthly"	=> "monthly stats",
			"exact"		=> "exact times",
			"total"		=> "total stats"
		);
		$type = ( isset($_GET['type']) && in_array( $_GET['type'], array_keys($types) ) ) ? $_GET['type'] : "monthly";
		$count = 0;
		$playerId = isset($_GET['player_id'])?(int)$_GET['player_id']:-1;
		$player = Model_Yae_Search::getPlayersSelect()->where("AND id=?",array($playerId))->getRow();
		if ( $player )
		{
			$playerId = $player['id'];
			if ( $type == "exact" )
			{
				$select = Model_Yae_Search::getPlayerTimesSelect($playerId);
				$select->page($page,$limit);
				$history = $select->getAll();
			}
			else if ( $type == "monthly" )
			{
				$select = Model_Yae_Search::getPlayerTimesMonthlySelect($playerId);
				$select->page($page,$limit);
				$history = $select->getAll();
			}
			else if ( $type == "total" )
			{
				$select = Model_Yae_Search::getPlayerTimesTotalSelect($playerId);
				$select->page($page,$limit);
				$history = $select->getAll();
			}
			$count = Tmvc_Model_Mysql::getConnection()->getCountCalcRows();
		}
		$this->view->assign('player', $player);
		$this->view->assign('history', $history);
		$this->view->assign('limit', $limit);
		$this->view->assign('page', $page);
		$this->view->assign('count', $count);
		$this->view->assign('currentType', $type);
		$this->view->assign('types', $types);
	}

	public function showServerAction()
	{
		$this->view->assign('siteTitle', "server details");

		$history = array();
		$limit = 20;
		$page = isset($_GET['page'])?(int)$_GET['page']:1;
		$types = array
		(
			"lastmonth"	=> "played this month",
			"exact"		=> "dates played",
			"alltime"	=> "all time stats"
		);
		$type = ( isset($_GET['type']) && in_array( $_GET['type'], array_keys($types) ) ) ? $_GET['type'] : "lastmonth";
		$count = 0;
		$serverId = isset($_GET['server_id'])?(int)$_GET['server_id']:-1;
		$server = Model_Yae_Search::getServersSelect()->where("AND id=?",array($serverId))->getRow();
		if ( $server )
		{
			$serverId = $server['id'];
			if ( $type == "exact" )
			{
				$select = Model_Yae_Search::getExactTimesPlayersSelect();
				$select->where("AND serverid=?",array($serverId));
				$select->page($page,$limit);
				$history = $select->getAll();
			}
			else if ( $type == "alltime" )
			{
				$select = Model_Yae_Search::getAllPlayersTimeSelect();
				$select->where("AND serverid=?",array($serverId));
				$select->page($page,$limit);
				$history = $select->getAll();
			}
			else if ( $type == "lastmonth" )
			{
				$select = Model_Yae_Search::getLastMonthPlayersSelect();
				$select->where("AND serverid=?",array($serverId));
				$select->page($page,$limit);
				$history = $select->getAll();
			}
			$count = Tmvc_Model_Mysql::getConnection()->getCountCalcRows();
		}
		$this->view->assign('server', $server);
		$this->view->assign('history', $history);
		$this->view->assign('limit', $limit);
		$this->view->assign('page', $page);
		$this->view->assign('count', $count);
		$this->view->assign('currentType', $type);
		$this->view->assign('types', $types);
	}
};