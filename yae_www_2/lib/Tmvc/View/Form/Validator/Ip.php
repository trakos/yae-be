<?php

	class Tmvc_View_Form_Validator_Ip extends Tmvc_View_Form_Validator_Abstract
	{
		public $errorMessages = array
		(
			"dontMatchRegex"	=>		"%label is not a valid ip.",
			"privRange"			=>		"%label given appears to be ip coming from a private range.",
			"resRange"			=>		"%label given appears to be ip from the reserved range."
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
			$value = filter_var($value, FILTER_VALIDATE_IP, FILTER_FLAG_IPV4 );
			if ( !$value )
			{
				return $this->formatMessage("dontMatchRegex");
			}
			$value = filter_var($value, FILTER_VALIDATE_IP, FILTER_FLAG_IPV4 | FILTER_FLAG_NO_PRIV_RANGE );
			if ( !$value )
			{
				return $this->formatMessage("privRange");
			}
			$value = filter_var($value, FILTER_VALIDATE_IP, FILTER_FLAG_IPV4 | FILTER_FLAG_NO_PRIV_RANGE | FILTER_FLAG_NO_RES_RANGE );
			if ( !$value )
			{
				return $this->formatMessage("resRange");
			}
			return null;
		}
	}