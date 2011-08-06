<?php
	$first = true;
	$k = 1;
	$menuPages = array("Search" => array("Index","index"), "Stats" => array("Stats", "stats"), "News" => array("News","index"));
	if($isLogged)
	{
		$menuPages = array_merge($menuPages, array("Add server" => array("user","addserver"),"Log out from ".$login->getUsername() => array("auth","logout") ) );
	}
	else
	{
		$menuPages = array_merge($menuPages, array("Log in to add server" => array("auth", "login"),"Create account" => array("auth", "register") ) );
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