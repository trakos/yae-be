<?php

	class View_Form_Validator_Login extends Tmvc_View_Form_Validator_Abstract
	{
		public $errorMessages = array
		(
			"dontMatchRegex"	=>		"Login can only consist of a-z letters, numbers and hyphen or underscore. It must start with letter, and cannot have two hyphens or underscores or hyphen and underscore right next to each other.",
			"loginTaken"		=>		"This login is already in use."
		);
		
		protected $_isAdminEdit=false;
		
		/**
		 * 
		 * @param isAdminEdit whether the user edited is different from currently logged in. When true uses "userid" parameter from $context, make sure it will be there.
		 */
		public function __construct($isAdminEdit=false)
		{
			$this->_isAdminEdit = $isAdminEdit;
		}
		
		
		/**
		 * Returns null when validation is succesfull, or string representing error message with explanation.
		 * @param unknown_type $value
		 * @return string
		 */
		public function validate($value, $context)
		{
			$regex = "#^([a-zA-Z])+(([-_])?([a-zA-Z0-9])+)*$#";
			if ( !preg_match($regex,$value) )
			{
				return $this->formatMessage("dontMatchRegex");
			}
			$q = "SELECT * FROM users WHERE username=?";
			$a = array($value);
			$user = Tmvc_Model_Mysql::getConnection()->vgetRow($q,$a);
			if ( $user )
			{
				if ( !Model_Auth::getInstance()->isLogged() )
				{
					return $this->formatMessage("loginTaken");
				}
				else
				{
					$currentUserId = Model_Auth::getInstance()->getUserId();
					if ( $this->_isAdminEdit )
					{
						$currentUserId = $context['userid'];
					}
					if ( $user['userid'] != $currentUserId )
					{
						return $this->formatMessage("loginTaken");
					}
				}
			}
			return NULL;
		}
	}