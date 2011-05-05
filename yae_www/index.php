<?php
	if(false)//!isset($dontblock) || $_SERVER['REMOTE_ADDR'] != '194.11.24.98' )
	{
	?>	
		<html>
		<head>
		</head>
		<body>
		<!-- Piwik -->
		<script type="text/javascript">
		var pkBaseURL = (("https:" == document.location.protocol) ? "https://piwik.s2.trakos.pl/" : "http://piwik.s2.trakos.pl/");
		document.write(unescape("%3Cscript src='" + pkBaseURL + "piwik.js' type='text/javascript'%3E%3C/script%3E"));
		</script><script type="text/javascript">
		try {
		var piwikTracker = Piwik.getTracker(pkBaseURL + "piwik.php", 2);
		piwikTracker.trackPageView();
		piwikTracker.enableLinkTracking();
		} catch( err ) {}
		</script><noscript><p><img src="http://piwik.s2.trakos.pl/piwik.php?idsite=2" style="border:0" alt="" /></p></noscript>
		<!-- End Piwik Tracking Code -->
		</body>
		</html>
	<?php
		die('yae project is currently suspended, due to lack of interest - in case you think it\'s needed, contact <a href="http://www.crossfire.nu/?x=user&mode=view&id=13943">trakos</a>');
	}
	session_start();
	require('classes/Timer.php');
	require('classes/Mysql.php');
	require('classes/Colorizer.php');
	require('classes/YaeSearch.php');
	require('classes/Crossfire.php');
	require('classes/Login.php');
	$timer = new Timer();
	$sql = new Mysql('localhost','dawn',"***REMOVED***",'dawn');
	$login = new Login($sql);

	
	$pages = array('search' => "Search", 'news' => "News", 'serverlist' => "Server list");
	if($login->isLogged())
	{
		$pages = array_merge($pages, array('login' => "Add server",'logout' => "Log out from ".$login->getUsername()));
	}
	else
	{
		$pages = array_merge($pages, array('login' => "Log in to add server",'register' => "Create account or change password"));
	}
	$action = 'search';
    if(isset($_GET['action']) && isset($pages[$_GET['action']]))
    {
        $action = $_GET['action'];
    }
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="pl" dir="ltr">
<head>
<meta http-equiv="Content-Type" content="application/xhtml+xml; charset=utf-8"/>
<meta http-equiv="Content-Language" content="en" />
<meta http-equiv="Description" content="YAE - you are exposed - yawn replacement for enemy territory - pbguid/etproguid/ip/nick matcher" />
<meta http-equiv="Keywords" content="et, enemy territory, yae, you are exposed, crossfire, yawn, wolfenstein, rtcw, return to castle wolfenstein, pbguid, ip, matches, anticheat, cheaters, trakos" />
<meta http-equiv="Robots" content="all" />
<meta name="google-site-verification" content="CUFrM0HOx5Zh6Hd_KCcKXGVGeBmQI8QguMfbp62NWPs" />
<title>You Are Exposed - <?php echo $pages[$action]; ?></title>
<link rel="stylesheet" href="style.css" type="text/css" />
<?php
	if(isset($_GET['layout']) || ( !isset($_SESSION['layout']) && $_GET['layout']=2) ) // yes, =, and not ==
	{
		$layouts = array( 1, 2, 3, 4);
		if( in_array( $_GET['layout'], $layouts ) )
		{
			$_SESSION['layout'] = $_GET['layout'];
		}
	}
	if(isset($_SESSION['layout']))
	{
		$lay = $_SESSION['layout'];
		echo '<link rel="stylesheet" href="styles/'.$lay.'/layout.css" type="text/css" />';
	}
