
<? $this->render("blocks/snippets"); ?>
<? if( empty($server) ) { ?>
<p class="error">Server with given id not found, 404 error!</p>
<? } else { ?>
	<table class='yae result_description'>
		<tr>
			<th colspan='2'>Server</th>
		</tr>
		<tr>
			<th>IP:</th>
			<? showYaeSearchValue($server, 'ip', "server"); ?>
		</tr>
		<tr>
			<th>Port:</th>
			<? showYaeSearchValue($server, 'port', "server"); ?>
		</tr>
		<tr>
			<th>Name:</th>
			<? showYaeSearchValue($server, 'name', "server"); ?>
		</tr>
		<tr>
			<th>Last online:</th>
			<? showYaeSearchValue($server, 'lastonline', "server"); ?>
		</tr>
		<tr>
			<th>Group by:</th>
			<td>
				<select name="groupby" id="groupby">
					<? foreach ( $groups as $groupKey => $groupName ) { ?>
						<option <?= ( $groupKey==$currentGroup ) ? 'selected="selected"' : '' ?> value="<?=$groupKey ?>">
							<?=$groupName ?>
						</option>
					<? } ?>
				</select>
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
						<a href="<?= $this->link("Search","showServer", array("server_id"=>$server["id"], "type"=>$typeKey, "group_by"=>$currentGroup)) ?>" class="tab">
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
						<? if ( $currentGroup == "ip" ) { ?>
							<th>ip</th>
						<? } else if ( $currentGroup == "etproguid" ) { ?>
							<th>etproguid</th>
						<? } else if ( $currentGroup == "slacid" ) { ?>
							<th>slacid</th>
							<th>slacnick</th>
						<? } else if ( $currentGroup == "pbguid" ) { ?>
							<th>pbguid</th>
						<? } else { ?>
							<th>nick</th>
							<th>ip</th>
							<th>pbguid</th>
							<th>etproguid</th>
							<th>slacnick</th>
							<th>slacid</th>
						<? } ?>
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
						<? if ( $currentGroup == "ip" ) { ?>
							<? showYaeSearchValue($entry, 'ip'); ?>
						<? } else if ( $currentGroup == "etproguid" ) { ?>
							<? showYaeSearchValue($entry, 'etproguid'); ?>
						<? } else if ( $currentGroup == "slacid" ) { ?>
							<? showYaeSearchValue($entry, 'slacid'); ?>
							<? showYaeSearchValue($entry, 'slacnick'); ?>
						<? } else if ( $currentGroup == "pbguid" ) { ?>
							<? showYaeSearchValue($entry, 'pbguid'); ?>
						<? } else { ?>
							<? showYaeSearchValue($entry, 'nick'); ?>
							<? showYaeSearchValue($entry, 'ip'); ?>
							<? showYaeSearchValue($entry, 'pbguid'); ?>
							<? showYaeSearchValue($entry, 'etproguid'); ?>
							<? showYaeSearchValue($entry, 'slacnick'); ?>
							<? showYaeSearchValue($entry, 'slacid'); ?>
						<? } ?>
						<td>
							<? $currentGroupId = $currentGroup=="id"?"playerid":$currentGroup; ?>
							<a class="important_link details_stats" rel="nofollow"href="<?= $this->link("Search","showPlayer", array("id_type"=>$currentGroup,"player_id"=>$entry[$currentGroupId])) ?>">
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
<p class='note'>Monthly & all time stats are updated nightly.</p>