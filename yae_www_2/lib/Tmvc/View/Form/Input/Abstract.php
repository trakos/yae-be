<?php

	abstract class Tmvc_View_Form_Input_Abstract
	{
		protected $_validators = array();
		protected $_value = null;
		protected $_validationErrorText = null;
		protected $_name;
		protected $_label;
		protected $_cssClass = "";
		
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
		 * @param array $context
		 * @return boolean
		 */
		public function validate($context)
		{
			$value = $this->getValue();
			$valid = false;
			foreach ( $this->_validators as $validator )
			{
				$error = $validator->validate($value, $context);
				if ( $error )
				{
					$this->setErrorText($error);
					return false;
				}
			}
			return true;
		}
		
		public function setErrorText( $errorText )
		{
			$this->_validationErrorText = str_replace("%label", $this->_label, $errorText);
			$this->_cssClass.= " invalid";
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