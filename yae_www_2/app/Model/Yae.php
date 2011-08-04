<?php
	class Model_Yae
	{
		/**
		 * @return Tmvc_Model_Mysql_BaseSelect
		 */
		public static function getPlayersSelect()
		{
			$select = Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT SQL_CALC_FOUND_ROWS nick, INET_NTOA(ip) AS ip, SUBSTR(pbguid,32-7,8) AS pbguid, SUBSTR(etproguid,40-9,10) AS etproguid, slacnick, IF(slacid=0,'',slacid) AS slacid, id FROM players %s");
			$select->order('id DESC');
			return $select;
		}
		
		public function format_value( $key, &$value, $type ) //$type = player or server
		{
			if(preg_match("/ip/", $key))
			{
				$flag = YaeSearch::flag($value);
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
				$altflag = basename($flag,".png");
				$show_value = "<img src='$flag' alt='$altflag'> $value";
			}
			else if($key == "nick" || $key == "name")
			{
				$show_value = Colorizer::colorize($value);
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
	}	