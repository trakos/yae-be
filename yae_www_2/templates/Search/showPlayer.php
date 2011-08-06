
<? $this->render("blocks/snippets"); ?>
<? if( empty($player) ) { ?>
<p class="error">Player with given id not found, 404 error!</p>
<? } else { ?>
	<table class='yae result_description'>
		<tr>
			<th colspan='2'>Player</th>
		</tr>
		<? if ( $idType == "slacid" ) { ?>
			<tr>
				<th>SLAC id:</th>
				<? showYaeSearchValue($player, 'slacid'); ?>
			</tr>
			<tr>
				<th>SLAC nick:</th>
				<? showYaeSearchValue($player, 'slacnick'); ?>
			</tr>
		<? } else if ( $idType == "ip" ) { ?>
			<tr>
				<th>IP:</th>
				<? showYaeSearchValue($player, 'ip'); ?>
			</tr>
		<? } else if ( $idType == "pbguid" ) { ?>
			<tr>
				<th>PunkBuster guid:</th>
				<? showYaeSearchValue($player, 'pbguid'); ?>
			</tr>
		<? } else if ( $idType == "etproguid" ) { ?>
			<tr>
			<th>ET Pro guid:</th>
				<? showYaeSearchValue($player, 'etproguid'); ?>
			</tr>
		<? } else { ?>
			<tr>
				<th>Nick:</th>
				<? showYaeSearchValue($player, 'nick'); ?>
			</tr>
			<tr>
				<th>IP:</th>
				<? showYaeSearchValue($player, 'ip'); ?>
			</tr>
			<tr>
				<th>PunkBuster guid:</th>
				<? showYaeSearchValue($player, 'pbguid'); ?>
			</tr>
			<tr>
				<th>ET Pro guid:</th>
				<? showYaeSearchValue($player, 'etproguid'); ?>
			</tr>
			<tr>
				<th>SLAC id:</th>
				<? showYaeSearchValue($player, 'slacid'); ?>
			</tr>
			<tr>
				<th>SLAC nick:</th>
				<? showYaeSearchValue($player, 'slacnick'); ?>
			</tr>
		<? } ?>
	</table>
	<table class="result wrap">
		<tr>
			<td>
				<? foreach ( $types as $typeKey => $typeName ) { ?>
					<? if ( $typeKey == $currentType ) { ?>
						<p class="tab selected"><?=$typeName ?></p>
					<? } else { ?>
						<a href="<?= $this->link("Search","showPlayer", array("player_id"=>$playerId, "type"=>$typeKey, "id_type"=>$idType)) ?>" class="tab">
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
						<? if ( $currentType == "exact" ) { ?>
							<? if ( $idType == "slacid" ) { ?>
								<th>nick</th>
								<th>ip</th>
								<th>etproguid</th>
							<? } else if ( $idType == "ip" ) { ?>
								<th>nick</th>
								<th>pbguid</th>
								<th>etproguid</th>
								<th>slacnick</th>
								<th>slacid</th>
							<? } else if ( $idType == "pbguid" ) { ?>
								<th>nick</th>
								<th>ip</th>
								<th>etproguid</th>
							<? } else if ( $idType == "etproguid" ) { ?>
								<th>nick</th>
								<th>ip</th>
								<th>pbguid</th>
								<th>slacnick</th>
								<th>slacid</th>
							<? } ?>
						<? } ?>
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
						<? if ( $currentType == "exact" ) { ?>
							<? if ( $idType == "slacid" ) { ?>
								<? showYaeSearchValue($entry, 'nick'); ?>
								<? showYaeSearchValue($entry, 'ip'); ?>
								<? showYaeSearchValue($entry, 'etproguid'); ?>
							<? } else if ( $idType == "ip" ) { ?>
								<? showYaeSearchValue($entry, 'nick'); ?>
								<? showYaeSearchValue($entry, 'pbguid'); ?>
								<? showYaeSearchValue($entry, 'etproguid'); ?>
								<? showYaeSearchValue($entry, 'slacnick'); ?>
								<? showYaeSearchValue($entry, 'slacid'); ?>
							<? } else if ( $idType == "pbguid" ) { ?>
								<? showYaeSearchValue($entry, 'nick'); ?>
								<? showYaeSearchValue($entry, 'ip'); ?>
								<? showYaeSearchValue($entry, 'etproguid'); ?>
							<? } else if ( $idType == "etproguid" ) { ?>
								<? showYaeSearchValue($entry, 'nick'); ?>
								<? showYaeSearchValue($entry, 'ip'); ?>
								<? showYaeSearchValue($entry, 'pbguid'); ?>
								<? showYaeSearchValue($entry, 'slacnick'); ?>
								<? showYaeSearchValue($entry, 'slacid'); ?>
							<? } ?>
						<? } ?>
						<td>
							<a class="important_link details_stats" rel="nofollow"href="<?= $this->link("Search","showServer", array("server_id"=>$entry['serverid'])) ?>">
								server stats »
							</a>
						</td>
					</tr>
					<? } ?>
				</table>
			</td>
		</tr>
	</table>
	<?=$this->paginator($page, $limit, $count, "Search", "showPlayer", array("player_id"=>$playerId, "type"=>$currentType, "id_type"=>$idType), "page") ?>
<? } ?>
<p class='note'>Monthly & all time stats are updated nightly.</p>