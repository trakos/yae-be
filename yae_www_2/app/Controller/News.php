<?php
class Controller_News extends Lib_Controller
{
	protected $_requiredUserLevel = LEVEL_UNLOGGED;
	
	public function indexAction()
	{
		$this->view->assign('siteTitle', "News");
		$this->view->assign('news', Model_News::getAllNewses());
	}
};