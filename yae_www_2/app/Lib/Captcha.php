<?php

class Lib_Captcha
{
	static public function generateCode()
	{
		$possible = '23456789bcdfghjkmnpqrstvwxyz';
		$code = '';
		$i = 0;
		while ($i < 6) {
			$code .= substr($possible, mt_rand(0, strlen($possible)-1), 1);
			$i++;
		}
		Tmvc_Model_Session::setSession("captcha", $code);
		return $code;
	}
	
	static public function getCode()
	{
		return Tmvc_Model_Session::getSession("captcha");
	}
	
	static public function renderCaptcha()
	{
		$captchaText = self::generateCode();
		
		//determine width and height for our image and create it
		$imgW = 200;
		$imgH = 50;
		$image = imagecreatetruecolor($imgW, $imgH);
		 
		//setup background color and border color
		$backgr_col = imagecolorallocate($image, 238,239,239);
		$border_col = imagecolorallocate($image, 208,208,208);
		 
		//let's choose color in range of purple color
		$text_col = imagecolorallocate($image, rand(80,100),rand(50,60),rand(70,80));
		 
		//now fill rectangle and draw border
		//imagefilledrectangle($image, 0, 0, $imgW, $imgH, $backgr_col);
		//imagerectangle($image, 0, 0, $imgW-1, $imgH-1, $border_col);
		
		$bgImage = imagecreatefromjpeg(SRC_PATH_RESOURCES . "/captchabg.jpg");
		imagecopy($image, $bgImage, 0, 0, 0, 0, 200, 50);
		imagedestroy($bgImage);
		
		// add some random lines
		$noise_color = imagecolorallocate($image, 130, 120, 120);
		for( $i=0; $i<10; $i++ ) {
			imageline($image, mt_rand(0,$imgW), mt_rand(0,$imgH), mt_rand(0,$imgW), mt_rand(0,$imgH), $noise_color);
		}
		
		//save fonts in same folder where you PHP captcha script is
		//name these fonts by numbers from 1 to 3
		//we shall choose different font each time
		$fonts = array();
		$iterator = new DirectoryIterator(SRC_PATH_RESOURCES . "/fonts/");
		foreach ( $iterator as $font )
		{
			if ( $font->isFile() )
			{
				$fonts[] = $font->getFilename();
			}
		}
		$fn = rand(1,count($fonts)) - 1;
		$font = SRC_PATH_RESOURCES . "/fonts/" . $fonts[$fn];
		 
		//setup captcha letter size and angle of captcha letters
		$font_size = $imgH / 2.2;
		$box = imagettfbbox($font_size, 0, $font, $captchaText);
		$startX = (int)($imgW - $box[4]-30) / 2;
		$y = (int)($imgH - $box[5]) / 2;
		for ( $i=0;$i<6;$i++ )
		{
			$angle = rand(-15,15);
			$x = $startX + (($box[4]/6) + 5)*$i;
			imagettftext($image, $font_size, $angle, $x, $y, $text_col, $font, $captchaText[$i]);
		}
		 
		//now we should output captcha image
		header("Content-type: image/png");
		imagepng($image);
		imagedestroy ($image);
	}
}