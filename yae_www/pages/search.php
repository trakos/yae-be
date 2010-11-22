<?php
	$shows = array
	( 
		"servers" => array
		( 
			'details' 	=>	"server", 
			'fieldclick'	=>	"servers",
			'query'	=>	"SELECT SQL_CALC_FOUND_ROWS INET_NTOA(ip) AS ip, port, name, UNIX_TIMESTAMP(lastonline) AS 'last online', id FROM servers %s ORDER BY lastonline DESC", 
			'fields'	=>	array("ip"=>"ip", "port"=>"port"), 
			'showform'	=>	true 
		),
		"players" => array
		( 
			'details'       =>      "player", 
			'fieldclick'    =>      "players", 
			'query'         =>      "SELECT SQL_CALC_FOUND_ROWS nick, INET_NTOA(ip) AS ip, SUBSTR(pbguid,32-7,8) AS pbguid, SUBSTR(etproguid,40-9,10) AS etproguid, slacnick, IF(slacid=0,'',slacid) AS slacid, id FROM players %s ORDER BY id DESC", 
			'fields'        =>      array("nick"=>"nick", "ip"=>"ip", "pbguid"=>"pbguid", "etproguid"=>"etproguid","slacid"=>"slacid","slacnick"=>"slacnick"),
			'showform'      =>      true 
		),
		"server" => array
		( 
			'details'     =>      "player",
			'fieldclick'	=>	"players",
			'query'	=>	"SELECT SQL_CALC_FOUND_ROWS UNIX_TIMESTAMP(playedfrom) AS 'played from', UNIX_TIMESTAMP(playedto) AS 'played to', nick, SUBSTR(pbguid,32-7,8) AS pbguid, SUBSTR(etproguid,40-9,10) AS etproguid, slacnick, IF(slacid=0,'',slacid) AS slacid, INET_NTOA(players.ip) AS ip, players.id as id from servers JOIN times ON servers.id = serverid JOIN players ON playerid = players.id %s ORDER BY playedto DESC", 
			'fields'	=>	array("id"=>"servers.id"), 
			'showform'	=>	false,
			'querydescribe'	=> 	"SELECT CONCAT(INET_NTOA(ip),':',port) AS 'server', name, UNIX_TIMESTAMP(lastonline) AS 'last online' FROM servers %s",
			'detailclick'	=>	false
		),
		"player" => array
		( 
			'details' 	=>	"server", 
			'fieldclick' 	=>	false, 
			'query' 	=>	"SELECT SQL_CALC_FOUND_ROWS UNIX_TIMESTAMP(playedfrom) AS 'played from', UNIX_TIMESTAMP(playedto) AS 'played to', CONCAT(INET_NTOA(servers.ip),':',servers.port) AS 'server', servers.name AS name, servers.id AS id FROM servers JOIN times on serverid = servers.id JOIN players on playerid = players.id %s "./*GROUP BY servers.ip,port*/" ORDER BY playedto DESC", 
			'fields' 	=>	array("id"=>"players.id"), 
			'showform'	=>	false, 
			'querydescribe' =>	"SELECT nick AS 'nick', INET_NTOA(ip) AS 'ip',  SUBSTR(pbguid,32-7,8) AS pbguid, SUBSTR(etproguid,40-9,10) AS etproguid, os, slacnick, IF(slacid=0,'',slacid) AS slacid FROM players %s",
			'detailclick'	=>	"players"
		), 
	); 
	if( isset($_GET['show']) && isset($shows[$_GET['show']]) ) 
	{
		$limit = 50;
		$show = $_GET['show']; 
		$def = $shows[$show]; 
		if( $def['showform'] ) 
		{
			YaeSearch::show_form( $show, $def['fields'], 'Search for '.$show);
		}
		else
		{
			$where = YaeSearch::create_where($def['fields']);
			$args = YaeSearch::create_args($def['fields']);
			$q = sprintf($def['querydescribe'], $where);
			$result = $sql->getRow($q, $args);
			$active_keys = $def['detailclick']?$shows[$def['detailclick']]['fields']:array();
			YaeSearch::show_description($show, $result, $active_keys, $def['detailclick']);
		}
		$where = YaeSearch::create_where($def['fields']);
		$start = isset($_GET['start'])?$_GET['start']:0;
		$q = sprintf($def['query']." LIMIT ?,$limit", $where);
		$a = YaeSearch::create_args($def['fields']);
		$a[] = $start;
		$result = $sql->getAll($q, $a);
		$active_keys = $def['fieldclick']?$shows[$def['fieldclick']]['fields']:array();
		YaeSearch::show_results($def['fieldclick'], $def['details'], $active_keys, $result);
		$q = "SELECT FOUND_ROWS()";
		$a = array();
		$count = $sql->getOne($q,$a);
		$current_page = floor($start/$limit)+1;
		$last_page = ceil($count/$limit);
		YaeSearch::show_pages(1,$last_page,$current_page,$limit);
	}
	else
	{
		YaeSearch::show_form("players", $shows['players']['fields'], 'Search for players', false);
		YaeSearch::show_form("servers", $shows['servers']['fields'], 'Search for servers', false);
	}
	if( count($_GET) == 0 )
	{
		echo "<p class='note'>For string searching, you can use '%' to match any chars, and '_' to match any char (e.g. only and at least one)</p>";
	}
?>
