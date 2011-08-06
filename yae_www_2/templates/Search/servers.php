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
				<td>
					<a rel="nofollow" href="<?= $this->link("Search","servers", array('ip'=>$server['ip'])) ?>">
						<?=View_Yae::formatValue('ip', $server['ip'], 'server') ?>
					</a>
				</td>
				<td>
					<a rel="nofollow" href="<?= $this->link("Search","servers", array('port'=>$server['port'])) ?>">
						<?=View_Yae::formatValue('port', $server['port'], 'server') ?>
					</a>
				</td>
				<td>
					<?=View_Yae::formatValue('name', $server['name'], 'server') ?>
				</td>
				<td>
					<?=View_Yae::formatValue('lastonline', $server['lastonline'], 'server') ?>
				</td>
				<td>
					<a class="important_link details_stats" rel="nofollow" href="<?= $this->link("Search","showServer", array("server_id"=>$server['id'])) ?>">server stats »</a>
				</td>
			</tr>
		<? } ?>
	</table>
	<?=$this->paginator($page, $limit, $count, "Search", "servers", $serversForm->getValues(), "page") ?>
<? } ?>