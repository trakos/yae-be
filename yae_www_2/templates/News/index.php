<? foreach($news as $n) { ?>
		<div class='news'>
		<h2><?=$n['title']?> <span class='date'>on <?=$n['date']?></span></h2>
		<div class='content'><?=$n['text']?></div>
		</div>
<? } ?>