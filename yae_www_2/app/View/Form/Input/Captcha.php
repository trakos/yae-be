<?php

	class View_Form_Input_Captcha extends Tmvc_View_Form_Input_Abstract
	{	
		public function render($formId)
		{
			$captchaLink = Tmvc_View::getInstance()->link("Authentication","captcha");
			echo "<tr>";
			echo "<td class='label'><label for='{$formId}_{$this->_name}' >$this->_label:</td>";
			echo "<td><img src='$captchaLink' alt='captcha' style='text-align:center;margin:5px;' /><br /><input class='$this->_cssClass' type='text' name='$this->_name' id='{$formId}_{$this->_name}' value='$this->_value' /></td>";
			echo "</tr>";
			if ( $this->_validationErrorText )
			{
				echo "<tr class='errorMessage'><td colspan='2'>$this->_validationErrorText</td></tr>";
			}
		}
	}