<?php
	$first = true;
	$k = 1;
	$menuPages = array("Search" => array("Index","index"), "Stats" => array("Stats", "stats"), "News" => array("News","index"));
	if($isLogged)
	{
		$menuPages = array_merge($menuPages, array("Manage servers" => array("UserServers","userServerList"),"Manage friends"=>array("UserFriends","index"),"Edit profile"=>array("EditProfile","index")) );
		if ( Model_Auth::getInstance()->isAtLeast(LEVEL_SUPERADMIN) )
		{
			$menuPages = array_merge($menuPages, array("Admin" => array("Admin","index")) );
		}
		$menuPages["Log out from ".$userName] = array("Authentication","logout");
	}
	else
	{
		$menuPages = array_merge($menuPages, array("Log in" => array("Authentication", "login"),"Create account" => array("Authentication", "register") ) );
	}
	foreach( $menuPages as $pageName => $controllerAction )
	{
		list($controller,$action) = $controllerAction;
		$class = "";
		if($first)
		{
			$class.=" first";
			$first = false;
		}
		/*if($key == $action)
		{
			$class.=" selected";
		}*/
		$class = trim($class);
		echo '<a href="'.$this->link($controller,$action).'" class="'.$class.'" id="menulink_'.$controller.'_'.$action.'"><span>0'.$k.'.</span>'.$pageName.'</a>';
		$k++;
	}