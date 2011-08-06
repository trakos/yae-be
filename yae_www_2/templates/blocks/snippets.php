<? function showYaeSearchValue($row, $valueIndex, $type="player") { ?>
	<td>
		<a rel="nofollow" href="<?= Tmvc_View::getInstance()->link("Search",$type=="player"?"players":"servers", array($valueIndex=>$row[$valueIndex])) ?>">
			<? $valueIndex = ($valueIndex=="ip"&&$type=="player")?"realip":$valueIndex; ?>
			<?=View_Yae::formatValue($valueIndex, $row[$valueIndex], $type) ?>
		</a>
	</td>
<? } ?>