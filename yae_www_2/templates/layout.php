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
	 <div id="contactFloatTab">
	 	<a class="handle" href="http://link-for-non-js-users.html">Content</a>
	 	<form method="post" action="<?= $this->link("Index","contact",array(),true) ?>">
	 		<div>
		 		<textarea cols="35" rows="10" name="message" style="width: 263px;height: 185px;"></textarea>
		 		<input type="submit" value="send" class="button" style="display:block;margin:0 auto;" />
		 	</div>
	 	</form>
	 </div>
	
	<noscript><p><img src="http://piwik.s2.trakos.pl/piwik.php?idsite=2" style="border:0" alt="" /></p></noscript>
	<div id="top-right">
		<? if ( !$isLogged ) { ?>
			<div id="top-right">
				<form method="post" action="<?= $this->link("Authentication","login",array(),true) ?>">
					<table class="loginForm form" style="margin-top:50px;">
						<tr>
							<td class="label"><label for="form_4e3e758d6e379_login">login:</label></td>
							<td><input type="text" value="" name="login" /></td>
						</tr>
						<tr>
							<td class="label"><label for="form_4e3e758d6e379_password">password:</label></td>
							<td><input type="password" value="" name="password" /></td>
						</tr>
						<tr><td></td><td><input type="submit" name="submitLogin" value="login" /></td></tr>
					</table>
				</form>
			</div>
		<? } ?>
	</div>
	<a href="index.php"><h1 class="top">YAE - you are exposed</h1></a>
	<div id="menu-l"><div id="menu-r"><div id="menu">
		<?php $this->render("blocks/menu"); ?>
	</div></div></div>
	<div id="content">
		<?php $this->render($controller . "/" . $action); ?>
		<? $GLOBALS['globalTimer']->stop(); ?>
		<p class='note'>Generated in <?=$GLOBALS['globalTimer']->getTotal() ?> seconds with use of <?=Tmvc_Model_Mysql::getConnection()->getNumOfQueries() ?> queries which took <?=Tmvc_Model_Mysql::getConnection()->getTimeSpentOnQueries() ?> seconds</p>
	</div>
	<div id="footer">by trakos, 2010 - 2011</div>
</body>
</html>