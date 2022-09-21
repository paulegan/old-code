<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	TCTS extension to standard pageTemplate class.  Includes
 //			a few methods for standard TCTS substitutions and 
 //			locale determination.
 // 
 // Created:		2001/01/02
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.2  2001/01/24 19:27:10  eganp
 // History:		Center list now removed if only 1 entry
 // History:		parseList now handles list keys correctly
 // History:
 // History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 // History:		Initial import of TCTS3.
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////
 					
 require_once("pageTemplate.php");
 
 class Page extends PageTemplate
 {
  var $classname = 'Page';
  var $context;
  
  function Page($page,&$context)
  {
   global $CENTER_2_LOCALE;
   
   $this->PageTemplate($page,$CENTER_2_LOCALE[$context['center']]);
   $this->context = &$context;
   $this->parseAppMenu();
  }

  function parseAppMenu()
  {
   global $UPDATE_OPS;
   
   // All update ops are mapped to 'update' for lookup
   if (in_array($this->context['form']['_op'],$UPDATE_OPS))
     $op = 'update';
   else
     $op = $this->context['form']['_op'];

   // Show all centers that this user can perform this operation on.
   $centers = $this->context['userPerms']->datasets($op);
   if (count($centers)>1)
     $this->parseList('_center',$centers,$this->context['center']);
   else
     $this->remove_block('_center_select');

   // Update & delete are covered by the view menu item
   if ($op=='update' || $op=='delete') $op = 'view';
   $opList = array_delete(array('update','delete'),
   	$this->context['userPerms']->operations($this->context['center']));
   // Show all ops available for this center (except delete & update)
   $this->parseList('_op',$opList,$op);

  }
  
  function parseList($name,$list,$selected,$showKey=1)
  {
   foreach ($list as $key => $item){
     $this->set_var($name."_value",notIndex($key)?$key:$item);
     $this->set_var($name."_name",$item.(($showKey && notIndex($key))?(" (".$key.")"):""));
     $this->set_var($name."_selected",(($selected==(notIndex($key)?$key:$item))?'selected="selected"':''));
     $this->parse_block($name."_options");
   }
  }
  
 }// class Page

?>
