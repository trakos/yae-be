<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
<meta http-equiv="Content-Language" content="pl"/>
<link rel="stylesheet" href="css/style.css" type="text/css"/>
<title>system callcenter</title>
<?php Lib_Includer_Javascript::getInstance()->output(); ?>
</head>
<body>
<div id="leftBar">
	<div id="topStatusInfo">
		<table>
			<tr>
				<th>Data:</th>
				<td><?php echo date("Y.m.d H:m"); ?></td>
			</tr>
			<tr>
				<th>Zalogowany:</th>
				<td>
					<?php if ( !$isLogged ) { ?>
						nie
					<?php } else { ?>
						<?php echo $userNameAndSurname; ?>
					<?php } ?>
				</td>
			</tr>
			<tr>
				<th>Poziom:</th>
				<td><?php echo $userLevelName; ?></td>
			</tr>
		</table>
	</div>
	<ul id="menu">
		<?php $k = 1;?>
		<?php if ( $isLogged ) { ?>
			<?php if ( $userLevel >= LEVEL_MANAGER ) { ?>
				<li>
					<a href="<?php echo $this->link("Deliverers","index"); ?>">
						<span>0<?php echo $k++; ?></span>
						Zleceniodawcy
					</a>
				</li>
			<?php } ?>
			<?php if ( $userLevel >= LEVEL_SUPERVISOR ) { ?>
				<li>
					<a href="<?php echo $this->link("Campaigns","index"); ?>">
						<span>0<?php echo $k++; ?></span>
						Kampanie
					</a>
				</li>
				<li>
					<a href="<?php echo $this->link("Forms","index"); ?>">
						<span>0<?php echo $k++; ?></span>
						Formularze
					</a>
				</li>
				<li>
					<a href="<?php echo $this->link("Statuses","index"); ?>">
						<span>0<?php echo $k++; ?></span>
						Słownik statusów
					</a>
				</li>
				<li>
					<a href="<?php echo $this->link("Users","index"); ?>">
						<span>0<?php echo $k++; ?></span>
						Użytkownicy
					</a>
				</li>
			<?php } ?>
			<?php if ( $userLevel == LEVEL_AGENT ) { ?>
				<li>
					<a href="<?php echo $this->link("FormShow","index"); ?>">
						<span>0<?php echo $k++; ?></span>
						Prowadzenie ankiety
					</a>
				</li>
			<?php } ?>
			<li>
				<a href="<?php echo $this->link("Reports","index"); ?>">
					<span>0<?php echo $k++; ?></span>
					Statystyki
				</a>
			</li>
			<li>
				<a href="<?php echo $this->link("Index","changepassword"); ?>">
					<span>0<?php echo $k++; ?></span>
					Zmień hasło
				</a>
			</li>
			<li>
				<a href="<?php echo $this->link("Index","logout"); ?>">
					<span>0<?php echo $k++; ?></span>
					Wyloguj
				</a>
			</li>
		<?php } else { ?>
			<li>
				<a href="<?php echo $this->link("Index","index"); ?>">
					<span>0<?php echo $k++; ?></span>
					Zaloguj
				</a>
			</li>
		<?php } ?>
	</ul>
</div>
<div id="content">
	<?php $this->render($controller . "/" . $action); ?>
</div>
<div style="clear:both;"></div>
<div id="footer">Copyright © 2011. Wszelkie prawa zastrzeżone.</div>
</body>
</html>