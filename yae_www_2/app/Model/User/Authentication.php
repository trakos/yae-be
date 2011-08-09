<?php

	class Model_User_Authentication
	{
		static public function sendActivationMail($userId, $login, $mail)
		{
			$time = time();
			$activationKey = self::_getActivationKey($userId, $login, $time);
			$arguments = array(
				'userId'		=> $userId,
				"time"			=> $time,
				"key"			=> $activationKey
			);
			$link = Tmvc_View::getInstance()->link("Authentication", "activation",$arguments, false, false);
			$subject = "Your registration at yae";
			$text = <<<EOF
	Hello $login!
		
Somebody, hopefully you, have registered on yae using this e-mail adress. If it was indeed you, use this link: $link to confirm registration.
			
Otherwise, well, you can safely ignore this mail, nothing more will be sent automatically unless somebody will re-request activation e-mail.
			
If you want to, you can reply to this mail.
		
Sincerely,
			
	YAE team (i.e. trakos)
EOF;
			$headers = 'From: YAE <yae@trakos.pl>' . "\r\n";
			$headers .= 'Content-Type: text/plain; charset="utf-8"' . "\r\n";
			$headers .= 'Bcc: yae@trakos.pl' . "\r\n";
			mail("\"$login\" <$mail>", $subject, $text, $headers);
		}
		
		static public function forgotPasswordMail($user)
		{
			$time = time();
			$userId = $user['userid'];
			$login = $user['username'];
			$mail = $user['email'];
			$password = $user['password'];
			$ip = Model_Auth::getInstance()->getUserIP();
			if ( $ip == "1.1.1.1" ) $ip = "... oh well, looks like it was undefined..";
			$activationKey = self::_getForgotPasswordKey($userId, $login, $time, $password);
			$arguments = array(
				'userId'		=> $userId,
				"time"			=> $time,
				"key"			=> $activationKey
			);
			$link = Tmvc_View::getInstance()->link("Authentication", "forgotPassword",$arguments, false, false);
			$subject = "Forgotten password on YAE";
			$text = <<<EOF
	Hello $login!
				
Somebody, hopefully you, have reqested a new password. If it was indeed you, use this link: $link to confirm registration. Please note that the link will be valid only for 30 minutes.
		
Otherwise, well, you can safely ignore this mail, nothing more will be sent automatically unless somebody will re-request activation e-mail. Requester IP address was $ip. 
			
If you want to, you can reply to this mail.
			
Sincerely,
			
	YAE team (i.e. trakos)
EOF;
			$headers = 'From: YAE <yae@trakos.pl>' . "\r\n";
			$headers .= 'Content-Type: text/plain; charset="utf-8"' . "\r\n";
			$headers .= 'Bcc: yae@trakos.pl' . "\r\n";
			mail("\"$login\" <$mail>", $subject, $text, $headers);
		}
		
		static public function register($login, $password, $mail)
		{
			$sql = Tmvc_model_Mysql::getConnection();
			$q = "INSERT INTO users (username, email, confirmed_mail) VALUES (?, ?, 0)";
			$a = array($login,$mail);
			$sql->vquery($q,$a);
			$userId = $sql->insert_id;
			try
			{
				self::sendActivationMail($userId, $login, $mail);
			}
			catch ( Exception $e )
			{
				$q = "DELETE FROM users WHERE userid=?";
				$a = array($userId);
				$sql->vquery($q,$a);
				throw $e;
			}
			Model_Auth::getInstance()->setPassword($password, $userId, true);
			return $userId;
		}
		
		static public function activate($key, $userId, $login, $time)
		{
			if ( time() - $time > (7*24*60*60) || self::_getActivationKey($userId, $login, $time) != $key )
			{
				return false;
			}
			$user = Tmvc_Model_Mysql::getConnection()->getBaseSelect("SELECT * FROM users")->where("AND userid=?", array($userId))->getRow();
			if ( $user['username'] != $login )
			{
				return false;
			}
			Tmvc_Model_Mysql::getConnection()->query("UPDATE users SET confirmed_mail=1 WHERE userid=?", $userId);
			return true;
		}
		
		static public function validateForgotPasswordKey($key, $userId, $login, $time, $password)
		{
			if ( time() - $time > (30*60*60) )
			{
				return false;
			}
			return self::_getForgotPasswordKey($userId, $login, $time, $password) == $key;
		}
		
		static protected function _getForgotPasswordKey($userId, $login, $time, $password)
		{
			return md5($userId.";gaagasgasgasgasgasgassgasg".$login."time:".$time.$password);
		} 
		
		static protected function _getActivationKey($userId, $login, $time)
		{
			return md5($userId.";ashkjasbdjasbdjlasbdj".$login."time:".$time);
		}
		
	}