<?php
	class Tmvc_View_Form
	{
		protected $_inputSections = array();
		protected $_buttonSections = array();
		protected $_sectionsNames = array();
		protected $_cssClass = null;
		protected $_method = "POST";
		protected $_action = "";
		protected $_id = "";
		
		public function __construct($id=null)
		{
			if ( $id )
			{
				$this->_id = $id;
			}
			else
			{
				$this->_id = uniqid("form_");
			}
		}
		
		public function addInputSection( $sectionKey, $sectionName )
		{
			$this->_sectionsNames[$sectionKey] = $sectionName;
			$this->_inputSections[$sectionKey] = array();
			$this->_buttonSections[$sectionKey] = array();
		}
		
		
		/**
		 * @param Tmvc_View_Form_Input_Abstract $input
		 * @param unknown_type $sectionKey
		 * @return Tmvc_View_Form_Input_Abstract
		 */
		public function addInput( Tmvc_View_Form_Input_Abstract $input, $sectionKey=null )
		{
			if ( !$sectionKey )
			{
				if ( empty($this->_inputSections) )
				{
					throw new Tmvc_Controller_Exception(E_FORM_NO_SECTION_ADDED, 'No form section previously added!');
				}
				$sectionKey = array_pop(array_keys($this->_sectionsNames));
			}
			$this->_inputSections[$sectionKey][] = $input;
			return $input;
		}
		
		public function addButton( Tmvc_View_Form_Button_Abstract $button, $sectionKey=null )
		{
			if ( !$sectionKey )
			{
				if ( empty($this->_inputSections) )
				{
					throw new Tmvc_Controller_Exception(E_FORM_NO_SECTION_ADDED, 'No form section previously added!');
				}
				$sectionKey = array_pop(array_keys($this->_sectionsNames));
			}
			$this->_buttonSections[$sectionKey][] = $button;
		}
		
		public function getInput( $sectionKey, $inputName )
		{
			if ( isset($this->_inputSections[$sectionKey]) )
			{
				foreach( $this->_inputSections[$sectionKey] as $input ) 
				{
					if ( $input->getName() == $inputName )
					{
						return $input;
					}
				}
			}
			return null;
		}
		
		public function setCssClass($className)
		{
			$this->_cssClass = $className;
		}
		
		protected function _renderSectionStart($sectionKey, $sectionName)
		{
			echo "<tr><th colspan='2'>$sectionName</th></tr>";
		}
		
		protected function _renderSectionEnd($sectionKey, $sectionName)
		{
		}
		
		public function render()
		{
			echo "<form action='$this->_action' method='$this->_method' id='$this->_id'>";
			foreach ( $this->_inputSections as $key => $inputs )
			{
				foreach ( $inputs as $input )
				{
					if ( $input instanceof Tmvc_View_Form_Input_Hidden )
					{
						$input->render($this->_id);
					}
				}
			}
			echo "<table class='$this->_cssClass'>";
			foreach ( $this->_inputSections as $key => $inputs )
			{
				$this->_renderSectionStart($key, $this->_sectionsNames[$key]);
				foreach ( $inputs as $input )
				{
					if ( !$input instanceof Tmvc_View_Form_Input_Hidden )
					{
						$input->render($this->_id);
					}
				}
				if ( !empty($this->_buttonSections[$key]) )
				{
					echo '<tr><td></td><td class="buttons">';
					foreach ( $this->_buttonSections[$key] as $button )
					{
						$button->render($this->_id);
					}
					echo '</td></tr>';
				}
				$this->_renderSectionEnd($key, $this->_sectionsNames[$key]);
			}
			echo '</table>';
			echo "</form>";
		}
		
		public function validate()
		{
			$valid = true;
			$values = $this->getValues();
			foreach ( $this->_inputSections as $key => $inputs )
			{
				foreach ( $inputs as $input )
				{
					$valid = $input->validate($values) && $valid;
				}
			}
			return $valid;
		}
		
		public function setMethod($method)
		{
			$this->_method = $method;
		}
		
		public function setAction($action)
		{
			$this->_action = $action;
		}
		
		public function getValues( )
		{
			$values = array();
			foreach ( $this->_inputSections as $key => $inputs )
			{
				foreach ( $inputs as $input )
				{
					$values[$input->getName()] = $input->getValue();
				}
			}
			return $values;			
		}
		
		public function setValues( $values )
		{
			foreach ( $this->_inputSections as $key => $inputs )
			{
				foreach ( $inputs as $input )
				{
					if ( isset($values[$input->getName()]) )
					{
						$input->setValue($values[$input->getName()]);
					}
				}
			}
		}
	}