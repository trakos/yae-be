<?php 
/**
 * @package Lib
 * @category Lib
 * @version 1.0
 */

/**
 * Klasa singletonowa głównego kontrolera, który na podstawie przesłanych danych
 * decyduje, zawartość której strony wyświetlamy użytkownikowi
 * 
 * @version 1.0
 * 
 */
class Lib_FrontController
{
	/**
	 * @var Lib_FrontController
	 */
	static protected $instance;
	/**
	 * Pobranie singletonowej instancji FrontControllera
	 * @throws Lib_ControllerException
	 * @return Lib_FrontController
	 */
	static public function getInstance()
	{
		if(!self::$instance)
		{
			self::$instance = new Lib_FrontController();
		}
		return self::$instance;
	}
	/**
	 * Aktualna akcja kontrolera
	 * 
	 * @var string
	 */
	protected $action;
	/** 
	 * Aktualny kontroler
	 * 
	 * @var string
	 */
	protected $controller;	
	
	/**
	 * 
	 * Konstruktor, inicjalizacja - wybór kontrolera/akcji
	 * 
	 * @throws Lib_FrontControllerException
	 * 
	 */
	protected function __construct()
	{
		$this->chooseController();
		$this->chooseAction();
	}
	
	/**
	 * Na podstawie danych użytkownika, wybierz obecny kontroler
	 * 
	 * @throws Lib_ControllerException
	 * @return void
	 */
	protected function chooseController()
	{
		$defaultController = 'Index';
		if(isset($_REQUEST['c']))
		{
			$this->controller = $_REQUEST['c'];
		}
		else
		{
			$this->controller = $defaultController;
		}
		try 
		{
			if(!class_exists('Controller_'.$this->controller,true))
			{
				throw new Lib_ControllerException(E_CONTROLLER_DOESNT_EXIST, "Kontroler ".$this->controller." nie istnieje!");
			}
		}
		catch(Exception $e)
		{
			throw new Lib_ControllerException(E_CONTROLLER_DOESNT_EXIST, "Kontroler ".$this->controller." nie istnieje!");
		}
	}
	
	/**
	 * Na podstawie danych użytkownika, wybierz akcję
	 * 
	 * @throws Lib_ControllerException
	 * @return void
	 */
	protected function chooseAction()
	{
		$defaultAction = 'index';
		if(isset($_REQUEST['a']))
		{
			$this->action = $_REQUEST['a'];
		}
		else
		{
			$this->action = $defaultAction;
		}
		if(!method_exists('Controller_'.$this->controller,$this->action.'Action'))
		{
			throw new Lib_ControllerException(E_ACTION_DOESNT_EXIST, "Akcja ".$this->action." nie istnieje w kontrolerze ".$this->controller.".");
		}
	}
	
	/**
	 * Wywołanie akcji wynikającej z danych wejściowych i wywołanie widoku
	 * @throws Lib_ControllerException
	 * @return void
	 */
	public function perform()
	{
		$this->performControllerAction($this->controller,$this->action);
	}
	
	/**
	 * Wywołanie konkretnej akcji konkretnego kontrolera, niezależnie od danych
	 * wejściowych wykrytych przez samego FrontControllera.
	 * 
	 * @param string $controller
	 * @param string $action
	 * @param array $additionalData
	 * @throws Lib_ControllerException
	 * @return void
	 */
	public function performControllerAction($controllerString,$actionString,$additionalData=array())
	{
		if(!class_exists('Controller_'.$controllerString,true))
		{
			throw new Lib_ControllerException(E_CONTROLLER_DOESNT_EXIST, "Kontroler $controllerString nie istnieje!");
		}
		if(!method_exists('Controller_'.$controllerString,$actionString.'Action'))
		{
			throw new Lib_ControllerException(E_ACTION_DOESNT_EXIST, "Akcja $actionString nie istnieje!");
		}
		$controllerName = 'Controller_'.$controllerString;
		/**
		 * Z założenia obiekt ten przeciąża Lib_Controller.
		 * @var $controller Lib_Controller
		 */
		$controller = new $controllerName();
		if ( ! $controller->canCurrentUserAccess() )
		{
			if ( Model_Auth::getInstance()->isLogged() )
			{
				throw new Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Niewystarczający poziom dostępu użytkownika!");
			}
			else
			{
				throw new Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Do obejrzenia tej podstrony musisz być zalogowany.");
			}
		}
		Lib_View::setAction($controllerString, $actionString);
		$data = $controller->{$actionString."Action"}();
		$controller->initView();
		Lib_View::getInstance()->renderPage();
	}
};