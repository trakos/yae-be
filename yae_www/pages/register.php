<?php
echo '<p>Registration temporarily disabled.</p>';
return;
function form($form_elements, $title, $button, $hidden='' )
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
	if(!empty($hidden))
	{
		foreach($hidden as $name => $value)
		{
			echo "<input type='hidden' name='$name' value='$value'>";
		}
	}
	echo '</form>';
}
function show_propositions($data)
{
	if(empty($data))
	{
		echo "<p class='error'>no matches</p>";
	}
	else
	{
		echo '<p class="note">Click the crossfire username you want to register as.</p>';
		echo "<table class='result'>";
		echo "<tr><th>found username</th></tr>";
		foreach( $data as $cfusername => $cfid )
		{
			// hasha dodałem, żeby user nie mógł podać innego cfusername niż to wyszukane z cf, żeby potem nie trzeba było preg_matchować nic
			echo "<tr><td><a href='?action=register&amp;cfid=$cfid&amp;cfusername=$cfusername&amp;cfhash=".cfhash($cfid,$cfusername)."'>$cfusername</a></td></tr>";
		}
		echo "</table>";
	}
}
function cfhash($cfid,$cfusername,$password='')
{
	return md5(sha1($password.strrev($cfusername).$cfid).'ĄĘŹŻĆ$@!Λ'.'lknaslknasflknasflknaslknaslfknaslkfaslknfslkanfalknalkfoi3hoqwsedfghjfgdghfdsfg35yeyhdthfdthi5oiq3toianfoiio35hiopaąęoe óaioiofgoanfokdsng');
}

/*if( isset($_GET['registerhash']) )
{
	echo '<p class="note">What? I\'m just registering @ yae and have to put it on as a website for a second, don\'t get so excited about it!</p>';
}
else */if( isset($_POST['password']) && isset($_POST['repeat']) && isset($_POST['cfusername']) && isset($_POST['cfid']) && isset($_POST['cfhash']) && $_POST['cfhash']==cfhash($_POST['cfid'],$_POST['cfusername']) )
{
	$password = $_POST['password'];
	$repeat = $_POST['repeat'];
	$cfid = $_POST['cfid'];
	$cfusername = $_POST['cfusername'];
	$cfhash = $_POST['cfhash'];
	if($login->cfidExists($cfid))
	{
		echo "<p class='error'>User already registered at YAE!</p>";
	}
	else if($password!=$repeat)
	{
		echo "<p class='error'>Passwords do not match!</p>";
		$cflinking = array( 'password' => 'password', 'repeat' => 'password' );
		$hidden = array( 'cfid' => $cfid, 'cfusername' => $cfusername, 'cfhash' => $cfhash );
		form($cflinking, 'Linking accounts', 'create', $hidden);
	}
	else if(strlen($password)<6)
	{
		echo "<p class='error'>Password must be at least 6 characters long!</p>";
		$cflinking = array( 'password' => 'password', 'repeat' => 'password' );
		$hidden = array( 'cfid' => $cfid, 'cfusername' => $cfusername, 'cfhash' => $cfhash );
		form($cflinking, 'Linking accounts', 'create', $hidden);
	}
	else
	{
		$reghash = cfhash($cfid,$cfusername,$password);
		$foundreghash = Crossfire::checkCFID($_GET['cfid']);
		if($reghash == $foundreghash)
		{
			$login->createUser($cfusername,$password,$cfid);
			echo "<p style='text-align:center'>Account created, you can log in using username $cfusername and the password you provided.</p>";
		}
		else
		{
			echo "<p class='note'>Now go to <a target='_blank' href='http://www.crossfire.nu/?x=user&mode=edit'>crossfire</a>, log in as '$cfusername', and change your website at 'Edit profile' to $reghash . Obviously, after validating you'll be free to revert it.</p>";
			echo "<p class='note' style='color:#f00;'>Don't close this page yet! You have to click validate after changing the website to finish the process!</p>";
			if( isset($_POST['another_try']) )
			{
				echo "<p class='error'>Your current crossfire website is: $foundreghash , change it to $reghash</p>";
			}
			$hidden = array( 'cfid' => $cfid, 'cfusername' => $cfusername, 'cfhash' => $cfhash, 'password' => $password, 'repeat' => $repeat, 'another_try' => '1' );
			form(array(), 'Linking accounts', 'validate', $hidden);
		}
	}	
}
else if( isset($_GET['cfusername']) && isset($_GET['cfid']) && isset($_GET['cfhash']) && $_GET['cfhash']==cfhash($_GET['cfid'],$_GET['cfusername']) )
{
	$cfid = $_GET['cfid'];
	$cfusername = $_GET['cfusername'];
	$cfhash = $_GET['cfhash'];
	if($login->cfidExists($cfid))
	{
		echo "<p class='error'>User already registered at YAE!</p>";
	}
	else
	{
		$cflinking = array( 'password' => 'password', 'repeat' => 'password' );
		$hidden = array( 'cfid' => $cfid, 'cfusername' => $cfusername, 'cfhash' => $cfhash );
		form($cflinking, 'Linking accounts', 'create', $hidden);
	}
}
else
{
	if (isset($_POST['username']))
	{
		$cfuser = $_POST['username'];
		$propositions = Crossfire::getIDs($cfuser);
		show_propositions($propositions);
	}
	else
	{
		echo '<p class="note">This page needs a <a href="http://crossfire.nu">crossfire</a> account. If you don\'t have one, create it first. Fill in your crossfire username:</p>';
	}
	$cfusername = array( 'username' => 'text' );
	form($cfusername, 'Linking account with crossfire', 'search');
}
?>
