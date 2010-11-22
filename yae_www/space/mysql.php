<?php

define('E_SQL_CONNECT',0);
define('E_SQL_QUERY',1);
define('E_SQL_NOT_ENOUGH_ARGS',2);
define('E_SQL_TOO_MUCH_ARGS',3);


class sqlException extends Exception
{
	protected $errno;
	protected $query;
	protected $args;
	protected $type;
	
	public function __construct( $errno, $error, $type, $query, $args )
	{
		// $type is one of defines at the top of this file
		parent::__construct( $error, EXCEPTIONCODE_MYSQL );
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


// TODO:	I should stop checking query in queryf fuction for existence of '?' characters in case user don't pass any additional arguments anyway
//	the check is there for now to make sure there is nothing made wrong in sql queries (i.e. ? marks without additional arguments)
class mysql extends mysqli
{
	public $timer;
	////////////////////////////////////////////////////////////////
	//	 	overloading functions			      //
	////////////////////////////////////////////////////////////////
	
	// connecting with the database
	public function __construct( ) 
	{
		$this->timer = new timer(true); // true means it's paused
		parent::__construct( MYSQL_HOSTNAME, MYSQL_USER, MYSQL_PASSWORD, MYSQL_DATABASE );
		if( $this->connect_errno )
		{
			throw sqlException( $this->connect_errno, $this->connect_error, E_SQL_CONNECT, "__construct", array() );
		}
	}
	
	// the $args argument is here only for sake of usage by queryf, which adds arguments list to exceptions when raised
	public function query( $query, $resultmode=MYSQLI_STORE_RESULT, $args=array() )
	{
		$this->timer->start($query);
		$result = parent::query( $query, $resultmode );
		$this->timer->stop();
		if( $this->errno )
		{
			throw new sqlException( $this->errno, $this->error, E_SQL_CONNECT, $query, $args );
		}
		return $result;
	}
	
        ////////////////////////////////////////////////////////
        //              adding functions                   	//
        ////////////////////////////////////////////////////////

	public function getTimeSpentOnQueries( ) { return $this->timer->getTotal(); }
	public function getNumOfQueries( ) { return $this->timer->count(); }
	public function getDebugStats( ) { return (string) $this->timer; }
	
	// the point of this function is to escape all not-sure strings before passing to mysql
	// each ? character in query is substituted with escaped arguments coming after $query
	// in the order they are given - if there is not enough args OR too much, mysqlException is raised
	// - this is done to ensure user know that he made this before there's something to regret
	public function queryf( $query )
	{
		$args = func_get_args();
		return $this->vqueryf( $query, $args, 1);
	}
	
	// as this function is most usually used by gettig the func_get_args, it needs to be shifted almost always
	// (i.e. get rid of first few elements in array) so there's an argument toShift in which you can let it to
	// do shifting done by putting how many times it should shift in this var
	public function vqueryf( $query, $args, $toShift = 0 )
	{
		while( $toShift != 0 )
		{
			array_shift($args);
			$toShift--;
		}
		if($args == NULL )
		{
			$args = array();
		}
		$query = $this->parseQuery( $query, $args );
		return $this->query( $query, MYSQLI_STORE_RESULT );
	}
	
	// getting one record with one column from table and returning its value, 
	// you must ensure that $column is really only one column and that there will be only one row 
	// or else you will just get the first one
	// null if not found
	public function getField ($query)
	{
		$args = func_get_args();
		return $this->vgetField( $query, $args, 1 );
	}
	
	public function vgetField( $query, $args, $toShift = 0 )
	{
                $result = $this->vqueryf( $query, $args, $toShift );
                $row = $result->fetch_row();
                $result->close();
                if( $row != NULL )
                {
                        return $row[0];
                }
                else
                {
                        return NULL;
                }
	}

	// returning one row as an associative array - if receiving more then returning the first one
	// however, it's not good performance-wise to omit LIMIT 1 when expecting more data
	// null if not found
	public function getRow ($query)
	{
		$args = func_get_args();
		return $this->vgetRow( $query, $args, 1 );
        }

	public function vgetRow( $query, $args, $toShift = 0 )
	{
		$result = $this->vqueryf( $query, $args, $toShift );
		$row = $result->fetch_assoc();
		$result->close();
                if ( $row != NULL )
                {
                        return $row;
                }
                else
                {
                        return NULL;
                }
	}

	// getting array of associative rows - simply, what we most usually are expecting from php
	// it is using queryf in its core, so one can use "?" signs in query and next arguments will be used to substitute them
	// returns empty array() if not found - it makes more sense for me, as output could be used for example in foreach statement
	public function getArray ( $query ) 
	{
		$args = func_get_args();
		return $this->vgetArray( $query, $args, 1 );
	}
	
	// toShift here will be just passed to vquery along with args
	public function vgetArray ( $query, $args, $toShift = 0 ) 
	{
		$result = $this->vqueryf( $query, $args, $toShift );
		$array = array();
		while( $row = $result->fetch_assoc() )
		{
			$array[] = $row;
		}
		return $array;
	}
	
	// opisanie tabeli
	public function opisz ($tabela) 
	{
		$query = "DESCRIBE $tabela";
		$results = array( );
		$result = mysql_query( $query, $this->sql );

		if( $result )
		{
			
			while ( $row = mysql_fetch_array( $result ) ) 
				array_push( $results, $row );
			$zwrot = $results;
			$result->close();
		}
		else
			$zwrot = FALSE;
			
		return $zwrot;
	}

	// zmiana wartosci w tabeli
	public function ustaw ($tabela, $set, $where) 
	{
		$query = "UPDATE $tabela SET $set WHERE $where";
		$result = mysql_query( $query, $this->sql );
		if ( $result )
			$zwrot = TRUE;
		else
			$zwrot = FALSE;
			
		return $zwrot;
	}

	// dodanie wartosci do tabeli
	public function wstaw ($tabela, $wartosci) 
	{
		$query = "INSERT INTO $tabela VALUES ($wartosci)";
		$result = mysql_query( $query, $this->sql );
		if ( $result )
			$zwrot =  TRUE;
		else
			$zwrot = FALSE;
		return $zwrot;
	}	

	// deleting a value
	public function usun ($tabela, $condition)  
	{
		$result = mysql_query("DELETE FROM $tabela WHERE $condition", $this->sql);
		if ( $result )
			$zwrot =  TRUE;
		else
			$zwrot = FALSE;
		return $zwrot;
	}
	
	// as I am using this function recursively, there's a chars argument to make strlening it every time not happen
	protected function parseQuery( $query, $args, $chars=-1 )
	{
		if($chars == -1)
		{
			$chars = strlen($query);
		}
		for( $char=0; $char < $chars; $char++ )
		{
			if( $query[$char] == '?' )
			{
				break;
			}
		}
		if( $char < $chars )
		{
			if(empty($args))
			{
				throw new sqlException( -1, "Not enough arguments passed to a queryf-type function.", E_SQL_NOT_ENOUGH_ARGS, $query, $args );
			}
			else
			{
				$q1 = "";
				if($char > 0)
				{
					$q1 = substr($query,0,$char);
				}
				$arg = $this->real_escape_string(array_shift($args));
				$q2 = "";
				if($char < $chars-1)
				{
					$q2 = $this->parseQuery(substr($query,$char+1),$args,$chars-$char-1);
				}
				return $q1.$arg.$q2;
			}
		}
		else if ( !empty($args) )
		{
			throw new sqlException( -1, "Too much arguments passed to a queryf-type function.", E_SQL_TOO_MUCH_ARGS, $query, $args );
		}
		else
		{
			return $query;
		}
	}
}
?>
