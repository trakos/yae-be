<?php
	class JavascriptIncluder
	{
		protected $files = array();
		protected $systemPath = '/var/www/bakyae/cf/';
		protected $wwwPath = 'http://trakos.pl/bakyae/cf/';
		protected $cachePath = 'js/cache/';
		
		public function __construct()
		{
		}
		
		public function addFile($path)
		{
			$this->files[] = $path;
		}
		
		public function output($onlyLinkTo=false)
		{
			$wwwPath = $this->wwwPath;
			$systemPath = $this->systemPath;
			$cachePath = $this->cachePath;
			if($onlyLinkTo)
			{
				foreach($this->files as $file)
				{
					echo "<script type='text/javascript' src='$wwwPath$file'> </script>\n";
				}
			}
			else
			{
				$text = '';
				foreach($this->files as $file)
				{
					$text.= file_get_contents($systemPath.$file);
				}
				$hash = md5($text);
				if( !file_exists("$systemPath$cachePath$hash.js") )
				{
					file_put_contents("$systemPath$cachePath$hash.js",$text);
				}
				echo "<script type='text/javascript' src='$wwwPath$cachePath$hash.js'></script>\n";
			}
			$this->files = array();
		}
	};