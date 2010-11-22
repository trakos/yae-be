<?php
	require('config.php');
	require('timer.php');
	require('mysql.php');
	function gtfo( $string, $index )
	{
		return mb_substr($string, 0, $index).mb_substr($string, $index+1);
	}
	function get_string_to(&$string,$char)
	{
		$s = "";
		$len = strlen($string);
		for($i=0;$i<$len&&$string[$i]!=$char;$i++)
		{
			$s.=$string[$i];
		}
		$string = mb_substr($string, $i+1);
		return $s;
	}
	$sql = new mysql();
	$timer = new timer();
	$lines = file('master_ban_index.txt');
	$i=0;
	$count = count($lines);
	$f = fopen('data.html', 'x');
	if(!$f)
	{
		die("data.html exists\n");
	}
	for($i=35700;$i<$count;$i++)//
	//foreach($lines as $line)
	{
		$line = $lines[$i];//
		if( $i%100 == 0 )
		{
			echo floor(($i/$count)*100)."% - $i / $count \n";
		}
		$line = trim($line);
		if( $line[0] != ';' && !empty($line) )
		{
			// [PBBans:125010 04.30.2009] cb69376f8d220db352b9455f9a9ec9db "##Diablaman##" "82.124.128.83" "MULTIHACK 70534"
			$line = gtfo( $line, 0 ); // '['
			$pbbans = get_string_to($line,']');
			$line = gtfo( $line, 0 ); // ' '
			$pbguid = get_string_to($line,' ');
			$line = gtfo( $line, 0 ); // '"'
			$nick = get_string_to($line,'"');
			$line = gtfo( $line, 0 ); // ' '
			$line = gtfo( $line, 0 ); // '"'
			$ip = get_string_to($line,'"');
			$line = gtfo( $line, 0 ); // ' '
			$line = gtfo( $line, 0 ); // '"'
			$reason = get_string_to($line,'"');
			$data = array
			(
				'pbbans'	=> $pbbans,
				'pbguid'	=> $pbguid,
				'nick'		=> $nick,
				'ip'		=> $ip,
				'reason'	=> $reason
			);
			$qbase="SELECT SQL_CALC_FOUND_ROWS 
				nick, INET_NTOA(ip) AS ip, SUBSTR(pbguid,32-7,8) AS pbguid, SUBSTR(etproguid,40-9,10) AS etproguid, id FROM players 
				WHERE %s
				ORDER BY id DESC";
			$q = sprintf($qbase, "pbguid='?'");
			$guidmatch = $sql->getRow($q,$pbguid);
			$q = sprintf($qbase, "ip=INET_ATON('?')");
			$ipmatch = $sql->getRow($q,$ip);
			if($ipmatch)
			{
				fwrite($f,'<a href="http:/'.'/trakos.pl/yae/index.php?show=players&nick=&ip='.$ip.'&pbguid=&etproguid=">'."$ip</a>");
				echo "$i: found ipmatch\n";
			}
			else if($guidmatch)
			{
				fwrite($f,'<a href="http:/'.'/trakos.pl/yae/index.php?show=players&nick=&ip=&pbguid='.$pbguid.'&etproguid=">'."$pbguid</a>");
				echo "$i: found guidmatch\n";
			}
			if($ipmatch || $guidmatch)
			{
				$last8 = substr('abcdef', -8);
				$s = $pbbans;
				get_string_to($s,':');
				$s = get_string_to($s,' ');
				fwrite($f,' - <a href="http:/'.'/www.pbbans.com/mbi-viewban-'.$last8.'-vb'.$s.'.html">'.$pbbans.'</a><br>'."\n");
			}
		}
		//$i++;
	}
	fwrite($f,"<p class='note'>Generated at ".date("Y-m-d H:i")." in ".$timer->getTotal()." seconds with use of ".$sql->getNumOfQueries()." queries which took ".$sql->getTimeSpentOnQueries()." seconds</p>");
	fclose($f);
?>
