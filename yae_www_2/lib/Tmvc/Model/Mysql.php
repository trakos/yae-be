<?php

/**
 * Class supporting mysql database.
 */
class Tmvc_Model_Mysql extends mysqli
{
	const E_SQL_CONNECT=1;
	const E_SQL_QUERY=2;
	const E_SQL_NOT_ENOUGH_ARGS=3;
	const E_SQL_TOO_MUCH_ARGS=4;
	const E_SQL_WRONG_ARG_TYPE=5;
	
	/** 
	 * @var Tmvc_Model_Timer
	 */
	public $timer;
	/**
	 * Number of rows from the last query. 
	 * @var integer
	 */
	public $lastRowNumber;
	/**
	 * Object instance
	 * @var Tmvc_Model_Mysql
	 */
	protected static $_instance;
	/**
	 * Gets instance of a database (uses DB_ constants that should be defined in configuration.php).
	 * @return Tmvc_Model_Mysql
	 */
	public static function getConnection()
	{
		if ( !self::$_instance )
		{
			self::$_instance = new self(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);
			self::$_instance->query("SET NAMES utf8");
		}
		return self::$_instance;
	}

	/**
	 * Constructor that connects to the database.
	 * @param string $hostname
	 * @param string $user
	 * @param string $password
	 * @param string $database
	 * @throws Tmvc_Model_Mysql_Exception
	 */
	public function __construct( $hostname, $user, $password, $database )
	{
		$this->timer = new Tmvc_Model_Timer(true); // true means it's paused
		parent::__construct( $hostname, $user, $password, $database );
		if( $this->connect_errno )
		{
			throw new Tmvc_Model_Mysql_Exception( $this->connect_errno, $this->connect_error, self::E_SQL_CONNECT, "__construct", array() );
		}
	}

	/**
	 * Substitues all '?' chars with a subsequent arguments (right after escaping them).
	 * The arguments count must be equal to the number of question signs, or an exception will be thrown.
	 * @see mysqli::query()
	 * @param $query You can use '?' characters, which will be later substituted with an escaped arguments.
	 * @return mysqli_result
	 */
	public function query( $query )
	{
		$args = func_get_args();
		return $this->vquery( $query, $args, 1);
	}

	/**
	 * It does pretty much the same as Mysqli::query, but throws exception in case of an error.
	 * @param $query
	 * @param $resultmode
	 * @param $args should be omitted when calling from outside, used internally to hand additional info to the exception.
	 * @throws Tmvc_Model_Mysql_Exception
	 * @return mysqli_result
	 */
	public function simplequery( $query, $resultmode=MYSQLI_STORE_RESULT, $args=array() )
	{
		$this->timer->start($query);
		$result = parent::query( $query, $resultmode );
		$this->timer->stop();
		if(is_object($result))
		{
			$this->lastRowNumber = $result->num_rows;
		}
		if( $this->errno )
		{
			throw new Tmvc_Model_Mysql_Exception( $this->errno, $this->error, self::E_SQL_QUERY, $query, $args );
		}
		return $result;
	}

