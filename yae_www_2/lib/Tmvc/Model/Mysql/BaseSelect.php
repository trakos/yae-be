<?php

class Tmvc_Model_Mysql_BaseSelect
{
	protected $_queryBase;
	protected $_selectArguments = array();
	protected $_groupArguments = array();
	protected $_wheres;
	protected $_whereArguments = array();
	protected $_havings;
	protected $_havingArguments = array();
	protected $_orders;
	protected $_orderArguments = array();
	protected $_limitOffset = 0;
	protected $_limitSize = 0;
	protected $_dbInstance = null;
	
	public function __construct( Tmvc_Model_Mysql $dbInstance, $queryBase = '', $selectArguments = array(), $groupArguments = array() )
	{
		$this->_dbInstance = $dbInstance;
		$this->_queryBase = $queryBase;
		$this->_selectArguments = $selectArguments;
		$this->_groupArguments = $groupArguments;
	}
	
	public function setQueryBase( $queryBase, $selectArguments = array(), $groupArguments = array() )
	{
		$this->_queryBase = $queryBase;
		$this->_selectArguments = $selectArguments;
		$this->_groupArguments = $groupArguments;
	}
	
	public function where($where, $arguments = array())
	{
		$this->_wheres[] = $where;
		$this->_whereArguments = array_merge($this->_whereArguments, $arguments);
	}
	
	public function resetWhere()
	{
		$this->_wheres = array();
		$this->_whereArguments = array();
	}
	
	public function having($having, $arguments = array())
	{
		$this->_havings[] = $having;
		$this->_havingArguments = array_merge($this->_havingArguments, $arguments);
	}
	
	public function resetHaving()
	{
		$this->_havings = array();
		$this->_havingArguments = array();
	}
	
	public function order($order, $arguments = array())
	{
		$this->_orders[] = $order;
		$this->_orderArguments = array_merge($this->_orderArguments, $arguments);
	}
	
	public function limit($offset, $size)
	{
		$this->_limitOffset = $offset;
		$this->_limitSize = $size;
	}
	
	public function page($number, $perPage)
	{
		$this->_limitOffset = $perPage*($number-1);
		$this->_limitSize = $perPage;
	}
	
	public function resetOrder()
	{
		$this->_orders = array();
		$this->_orderArguments = array();
	}
	
	public function __toString()
	{
		if ( empty($this->_queryBase) )
		{
			throw new Tmvc_Model_Mysql_Exception(E_MYSQL, "No query base given to the baseSelect, and conversion to string occured!");
		}
		$where = $this->_getWhere();
		$having = $this->_getHaving();
		$order = $this->_getOrderBy();
		$limit = $this->_getLimit();
		$arguments = array_merge($this->_selectArguments, $this->_whereArguments, $this->_groupArguments, $this->_havingArguments, $this->_orderArguments );
		$query = sprintf($this->_queryBase, $where)." $having $order $limit";
		return $this->_dbInstance->parseQuery($query, $arguments);
	}
	
	public function getAll()
	{
		return $this->_dbInstance->getAll((string)$this);
	}
	
	public function getAllEnumerate()
	{
		return $this->_dbInstance->getAllEnumerate((string)$this);
	}
	
	public function getCol()
	{
		return $this->_dbInstance->getCol((string)$this);
	}
	
	public function getOne()
	{
		return $this->_dbInstance->getOne((string)$this);
	}
	
	public function getRow()
	{
		return $this->_dbInstance->getRow((string)$this);
	}
	
	protected function _getWhere()
	{
		if ( empty($this->_wheres) ) return "";
		return 'WHERE 1 '.implode(" ", $this->_wheres).'';
	}
	
	protected function _getHaving()
	{
		if ( empty($this->_havings) ) return "";
		return 'HAVING 1 AND ('.implode(" ", $this->_havings).')';
	}
	
	protected function _getOrderBy()
	{
		if ( empty($this->_orders) ) return "";
		return 'ORDER BY '.implode(",", $this->_orders);
	}
	
	protected function _getLimit()
	{
		if ( !$this->_limitSize ) return '';
		return "LIMIT $this->_limitOffset,$this->_limitSize";
	}
}
