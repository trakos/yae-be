<h2>Logowanie</h2>
<h4>Logowanie do systemu callcenter</h4>
<p id="successMsg"><?php echo $successMsg; ?></p>
<p id="failureMsg"><?php echo $failureMsg; ?></p>
<?php if (!$isLogged ) { ?>
	<form method="post">
		<table class="auth auto">
			<tr>
				<th>login:</th>
				<td><input type="text" name="login" value="" /></td>
			</tr>
			<tr>
				<th>hasło:</th>
				<td><input type="password" name="password" value="" /></td>
			</tr>
			<tr>
				<td colspan="2"><input type="submit" value="login" /></td>
			</tr>
		</table>
	</form>
<?php } else { ?>
	<p>Skorzystaj z menu po lewej, aby uzyskać dostęp do funkcjonalności systemu.</p>
<?php } ?>