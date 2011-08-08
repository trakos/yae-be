<?php
/**
 * Abstract class of all controllers.
 */
abstract class Tmvc_Controller_Abstract
{
	/**
	 * View instance.
	 * @var Tmvc_View
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
		return Tmvc_Model_Auth::getInstance()->isAtLeast($this->_requiredUserLevel);
	} 
	
	/**
	 * Constructor, creates a view instances.
	 */
	public function __construct()
	{
		$this->view = Tmvc_View::getInstance();
		$this->init();
	}
	
	/**
	 * Add javascript file to the head section of page (through the Includer).
	 */
	public function addJavascript($path)
	{
		Tmvc_View_Includer_Javascript::getInstance()->addFile($path);
	}
	
	/**
	 * Add css file to the head section of page (through the Includer)
	 */
	public function addStyle($path)
	{
		Tmvc_View_Includer_Css::getInstance()->addFile($path);
	}
	
	/**
	 * Prints out php variable to the javascript.
	 * @param string $name name of a variable in javascript which will receive this value.
	 * @param string $value value to assign.
	 */
	public function addJavascriptVariable($name,$value)
	{
		Tmvc_View_Includer_Javascript::getInstance()->addScriptText("var ".$name." = ".json_encode($value).";\n");
	}
	
	/**
	 * Redirect method, calls die after setting redirect header.
	 * @param string $controller
	 * @param string $action
	 * @param array $arguments
	 */
	protected function _redirect($controller,$action='Index',$arguments=array())
	{
		$url = Tmvc_View::getInstance()->link($controller,$action,$arguments,false,false);
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
	 * Saves the variable in a session with a TMVC_ prefix for the key.
	 * @param string $key
	 * @param unknown_type $value
	 */
	protected function _setSession($key, $value)
	{
		Tmvc_Model_Session::setSession($key,$value);
	}
	
	/**
	 * Retrieves session variables set with _setSession
	 * @param string $key
	 * @return unknown_type
	 */
	protected function _getSession($key)
	{
		return Tmvc_Model_Session::getSession($key);
	}
	
	/**
	 * Unsert session variable set with _setSession
	 * @param unknown_type $key
	 */
	protected function _unsetSession($key)
	{
		Tmvc_Model_Session::unsetSession($key);
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