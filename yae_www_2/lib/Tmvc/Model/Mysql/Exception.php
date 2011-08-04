<?php

class Tmvc_Model_Mysql_Exception extends Exception
{
	protected $errno;
	protected $query;
	protected $args;
	protected $type;

	public function __construct( $errno, $error, $type, $query, $args )
	{
		// $type is one of defines at the top of this file
		parent::__construct( $error, E_MYSQL );
		$this->query = $query;
		$this->type = $type;
		$this->errno = $errno;
		$this->args = $args;
	}

	public function getErrno() { return $this->errno; }
	public function getQuery() { return $this->query; }
	public function getArgs() { return $this->args; }
	public function getType() { return $this->type; }
}