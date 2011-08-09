<div style="margin: 80px auto;width:50%;">
	<? if ( $activated ) { ?>
		<table class="successMessage" style="width:100%">
			<tr><th colspan="2">Activation successful!</th></tr>
			<tr><td>Let me check it... yeah, it looks like it is really you. OK, I've activated your account. Go ahead and use the login form in the upper right corner.</td></tr>
		</table>
	<? } else { ?>
		<table class="failureMessage" style="width:100%">
			<tr><th colspan="2">There was some problem</th></tr>
			<tr>
				<td>
					<? if ( $noArguments ) { ?>
						You have not provided all required parameters. Maybe you copied the link without some characters in the link? Or, if you clicked the link, try copying it instead? Some mail clients could broke it.
					<? } else if ( $keyTooOld ) { ?>
						The key you provided is too old. Use <a href="<?= $this->url("Authentication", "resendActivation") ?>">this link</a> to re-request it.
					<? } else if ( $keyIncorrect ) { ?>
						The key you provided is incorrect. Maybe you copied the link without some characters in the link? Or, if you clicked the link, try copying it instead? Some mail clients could broke it.
					<? } else if ( $userNotFound ) { ?>
						The user id you have provided does not exist.
					<? } else if ( $alreadyActivated ) { ?>
						Well, that is not really a problem, but that account is already activated. Go ahead and log in (form is in the upper right corner).
					<? } else { ?>
						Well, that is strange, but the system is unsure whether your account was activated. Hm, maybe just go ahead and check whether you can log in?
					<? } ?>
				</td>
			</tr>
		</table>
	<? } ?>
</div>