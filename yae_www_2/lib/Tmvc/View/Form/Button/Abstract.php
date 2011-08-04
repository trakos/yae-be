<?php

	abstract class Tmvc_View_Form_Button_Abstract
	{
		protected $_name;
		protected $_label;
		
		/**
		 * @param $value
		 * @param Tmvc_View_Form_Validator_Abstract $validator
		 */
		public function __construct($name, $label)
		{
			$this->_name = $name;
			$this->_label = $label;
		}
		
		abstract public function render($formId);
	}