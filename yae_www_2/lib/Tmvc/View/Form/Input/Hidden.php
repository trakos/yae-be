<?php

	class Tmvc_View_Form_Input_Hidden extends Tmvc_View_Form_Input_Abstract
	{		
		public function render($formId)
		{
			echo "<input type='hidden' name='$this->_name' id='{$formId}_{$this->_name}' value='$this->_value' />";
		}
	}