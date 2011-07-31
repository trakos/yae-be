<?php

class Model_Auth_Exception extends Exception
{
	public function __construct($code,$message)
	{
		parent::__construct($message, $code);
	}
};