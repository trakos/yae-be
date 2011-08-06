
<? $this->render("blocks/snippets"); ?>
<? $playersForm->render(); ?>
<? if( empty($players) ) { ?>
	<p class="error">no matches</p>
<? } else { ?>
	<table class="result wrap">
		<tr>
			<td>
				<? foreach ( $types as $typeKey => $typeName ) { ?>
					<? if ( $typeKey == $currentType ) { ?>
						<p class="tab selected"><?=$typeName ?></p>
					<? } else { ?>
						<? $arguments = $playersForm->getValues(); ?>
						<? $arguments['type'] = $typeKey; ?>
						<a href="<?= $this->link("Search","players", $arguments) ?>" class="tab">
							<?=$typeName ?>
						</a>
					<? } ?>
				<? } ?>
			</td>
		</tr>
		<tr>
			<td>
				<table class="result yae">
					<tr>
						<? if ( $currentType == "slac" ) { ?>
							<th>slacnick</th>
							<th>slacid</th>
							<th>nicks count</th>
							<th>example</th>
							<th>ip adresses count</th>
							<th>example</th>
							<th>etproguids count</th>
							<th>entries</th>
							<th>servers stats</th>
						<? } else if ( $currentType == "pb" )  { ?>
							<th>pbguid</th>
							<th>nicks count</th>
							<th>example</th>
							<th>ip adresses count</th>
							<th>example</th>
							<th>etproguids count</th>
							<th>entries</th>
							<th>servers stats</th>
						<? } else if ( $currentType == "ip" )  { ?>
							<th>ip</th>
							<th>nicks count</th>
							<th>example</th>
							<th>pbguid count</th>
							<th>etproguids count</th>
							<th>slacids count</th>
							<th>exact entries</th>
							<th>servers stats</th>
						<? } else if ( $currentType == "etpro" )  { ?>
							<th>etproguid</th>
							<th>nicks count</th>
							<th>example</th>
							<th>ip adresses count</th>
							<th>example</th>
							<th>pbguid count</th>
							<th>slacids count</th>
							<th>entries</th>
							<th>servers stats</th>
						<? } else { ?>
							<th>nick</th>
							<th>ip</th>
							<th>pbguid</th>
							<th>etproguid</th>
							<th>slacnick</th>
							<th>slacid</th>
							<th>servers stats</th>
						<? } ?>
					</tr>
					<? foreach ( $players as $player ) { ?>
						<tr>
							<? if ( $currentType == "slac" ) { ?>
								<?=showYaeSearchValue($player,"slacnick") ?>
								<?=showYaeSearchValue($player,"slacid") ?>
								<td><?=$player['nick_count']; ?></td>
								<?=showYaeSearchValue($player,"nick") ?>
								<td><?=$player['ip_count']; ?></td>
								<?=showYaeSearchValue($player,"ip") ?>
								<td><?=$player['etproguid_count']; ?></td>
								<td>
									<? $arguments = $playersForm->getValues(); ?>
									<? $arguments['slacid'] = $player['slacid']; ?>
									<a class="important_link details_stats" rel="nofollow" href="<?=$this->link("Search","players", $arguments) ?>">
										non-grouped »
									</a>
								</td>
								<td>
									<a class="important_link details_stats" rel="nofollow" href="<?=$this->link("Search","showPlayer", array("id_type"=>"slacid",'player_id'=>$player['slacid'])) ?>">
										servers »
									</a>
								</td>
							<? } else if ( $currentType == "pb" ) { ?>
								<?=showYaeSearchValue($player,"pbguid") ?>
								<td><?=$player['nick_count']; ?></td>
								<?=showYaeSearchValue($player,"nick") ?>
								<td><?=$player['ip_count']; ?></td>
								<?=showYaeSearchValue($player,"ip") ?>
								<td><?=$player['etproguid_count']; ?></td>
								<td>
									<? $arguments = $playersForm->getValues(); ?>
									<? $arguments['pbguid'] = $player['pbguid']; ?>
									<a class="important_link details_stats" rel="nofollow" href="<?=$this->link("Search","players", $arguments) ?>">
										non-grouped »
									</a>
								</td>
								<td>
									<a class="important_link details_stats" rel="nofollow" href="<?=$this->link("Search","showPlayer", array("id_type"=>"pbguid",'player_id'=>$player['pbguid'])) ?>">
										servers »
									</a>
								</td>
							<? } else if ( $currentType == "ip" ) { ?>
								<?=showYaeSearchValue($player,"ip") ?>
								<td><?=$player['nick_count']; ?></td>
								<?=showYaeSearchValue($player,"nick") ?>
								<td><?=$player['pbguid_count']; ?></td>
								<td><?=$player['etproguid_count']; ?></td>
								<td><?=$player['slacid_count']; ?></td>
								<td>
									<? $arguments = $playersForm->getValues(); ?>
									<? $arguments['ip'] = $player['ip']; ?>
									<a class="important_link details_stats" rel="nofollow" href="<?=$this->link("Search","players", $arguments) ?>">
										non-grouped »
									</a>
								</td>
								<td>
									<a class="important_link details_stats" rel="nofollow" href="<?=$this->link("Search","showPlayer", array("id_type"=>"ip",'player_id'=>$player['ip'])) ?>">
										servers »
									</a>
								</td>
							<? } else if ( $currentType == "etpro" ) { ?>
								<?=showYaeSearchValue($player,"etproguid") ?>
								<td><?=$player['nick_count']; ?></td>
								<?=showYaeSearchValue($player,"nick") ?>
								<td><?=$player['ip_count']; ?></td>
								<?=showYaeSearchValue($player,"ip") ?>
								<td><?=$player['pbguid_count']; ?></td>
								<td><?=$player['slacid_count']; ?></td>
								<td>
									<? $arguments = $playersForm->getValues(); ?>
									<? $arguments['etproguid'] = $player['etproguid']; ?>
									<a class="important_link details_stats" rel="nofollow" href="<?=$this->link("Search","players", $arguments) ?>">
										non-grouped »
									</a>
								</td>
								<td>
									<a class="important_link details_stats" rel="nofollow" href="<?=$this->link("Search","showPlayer", array("id_type"=>"etproguid",'player_id'=>$player['etproguid'])) ?>">
										servers »
									</a>
								</td>
							<? } else { ?>
								<?=showYaeSearchValue($player,"nick") ?>
								<?=showYaeSearchValue($player,"ip") ?>
								<?=showYaeSearchValue($player,"pbguid") ?>
								<?=showYaeSearchValue($player,"etproguid") ?>
								<?=showYaeSearchValue($player,"slacnick") ?>
								<?=showYaeSearchValue($player,"slacid") ?>
								<td>
									<a class="important_link details_stats" rel="nofollow" href="<?= $this->link("Search","showPlayer", array("id_type"=>"id","player_id"=>$player['id'])) ?>">
										servers »
									</a>
								</td>
							<? } ?>
						</tr>
					<? } ?>
				</table>
			</td>
		</tr>
	</table>	
	<? $arguments = $playersForm->getValues(); ?>
	<? $arguments['type'] = $currentType; ?>
	<?=$this->paginator($page, $limit, $count, "Search", "players", $arguments, "page") ?>
<? } ?>
<p class='note'>Monthly & all time stats are updated nightly.</p>