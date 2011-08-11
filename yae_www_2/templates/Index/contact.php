<div style="margin: 0 auto;width:50%;">
	<? if ( !Model_Auth::getInstance()->isLogged() ) { ?>
		<p class="infoMessage">Please leave your e-mail address in the message if you expect any answer :)</p>
	<? } ?>
	<? $contactForm->render(); ?>
</div>