<?php
	function flag($ip)
	{
		@$flag = strtolower(geoip_country_code_by_name($ip));
		if(empty($flag))
		{
			return "images/icons/world.png";
		}
		else
		{
			return "images/flags/$flag.png";
		}
	}
	function format_value( $key, &$value, $type ) //$type = player or server
	{
		if(preg_match("/ip/", $key))
		{
			$flag = flag($value);
			if( $type == "player" )
			{
				if (preg_match('/^([0-9]*)\.([0-9]*)\.([0-9]*)\.([0-9]*)$/', $value, $matches ))
				{
					$d = $matches[1];
					$c = $matches[2];
					$b = $matches[3];
					$a = $matches[4];
					$value = "$d.$c.$b.*";
				}
			}
			$show_value = "<img src='$flag' alt='$flag'> $value";
		}
		else if($key == "nick" || $key == "name")
		{
			$show_value = colorize($value);
		}
		else if($key == "played to" || $key == "played from" || $key == "last online" )
		{
			if( $value > time() || ( $type == "server" && $value > time()-600 ) )
			{
				$show_value = "<span style='font-weight: bold; color: #ff8000;'>ONLINE</span>";
			}
			else if( $value == 0 )
			{
				$show_value = "<span style='font-weight: bold; color: #ff0000;'>NEVER</span>";
			}
			else
			{
				$day = 60*60*24;
				if( date("Y",$value) == date("Y") )
				{
					// numer dnia od 2009-1-1 :)
					$start = mktime(0,0,0,1,1,2009,1);
					$current_day = floor((time()-$start)/$day);
					$date_day = floor(($value-$start)/$day);
					if( $date_day == $current_day )
					{
						$show_value = date("\\t\o\d\a\y H:i",$value);
					}
					else if( $date_day >= $current_day-1 )
					{
						$show_value = date("\y\e\s\\t\e\\r\d\a\y H:i",$value);
					}
					else if( $date_day >= $current_day-6 )
					{
						$show_value = date("l H:i",$value);
					}
					else
					{
						$show_value = date("m-d H:i",$value);
					}					
				}
				else
				{
					$show_value = date("Y-m-d H:i",$value);
				}
			}
		}
		else
		{
			$show_value = $value;
		}
		return $show_value;
	}
	
	function show_form( $form_name, $form_elements, $title, $left = true )
	{
		echo '<form method="get" action="">';
		if($left)
		{
			echo '<table class="search_form">';
		}
		else
		{
			echo '<table>';
		}
		echo "<tr><th colspan='2'><input type='hidden' name='show' value='$form_name' />$title</th></tr>";
		foreach( $form_elements as $element => $sqlname )
		{
			$value = isset($_GET[$element])?$_GET[$element]:"";
			echo "<tr><td>$element:</td><td><input type='text' name='$element' value='$value' /></td></tr>";
		}
		echo "<tr><td colspan='2'><input type='submit' class='button' value='search' /></td></tr>";
		echo '</table>';
		echo '</form>';
	}
	function show_results( $fieldclick_name, $details_name, $fields, $data )
	{
		if(empty($data))
		{
			echo "<p class='error'>no matches</p>";
			return;
		}
		echo "<table class='result'>";
		echo "<tr>";
		foreach( $data[0] as $key => $val )
		{
			if($key=='id')
			{
				$key = "details";
			}
			echo "<th>$key</th>";
		}
		echo "</tr>";
		foreach( $data as $row )
		{
			echo "<tr>";
			foreach($row as $key => $value)
			{
				$show_value = format_value( $key, $value, (isset($row['nick']) || $key == "played from" || $key == "played to ")?"player":"server");
				if($key == "id" )
				{
					echo "<td><a href='?show=$details_name&amp;id=$value'>&raquo;</a></td>";
				}
				else if( !empty($fieldclick_name) && isset($fields[$key]) )
				{
					echo "<td><a href=\"?show=$fieldclick_name&amp;$key=".urlencode($value)."\">$show_value</a></td>";
				}
				else
				{
					echo "<td>$show_value</td>";
				}
			}
			echo "</tr>";
		}
		echo "</table>";
	}
	function show_description($name, $data, $fields, $onclick )
	{
		if(!$data)
		{
			echo "<p class='error'>$name not found</p>";
			return false;
		}
		echo "<table class='result_description'>";
		echo "<th colspan='2'>$name</th>";
		foreach($data as $key => $value)
		{
			$show_value = format_value( $key, $value, isset($data['nick'])?"player":"server");
			echo "<tr><th>$key:</th>";
			if( !empty($fields) && isset($fields[$key]) )
			{
				echo "<td><a href='?show=$onclick&amp;$key=".urlencode($value)."'>$show_value</a></td>";
			}
			else
			{
				echo "<td>$show_value</td>";
			}
			echo "</tr>";
		}
		echo "</table>";
	}
	function create_where( $keys )
	{
		$where = "WHERE 1 AND";
		foreach($keys as $key => $sqlkey)
		{
			if(isset($_GET[$key]) && !empty($_GET[$key]) )
			{
				if ( $key == 'ip' )
				{
					$where.=" $sqlkey >= INET_ATON('?') AND $sqlkey <= INET_ATON('?') AND";
				}
				else if( $key == 'nick' )
				{
					$where.=" (nick LIKE '?' OR colorless LIKE '?' ) AND";
				}
				else
				{
					$where.=" $sqlkey LIKE '?' AND";
				}
			}
		}
		return substr($where, 0, -4);
	}
	function create_args( $keys )
	{
		$args = array();
		foreach($keys as $key => $sqlkey)
		{
			if(isset($_GET[$key]) && !empty($_GET[$key]))
			{
				$value = $_GET[$key];
				if($key == 'etproguid' || $key == 'pbguid')
				{
					$len = array('etproguid' => 40, 'pbguid' => 32);
					$value = str_pad($value,$len[$key],'_',STR_PAD_LEFT);
				}
				else if($key == 'nick')
				{
					$args[] = $value; // dodajemy dwa razy
				}
				else if($key == 'ip')
				{
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
						
						// use start and end
						$value = $end;
						$args[] = $start;
					}
					else
					{
						$args[] = $value;
					}
				}
				$args[] = $value;
			}
		}
		return $args;
	}
	function show_pages($first_page,$last_page,$current_page,$limit)
	{
		if($first_page == $last_page)
		{
			return;
		}
		echo "<div id='pages'>";
		$first = true;
		$s = "?";
		foreach( $_GET as $k => $v )
		{
			if($k == 'start')
			{
				continue;
			}
			if($first)
			{
				$first = false;
			}
			else
			{
				$s.="&amp;";
			}
			$s.="$k=".urlencode($v);
		}
		for( $i=$first_page; $i<=$last_page; $i++ )
		{
			if($i != $current_page)
			{
				$link = $s."&amp;start=".$limit*($i-1);
				echo "<a href='$link'>$i</a> ";
			}
			else
			{
				echo "<span>$i</span> ";
			}
		}
		echo "</div>";
	}
?>
