<?php

class Tmvc_Model_Mysql_BaseSelect
{
	protected $_queryBase;
	protected $_selectArguments = array();
	protected $_groups = array();
	protected $_groupArguments = array();
	protected $_wheres = array();
	protected $_whereArguments = array();
	protected $_havings = array();
	protected $_havingArguments = array();
	protected $_orders = array();
	protected $_orderArguments = array();
	protected $_limitOffset = 0;
	protected $_limitSize = 0;
	/**
	 * @var Tmvc_Model_Mysql
	 */
	protected $_dbInstance = null;
	
	public function __construct( Tmvc_Model_Mysql $dbInstance, $queryBase = '', $selectArguments = array())
	{
		$this->_dbInstance = $dbInstance;
		$this->_queryBase = $queryBase;
		$this->_selectArguments = $selectArguments;
	}
	
	/**
	 * @param string $queryBase
	 * @param array $selectArguments
	 * @param array $groupArguments
	 * @return Tmvc_Model_Mysql_BaseSelect
	 */
	public function setQueryBase( $queryBase, $selectArguments = array() )
	{
		$this->_queryBase = $queryBase;
		$this->_selectArguments = $selectArguments;
		$this->_groupArguments = $groupArguments;
		return $this;
	}
	
	/**
	 * @param string $where
	 * @param array $arguments
	 * @return Tmvc_Model_Mysql_BaseSelect
	 */
	public function where($where, $arguments = array())
	{
		$this->_wheres[] = $where;
		$this->_whereArguments = array_merge($this->_whereArguments, $arguments);
		return $this;
	}
	
	/**
	 * @return Tmvc_Model_Mysql_BaseSelect
	 */
	public function resetWhere()
	{
		$this->_wheres = array();
		$this->_whereArguments = array();
		return $this;
	}
	
	/**
	 * @param string $group
	 * @param array $arguments
	 * @return Tmvc_Model_Mysql_BaseSelect
	 */
	public function group($group, $arguments = array())
	{
		$this->_groups[] = $group;
		$this->_groupArguments = array_merge($this->_orderArguments, $arguments);
		return $this;
	}
	
	/**
	 * @return Tmvc_Model_Mysql_BaseSelect
	 */
	public function resetGroup()
	{
		$this->_groups = array();
		$this->_groupArguments = array();
		return $this;
	}
	
	/**
	 * @param string $having
	 * @param array $arguments
	 * @return Tmvc_Model_Mysql_BaseSelect
	 */
	public function having($having, $arguments = array())
	{
		$this->_havings[] = $having;
		$this->_havingArguments = array_merge($this->_havingArguments, $arguments);
		return $this;
	}
	
	/**
	 * @return Tmvc_Model_Mysql_BaseSelect
	 */
	public function resetHaving()
	{
		$this->_havings = array();
		$this->_havingArguments = array();
		return $this;
	}
	
	/**
	 * @param string $order
	 * @param array $arguments
	 * @return Tmvc_Model_Mysql_BaseSelect
	 */
	public function order($order, $arguments = array())
	{
		$this->_orders[] = $order;
		$this->_orderArguments = array_merge($this->_orderArguments, $arguments);
		return $this;
	}
	
	/**
	 * @return Tmvc_Model_Mysql_BaseSelect
	 */
	public function resetOrder()
	{
		$this->_orders = array();
		$this->_orderArguments = array();
		return $this;
	}
	
	/**
	 * @param int $offset
	 * @param int $size
	 * @return Tmvc_Model_Mysql_BaseSelect
	 */
	public function limit($offset, $size)
	{
		$this->_limitOffset = $offset;
		$this->_limitSize = $size;
		return $this;
	}
	
	/**
	 * @param int $number
	 * @param int $perPage
	 * @return Tmvc_Model_Mysql_BaseSelect
	 */
	public function page($number, $perPage)
	{
		$this->_limitOffset = $perPage*($number-1);
		$this->_limitSize = $perPage;
		return $this;
	}
	
	public function toString()
	{
		if ( empty($this->_queryBase) )
		{
			throw new Tmvc_Model_Mysql_Exception(E_MYSQL, "No query base given to the baseSelect, and conversion to string occured!");
		}
		$where = $this->_getWhere();
		$group = $this->_getGroupBy();
		$having = $this->_getHaving();
		$order = $this->_getOrderBy();
		$limit = $this->_getLimit();
		$arguments = array_merge($this->_selectArguments, $this->_whereArguments, $this->_groupArguments, $this->_havingArguments, $this->_orderArguments );
		$query = $this->_queryBase." $where $group $having $order $limit";
		return $this->_dbInstance->parseQuery($query, $arguments);
	}
	
	public function getAll()
	{
		return $this->_dbInstance->getAll($this->toString());
	}
	
	public function getAllEnumerate()
	{
		return $this->_dbInstance->getAllEnumerate($this->toString());
	}
	
	public function getCol()
	{
		return $this->_dbInstance->getCol($this->toString());
	}
	
	public function getOne()
	{
		return $this->_dbInstance->getOne($this->toString());
	}
	
	public function getRow()
	{
		return $this->_dbInstance->getRow($this->toString());
	}
	
	protected function _getWhere()
	{
		if ( empty($this->_wheres) ) return "";
		return 'WHERE 1 '.implode(" ", $this->_wheres).'';
	}
	
	protected function _getGroupBy()
	{
		if ( !$this->_groups ) return '';
		return 'GROUP BY '.implode(",", $this->_groups);
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