?>
<link rel="icon" type="image/png" href="favicon.ico"/>
<link rel="stylesheet" href="/cf/js/extjs/themes/default/css/ext-all.css" />
<link rel="stylesheet" href="/cf/account/account.css" />
<?php
	require('classes/JavascriptIncluder.php');
	$js = new JavascriptIncluder;
	/*$js->addFile("js/extjs/ext-base.js");
	$js->addFile("js/extjs/ext-all-debug.js");
	$js->output(true);
	$js->addFile("js/tjs/sgif.js");
	$js->addFile("js/tjs/VColumnLayout.js");
	$js->addFile("js/tjs/CenterPanel.js");
	$js->addFile("js/tjs/ProgressBarOverride.js");
	$js->addFile("js/tjs/LastValidationMsgOverride.js");
	$js->addFile("js/tjs/PanelMasking.js");
	$js->addFile("js/tjs/base64.js");
	$js->addFile("account/step0.js");
	$js->addFile("account/step1.js");
	$js->addFile("account/step2.js");
	$js->addFile("account/step3.js");
	$js->addFile("account/step4.js");
	$js->addFile("account/step5.js");
	$js->addFile("account/registerWizard.js");
	$js->output();*/
?>
	<script type="text/javascript">
		/*Ext.onReady(function()
		{
			var registerWindow = new Ext.Window(
			{
				layout:		'fit'
				,width:		780
				,height:	300
				,border:	false
				,modal:		true
				,title:		'Registering / password reminding for trakos.pl'
				,closeAction:'hide'
				,items:		registerWizard
			});
			Ext.get('menulink_register').on('click',function(e)
			{
				e.stopEvent();
				registerWindow.show();
			});
			registerWizard.updateTitle = function(title)
			{
				registerWindow.setTitle(title);
			};
			registerWizard.finishRegistration = function()
			{
				registerWizard.setActiveItem(0);
				registerWindow.hide();
			};
			registerWizard.cancelRegistration = registerWizard.finishRegistration;
		});*/
	</script>
<!-- Piwik -->
<script type="text/javascript">
var pkBaseURL = (("https:" == document.location.protocol) ? "https://piwik.s2.trakos.pl/" : "http://piwik.s2.trakos.pl/");
document.write(unescape("%3Cscript src='" + pkBaseURL + "piwik.js' type='text/javascript'%3E%3C/script%3E"));
</script><script type="text/javascript">
try {
var piwikTracker = Piwik.getTracker(pkBaseURL + "piwik.php", 2);
piwikTracker.trackPageView();
piwikTracker.enableLinkTracking();
} catch( err ) {}
</script>
<!-- End Piwik Tracking Code -->
</head>
<body>
<noscript><p><img src="http://piwik.s2.trakos.pl/piwik.php?idsite=2" style="border:0" alt="" /></p></noscript>
<a href="index.php"><h1 class="top">YAE - you are exposed</h1></a>
<div id="menu-l"><div id="menu-r"><div id="menu">
<?php
	$first = true;
	$k = 1;
	foreach( $pages as $key=>$page )
	{
		$class = "";
		if($first)
		{
			$class.=" first";
			$first = false;
		}
		if($key == $action)
		{
			$class.=" selected";
		}
		$class = trim($class);
		echo '<a href="?action='.$key.'" class="'.$class.'" id="menulink_'.$key.'"><span>0'.$k.'.</span>'.$page.'</a>';
		$k++;
	}
?>
</div></div></div>
<div id="content">
<?php
	switch($action)
	{
		case 'serverlist':
			$action = "search";
			$_GET['show'] = "servers";
			break;
	}
	try
	{
		require("pages/$action.php");
	}
	catch(sqlException $e)
	{
		echo "<p class='error'>Unexpected exception, sorry!</p>";
		$string = "Page:$action\nGET: ".print_r($_GET,true)."\nPOST: ".print_r($_POST,true)."\n$e\n".$sql->timer;
		file_put_contents("log/".time(),$string);
		//mail('trakos@trakos.pl', 'YAE sql exception', "Page:$action\nGET: ".print_r($_GET,true)."\nPOST".print_r($_POST,true)."\n$e\n".$sql->timer);
	}
	$timer->stop();
	echo "<p class='note'>Generated in ".$timer->getTotal()." seconds with use of ".$sql->getNumOfQueries()." queries which took ".$sql->getTimeSpentOnQueries()." seconds</p>";
?>
</div>
<div id="footer">by trakos<?php
	if( isset($_SESSION['layout']) && $_SESSION['layout'] == 1 )
	{
		echo "; layout by h3fty";
	}
?>, 2010 </div>
</body>
</html>
