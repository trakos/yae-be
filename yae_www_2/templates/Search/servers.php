<? $this->render("blocks/snippets"); ?>
<? $serversForm->render(); ?>
<? if( empty($servers) ) { ?>
	<p class="error">no matches</p>
<? } else { ?>
	<table class="yae result">
		<tr>
			<th>ip</th>
			<th>port</th>
			<th>name</th>
			<th>last online</th>
			<th>details</th>
		</tr>
		<? foreach ( $servers as $server ) { ?>
			<tr>
				<? showYaeSearchValue($server, 'ip', "server"); ?>
				<? showYaeSearchValue($server, 'port', "server"); ?>
				<? showYaeSearchValue($server, 'name', "server"); ?>
				<? showYaeValue($server, 'lastonline', "server"); ?>
				<td>
					<a class="important_link details_stats" rel="nofollow" href="<?= $this->link("Search","showServer", array("server_id"=>$server['id'])) ?>">server stats »</a>
				</td>
			</tr>
		<? } ?>
	</table>
	<?=$this->paginator($page, $limit, $count, "Search", "servers", $serversForm->getValues(), "page") ?>
<? } ?>
<p class='note'>Monthly & all time stats are updated nightly.</p>