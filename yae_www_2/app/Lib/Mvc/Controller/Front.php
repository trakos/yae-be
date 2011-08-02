<?php 
/**
 * @package Lib
 * @category Lib
 */

/**
 * Singleton class of front controller, which decides what content show to user based on a data sent.
 */
class Lib_Mvc_Controller_Front
{
	/**
	 * @var Lib_Mvc_Controller_Front
	 */
	static protected $instance;
	/**
	 * Fetch instance of front controller.
	 * @throws Lib_Mvc_Controller_Exception
	 * @return Lib_Mvc_Controller_Front
	 */
	static public function getInstance()
	{
		if(!self::$instance)
		{
			self::$instance = new static();
		}
		return self::$instance;
	}
	/**
	 * Current controller action.
	 * 
	 * @var string
	 */
	protected $action;
	/** 
	 * Current controiller.
	 * 
	 * @var string
	 */
	protected $controller;	
	
	/**
	 * 
	 * Constructor, choosing controller and action.
	 * 
	 * @throws Lib_Mvc_Controller_Exception
	 * 
	 */
	protected function __construct()
	{
		$this->chooseController();
		$this->chooseAction();
	}
	
	/**
	 * Decide which controller to use.
	 * 
	 * @throws Lib_Mvc_Controller_Exception
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
				throw new Lib_Mvc_Controller_Exception(E_CONTROLLER_DOESNT_EXIST, "Controller ".$this->controller." does not exist!");
			}
		}
		catch(Exception $e)
		{
			throw new Lib_Mvc_Controller_Exception(E_CONTROLLER_DOESNT_EXIST, "Controller ".$this->controller." does not exist!");
		}
	}
	
	/**
	 * Decide which controiller action to call.
	 * 
	 * @throws Lib_Mvc_Controller_Exception
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
			throw new Lib_Mvc_Controller_Exception(E_ACTION_DOESNT_EXIST, "Action ".$this->action." does not exist in controller ".$this->controller.".");
		}
	}
	
	/**
	 * Call previously chosend action and render the view.
	 * @throws Lib_Mvc_Controller_Exception
	 * @return void
	 */
	public function perform()
	{
		$this->performControllerAction($this->controller,$this->action);
	}
	
	/**
	 * Call explicitly givcen action of an explicitly given controller.
	 * 
	 * @param string $controller
	 * @param string $action
	 * @param array $additionalData
	 * @throws Lib_Mvc_ControllerException
	 * @return void
	 */
	public function performControllerAction($controllerString,$actionString,$additionalData=array())
	{
		if(!class_exists('Controller_'.$controllerString,true))
		{
			throw new Lib_Mvc_Controller_Exception(E_CONTROLLER_DOESNT_EXIST, "Controller $controllerString does not exist!");
		}
		if(!method_exists('Controller_'.$controllerString,$actionString.'Action'))
		{
			throw new Lib_Mvc_Controller_Exception(E_ACTION_DOESNT_EXIST, "Action $actionString doest not exist!");
		}
		$controllerName = 'Controller_'.$controllerString;
		/**
		 * This object should extend Lib_Mvc_Controller_Abstract
		 * @var $controller Lib_Mvc_Controller_Abstract
		 */
		$controller = new $controllerName();
		if ( ! $controller->canCurrentUserAccess() )
		{
			if ( Model_Auth::getInstance()->isLogged() )
			{
				throw new Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "Insufficient privileges!");
			}
			else
			{
				throw new Model_Auth_Exception(E_INSUFFICIENT_PRIVILEGES, "You must be logged in to see this page.");
			}
		}
		Lib_Mvc_View::setAction($controllerString, $actionString);
		$data = $controller->{$actionString."Action"}();
		$controller->initView();
		Lib_Mvc_View::getInstance()->renderPage();
	}
};