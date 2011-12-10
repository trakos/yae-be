<?php 
/**
 * @package		Main
 * @category	Main
 * @author		Piotr Stankowski
 * @version		1.0
 */
	define('T_DEBUG', 0);
	/* sciezki systemowe */
	define('SRC_PATH_MAIN', getcwd()."/..");
	define('SRC_PATH_LIB',SRC_PATH_MAIN.'/lib');
	define('SRC_PATH_APP',SRC_PATH_MAIN.'/app');
	define('SRC_PATH_VIEW',SRC_PATH_MAIN.'/templates');
	define('SRC_PATH_RESOURCES',SRC_PATH_MAIN.'/resources');
	define('SRC_PATH_JS', './js/');
	define('SRC_PATH_JS_CACHE', '/cache/');
	define('SRC_PATH_CSS', './css/');
	define('SRC_PATH_CSS_CACHE', '/cache/');
	/* sciezki www */
	define('WWW_PATH_MAIN','http://yaetmp.s2.trakos.pl/');
	define('WWW_PATH_JS',WWW_PATH_MAIN.'/js/');
	define('WWW_PATH_CSS',WWW_PATH_MAIN.'/css/');
	/* baza danych */
	define('DB_HOST','localhost');
	define('DB_USER', 'dawn');
	define('DB_PASSWORD',"***REMOVED***");
	define('DB_DATABASE','dawn');
	/* poziomy */
	define('LEVEL_UNLOGGED', 0);
	define('LEVEL_USER', 10);
	define('LEVEL_SUPERADMIN', 100);
	/* kody błędów yae */
	define('E_SERVER_EXISTS', 200);
	define('E_SERVER_UNAVAILABLE', 201);
	define('E_SERVER_WRONG_PASSWORD', 202);
	define('E_SERVER_DOESNT_EXISTS', 203);
	define('E_SERVER_RCON_ENC_EMPTY', 204);
	define('E_USER_DOESNT_EXISTS', 210);
	define('E_INVITE_ALREADY_EXISTS', 211);
	define('E_INVITE_IGNORED', 212);
	define('E_INVITE_ALREADY_ACCEPTED', 213);
	define('E_INVITE_DOESNT_EXISTS', 214);
	define('E_INVITE_UNEXPECTED_STATUS', 215);
	define('E_INVITE_SELF', 216);
	/* kody błędów wewnętrznych */
	define('E_CONTROLLER_DOESNT_EXIST', 101);
	define('E_ACTION_DOESNT_EXIST', 102);
	define('E_NOT_AUTHORIZED', 103);
	define('E_ALREADY_AUTHORIZED', 104);
	define('E_INSUFFICIENT_PRIVILEGES',105);
	define('E_INI_UNREADABLE_OR_DOESNT_EXIST',150);
	define('E_INI_PARSING_ERROR', 151);
	define('E_INI_INVALID_SECTION_OR_VALUE',152);
	define('E_MYSQL',153);
	define('E_FORM_NO_SECTION_ADDED',170);
	/* stałe yae */
	define('YAE_SERVER_OFF_DATE', "2038-01-19 04:14:07");
	define('YAE_SERVER_OFF_TIME', 2147483647);
