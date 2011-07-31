<?php
class Lib_Mvc_Includer_Css extends Lib_Mvc_Includer_Abstract
{
	protected $_files = array();
	protected $_systemPath = SRC_PATH_CSS;
	protected $_wwwPath = WWW_PATH_CSS;
	protected $_cachePath = SRC_PATH_CSS_CACHE;
	protected $_fileExtension = "css";

	protected function _printIncludeFile($path)
	{
		echo "<link rel='stylesheet' href='$path' type='text/css' />\n";
		
	}
	
	protected function _printText($text)
	{
		echo "<style type='text/css'>";
		echo $text;
		echo "</style>";
	}
};
