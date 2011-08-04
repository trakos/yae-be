<?php


/**
 * Basic timing class with array of times (possibly associative, if we use key arguments
 */
class Tmvc_Model_Timer
{
	protected $_startTime = 0;
	protected $_currentlyTiming = "";
	public $times = array();
	
	/**
	 * 
	 * if paused_or_key is string, then we are starting counting under the 'key' position in array, otherwise
	 * if it is true we are not starting timer
	 * if false or not set we are counting under the '0' position
	 * @param unknown_type $pausedOrKey
	 */
	public function __construct($pausedOrKey=false)
	{
		if( is_string($pausedOrKey) )
		{
			$this->start($pausedOrKey);
		}
		else if( !$pausedOrKey )
		{
			$this->start();
		}
	}
	
	public function start( $key = "")
	{
		$this->stop();
		$this->_startTime = microtime(true);
		$this->_currentlyTiming = $key;		
	}
	
	public function stop()
	{
		if( $this->_startTime )
		{
			$time = microtime(true) - $this->_startTime;
			$this->_startTime = 0;
			if( $this->_currentlyTiming )
			{
				$this->times[$this->_currentlyTiming] = $time;
			}
			else
			{
				$this->times[] = $time;
			}
		}
	}

	public function getTotal()
	{
		$this->stop();
		return $this->round( array_sum($this->times) );
	}
	
	public function count()
	{
		return count($this->times);		
	}

	public function getTimes()
	{
		return $this->times;
	}

	public function __toString()
	{
		$s="";
		foreach( $this->times as $key => $time )
		{
			$s.=sprintf("%20s: %f\n", $key, $time);
		}
		return $s;
	}
	
	protected function round( $number )
	{
		return round( $number *10000 )/10000;
	}
	
}
