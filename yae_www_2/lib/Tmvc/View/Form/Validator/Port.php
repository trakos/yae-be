<?php

	class Tmvc_View_Form_Validator_Port extends Tmvc_View_Form_Validator_Abstract
	{
		public $errorMessages = array
		(
			"dontMatchRegex"	=>		"%label is not a valid port number.",
			"tooLow"			=>		"%label is lower than 1024.",
			"tooHi"				=>		"%label is higher than 49151."
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
			$value = filter_var($value, FILTER_VALIDATE_INT );
			if ( !$value )
			{
				return $this->formatMessage("dontMatchRegex");
			}
			else if ( intval($value) < 1024 )
			{
				return $this->formatMessage("tooLow");
			}
			else if ( intval($value) > 49151 )
			{
				return $this->formatMessage("tooHi");
			}
			return null;
		}
	}