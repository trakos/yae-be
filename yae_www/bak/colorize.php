<?php

function colorize( $nick )
{
	$hexa = array
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
	$search = array();
	$replace = array();
	foreach( $hexa as $char => $color )
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

		



?>
