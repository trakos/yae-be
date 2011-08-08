<?php

	class Tmvc_View_Form_Validator_StringLength extends Tmvc_View_Form_Validator_Abstract
	{
		public $errorMessages = array
		(
			"tooShort"			=>	"%label must be at least %min characters long.",
			"tooLong"			=>	"%label cannot be more than %max characters long."
		);
		
		protected $_min;
		protected $_max;
		
		public function formatMessage($key)
		{
			$msg = parent::formatMessage($key);
			$msg = str_replace("%min", $this->_min, $msg);
			$msg = str_replace("%max", $this->_max, $msg);
			return $msg;
		}
		
		public function __construct($min, $max=PHP_INT_MAX)
		{
			$this->_min = $min;
			$this->_max = $max;
		}
		
		
		/**
		 * Returns null when validation is succesfull, or string representing error message with explanation.
		 * @param unknown_type $value
		 * @return string
		 */
		public function validate($value, $context)
		{
			if ( strlen($value) < $this->_min )
			{
				return $this->formatMessage("tooShort");
			}
			else if ( strlen($value) > $this->_max )
			{
				return $this->formatMessage("tooLong");
			}
			return NULL;
		}
	}