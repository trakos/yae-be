<?php

	abstract class Tmvc_View_Form_Input_Abstract
	{
		protected $_validators = array();
		protected $_value = null;
		protected $_validationErrorText = null;
		protected $_name;
		protected $_label;
		
		/**
		 * @param $value
		 * @param Tmvc_View_Form_Validator_Abstract $validator
		 */
		public function __construct($name, $label, $value=null, Tmvc_View_Form_Validator_Abstract $validator=NULL)
		{
			$this->_name = $name;
			$this->_label = $label;
			$this->setValue($value);
			if ( $validator )
			{
				$this->addValidator($validator);
			}
		}
		
		/**
		 * @param Tmvc_View_Form_Validator_Abstract $validator
		 */
		public function addValidator( Tmvc_View_Form_Validator_Abstract $validator )
		{
			$this->_validators[] = $validator;
		}
		
		/**
		 * @return boolean
		 */
		public function validate()
		{
			$value = $this->getValue();
			$valid = false;
			foreach ( $this->_validators as $validator )
			{
				$this->_validationErrorText = $validator->validate($value);
				if ( $this->_validationErrorText )
				{
					return false;
				}
			}
			return true;
		}
		
		public function getName()
		{
			return $this->_name;
		}
		
		public function getValue()
		{
			return $this->_value;
		}
		
		public function setValue($value)
		{
			$this->_value = $value;
		}
		
		abstract public function render($formId);
	}