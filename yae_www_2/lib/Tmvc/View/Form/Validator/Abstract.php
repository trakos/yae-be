<?php

	abstract class Tmvc_View_Form_Validator_Abstract
	{
		/**
		 * Returns null when validation is succesfull, or string representing error message with explanation.
		 * @param unknown_type $value
		 * @return string
		 */
		abstract public function validate($value);
	}