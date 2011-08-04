<?php

	class Tmvc_View_Form_Button_Submit extends Tmvc_View_Form_Button_Abstract
	{
		public function render($formId)
		{
			echo "<input class='button' type='submit' value='$this->_label' name='$this->_name' />";
		}
	}