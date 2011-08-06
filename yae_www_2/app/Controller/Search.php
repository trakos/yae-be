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
		$types = array
		(
			"id"		=> "non-grouped",
			"slac"		=> "group by SLAC",
			"pb"		=> "group by PB",
			"ip"		=> "group by IP",
			"etpro"		=> "group by etpro guid"
		);
		$type = ( isset($_GET['type']) && in_array( $_GET['type'], array_keys($types) ) ) ? $_GET['type'] : "id";
		$count = 0;
		if ( $playersForm->validate() )
		{
			if ( $type == "slac" )
			{
				$select = Model_Yae_Search::getPlayersSlacSelect();
			}
			else if ( $type == "pb" )
			{
				$select = Model_Yae_Search::getPlayersPbSelect();
			}
			else if ( $type == "ip" )
			{
				$select = Model_Yae_Search::getPlayersIpSelect();
			}
			else if ( $type == "etpro" )
			{
				$select = Model_Yae_Search::getPlayersEtproSelect();
			}
			else
			{
				$select = Model_Yae_Search::getPlayersSelect();
			}
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
		$this->view->assign('currentType', $type);
		$this->view->assign('types', $types);
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
			"exact"		=> "exact times",
			"monthly"	=> "monthly stats",
			"total"		=> "all time stats"
		);
		$type = ( isset($_GET['type']) && in_array( $_GET['type'], array_keys($types) ) ) ? $_GET['type'] : "exact";
		$count = 0;
		$playerId = isset($_GET['player_id'])?$_GET['player_id']:-1;
		$idType = isset($_GET['id_type'])?$_GET['id_type']:'id';
		if ( !in_array($idType,array("id","ip","pbguid","slacid","etproguid")) )
		{
			$idType = "id";
		}
		if ( $idType == "id" )
		{
			$player = Model_Yae_Search::getPlayersSelect()->where("AND id=?",array($playerId))->getRow();
		}
		else if ( $idType == "slacid" )
		{
			$select = Model_Yae_Search::getPlayersSelect();
			$select->where("AND slacid=?",array($playerId));
			$select->Where("AND slacnick<>''");
			$select->Where("AND slacnick IS NOT NULL");
			$player = $select->getRow();
			if ( !$player )
			{
				$player = Model_Yae_Search::getPlayersSelect()->where("AND slacid=?",array($playerId))->getRow();
			}
		}
		else if ( $idType == "ip" )
		{
			$player = array($idType=>$playerId,"realip"=>$playerId);
		}
		else
		{
			$player = array($idType=>$playerId);
		}
		if ( $player )
		{
			if ( $type == "exact" )
			{
				$select = Model_Yae_Search::getPlayerTimesSelect();
			}
			else if ( $type == "monthly" )
			{
				$select = Model_Yae_Search::getPlayerTimesMonthlySelect();
			}
			else if ( $type == "total" )
			{
				$select = Model_Yae_Search::getPlayerTimesAllTimeSelect();
			}
			switch ( $idType )
			{
				case "ip":			
				{
					$ip = $playerId;
					if( preg_match('/^([0-9]*)\.?([0-9*%]*)\.?([0-9*%]*)\.?([0-9*%]*)$/', trim($ip), $matches ) )
					{
						$ip = "$matches[1].$matches[2].$matches[3].0";
					}
					$select->where("AND INET_NTOA(players.ip & 4294967040)=?",array($ip));
					break;	
				}
				case "pbguid":		$select->where("AND pbguid LIKE ?",array("%$playerId")); break;
				case "slacid":		$select->where("AND slacid=?",array($playerId)); break;
				case "etproguid":	$select->where("AND etproguid LIKE ?",array("%$playerId")); break;
				default:			$select->where("AND playerid LIKE ?",array($playerId)); break;	
			}
			$select->page($page,$limit);
			$history = $select->getAll();
			$count = Tmvc_Model_Mysql::getConnection()->getCountCalcRows();
		}
		$this->view->assign('idType', $idType);
		$this->view->assign('playerId', $playerId);
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
		$groups = array
		(
			"id"		=> "do not group",
			"ip"		=> "by IP address",
			"etproguid"	=> "by EtPro guid",
			"slacid"	=> "by SLAC id",
			"pbguid"	=> "by PunkBuster guid"
		);
		$groupBy = ( isset($_GET['group_by']) && in_array( $_GET['group_by'], array_keys($groups) ) ) ? $_GET['group_by'] : "id";
		$types = array
		(
			"lastmonth"	=> "last 30 days",
			"exact"		=> "exact times",
			"alltime"	=> "all time stats"
		);
		if ( $groupBy != "id" ) unset($types["exact"]);
		$type = ( isset($_GET['type']) && in_array( $_GET['type'], array_keys($types) ) ) ? $_GET['type'] : "lastmonth";
		$count = 0;
		$serverId = isset($_GET['server_id'])?(int)$_GET['server_id']:-1;
		$server = Model_Yae_Search::getServersSelect()->where("AND id=?",array($serverId))->getRow();
		if ( $server )
		{
			$serverId = $server['id'];
			if ( $type == "exact" )
			{
				$select = Model_Yae_Search::getServerPlayersExactTimes();
			}
			else if ( $type == "alltime" )
			{
				$select = Model_Yae_Search::getServerPlayersAllTimeSelect();
			}
			else if ( $type == "lastmonth" )
			{
				$select = Model_Yae_Search::getServerPlayersLastMonthSelect();
			}
			if ( $groupBy == "ip" )
			{
				$select->group("players.ip");
			}
			else if ( $groupBy == "etproguid" )
			{
				$select->group("etproguid");
			}
			else if ( $groupBy == "slacid" )
			{
				$select->group("slacid");
				$select->where("AND slacid <> 0");
			}
			else if ( $groupBy == "pbguid" )
			{
				$select->group("pbguid");
			}
			else
			{
				$select->group("players.id");
			}
			$select->where("AND serverid=?",array($serverId));
			$select->page($page,$limit);
			$history = $select->getAll();
			$count = Tmvc_Model_Mysql::getConnection()->getCountCalcRows();
		}
		$this->view->assign('server', $server);
		$this->view->assign('history', $history);
		$this->view->assign('limit', $limit);
		$this->view->assign('page', $page);
		$this->view->assign('count', $count);
		$this->view->assign('currentType', $type);
		$this->view->assign('types', $types);
		$this->view->assign('groups', $groups);
		$this->view->assign('currentGroup', $groupBy);
	}
};