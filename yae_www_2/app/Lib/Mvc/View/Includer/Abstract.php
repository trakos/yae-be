<?php
abstract class Lib_Mvc_View_Includer_Abstract
{
	protected $_files = array();
	protected $_systemPath = null;
	protected $_wwwPath = null;
	protected $_cachePath = null;
	protected $_fileExtension = null;

	/**
	 * @var array
	 */
	protected static $_instances;
	
	/**
	 * @var string
	 */
	protected $_includeText='';
	
	/**
	 * Get instance.
	 * @return Lib_Mvc_Includer_Abstract
	 */
	public static function getInstance()
	{
		$className = get_called_class();
		if ( !isset(self::$_instances[$className]) )
		{
			self::$_instances[$className] = new static();	
		}
		return self::$_instances[$className];
	}

	protected function __construct()
	{
	}
	
	abstract protected function _printIncludeFile($path);
	abstract protected function _printText($text);

	public function addFile($path)
	{
		$this->_files[] = $path;
	}
	
	public function addScriptText($text)
	{
		$this->_includeText.=$text;
	}

	public function output($onlyLinkTo=true)
	{
		$wwwPath = $this->_wwwPath;
		$systemPath = $this->_systemPath;
		$cachePath = $this->_cachePath;
		$extension = $this->_fileExtension;
		if($onlyLinkTo)
		{
			foreach($this->_files as $file)
			{
				$this->_printIncludeFile($wwwPath.$file);
			}
		}
		else
		{
			$text = '';
			foreach($this->_files as $file)
			{
				$text.= file_get_contents($systemPath.$file);
			}
			$hash = md5($text);
			if( !file_exists("$systemPath$cachePath$hash.$extension") )
			{
				file_put_contents("$systemPath$cachePath$hash.$extension",$text);
			}
			$this->_printIncludeFile("$wwwPath$cachePath$hash.$extension");
		}
		if(!empty($this->_includeText))
		{
			$this->_printText($this->_includeText);
		}
		$this->_files = array();
		$this->_includeText = "";
	}
};
