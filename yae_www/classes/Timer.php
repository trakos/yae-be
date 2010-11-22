<?php


// basic timing class with array of times (possibly associative, if we use key arguments
class Timer
{
	protected $start_time = 0;
	protected $currently_timing = "";
	public $times = array();

	// if paused_or_key is string, then we are starting counting under the 'key' position in array, otherwise
	// if it is true we are not starting timer
	// if false or not set we are counting under the '0' position
	public function __construct($paused_or_key=false)
	{
		if( is_string($paused_or_key) )
		{
			$this->start($paused_or_key);
		}
		else if( !$paused_or_key )
		{
			$this->start();
		}
	}
	
	public function start( $key = "")
	{
		$this->stop();
		$this->start_time = microtime(true);
		$this->currently_timing = $key;		
	}
	
	public function stop()
	{
		if( $this->start_time )
		{
			$time = microtime(true) - $this->start_time;
			$this->start_time = 0;
			if( $this->currently_timing )
			{
				$this->times[$this->currently_timing] = $time;
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
