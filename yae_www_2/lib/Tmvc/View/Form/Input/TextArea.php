<?php

	class Tmvc_View_Form_Input_TextArea extends Tmvc_View_Form_Input_Abstract
	{		
		public function render($formId)
		{
			echo "<tr><td class='label' colspan='2'><label for='{$formId}_{$this->_name}' >$this->_label:</td></tr>";
			echo "<tr><td colspan='2'><textarea class='$this->_cssClass' cols='40' rows='4' name='$this->_name' id='{$formId}_{$this->_name}'>$this->_value</textarea></td></tr>";
			if ( $this->_validationErrorText )
			{
				echo "<tr class='errorMessage'><td colspan='2'>$this->_validationErrorText</td></tr>";
			}
		}
	}