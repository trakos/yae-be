<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="pl" dir="ltr">
<head>
	<meta http-equiv="Content-Type" content="application/xhtml+xml; charset=utf-8"/>
	<meta http-equiv="Content-Language" content="en" />
	<meta http-equiv="Description" content="YAE - you are exposed - yawn replacement for enemy territory - pbguid/etproguid/ip/nick matcher" />
	<meta http-equiv="Keywords" content="et, enemy territory, yae, you are exposed, crossfire, yawn, wolfenstein, rtcw, return to castle wolfenstein, pbguid, ip, matches, anticheat, cheaters, trakos" />
	<meta http-equiv="Robots" content="all" />
	<meta name="google-site-verification" content="CUFrM0HOx5Zh6Hd_KCcKXGVGeBmQI8QguMfbp62NWPs" />
	<link rel="icon" type="image/png" href="favicon.ico"/>
	<title>You Are Exposed - <?=$siteTitle ?></title>
	<?php Tmvc_View_Includer_Javascript::getInstance()->output(); ?>
	<?php Tmvc_View_Includer_Css::getInstance()->output(); ?>
	<!-- Piwik -->
	<script type="text/javascript">
		var pkBaseURL = (("https:" == document.location.protocol) ? "https://piwik.s2.trakos.pl/" : "http://piwik.s2.trakos.pl/");
		document.write(unescape("%3Cscript src='" + pkBaseURL + "piwik.js' type='text/javascript'%3E%3C/script%3E"));
	</script>
	<script type="text/javascript">
	try 
	{
		var piwikTracker = Piwik.getTracker(pkBaseURL + "piwik.php", 2);
		piwikTracker.trackPageView();
		piwikTracker.enableLinkTracking();
	}
	catch( err ) 
	{
	}
	</script>
	<!-- End Piwik Tracking Code -->
</head>
<body>
	<noscript><p><img src="http://piwik.s2.trakos.pl/piwik.php?idsite=2" style="border:0" alt="" /></p></noscript>
	<a href="index.php"><h1 class="top">YAE - you are exposed</h1></a>
	<div id="menu-l"><div id="menu-r"><div id="menu">
		<?php $this->render("blocks/menu"); ?>
	</div></div></div>
	<div id="content">
		<?php $this->render($controller . "/" . $action); ?>
		<? $GLOBALS['globalTimer']->stop(); ?>
		"<p class='note'>Generated in <?=$GLOBALS['globalTimer']->getTotal() ?> seconds with use of <?=Tmvc_Model_Mysql::getConnection()->getNumOfQueries() ?> queries which took <?=Tmvc_Model_Mysql::getConnection()->getTimeSpentOnQueries() ?> seconds</p>";
	</div>
	<div id="footer">by trakos, 2010 - 2011</div>
</body>
</html>