<?php
/**
 * Abstract class of all controllers.
 */
abstract class Lib_Mvc_Controller_Abstract
{
	/**
	 * View instance.
	 * @var Lib_Mvc_View
	 */
	public $view;
	
	/**
	 * Default required user level to be allowed to use controller.
	 */
	protected $_requiredUserLevel = 99999;
	
	/**
	 * Check, if currently logged user has sufficient privileges to get access to this controller.
	 * @return bool
	 */
	public function canCurrentUserAccess()
	{
		return Lib_Mvc_Model_Auth::getInstance()->isAtLeast($this->_requiredUserLevel);
	} 
	
	/**
	 * Constructor, creates a view instances.
	 */
	public function __construct()
	{
		$this->view = Lib_Mvc_View::getInstance();
		$this->init();
	}
	
	/**
	 * Add javascript file to the head section of page (through the Includer).
	 */
	public function addJavascript($path)
	{
		Lib_Mvc_View_Includer_Javascript::getInstance()->addFile($path);
	}
	
	/**
	 * Add css file to the head section of page (through the Includer)
	 */
	public function addStyle($path)
	{
		Lib_Mvc_View_Includer_Css::getInstance()->addFile($path);
	}
	
	/**
	 * Prints out php variable to the javascript.
	 * @param string $name name of a variable in javascript which will receive this value.
	 * @param string $value value to assign.
	 */
	public function addJavascriptVariable($name,$value)
	{
		Lib_Mvc_View_Includer_Javascript::getInstance()->addScriptText("var ".$name." = ".json_encode($value).";\n");
	}
	
	/**
	 * Metoda służąca do przekierowania na inną stronę wewnątrz systemu.
	 * @param string $controller
	 * @param string $action
	 * @param array $arguments
	 */
	protected function _redirect($controller,$action='Index',$arguments=array())
	{
		$url = Lib_Mvc_View::getInstance()->link($controller,$action,$arguments,false);
		header( "Location: ".$url );
		die();
	}
	
	/**
	 * Get variable from the request parameters sent by the user. NULL will be received when $key was not defined.
	 * @param string $key
	 * @return string
	 */
	protected function _get( $key )
	{
		if( isset($_REQUEST[$key]) ) {
			return $_REQUEST[$key];
		} else {
			return NULL;
		}
	}
	
	/**
	 * Object initialization, called within constructor.
	 */
	abstract public function init();
	/**
	 * Intialization just before calling the renderer, already after the controller action.
	 */
	abstract public function initView();
};