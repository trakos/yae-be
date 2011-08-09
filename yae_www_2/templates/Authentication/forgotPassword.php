<div style="margin: 80px auto;width:50%;">
	<? if ( $passwordChanged ) { ?>
		<table class="successMessage" style="width:100%">
			<tr><th colspan="2">Password changed!</th></tr>
			<tr><td>Finally... go ahead, login using the form in the upper right corner.</td></tr>
		</table>
	<? } else if ( !$keyCorrect ) { ?>
		<table class="failureMessage" style="width:100%">
			<tr><th colspan="2">There was some problem</th></tr>
			<tr>
				<td>
					<? if ( $noArguments ) { ?>
						You have not provided all required parameters. Maybe you copied the link without some characters in the link? Or, if you clicked the link, try copying it instead? Some mail clients could broke it.
					<? } else if ( $keyTooOld ) { ?>
						The key you provided is older than 30 minutes. Use <a href="<?= $this->url("Authentication", "requestForgotPassword") ?>">this link</a> to re-request it.
					<? } else if ( $keyIncorrect ) { ?>
						The key you provided is incorrect. Maybe you copied the link without some characters in the link? Or, if you clicked the link, try copying it instead? Some mail clients could broke it. The link is working only once, if you had used it, you have to <a href="<?= $this->url("Authentication", "requestForgotPassword") ?>">re-request it</a>. 
					<? } else if ( $userNotFound ) { ?>
						The user id you have provided does not exist.
					<? } else if ( $notActivated ) { ?>
						This user is not even activated. Use <a href="<?= $this->url("Authentication", "resendActivation") ?>">this link</a> to re-request activation link, and use it first.
					<? } else { ?>
						Well, that is strange, but the system is unsure what happened, but it didn't allow you to change the password.
					<? } ?>
				</td>
			</tr>
		</table>
	<? } else { ?>
		<? $changePasswordForm->render(); ?>
	<? } ?>
</div>