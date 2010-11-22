<?php
function form($form_elements, $title, $button)
{
	echo '<form method="POST">';
	echo '<table>';
	echo "<tr><th colspan='2'>$title</th></tr>";
	foreach( $form_elements as $element => $type )
	{
		$value = isset($_POST[$element])?$_POST[$element]:"";
		echo "<tr><td>$element:</td><td><input type='$type' name='$element' value='$value'></td></tr>";
	}
	echo "<tr><td colspan='2'><input type='submit' class='button' value='$button'></td></tr>";
	echo '</table>';
	echo '</form>';
}

function rcon( $ip, $port, $passwd )
{
	$cmd = 'status';
	$fp = fsockopen("udp:/"."/$ip", "$port");
	stream_set_blocking($fp, FALSE);
	$cmd = "\xff\xff\xff\xffrcon \"$passwd\" $cmd";
	fputs($fp, $cmd, strlen($cmd));
	usleep(0.4*1000000);
	$res="";
	do
	{
	        $buf = fread($fp, 2048);
        	$res .= substr($buf, 4);
	} while( !empty($buf) );
	usleep(0.4*1000000);
	if ( trim($res) == "print\nBad rconpassword." )
	{
		return 'badpass';
	}
	$res = explode("\n", trim($res));
	if(count($res) < 2)
	{
		return 'timeout';
	}
	return 'ok';
}

if(isset($_POST['login']) && isset($_POST['password']))
{
	if( !$login->isLogged() )
	{
		echo "<p class='error'>Wrong data</p>";
	}
}
if($login->isLogged())
{
	if(isset($_POST['ip']) && isset($_POST['port']) && isset($_POST['rconPassword']) )
	{
		$id = $login->getUserID();
		$ip = gethostbyname($_POST['ip']);
		$port = $_POST['port'];
		$passwd = $_POST['rconPassword'];
		$pass = exec("./enc ".escapeshellarg($passwd));
		$q = "INSERT INTO servers (userid,gameid,ip,port,password,added_at,added_from_ip) VALUES ('?','?',INET_ATON('?'),'?','?',NOW(),INET_ATON('?'))";
		$a = array($id,1,$ip,$port,$pass,$login->getUserIP());
		$r = rcon( $ip, $port, $passwd );
		if( $r == "ok" )
		{
			$r = $sql->vgetRow("SELECT * FROM servers WHERE ip=INET_ATON('?') AND port='?'", array($ip,$port));
			if($r && $r['userid'] == $id )
			{
				$q = "UPDATE servers SET password='?',connectionfails=0,passfails=0,nextcheck=0 WHERE ip=INET_ATON('?') AND port='?'";
				$a = array($pass,$ip,$port);
				$sql->vqueryf($q,$a);
				echo "<p class='error'>Server already added to the database, password updated</p>";
			}
			else if($r)
			{
				echo "<p class='error'>Server already added by other user!</p>";
			}
			else
			{
				$sql->vqueryf($q,$a);
				echo "<p>Server added</p>";
				return;
			}
		}
		else if( $r == "badpass" )
		{
			echo "<p class='error'>Wrong password</p>";
		}
		else if( $r == "timeout" )
		{
			echo "<p class='error'>Timeout - probably server unavaible</p>";
		}
	}
	$serverform = array( 'ip' => 'text', 'port' => 'text', 'rconPassword' => 'password' );
	form($serverform, 'Add server', 'add');
	return;
}
$loginform = array( 'login' => 'text', 'password' => 'password' );
form($loginform, 'Login', 'login');
?>
