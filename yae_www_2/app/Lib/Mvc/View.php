<?php 
/**
 * @package Lib
 * @category Lib
 * @version 1.0
 */

/**
 * Klasa singletonowa obiektu widoku, który odpowiada za renderowanie widoków.
 * 
 * @version 1.0
 * 
 */
class Lib_Mvc_View
{
	/**
	 * @var Lib_Mvc_View
	 */
	static protected $instance;
	/**
	 * Aktualne zmienne przekazywane do wywoływanych widoków
	 * @var array
	 */
	protected $viewVariables=array();
	/**
	 * Nazwa aktualnie wywoływanego kontrolera
	 * @var string
	 */
	static protected $controllerName;
	/**
	 * Nazwa aktualnie wywoływanej akcji
	 * @var string
	 */
	static protected $actionName;
	/**
	 * Stos stanów zmiennych widoku, tak, żeby każdy subrender (render z wewnątrz widoku)
	 * miał zmienne wyassignowane w tym widoku i wszystkie które miał ten widok,
	 * ale nie te assignowane przed poprzednim subrenderem.
	 * 
	 * @var array
	 */
	protected $stackOfViewVariablesState=array();
	/**
	 * Pobranie singletonowej instancji View
	 * @return Lib_Mvc_View
	 */
	static public function getInstance()
	{
		if(!self::$instance)
		{
			self::$instance = new Lib_Mvc_View();
		}
		return self::$instance;
	}
	
	/**
	 * Konstruktor protected, korzystać z getInstance.
	 */
	protected function __construct()
	{
		
	}
	
	/**
	 * Pobiera wartość z już zassignowanej wartości, zwraca NULL jeśli nie znajdzie takiej
	 * @param string name
	 */
	public function getAssignedValue($name)
	{
		if(!isset($this->viewVariables[$name])) 
		{
			return NULL;
		}
		else
		{
			return $this->viewVariables[$name];
		}
	}
	
	/**
	 * Przypisuje wartość $value pod nazwę $name do zmiennych widoku
	 * @param string $name
	 * @param string $value
	 */
	public function assign($name,$value)
	{
		$this->viewVariables[$name] = $value;
	}
	/**
	 * Przypisuje wszystkie wartości spod $array pod ich klucze w tablicy jako nazwy do zmiennych widoku.
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
	 * Ustawia akcję i kontroler
	 * @param string $controller
	 * @param string $action
	 */
	public function setAction($controller, $action)
	{
		self::$controllerName = $controller;
		self::$actionName = $action;
	}	
	
	/**
	 * Renderuje layout.
	 */
	public function renderPage()
	{
		$this->assign("controller", self::$controllerName);
		$this->assign("action", self::$actionName);
		$this->render("layout");
	}

	/**
	 * Renderuje widok $viewName.
	 * @param string $viewName
	 */
	public function render($viewName)
	{
		// przypisanie danych do widoku
		foreach($this->viewVariables as $k => $v)
		{
			$$k = $v;
		}
		// zapisanie stanu zmiennych
		array_push($this->stackOfViewVariablesState,$this->viewVariables);
		// wywołanie widoku
		include(SRC_PATH_VIEW.'/'.$viewName.'.php');
		// usunięcie stanu zmiennych
		array_pop($this->stackOfViewVariablesState);
		// cofnięcie do stanu sprzed pierwszego assigna (ostatni ze stosu, ale bez zdejmowania)
		if(empty($this->stackOfViewVariablesState))
		{
			$this->viewVariables = array();
		}
		else
		{
			$this->viewVariables = $this->stackOfViewVariablesState[count($this->stackOfViewVariablesState)-1];
		}
	}
	
	public function link ($controller,$action,$arguments=array(),$onlyArguments=false,$encodeAmp=false)
	{
		$arguments['c'] = $controller?$controller:self::$controllerName;
		$arguments['a'] = $action?$action:self::$actionName;
		$url = '?'.http_build_query($arguments,'_',$encodeAmp?'&amp;':'&');
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
};
?>