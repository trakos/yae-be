<?php

/**
 * Klasa obsługująca połączenie z bazą Model_Mysql
 */
class Model_Mysql extends mysqli
{
	const E_SQL_CONNECT=1;
	const E_SQL_QUERY=2;
	const E_SQL_NOT_ENOUGH_ARGS=3;
	const E_SQL_TOO_MUCH_ARGS=4;
	const E_SQL_WRONG_ARG_TYPE=5;
	/**
	 * liczba wierszy z ostatniego zapytania 
	 * @var integer
	 */
	public $lastRowNumber;
	/**
	 * Instancja Model_Mysql
	 * @var Model_Mysql
	 */
	protected static $instance;
	/**
	 * pobiera instancję bazy danych (dane zdefiniowane w configuration.php)
	 * @return Model_Mysql
	 */
	public static function getConnection()
	{
		if ( !self::$instance )
		{
			self::$instance = new self(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);
			self::$instance->query("SET NAMES utf8");
		}
		return self::$instance;
	}

	/**
	 * Konstruktor nawiązujący połączenie z bazą danych.
	 * @param unknown_type $hostname
	 * @param unknown_type $user
	 * @param unknown_type $password
	 * @param unknown_type $database
	 * @throws Model_Mysql_Exception
	 */
	public function __construct( $hostname, $user, $password, $database )
	{
		parent::__construct( $hostname, $user, $password, $database );
		if( $this->connect_errno )
		{
			throw new Model_Mysql_Exception( $this->connect_errno, $this->connect_error, self::E_SQL_CONNECT, "__construct", array() );
		}
	}

	/**
	 * Podmienia wszystkie znaki '?' na kolejne przekazane mu argumenty, po uprzednim ich wyescapowaniu.
	 * Wymaga podania tylu samych argumentów co znaków zapytania, w przeciwnym wypadku wyrzuci wyjątek.
	 * @see mysqli::query()
	 * @param $query  Można używać znaków ?, które później zostaną podstawione za kolejne argumenty po wyescapowaniu.
	 * @return mysqli_result
	 */
	public function query( $query )
	{
		$args = func_get_args();
		return $this->vquery( $query, $args, 1);
	}

	/**
	 * Taka sama jak metoda Mysqli::query, ale w przypadku błędu wyrzuca wyjątek.
	 * @param $query
	 * @param $resultmode
	 * @param $args można zignorować, używane wewnętrznie do przekazywania informacji do wyjątków do generowanie wyjątków w razie niepowodzenia.
	 * @return mysqli_result
	 */
	public function simplequery( $query, $resultmode=MYSQLI_STORE_RESULT, $args=array() )
	{
		//fb($query);
		$result = parent::query( $query, $resultmode );
		if(is_object($result))
		{
			$this->lastRowNumber = $result->num_rows;
		}
		if( $this->errno )
		{
			throw new Model_Mysql_Exception( $this->errno, $this->error, self::E_SQL_QUERY, $query, $args );
		}
		return $result;
	}

	/**
	 * Podobne jak query, ale przyjmuje tablicę argumentów na drugim miejscu.
	 * @param $query
	 * @param $args
	 * @param $toShift ile pierwszych argumentów w $args zignorować.
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
	 * Pobiera wartość pierwszej kolumny z pierwszego wiersza, albo null gdy brak wierszy.
	 * @param $query  Można używać znaków ?, które później zostaną podstawione za kolejne argumenty po wyescapowaniu.
	 * @return string (or null)
	 */
	public function getOne ($query)
	{
		$args = func_get_args();
		return $this->vgetOne( $query, $args, 1 );
	}

	/**
	 * To samo co getOne, ale przyjmuje tablicę do podmian za ? na drugim miejscu.
	 * @param unknown_type $query
	 * @param unknown_type $args
	 * @param unknown_type $toShift
	 * @return string (lub null)
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
	 * Pobiera wartości z pierwszej kolumny dla każdego wiersza, zwraca pusty array gdy brak wyników.
	 * @param $query  Można używać znaków ?, które później zostaną podstawione za kolejne argumenty po wyescapowaniu.
	 * @return string (or null)
	 */
	public function getCol ($query)
	{
		$args = func_get_args();
		return $this->vgetCol( $query, $args, 1 );
	}

	/**
	 * To samo co getCol, ale przyjmuje tablicę do podmian za ? na drugim miejscu.
	 * @param unknown_type $query
	 * @param unknown_type $args
	 * @param unknown_type $toShift
	 * @return string (lub null)
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
	 * Zwraca pierwszy wiersz jak tablica asocjacyjna, albo null gdy brak wierszy.
	 * @param $query  Można używać znaków ?, które później zostaną podstawione za kolejne argumenty po wyescapowaniu.
	 * @return array (lub null)
	 */
	public function getRow ($query)
	{
		$args = func_get_args();
		return $this->vgetRow( $query, $args, 1 );
	}

	/**
	 * To samo co getRow, ale przyjmuje tablicę do podmian za ? na drugim miejscu.
	 * @param $query
	 * @param $args
	 * @param $toShift
	 * @return array (lub null)
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
	 * Pobiera wszystkie wierssze jako tablica asocjacyjna.
	 *
	 * @param unknown_type $query Można używać znaków ?, które później zostaną podstawione za kolejne argumenty po wyescapowaniu.
	 * @return array
	 */
	public function getAll ( $query )
	{
		$args = func_get_args();
		return $this->vgetAll( $query, $args, 1 );
	}

	/**
	 * To samo co getAll, ale przyjmuje tablicę do podmian za ? na drugim miejscu.
	 * @param unknown_type $query
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
	 * To samo co getAll, ale wiersza są tablicami numerowanymi, a nie asocjacyjnymi.
	 * @param $query
	 * @return array
	 */
	public function getAllEnumerate ( $query )
	{
		$args = func_get_args();
		return $this->vgetAllEnumerate( $query, $args, 1 );
	}

	/**
	 * To samo co getAllEnumerate, ale przyjmuje tablicę do podmian za ? na drugim miejscu.
	 * @param $query
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
	 * Podstawia argumenty pod znaki '?'
	 * @param $query
	 * @param $args
	 * @param $chars używane tylko w wywołaniach rekurencyjnych
	 */
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
				throw new Model_Mysql_Exception( -1, "Not enough arguments passed to a queryf-type function.", self::E_SQL_NOT_ENOUGH_ARGS, $query, $args );
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
						throw new Model_Mysql_Exception( -1, "Wrong type of argument!", self::E_SQL_WRONG_ARG_TYPE, $query, $args );
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
			throw new Model_Mysql_Exception( -1, "Too much arguments passed to a queryf-type function.", self::E_SQL_TOO_MUCH_ARGS, $query, $args );
		}
		else
		{
			return $query;
		}
	}
}
