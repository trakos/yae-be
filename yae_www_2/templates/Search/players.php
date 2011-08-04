<? $playersForm->render(); ?>
<? if( empty($players) ) { ?>
	<p class="error">no matches</p>
<? } else { ?>
	<table class="result">
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
						<?=$player['nick']; ?>
					</a>
				</td>
				<td>
					<a rel="nofollow" href="?show=players&amp;ip=<?=urlencode($player['ip']) ?>">
						<img alt="be" src="images/flags/be.png" /> <?=$player['ip'] ?>
					</a>
				</td>
				<td>
					<a rel="nofollow" href="?show=players&amp;pbguid=<?=urlencode($player['pbguid']) ?>"><?=$player['pbguid']; ?></a>
				</td>
				<td>
					<a rel="nofollow" href="?show=players&amp;etproguid=<?=urlencode($player['etproguid']) ?>"><?=$player['etproguid']; ?></a>
				</td>
				<td>
					<a rel="nofollow" href="?show=players&amp;slacnick=<?=urlencode($player['slacnick']) ?>"><?=$player['slacnick']; ?></a>
				</td>
				<td>
					<a rel="nofollow" href="?show=players&amp;slacid=<?=urlencode($player['slacid']) ?>"><?=$player['slacid']; ?></a>
				</td>
				<td>
					<a rel="nofollow" href="?show=player&amp;id=<?=urlencode($player['id']) ?>">»</a>
				</td>
			</tr>
		<? } ?>
	</table>
<? } ?>
<p class="note">Generated in 0.1699 seconds with use of 2 queries which took 0.1601 seconds</p></div>