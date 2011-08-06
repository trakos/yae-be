<? $playersForm->render(); ?>
<? if( empty($players) ) { ?>
	<p class="error">no matches</p>
<? } else { ?>
	<table class="result yae">
		<tr>
			<th>nick</th>
			<th>ip</th>
			<th>pbguid</th>
			<th>etproguid</th>
			<th>slacnick</th>
			<th>slacid</th>
			<th>details</th>
		</tr>
		<? foreach ( $players as $player ) { ?>
			<tr>
				<td>
					<a rel="nofollow" href="<?= $this->link("Search","players", array('nick'=>$player['nick'])) ?>">
						<?=View_Yae::formatValue('nick', $player['nick'], 'player') ?>
					</a>
				</td>
				<td>
					<a rel="nofollow" href="<?= $this->link("Search","players", array('ip'=>$player['ip'])) ?>">
						<?=View_Yae::formatValue('ip', $player['realip'], 'player') ?>
					</a>
				</td>
				<td>
					<a rel="nofollow" href="<?= $this->link("Search","players", array('pbguid'=>$player['pbguid'])) ?>">
						<?=View_Yae::formatValue('pbguid', $player['pbguid'], 'player') ?>
					</a>
				</td>
				<td>
					<a rel="nofollow" href="<?= $this->link("Search","players", array('etproguid'=>$player['etproguid'])) ?>">
						<?=View_Yae::formatValue('etproguid', $player['etproguid'], 'player') ?>
					</a>
				</td>
				<td>
					<a rel="nofollow" href="<?= $this->link("Search","players", array('slacnick'=>$player['slacnick'])) ?>">
						<?=View_Yae::formatValue('slacnick', $player['slacnick'], 'player') ?>
					</a>
				</td>
				<td>
					<a rel="nofollow" href="<?= $this->link("Search","players", array('slacid'=>$player['slacid'])) ?>">
						<?=View_Yae::formatValue('slacid', $player['slacid'], 'player') ?>
					</a>
				</td>
				<td>
					<a class="important_link details_stats" rel="nofollow" href="<?= $this->link("Search","showPlayer", array("player_id"=>$player['id'])) ?>">
						player stats »
					</a>
				</td>
			</tr>
		<? } ?>
	</table>
	<?=$this->paginator($page, $limit, $count, "Search", "players", $playersForm->getValues(), "page") ?>
<? } ?>