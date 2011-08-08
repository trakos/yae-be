<?php

	abstract class Tmvc_View_Form_Validator_Abstract
	{
		public $errorMessages = array();
		
		public function formatMessage($key)
		{
			return $this->errorMessages[$key];
		}
		
		/**
		 * Returns null when validation is succesfull, or string representing error message with explanation.
		 * @param unknown_type $value
		 * @param array $context
		 * @return string
		 */
		abstract public function validate($value, $context);
	}