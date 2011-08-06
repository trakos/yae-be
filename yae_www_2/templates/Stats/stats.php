<?php 
	function showLink($typeKey, $typeName, $currentType) 
	{
		if ( $typeKey == $currentType )
		{
			return "<span>&raquo; $typeName</span>";
		}
		return "<a href='".Tmvc_View::getInstance()->link("Stats","stats", array("type"=>$typeKey))."'>&raquo; $typeName</a>";
	}
?>
<table class="yae result_description statsmenu">
	<tr><th colspan="3">stats</th></tr>
	<tr>
		<td rowspan="6">Most active players</td>
		<td rowspan="2">by SLAC id</td>
		<td style="width:100px;"><?=showLink("slacmonth", "last 30 days", $currentType) ?></td>
	</tr>
	<tr>
		<td><?=showLink("slacall", "all time", $currentType) ?></td>
	</tr>
	<tr>
		<td rowspan="2">by PunkBuster guid</td>
		<td><?=showLink("pbmonth", "last 30 days", $currentType) ?></td>
	</tr>
	<tr>
		<td><?=showLink("pball", "all time", $currentType) ?></td>
	</tr>
	<tr>
		<td rowspan="2">by nick/guid combinations</td>
		<td><?=showLink("plrsmonth", "last 30 days", $currentType) ?></td>
	</tr>
	<tr>
		<td><?=showLink("plrsall", "all time", $currentType) ?></td>
	</tr>
	<tr>
		<td rowspan="2" colspan="2">Most active servers</td>
		<td><?=showLink("srvmonth", "last 30 days", $currentType) ?></td>
	</tr>
	<tr>
		<td><?=showLink("srvall", "all time", $currentType) ?></td>
	</tr>
	<? /* foreach ( $types as $typeKey => $typeName ) { ?>
		<tr>
			<? if ( $typeKey == $currentType ) { ?>
				<td class="selected"><p><?=$typeName ?></p></td>
			<? } else { ?>
				<td>
					<a href="<?= $this->link("Stats","stats", array("type"=>$typeKey)) ?>" class="tab">
						<?=$typeName ?>
					</a>
				</td>
			<? } ?>
		</tr>
	<? } */ ?>
</table>
<table class="yae result">
	<tr>
		<? if ( $currentType == "srvmonth" || $currentType == "srvall" ) { ?>
			<th>times spent by all players (sum)</th>
			<th>server</th>
			<th>name</th>
		<? } else { ?>
			<th>times spent on all servers (sum)</th>
			<? if ( $currentType == "plrsall" || $currentType == "plrsmonth" ) { ?>
				<th>nick</th>
				<th>ip</th>
				<th>pbguid</th>
				<th>etproguid</th>
				<th>slacnick</th>
				<th>slacid</th>
			<? } else if ( $currentType == "pbmonth" || $currentType == "pball" ) { ?>
				<th>pbguid</th>
				<th>nicks count</th>
				<th>example</th>
				<th>ip adresses count</th>
				<th>example</th>
				<th>etproguids count</th>
				<th>example</th>
			<? } else if ( $currentType == "slacmonth" || $currentType == "slacall" ) { ?>
				<th>slacnick</th>
				<th>slacid</th>
				<th>nicks count</th>
				<th>example</th>
				<th>ip adresses count</th>
				<th>example</th>
				<th>etproguids count</th>
				<th>example</th>
			<? } ?>
		<? } ?>
		<th></th>
	</tr>
	<? foreach ( $history as $entry ) { ?>
	<tr>
		<td>
			<?=View_Yae::formatValue('minutesplayed', $entry['minutesplayed'], 'player') ?>
		</td>
		<? if ( $currentType == "srvmonth" || $currentType == "srvall" ) { ?>
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
		<? } else { ?>
			<? if ( $currentType == "plrsall" || $currentType == "plrsmonth" ) { ?>
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
			<? } else if ( $currentType == "pbmonth" || $currentType == "pball" ) { ?>
				<td>
					<a rel="nofollow" href="<?= $this->link("Search","players", array('pbguid'=>$entry['pbguid'])) ?>">
						<?=View_Yae::formatValue('pbguid', $entry['pbguid'], 'player') ?>
					</a>
				</td>
			<? } else if ( $currentType == "slacmonth" || $currentType == "slacall" ) { ?>
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
			<? } ?>
			<? if ( $currentType == "pbmonth" || $currentType == "pball" || $currentType == "slacmonth" || $currentType == "slacall" ) { ?>
				<td>
					<?= $entry['nick_count'] ?>
				</td>
				<td>
					<a rel="nofollow" href="<?= $this->link("Search","players", array('nick'=>$entry['nick'])) ?>">
						<?=View_Yae::formatValue('nick', $entry['nick'], 'player') ?>
					</a>
				</td>
				<td>
					<?= $entry['ip_count'] ?>
				</td>
				<td>
					<a rel="nofollow" href="<?= $this->link("Search","players", array('ip'=>$entry['ip'])) ?>">
						<?=View_Yae::formatValue('ip', $entry['realip'], 'player') ?>
					</a>
				</td>
				<td>
					<?= $entry['etproguid_count'] ?>
				</td>
				<td>
					<a rel="nofollow" href="<?= $this->link("Search","players", array('etproguid'=>$entry['etproguid'])) ?>">
						<?=View_Yae::formatValue('etproguid', $entry['etproguid'], 'player') ?>
					</a>
				</td>
			<? } ?>
			<? if ( $currentType == "pbmonth" || $currentType == "pball" ) { ?>
				<td>
					<a class="important_link details_stats" rel="nofollow"href="<?=$this->link("Search","players", array('pbguid'=>$entry['pbguid'])) ?>">
						player entries »
					</a>
				</td>
			<? } else if ( $currentType == "slacmonth" || $currentType == "slacall" ) { ?>
				<td>
					<a class="important_link details_stats" rel="nofollow"href="<?=$this->link("Search","players", array('slacid'=>$entry['slacid'])) ?>">
						player entries »
					</a>
				</td>
			<? } ?>
		<? } ?>
	</tr>
	<? } ?>
</table>
<?=$this->paginator($page, $limit, $count, "Stats", "stats", array("type"=>$currentType), "page") ?>
