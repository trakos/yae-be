<?php
	// require_once('firebug.php');
	// wydłużenie maksymalnego czasu wykonywania
	set_time_limit(120);
	// rozpoczęcie sesjii php
	session_start();
	// includowanie rzeczy koniecznych do rozpoczęcia działania
	require_once('../configuration.php');
	// autoincludowanie plików zgodnie z nazwami klas
	set_include_path( get_include_path() . PATH_SEPARATOR . SRC_PATH_LIB . PATH_SEPARATOR . SRC_PATH_APP . PATH_SEPARATOR . SRC_PATH_APP . '/Controller' );
	function autoload($class) 
	{
		$file = str_replace('_', '/', $class); 
		include($file.".php");
	}
	spl_autoload_register('autoload');
	// obsługa błędów poprzez exceptions
	if(!defined('E_DEPRECATED'))
	{
		define('E_DEPRECATED',2048);
	}
	function errorHandler($errno, $errstr, $errfile, $errline ) 
	{
		if($errno == E_STRICT || $errno == E_DEPRECATED )
		{
			// ignorujemy
		}
		else
		{
			throw new Tmvc_Controller_PHPErrorException($errstr, 0, $errno, $errfile, $errline);
		}
	}
	function printError($msg)
	{
		ob_end_clean();
		?>
			<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
			<html>
			<head>
			<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
			<meta http-equiv="Content-Language" content="pl"/>
			<title>You Are Exposed - Error</title>
			</head>
			<body>
			<?php
				echo $msg;
			?>
			</body>
			</html>
		<?php
	}
	set_error_handler("errorHandler");
	// start timera
	$globalTimer = new Tmvc_Model_Timer();
	// buforowanie outputu
	ob_start();
	// bootstrapowanie pożadanych instancji obiektów
	Model_Auth::getInstance();
	// wywołanie strony i obsługa jej ewentualnych błędów
	try
	{
		try
		{
			Tmvc_Controller_Front::getInstance()->perform();
		}
		// debug catch
		catch(Exception $e)
		{
			if ( !defined("T_DEBUG") || !T_DEBUG )
			{
				ob_end_clean();
				throw $e;
			}
			?>
				<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
				<html>
				<head>
				<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
				<meta http-equiv="Content-Language" content="pl"/>
				<title>YAE system exception</title>
				</head>
				<body>
				<?php
					echo $e->getMessage();
					if ( $e instanceof Tmvc_Controller_PHPErrorException )
					{
						echo "<br>";
						echo $e->getRealMessage();
					}
					echo '<pre>';
					$trace = $e->getTrace();
					foreach ( $trace as $k => $tracePoint )
					{
						$args = "";
						if ( isset($tracePoint['args']) && is_array($tracePoint['args']) )
						{
							foreach ( $tracePoint['args'] as $arg )
							{
								try 
								{
									$args.= json_encode($arg).",";
								}
								catch(Exception $e)
								{
									$args.= var_dump($arg).",";
								}
							}	
						}
						$function = isset($tracePoint['class']) ? $tracePoint['class'].$tracePoint['type'].$tracePoint['function'] : $tracePoint['function'];
						$line = isset($tracePoint['line']) ? $tracePoint['line'] : 0;
						$file = isset($tracePoint['file']) ? $tracePoint['file'] : 'unknown';
						printf("#%-2s %100s:%-4s %50s(%s)\n", $k, $file, $line, $function, $args);
					}
				?>
				</body>
				</html>
			<?php
		}
	}
	// non-debug catches
	catch(Tmvc_Model_Auth_Exception $e)
	{
		printError($e->getMessage());
	}
	catch(Tmvc_ControllerException $e)
	{
		header("HTTP/1.0 404 Not Found");
		printError("404 not found");
	}
	catch(Tmvc_Model_Mysql_Exception $e)
	{
		printError("There was a problem accessing the database.");
	}
	/*catch(Exception $e)
	{
		printError("Internal error occurred.");
	}*/
	
	