<? $this->render("blocks/snippets"); ?>
<? $this->render("UserServers/submenu"); ?>

<? if ( empty($servers) ) { ?>
	<table class="yae result"><tr><td><p class="infoMessage">Currently you have no servers added.</p></td></tr></table>
<? } else { 3?>
	<table class="yae result">
		<? if ( $errorMessage || $infoMessage || $successMessage ) { ?>
			<tr>
				<td colspan="6">
				<? if ( $successMessage ) { ?>
					<p class="successMessage"><?=$successMessage ?></p>
				<? } ?>
				<? if ( $errorMessage ) { ?>
					<p class="errorMessage"><?=$errorMessage ?></p>
				<? } ?>
				<? if ( $infoMessage > 0 ) { ?>
					<p class="infoMessage"><?=$infoMessage ?></p>
				<? } ?>
				</td>
			</tr>
		<? } ?>
		<tr>
			<th>ip</th>
			<th>port</th>
			<th>name</th>
			<th>last online</th>
			<th>next check</th>
			<th>action</th>
		</tr>
		<? foreach ( $servers as $server ) { ?>
			<tr>
				<? showYaeValue($server, 'ip', "server"); ?>
				<? showYaeValue($server, 'port', "server"); ?>
				<? showYaeValue($server, 'name', "server"); ?>
				<? showYaeValue($server, 'lastonline', "server"); ?>
				<? showYaeValue($server, 'nextcheck', "server"); ?>
				<td>
					<a class="important_link icon ipassword" href="<?= $this->link("UserServers","changeServerPassword", array("server_id"=>$server['id'])) ?>">change password</a>
					<? if ( $server['nextcheck'] == YAE_SERVER_OFF_TIME ) { ?>
						<a class="important_link icon iactivate" href="<?= $this->link("UserServers","activateServer", array("server_id"=>$server['id'])) ?>">activate server checks</a>
					<? } else { ?>
						<? if ( $server['nextcheck'] - time() > 3600 ) { ?>
							<a class="important_link icon iactivate" href="<?= $this->link("UserServers","activateServer", array("server_id"=>$server['id'])) ?>">check server now</a>
						<? } ?>
						<a class="important_link icon ideactivate" href="<?= $this->link("UserServers","deactivateServer", array("server_id"=>$server['id'])) ?>">deactivate server checks</a>
					<? } ?>
					<a class="important_link icon istats" href="<?= $this->link("Search","showServer", array("server_id"=>$server['id'])) ?>">server stats</a>
				</td>
			</tr>
		<? } ?>
	</table>
<? } ?>