	/**
	 * Similar as query, but accepts array of an arguments as a second argument.
	 * @param $query you can use '?' characters, which will be later substituted with an escaped arguments.
	 * @param $args
	 * @param $toShift how much first elements in $args should be omitted.
	 * @return mysqli_result
	 */
	public function vquery( $query, $args, $toShift = 0 )
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
		else if( count($args)==1 && is_array($args[0]))
		{
			$args = $args[0];
		}
		$query = $this->parseQuery( $query, $args );
		return $this->simplequery( $query, MYSQLI_STORE_RESULT, $args );
	}

	/**
	 * Returns value of a first column from the first row, or null in case of lack of rows.
	 * @param $query you can use '?' characters, which will be later substituted with an escaped arguments.
	 * @return string (or null)
	 */
	public function getOne ($query)
	{
		$args = func_get_args();
		return $this->vgetOne( $query, $args, 1 );
	}

	/**
	 * Same as getOne, but substitues ? in $query with $args values.
	 * @param string $query you can use '?' characters, which will be later substituted with an escaped arguments.
	 * @param array $args
	 * @param int $toShift
	 * @return string (or null)
	 */
	public function vgetOne( $query, $args, $toShift = 0 )
	{
		$result = $this->vquery( $query, $args, $toShift );
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

	/**
	 * Gets value from the first column of each row, empty array in case of empty result.
	 * @param $query you can use '?' characters, which will be later substituted with an escaped arguments.
	 * @return string (or null)
	 */
	public function getCol ($query)
	{
		$args = func_get_args();
		return $this->vgetCol( $query, $args, 1 );
	}

	/**
	 * Same as getCoil, but substitues ? in $query with $args values.
	 * @param unknown_type $query you can use '?' characters, which will be later substituted with an escaped arguments.
	 * @param unknown_type $args
	 * @param unknown_type $toShift
	 * @return string (or null)
	 */
	public function vgetCol( $query, $args, $toShift = 0 )
	{
		$result = $this->vquery( $query, $args, $toShift );
		$array = array();
		while( $row = $result->fetch_row() )
		{
			$array[] = $row[0];
		}
		$result->close();
		return $array;
	}

	/**
	 * Returns first row as an associative array, or null in case of empty result ( no rows ).
	 * @param $query tou can use '?' characters, which will be later substituted with an escaped arguments.
	 * @return array (or null)
	 */
	public function getRow ($query)
	{
		$args = func_get_args();
		return $this->vgetRow( $query, $args, 1 );
	}

	/**
	 * Same as getRow, but substitues ? in $query with $args values.
	 * @param $query you can use '?' characters, which will be later substituted with an escaped arguments.
	 * @param $args
	 * @param $toShift
	 * @return array (or null)
	 */
	public function vgetRow( $query, $args, $toShift = 0 )
	{
		$result = $this->vquery( $query, $args, $toShift );
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

	/**
	 * Returns all rows as an associative array.
	 *
	 * @param unknown_type $query you can use '?' characters, which will be later substituted with an escaped arguments.
	 * @return array
	 */
	public function getAll ( $query )
	{
		$args = func_get_args();
		return $this->vgetAll( $query, $args, 1 );
	}

	/**
	 * Same as getAll, but substitues ? in $query with $args values.
	 * @param unknown_type $query you can use '?' characters, which will be later substituted with an escaped arguments.
	 * @param unknown_type $args
	 * @param unknown_type $toShift
	 */
	public function vgetAll ( $query, $args, $toShift = 0 )
	{
		$result = $this->vquery( $query, $args, $toShift );
		$array = array();
		while( $row = $result->fetch_assoc() )
		{
			$array[] = $row;
		}
		$result->close();
		return $array;
	}

	/**
	 * Same as getAll, but rows are enumerated arrays instead of associative.
	 * @param $query
	 * @return array
	 */
	public function getAllEnumerate ( $query )
	{
		$args = func_get_args();
		return $this->vgetAllEnumerate( $query, $args, 1 );
	}

	/**
	 * Same as getAllEnumerate, but substitues ? in $query with $args values.
	 * @param $query you can use '?' characters, which will be later substituted with an escaped arguments.
	 * @param $args
	 * @param $toShift
	 * @return array
	 */
	public function vgetAllEnumerate ( $query, $args, $toShift = 0 )
	{
		$result = $this->vquery( $query, $args, $toShift );
		$array = array();
		while( $row = $result->fetch_row() )
		{
			$array[] = $row;
		}
		$result->close();
		return $array;
	}
	
	/**
	 * @param string $queryBase
	 * @param array $selectArguments
	 * @param array $groupArguments
	 * @return Tmvc_Model_Mysql_BaseSelect
	 */
	public function getBaseSelect( $queryBase = '', $selectArguments = array(), $groupArguments = array() )
	{
		return new Tmvc_Model_Mysql_BaseSelect($this, $queryBase, $selectArguments, $groupArguments);
	}

	/**
	 * Puts arguments in place of '?' signs, it is an internal function, but can be used outside for debugging purposes.
	 * @param $query
	 * @param $args
	 * @param $chars used only in recurrent calls.
	 * @throws Tmvc_Model_Mysql_Exception
	 */
	public function parseQuery( $query, $args, $chars=-1 )
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
				throw new Tmvc_Model_Mysql_Exception( E_MYSQL, "Not enough arguments passed to a queryf-type function.", self::E_SQL_NOT_ENOUGH_ARGS, $query, $args );
			}
			else
			{
				$q1 = "";
				if($char > 0)
				{
					$q1 = substr($query,0,$char);
				}
				$arg = array_shift($args);
				switch (true)
				{
					case is_null($arg):
						$arg='NULL';
						break;
					case is_bool($arg):
						$arg=$arg?'"1"':'"0"';
						break;
					case is_int($arg):
						$arg = $arg;//;)
						break;
					case is_float($arg):
					case is_string($arg):
					case is_object($arg):
						$arg = "'".$this->real_escape_string($arg)."'";
						break;
					default:
						throw new Tmvc_Model_Mysql_Exception( E_MYSQL, "Wrong type of argument!", self::E_SQL_WRONG_ARG_TYPE, $query, $args );
						break;
				}
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
			throw new Tmvc_Model_Mysql_Exception( E_MYSQL, "Too much arguments passed to a queryf-type function.", self::E_SQL_TOO_MUCH_ARGS, $query, $args );
		}
		else
		{
			return $query;
		}
	}
	
	public function getCountCalcRows()
	{
		return $this->getOne("SELECT FOUND_ROWS()");
	}

	public function getTimeSpentOnQueries( ) { return $this->timer->getTotal(); }
	public function getNumOfQueries( ) { return $this->timer->count(); }
	public function getDebugStats( ) { return (string) $this->timer; }
}
