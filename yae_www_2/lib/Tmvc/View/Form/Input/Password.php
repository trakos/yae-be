<?php

	class Tmvc_View_Form_Input_Password extends Tmvc_View_Form_Input_Abstract
	{		
		public function render($formId)
		{
			echo "<tr>";
			echo "<td class='label'><label for='{$formId}_{$this->_name}' >$this->_label:</td>";
			echo "<td><input class='$this->_cssClass' type='password' name='$this->_name' id='{$formId}_{$this->_name}' value='$this->_value' /></td>";
			echo "</tr>";
			if ( $this->_validationErrorText )
			{
				echo "<tr class='errorMessage'><td colspan='2'>$this->_validationErrorText</td></tr>";
			}
		}
	}