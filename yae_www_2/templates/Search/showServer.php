
<? if( empty($server) ) { ?>
<p class="error">Server with given id not found, 404 error!</p>
<? } else { ?>
	<table class='yae result_description'>
		<tr>
			<th colspan='2'>Server</th>
		</tr>
		<tr>
			<th>IP:</th>
			<td>
				<a rel="nofollow" href="<?= $this->link("Search","servers", array('ip'=>$server['ip'])) ?>">
					<?=View_Yae::formatValue('ip', $server['ip'], 'server') ?>
				</a>
			</td>
		</tr>
		<tr>
			<th>Port:</th>
			<td>
				<a rel="nofollow" href="<?= $this->link("Search","servers", array('port'=>$server['port'])) ?>">
					<?=View_Yae::formatValue('port', $server['port'], 'server') ?>
				</a>
			</td>
		</tr>
		<tr>
			<th>Name:</th>
			<td>
				<?=View_Yae::formatValue('name', $server['name'], 'server') ?>
			</td>
		</tr>
		<tr>
			<th>Last online:</th>
			<td>
				<?=View_Yae::formatValue('lastonline', $server['lastonline'], 'server') ?>
			</td>
		</tr>
	</table>
	<table class="result wrap">
		<tr>
			<td>
				<? foreach ( $types as $typeKey => $typeName ) { ?>
					<? if ( $typeKey == $currentType ) { ?>
						<p class="tab selected"><?=$typeName ?></p>
					<? } else { ?>
						<a href="<?= $this->link("Search","showServer", array("server_id"=>$server["id"], "type"=>$typeKey)) ?>" class="tab">
							<?=$typeName ?>
						</a>
					<? } ?>
				<? } ?>
			</td>
		</tr>
		<tr>
			<td>
				<table class="yae result">
					<tr>
						<? if ( $currentType == "exact" ) { ?>
							<th>played from</th>
							<th>played to</th>
						<? } else if ( $currentType == "alltime" || $currentType == "lastmonth" ) { ?>
							<th>time played (sum)</th>
						<? } ?>
						<th>nick</th>
						<th>ip</th>
						<th>pbguid</th>
						<th>etproguid</th>
						<th>slacnick</th>
						<th>slacid</th>
						<th>details</th>
					</tr>
					<? foreach ( $history as $entry ) { ?>
					<tr>
						<? if ( $currentType == "exact" ) { ?>
							<td>
								<?=View_Yae::formatValue('playedfrom', $entry['playedfrom'], 'player') ?>
							</td>
							<td>
								<?=View_Yae::formatValue('playedto', $entry['playedto'], 'player') ?>
							</td>
						<? } else if ( $currentType == "alltime" || $currentType == "lastmonth" ) { ?>
							<td>
								<?=View_Yae::formatValue('minutesplayed', $entry['minutesplayed'], 'player') ?>
							</td>
						<? } ?>
						<td>
							<a rel="nofollow" href="<?= $this->link("Search","players", array('nick'=>$entry['nick'])) ?>">
								<?=View_Yae::formatValue('nick', $entry['nick'], 'player') ?>
							</a>
						</td>
						<td>
							<a rel="nofollow" href="<?= $this->link("Search","players", array('ip'=>$entry['ip'])) ?>">
								<?=View_Yae::formatValue('ip', $entry['realip'], 'player') ?>
							</a>
						</td>
						<td>
							<a rel="nofollow" href="<?= $this->link("Search","players", array('pbguid'=>$entry['pbguid'])) ?>">
								<?=View_Yae::formatValue('pbguid', $entry['pbguid'], 'player') ?>
							</a>
						</td>
						<td>
							<a rel="nofollow" href="<?= $this->link("Search","players", array('etproguid'=>$entry['etproguid'])) ?>">
								<?=View_Yae::formatValue('etproguid', $entry['etproguid'], 'player') ?>
							</a>
						</td>
						<td>
							<a rel="nofollow" href="<?= $this->link("Search","players", array('slacnick'=>$entry['slacnick'])) ?>">
								<?=View_Yae::formatValue('slacnick', $entry['slacnick'], 'player') ?>
							</a>
						</td>
						<td>
							<a rel="nofollow" href="<?= $this->link("Search","players", array('slacid'=>$entry['slacid'])) ?>">
								<?=View_Yae::formatValue('slacid', $entry['slacid'], 'player') ?>
							</a>
						</td>
						<td>
							<a class="important_link details_stats" rel="nofollow"href="<?= $this->link("Search","showPlayer", array("player_id"=>$entry['playerid'])) ?>">
								player stats »
							</a>
						</td>
					</tr>
					<? } ?>
				</table>
			</td>
		</tr>
	</table>
	<?=$this->paginator($page, $limit, $count, "Search", "showServer", array("server_id"=>$server["id"], "type"=>$currentType), "page") ?>
<? } ?>