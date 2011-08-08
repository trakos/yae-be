<?php

	class Tmvc_View_Form_Validator_Confirm extends Tmvc_View_Form_Validator_Abstract
	{
		public $errorMessages = array
		(
			"dontMatch"			=>	"Value of \"%label\" differs from \"%confirm\" value."
		);
		
		protected $_confirmIndex;
		protected $_confirmLabel;
		
		public function formatMessage($key)
		{
			$msg = parent::formatMessage($key);
			$msg = str_replace("%confirm", $this->_confirmLabel, $msg);
			return $msg;
		}
		
		public function __construct($confirmIndex, $confirmLabel)
		{
			$this->_confirmIndex = $confirmIndex;
			$this->_confirmLabel = $confirmLabel;
		}
		
		
		/**
		 * Returns null when validation is succesfull, or string representing error message with explanation.
		 * @param unknown_type $value
		 * @return string
		 */
		public function validate($value, $context)
		{
			if ( $value != $context[$this->_confirmIndex] )
			{
				return $this->formatMessage("dontMatch");
			}
			return NULL;
		}
	}