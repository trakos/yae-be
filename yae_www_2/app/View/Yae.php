<?php
	class View_Yae
	{
		static protected function getFlagForIp($ip)
		{
			try 
			{
				$flag = strtolower(geoip_country_code_by_name($ip));
			}
			catch( Exception $e )
			{
			}
			if(empty($flag))
			{
				return "images/icons/world.png";
			}
			else
			{
				return "images/flags/$flag.png";
			}
		}
		
		/**
		 * Function to format yae value for the display.
		 * @param unknown_type $key key of the value, column name from mysql
		 * @param unknown_type $value value to format
		 * @param string $type can be either player or server
		 * @return string
		 */
		static public function formatValue( $key, $value, $type )
		{
			if(preg_match("/ip/", $key))
			{
				$flag = self::getFlagForIp($value);
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
				$show_value = self::etColorize($value);
			}
			else if($key == "minutesplayed" )
			{
				$show_value = "";
				if( $value >= (240*60) )
				{
					$days = floor($value/(24*60));
					$show_value.= $days." days ";
					$value-= ($days*24*60);
				}
				if( $value >= 60 )
				{
					$minutes = floor($value/60);
					$show_value.= $minutes." hours ";
					$value-= ($minutes*60);
				}
				if ( round($value) > 0 )
				{
					$show_value.= round($value)." minutes";
				}
			}
			else if($key == "playedto" || $key == "playedfrom" || $key == "lastonline" || $key == "nextcheck" )
			{
				if( $type == "server" && $value > time()-600 )
				{
					$show_value = "<span style='font-weight: bold; color: #ff8000;'>ONLINE</span>";
				}
				else
				{
					$show_value = self::formatTime($value, $key != "nextcheck", $key == "nextcheck", $key != "playedto" && $key != "playedfrom");	
				}
			}
			else
			{
				$show_value = $value;
			}
			return $show_value;
		}
		
		static public function formatTime( $time, $notInFuture = true, $notInPast = false, $showAsTimeDifference = true )
		{
			if( abs($time - time()) < 10 || ( $notInFuture && $time > time() ) )
			{
				$show_value = "<span style='font-weight: bold; color: #ff8000;'>NOW</span>";
			}
			else if ( $time == 0 )
			{
				$show_value = "<span style='font-weight: bold; color: #ff0000;'>NEVER</span>";
			}
			else if ( $notInPast && $time < time() )
			{
				$show_value = "offline";
			}
			else if ( $time >= YAE_SERVER_OFF_TIME )
			{
				$show_value = "<span style='font-weight: bold; color: #ff0000;'>DISABLED</span>";
			}
			else
			{
				$day = 60*60*24;
				if ( $time > time() && $showAsTimeDifference && $time-time() < $day*30  )
				{
					$diff = $time-time();
					if ( $diff < 60 )
					{
						$show_value = "in $diff seconds";
					}
					else if ( $diff < 3600 )
					{
						$show_value = "in ".round($diff/60)." minutes";
					}
					else if ( $diff < $day )
					{
						$show_value = "in ".round($diff/3600)." hours";
					}
					else
					{
						$show_value = "in ".round($diff/$day)." days";
					}
					$show_value = "<a title='".date("d.m.Y H:i",$time)."'>".$show_value.'</a>';
				}
				else if ( $showAsTimeDifference && time()-$time < $day*30 )
				{
					$diff = time()-$time;
					if ( $diff < 60 )
					{
						$show_value = "$diff seconds ago";
					}
					else if ( $diff < 3600 )
					{
						$show_value = round($diff/60)." minutes ago";
					}
					else if ( $diff < $day )
					{
						$show_value = round($diff/3600)." hours ago";
					}
					else
					{
						$show_value = round($diff/$day)." days ago";
					}
					$show_value = "<a title='".date("d.m.Y H:i",$time)."'>".$show_value.'</a>';
				}
				else if( date("Y",$time) == date("Y") )
				{
					// tu można niby uciąć rok, ale czy to czytelne?
					$show_value = date("d.m.Y H:i",$time);
				}
				else
				{
					$show_value = date("d.m.Y H:i",$time);
				}
			}
			return $show_value;
		}
		
		static protected $hexa = array
		(
			'0' =>  "000000",
			'1' =>  "FF0000",
			'2' =>  "00FF00",
			'3' =>  "AAAA00",//"FFFF00",
			'4' =>  "0000FF",
			'5' =>  "00FFFF",
			'6' =>  "FF00FF",
			'7' =>  "AFAFAF",//"FFFFFF",
			'8' =>  "FF7F00",
			'9' =>  "7F7F7F",
			'/' =>  "FFFF7F",
			':' =>  "BFBFBF",
			';' =>  "BFBFBF",
			//'<' =>  "007F00",
			'>' =>  "00007F",
			'=' =>  "7F7F00",
			'*' =>  "FFFFFF",
			'-' =>  "999933",
			'+' =>  "993300",
			'?' =>  "7F0000",
			'a' =>  "FF9919",
			'b' =>  "007F7F",
			'c' =>  "7F007F",
			'd' =>  "007fFF",
			'e' =>  "7F00FF",
			'f' =>  "3399CC",
			'g' =>  "CCFFCC",
			'h' =>  "006633",
			'i' =>  "FF0033",
			'j' =>  "B21919",
			'k' =>  "993300",
			'l' =>  "CC9933",
			'm' =>  "999933",
			'n' =>  "FFFFBF",
			'o' =>  "FFFF7F",
			'p' =>  "000000",
			'q' =>  "FF0000",
			'r' =>  "00FF00",
			's' =>  "FFFF00",
			't' =>  "0000FF",
			'u' =>  "00FFFF",
			'v' =>  "FF00FF",
			'w' =>  "FFFFFF",
			'x' =>  "FF7F00",
			'y' =>  "7F7F7F",
			'z' =>  "BFBFBF",
			'!' =>  "8a704b",
			'@' =>  "7F3F00",
			'#' =>	"670c6c",
			'$' =>	"2378cb",
			'&' =>	"36789a",
			'*' =>	"f2f4f3",
			'(' =>	"00330f",
			')' =>	"941028"		
		);	
	
		static public function etColorize( $nick )
		{
			$search = array();
			$replace = array();
			foreach( self::$hexa as $char => $color )
			{
				$search[] = "^$char";
				$search[] = "^".strtoupper($char);
				$replace[] = "</span><span style='color: #$color;'>";
				$replace[] = "</span><span style='color: #$color;'>";
			}
			$search[] = " </span>";
			$replace[] = "&nbsp;</span>";
			$nick = str_replace("^<", "</span><span style='color: #007F00;'>", $nick);
			$nick = "<span style='color: #5f5f5f;'>".$nick."</span>";
			return str_replace($search,$replace,$nick);
		}
	}	