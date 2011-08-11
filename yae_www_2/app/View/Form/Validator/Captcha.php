<?php

	class View_Form_Validator_Captcha extends Tmvc_View_Form_Validator_Abstract
	{
		public $errorMessages = array
		(
			"captcha"		=>		"%label value does not match captcha text."
		);		
		
		/**
		 * Returns null when validation is succesfull, or string representing error message with explanation.
		 * @param unknown_type $value
		 * @return string
		 */
		public function validate($value, $context)
		{
			$captchaText = Lib_Captcha::getCode();
			if ( $captchaText != $value )
			{
				return $this->formatMessage("captcha");
			}
			return NULL;
		}
	}