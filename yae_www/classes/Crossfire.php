<?php

class Crossfire
{
	static protected $sem;
	
	static public function getIDs($nick)
	{
		self::blockForCFRefresh();
		$cf = curl_init();
		curl_setopt($cf, CURLOPT_URL, "http://www.crossfire.nu/ajax/test.php?json=true&input=".$nick);
		curl_setopt($cf, CURLOPT_RETURNTRANSFER, 1);
		$page = curl_exec($cf);
		curl_close($cf);
		$page = str_replace(',]',']',$page);
		$result = json_decode($page,true);
		$data = array();
		if(is_array($result['results']))
		{
			foreach($result['results'] as $row)
			{
				$data[$row['value']] = $row['id'];
			}
		}
		return $data;
	}
	static public function checkCFID($cfid)
	{
		self::blockForCFRefresh();
		$cfid = 0+$cfid+0;
		$cf = curl_init();
		curl_setopt($cf, CURLOPT_URL, "http://www.crossfire.nu/?x=user&mode=view&id=".$cfid);
		curl_setopt($cf, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($cf, CURLOPT_INTERFACE, 'eth0:0');
		$page = curl_exec($cf);
		curl_close($cf);
		$pattern = '/<tr>[\r\n\t ]{1,50}<td class="SexyBoldFont">Website:<\/td>[\r\n\t ]{1,50}<td><a href="(.*)" target="_blank">(.*)<\/a><\/td>[\r\n\t ]{1,50}<\/tr>/';
		if(preg_match($pattern,$page,$matches))
		{
			$address = $matches[1];
			$pattern = '/^http:\/\/(.*)$/';
			if(preg_match($pattern,$address,$matches))
			{
				$hash = trim($matches[1]);
				return $hash;
			}
		}
		return '';
	}
	
	static public function sendPM($cfid,$subject,$body)
	{
		self::blockForCFRefresh();
		$cf = curl_init();
		curl_setopt($cf, CURLOPT_URL, "http://www.crossfire.nu/?x=message&mode=compose");
		curl_setopt($cf, CURLOPT_RETURNTRANSFER, 1);
		//curl_setopt($cf, CURLOPT_COOKIE, "xfire_session=e0c833d70983f239161fbc4300c6ec54");//trakos
		curl_setopt($cf, CURLOPT_COOKIE, "xfire_session=5c6568c92e6996aba2b60666dd309123");
		curl_setopt($cf, CURLOPT_HEADER, true);
		curl_setopt($cf, CURLOPT_COOKIESESSION, true);
		$page = curl_exec($cf);
		$pattern = '/<input type="hidden" value="([A-F0-9]{40})" name="confirmcode" \/>/';
		preg_match($pattern, $page, $matches);
		//print_r($matches);
		$confirmcode = $matches[1];
		//echo $confirmcode;
		curl_close($cf);
	
		$pattern = "/Set-Cookie: PHPSESSID=([a-z0-9]*); path=(.*)/";
		if(preg_match($pattern,$page,$matches))
		{
			$sessid = $matches[1];
			$path = $matches[2];
		}	
		//echo $sessid;
		//echo $path;
		//die();
		$cf = curl_init();
        curl_setopt($cf, CURLOPT_URL, "http://www.crossfire.nu/?x=message&mode=compose");
        curl_setopt($cf, CURLOPT_RETURNTRANSFER, 1);
		curl_setopt($cf, CURLOPT_REFERER, "http://www.crossfire.nu/?x=message&mode=compose");
        $id = 13943;
		$post = array
        (
				'userIDAS'      => $cfid,
				'subject'       => $subject,
				'body'          => $body,
				'confirmcode'	=> $confirmcode
		);
		curl_setopt($cf, CURLOPT_POSTFIELDS, $post);
        curl_setopt($cf, CURLOPT_COOKIE, "xfire_session=5c6568c92e6996aba2b60666dd309123; PHPSESSID=$sessid");
        $page = curl_exec($cf);
        curl_close($cf);
		echo $page;
	}
	
	static public function blockForCFRefresh()
	{
		self::$sem = sem_get(12345);
		sem_acquire(self::$sem);
		register_shutdown_function("Crossfire::unblockCFRefresh");
	}
	
	static public function unblockCFRefresh()
	{
		sleep(5);
		sem_release(self::$sem);
	}
}
?>