<?php 
/**
 * @package Lib
 * @category Lib
 * @version 1.0
 */

/**
 * Singleton class of a view, which renders content.
 * 
 * @version 1.0
 * 
 */
class Tmvc_View
{
	/**
	 * @var Tmvc_View
	 */
	static protected $_instance;
	/**
	 * Current variables which will be given to the rendered view.
	 * @var array
	 */
	protected $_viewVariables=array();
	/**
	 * Name of a currently called controller.
	 * @var string
	 */
	static public $controllerName;
	/**
	 * Name of an currently called action.
	 * @var string
	 */
	static public $actionName;
	/**
	 * Stack of view variables, each subrender (render called from within other render)
	 * will have all variables assigned in a parent view plus all variables that the parent view could access,
	 * but without variables assigned before other subrender calls.
	 * 
	 * @var array
	 */
	protected $_stackOfViewVariablesState=array();
	/**
	 * Returns instance of a current object.
	 * @return Tmvc_View
	 */
	static public function getInstance()
	{
		if(!self::$_instance)
		{
			self::$_instance = new Tmvc_View();
		}
		return self::$_instance;
	}
	
	/**
	 * Protected constructor, use getInstance.
	 */
	protected function __construct()
	{
		
	}
	
	/**
	 * Returns value of an already assigned values, NULL if not set.
	 * @param string name
	 */
	public function getAssignedValue($name)
	{
		if(!isset($this->_viewVariables[$name])) 
		{
			return NULL;
		}
		else
		{
			return $this->_viewVariables[$name];
		}
	}
	
	/**
	 * Assign $value under name $name.
	 * @param string $name
	 * @param string $value
	 */
	public function assign($name,$value)
	{
		$this->_viewVariables[$name] = $value;
	}
	/**
	 * Assigns all values from $array under their corresponding key.
	 * @param array $array
	 */
	public function assignArray($array)
	{
		foreach($array as $key => $value)
		{
			$this->assign($key,$value);
		}
	}
	/**
	 * Sets controller and action.
	 * @param string $controller
	 * @param string $action
	 */
	public function setAction($controller, $action)
	{
		self::$controllerName = $controller;
		self::$actionName = $action;
	}	
	
	/**
	 * Renders layout.
	 */
	public function renderPage()
	{
		$this->assign("controller", self::$controllerName);
		$this->assign("action", self::$actionName);
		$this->render("layout");
	}

	/**
	 * Renders view $viewName.
	 * @param string $viewName
	 */
	public function render($viewName)
	{
		// przypisanie danych do widoku
		foreach($this->_viewVariables as $k => $v)
		{
			$$k = $v;
		}
		// zapisanie stanu zmiennych
		array_push($this->_stackOfViewVariablesState,$this->_viewVariables);
		// wywołanie widoku
		include(SRC_PATH_VIEW.'/'.$viewName.'.php');
		// usunięcie stanu zmiennych
		array_pop($this->_stackOfViewVariablesState);
		// cofnięcie do stanu sprzed pierwszego assigna (ostatni ze stosu, ale bez zdejmowania)
		if(empty($this->_stackOfViewVariablesState))
		{
			$this->_viewVariables = array();
		}
		else
		{
			$this->_viewVariables = $this->_stackOfViewVariablesState[count($this->_stackOfViewVariablesState)-1];
		}
	}
	
	public function link($controller,$action,$arguments=array(),$onlyArguments=false,$encode=true)
	{
		$arguments['c'] = $controller?$controller:self::$controllerName;
		$arguments['a'] = $action?$action:self::$actionName;
		$url = '?'.http_build_query($arguments,'_',$encode?'&amp;':'&');
		if ( $onlyArguments )
		{
			return $url;
		}
		else 
		{
			return WWW_PATH_MAIN.'/'.$url;
		}
	}
	
	public function secToTime($secondsCount)
	{
		return gmdate("H:i:s",$secondsCount);
	}
	
	public function horizontalBar($value, $max)
	{
		$percent = $max==0?1:$value/$max;
		$height = 15;
		$width = 230;
		$barWidth = $width*$percent;
		$bgWidth = $width - $barWidth;
		$string = "<div style='text-align:center; width: 80px; display: inline-block'>$value / $max</div>";
		$string.= "<div style='margin: 0 0 -2px 5px; height: ${height}px; width: ${barWidth}px; background: #85ACDE; display: inline-block;'></div>";
		$string.= "<div style='margin: 0 0 -2px 0px; height: ${height}px; width: ${bgWidth}px; background: #cfcfcf; display: inline-block;'></div>";
		return $string;
	}
		
	public function paginator($page,$limit,$count,$controller,$action, $arguments, $pageArgument)
	{
		if ( !$count || $count <= $limit )
		{
			return;
		}
		$lastPage = ceil($count/$limit);
		echo "<div id='pages'>";
		for( $i=1; $i<=$lastPage; $i++ )
		{
			if ( $lastPage > 100 )
			{
				if ( abs($page-$i) >= 100 && ($i%100)!=0 )
				{
					continue;
				}
			}
			if( $i != $page )
			{
				$arguments[$pageArgument] = $i;
				echo "<a href='".$this->link($controller,$action,$arguments)."' rel='nofollow'>$i</a> ";
			}
			else
			{
				echo "<span>$i</span> ";
			}
		}
		echo "</div>";
	}
};
?>