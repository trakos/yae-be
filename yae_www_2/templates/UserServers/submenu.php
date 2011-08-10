<table class="yae result_description submenu">
	<? foreach ( $submenu as $action => $name ) { ?>
		<tr>
			<td>
				<? if ( $action != $currentAction ) { ?>
					<a href="<?=$this->link("UserServers", $action) ?>"><?=$name ?></a>
				<? } else { ?>
					<span><?= $name ?></span>
				<? } ?>
			</td>
		</tr>
	<? } ?>
</table>