<?php
class Lib_Mvc_Includer_Javascript extends Lib_Mvc_Includer_Abstract
{
	protected $_files = array();
	protected $_systemPath = SRC_PATH_JS;
	protected $_wwwPath = WWW_PATH_JS;
	protected $_cachePath = SRC_PATH_JS_CACHE;
	protected $_fileExtension = "js";

	protected function _printIncludeFile($path)
	{
		echo "<script type='text/javascript' src='$path'> </script>\n";
	}
	
	protected function _printText($text)
	{
		echo "<script type='text/javascript'>";
		echo $text;
		echo "</script>";
	}
};
