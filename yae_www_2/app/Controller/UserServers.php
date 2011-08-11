<?php
class Controller_UserServers extends Lib_Controller
{
	protected $_requiredUserLevel = LEVEL_USER;
	
	protected function _randomizeCaptcha()
	{
		$possible = '23456789bcdfghjkmnpqrstvwxyz';
		$code = '';
		$i = 0;
		while ($i < 6) {
			$code .= substr($possible, mt_rand(0, strlen($possible)-1), 1);
			$i++;
		}
		$this->_setSession("captcha", $code);
		return $code;
	} 
	
	public function initView()
	{
		parent::initView();
		$this->view->assign("submenu", array("userServerList"=>"Server list","addServer"=>"Add server"));
	}
	
	public function userServerListAction()
	{
		$servers = Model_User_Servers::getUserServers();
		$this->useSessionMessages();
		$this->view->assign("siteTitle", "server list");
		$this->view->assign("servers", $servers);
	}
	
	public function addServerAction()
	{
		$addForm = new View_Form_AddServer();
		$addForm->setValues($_REQUEST);
		
		if ( !empty($_POST) && $addForm->validate() )
		{
			$values = $addForm->getValues();
			$ip = $values['ip'];
			$port = $values['port'];
			$rconPassword = $values['rcon'];
			try
			{
				Model_User_Servers::addServer($ip, $port, $rconPassword);
				$this->_setSession("successMessage", "Server added successfully.");
				$this->_redirect("UserServers", "userServerList");
			}
			catch ( Model_Yae_Exception $e )
			{
				if ( $e->getCode() == E_SERVER_UNAVAILABLE )
				{
					$addForm->getInput("addServer", "ip")->setErrorText("The server is unavailable, check your ip and port again.");
				}
				else if ( $e->getCode() == E_SERVER_WRONG_PASSWORD )
				{
					$addForm->getInput("addServer", "rcon")->setErrorText("The rcon password is incorrect.");
				}
				else if ( $e->getCode() == E_SERVER_EXISTS )
				{
					$addForm->getInput("addServer", "ip")->setErrorText("The server with this ip/port pair is already added to yae, probably by somebody else.");
				}
				else
				{
					throw $e;
				}
			}
		}
		
		$this->_randomizeCaptcha();
		$addForm->clearCaptcha();
		$this->view->assign("siteTitle", "add new server");
		$this->view->assign("addForm", $addForm);
		
	}
	
	public function changeServerPasswordAction()
	{
		$changeForm = new View_Form_ChangeServerPassword();
		$changeForm->setValues($_REQUEST);
		$serverId = $this->_get('server_id');
		
		if ( !empty($_POST) && $changeForm->validate() )
		{
			$values = $changeForm->getValues();
			$rconPassword = $values['rcon'];
			try
			{
				Model_User_Servers::changeServerPassword($serverId, $rconPassword);
				$this->_setSession("successMessage", "Password changed successfully.");
				$this->_redirect("UserServers", "userServerList");
			}
			catch ( Model_Yae_Exception $e )
			{
				if ( $e->getCode() == E_SERVER_DOESNT_EXISTS || $e->getCode() == E_INSUFFICIENT_PRIVILEGES )
				{
					$this->_setSession("errorMessage", "Something went wrong, server was not found.");
					$this->_redirect("UserServers", "userServerList");
				}
				else if ( $e->getCode() == E_SERVER_UNAVAILABLE )
				{
					$this->_setSession("errorMessage", "The server is unavailable.");
					$this->_redirect("UserServers", "userServerList");
				}
				else if ( $e->getCode() == E_SERVER_WRONG_PASSWORD )
				{
					$changeForm->getInput("changeServerPassword", "rcon")->setErrorText("The rcon password is incorrect.");
				}
				else
				{
					throw $e;
				}
			}
		}
		
		$this->_randomizeCaptcha();
		$changeForm->clearCaptcha();
		$this->view->assign("siteTitle", "change server password");
		$this->view->assign("changeForm", $changeForm);
	}
	
	public function activateServerAction()
	{
		$serverId = $this->_get("server_id");
		try
		{
			Model_User_Servers::enableServer($serverId);
			$this->_setSession("successMessage", "Server check scheduled succesfully.");
		}
		catch ( Model_Yae_Exception $e )
		{
			if ( $e->getCode() != E_SERVER_DOESNT_EXISTS && $e->code != E_INSUFFICIENT_PRIVILEGES )
			{
				throw $e;
			}
			$this->_setSession("errorMessage", "Something went wrong, server was not found.");
		}
		$this->_redirect("UserServers", "userServerList");
	}
	
	public function deactivateServerAction()
	{
		$serverId = $this->_get("server_id");
		try
		{
			Model_User_Servers::disableServer($serverId);
			$this->_setSession("successMessage", "Server deactivated succesfully.");
		}
		catch ( Model_Yae_Exception $e )
		{
			if ( $e->getCode() != E_SERVER_DOESNT_EXISTS && $e->code != E_INSUFFICIENT_PRIVILEGES )
			{
				throw $e;
			}
			$this->_setSession("errorMessage", "Something went wrong, server was not found.");
		}
		$this->_setSession("deactivatedServer", $serverId);
		$this->_redirect("UserServers", "userServerList");
	}
	
};