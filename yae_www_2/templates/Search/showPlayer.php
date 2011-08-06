
<? if( empty($player) ) { ?>
<p class="error">Player with given id not found, 404 error!</p>
<? } else { ?>
	<table class='yae result_description'>
		<tr>
			<th colspan='2'>Player</th>
		</tr>
		<tr>
			<th>Nick:</th>
			<td>
				<a rel="nofollow" href="<?= $this->link("Search","players", array('nick'=>$player['nick'])) ?>">
					<?=View_Yae::formatValue('nick', $player['nick'], 'player') ?>
				</a>
			</td>
		</tr>
		<tr>
			<th>IP:</th>
			<td>
				<a rel="nofollow" href="<?= $this->link("Search","players", array('ip'=>$player['ip'])) ?>">
					<?=View_Yae::formatValue('ip', $player['realip'], 'player') ?>
				</a>
			</td>
		</tr>
		<tr>
			<th>PunkBuster guid:</th>
			<td>
				<a rel="nofollow" href="<?= $this->link("Search","players", array('pbguid'=>$player['pbguid'])) ?>">
					<?=View_Yae::formatValue('pbguid', $player['pbguid'], 'player') ?>
				</a>
			</td>
		</tr>
		<tr>
			<th>ET Pro guid:</th>
			<td>
				<a rel="nofollow" href="<?= $this->link("Search","players", array('etproguid'=>$player['etproguid'])) ?>">
					<?=View_Yae::formatValue('etproguid', $player['etproguid'], 'player') ?>
				</a>
			</td>
		</tr>
		<tr>
			<th>SLAC id:</th>
			<td>
				<a rel="nofollow" href="<?= $this->link("Search","players", array('slacid'=>$player['slacid'])) ?>">
					<?=View_Yae::formatValue('slacid', $player['slacid'], 'player') ?>
				</a>
			</td>
		</tr>
		<tr>
			<th>SLAC nick:</th>
			<td>
				<a rel="nofollow" href="<?= $this->link("Search","players", array('slacnick'=>$player['slacnick'])) ?>">
					<?=View_Yae::formatValue('slacnick', $player['slacnick'], 'player') ?>
				</a>
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
						<a href="<?= $this->link("Search","showPlayer", array("player_id"=>$player["id"], "type"=>$typeKey)) ?>" class="tab">
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
						<? } else if ( $currentType == "monthly" || $currentType == "total" ) { ?>
							<th>time played (sum)</th>
							<? if ( $currentType == "monthly" ) { ?>
								<th>month.year</th>
							<? } ?>
						<? } ?>
						<th>server</th>
						<th>name</th>
						<th></th>
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
						<? } else if ( $currentType == "monthly" || $currentType == "total" ) { ?>
							<td>
								<?=View_Yae::formatValue('minutesplayed', $entry['minutesplayed'], 'player') ?>
							</td>
							<? if ( $currentType == "monthly" ) { ?>
								<td>
									<?= sprintf("%02d.%04d",$entry['month'], $entry['year']) ?>
								</td>
							<? } ?>
						<? } ?>
						<td>
							<?=View_Yae::formatValue('server', $entry['server'], 'server') ?>
						</td>
						<td>
							<?=View_Yae::formatValue('name', $entry['name'], 'server') ?>
						</td>
						<td>
							<a class="important_link details_stats" rel="nofollow"href="<?= $this->link("Search","showServer", array("server_id"=>$entry['id'])) ?>">
								server stats »
							</a>
						</td>
					</tr>
					<? } ?>
				</table>
			</td>
		</tr>
	</table>
	<?=$this->paginator($page, $limit, $count, "Search", "showPlayer", array("player_id"=>$player["id"], "type"=>$currentType), "page") ?>
<? } ?>