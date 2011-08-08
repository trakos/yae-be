<?php

	class Tmvc_View_Form_Validator_Email extends Tmvc_View_Form_Validator_Abstract
	{
		public $errorMessages = array
		(
			"dontMatchRegex"	=>		"%label is not in a valid format."
		);
		
		
		/**
		 * Returns null when validation is succesfull, or string representing error message with explanation.
		 * @param unknown_type $value
		 * @return string
		 */
		public function validate($value, $context)
		{
			if ( empty($value) )
			{
				return null;
			}
			$mail = filter_var($value, FILTER_VALIDATE_EMAIL);
			if ( !$mail )
			{
				return $this->formatMessage("dontMatchRegex");
			}
			return null;
		}
	}