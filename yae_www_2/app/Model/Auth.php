<?php

/**
 * Klasa obsługująca logowanie, autoryzację i zmianę hasła użytkownika.
 */
class Model_Auth
{
	/**
	 * Dane zapisane w sesjii.
	 * @var array
	 */
	protected $data = array();
	/**
	 * login użytkownika.
	 * @var string
	 */
	protected $uid = "";
	/**
	 * Poziom aktualnie zalogowanego użytkownika.
	 * @var int
	 */
	protected $userLevel = LEVEL_UNLOGGED;
	/**
	 * Czy jesteś zalogowany.
	 * 
	 * @var boolean
	 */
	protected $isLogged = false;

	/**
	 * Instancja obiektu.
	 * @var Model_Auth
	 */
	static protected $instance=NULL;

	/**
	 * Zwraca instancję singletonu Model_Auth.
	 * @return Model_Auth
	 */
	static public function getInstance()
	{
		if(!self::$instance)
		{
			self::$instance = new Model_Auth();
		}
		return self::$instance;
	}

	protected function __construct()
	{
		if( isset($_SESSION['loggedData']) && isset($_SESSION['loggedUid'])  && isset($_SESSION['loggedUserLevel']) )
		{
			$this->data = $_SESSION['loggedData'];
			$this->uid = $_SESSION['loggedUid'];
			$this->userLevel = $_SESSION['loggedUserLevel'];
			$this->isLogged = true;
		}
	}

	/**
	 * Wylogowuje z systemu.
	 */
	public function logout()
	{
		unset($_SESSION['loggedData']);
		unset($_SESSION['loggedUid']);
		unset($_SESSION['loggedUserLevel']);
		$this->data = array();
		$this->uid = 0;
		$this->userLevel = LEVEL_UNLOGGED;
		$this->isLogged = false;
	}

	/**
	 * Informuje o tym, czy użytkownik jest zalogowany.
	 * @return boolean
	 */
	public function isLogged()
	{
		return $this->isLogged;
	}

	/**
	 * Próbuje zalogować, zwraca true dla sukcesu i false gdy dane są niepoprawne.
	 * @param string $username
	 * @param string $password
	 * @return boolean
	 */
	public function tryToLogIn($username,$password)
	{
		$query = 
		"
			SELECT 
				login, imie, nazwisko, poziom
			FROM 
				uzytkownicy
				JOIN grupy_uzytk USING(id_grupy)
			WHERE 
				login=? 
				AND haslo=?
		";
		$user = Model_Mysql::getConnection()->getRow($query, $username,$password);
		if($user)
		{
			$this->uid = $_SESSION['loggedUid'] = $user['login'];
			$this->data = $_SESSION['loggedData'] = $user;
			$this->userLevel = $_SESSION['loggedUserLevel'] = $user['poziom'];
			$this->isLogged = true;
			return true;
		}
		else
		{
			return false;
		}
	}


	/**
	 * Zwraca IP połączenia które pobrało stronę lub "1.1.1.1" w przypadku gdy nie uda się go ustalić.
	 * @return string
	 */
	public function getUserIP()
	{
		$ip;
		if (getenv("HTTP_CLIENT_IP"))
		{
			$ip = getenv("HTTP_CLIENT_IP");
		}
		else if(getenv("HTTP_X_FORWARDED_FOR"))
		{
			$ip = getenv("HTTP_X_FORWARDED_FOR");
		}
		else if(getenv("REMOTE_ADDR"))
		{
			$ip = getenv("REMOTE_ADDR");
		}
		else
		{
			$ip = "1.1.1.1";
		}
		return $ip;
	}

	/**
	 * Ustawia hasło dla użytkownika $login bądź aktualnie zalogowanego, gdy nie poda się $login.
	 * Sprawdzane są prawa dostępu, agent może zmienić tylko swoje, supervisor lub wyżej każde.
	 * 
	 * @param string $password
	 * @param string $login
	 * @throws Model_Auth_Exception
	 */
	public function setPassword($password, $login=false)
	{
		if ( !$login && $this->isLogged )
		{
			Model_Mysql::getConnection()->query("UPDATE uzytkownicy SET haslo=? WHERE login=?", $password, $this->uid);
		}
		else if ( $this->userLevel >= LEVEL_SUPERVISOR )
		{
			Model_Mysql::getConnection()->query("UPDATE uzytkownicy SET haslo=? WHERE login=?", $password, $login);
		}
		else
		{
			throw new Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Niewystarczający poziom dostępu użytkownika!");
		}
	}

	/**
	 * Zwraca wartości danych zapisanych w sesjii dla zalogowanego użytkownika (kolumny tabeli uzytkownicy)
	 * @param string $name
	 * @throws Model_Auth_Exception
	 * @return string
	 */
	public function get($name)
	{
		if ( !$this->isLogged )
		{
			throw new Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Próba pobrania danych użytkownika pomimo braku zalogowania!");
		}
		else if ( !isset($this->data[$name]))
		{
			throw new Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Próba pobrania nieistniejących danych użytkownika!");
		}
		return $this->data[$name];
	}

	/**
	 * Zwraca login zalogowanego użytkownika.
	 */
	public function getUserId()
	{
		return $this->uid;
	}

	/**
	 * Zwraca poziom aktualnie zalogowanego użytkownika.
	 */
	public function getUserLevel()
	{
		return $this->userLevel;
	}

	/**
	 * Sprawdza czy poziom użytkownika jest na poziomie $status lub wyższym.
	 * @param int $status
	 * @return boolean
	 */
	public function isAtLeast($status)
	{
		return $this->userLevel >= $status;
	}
}
