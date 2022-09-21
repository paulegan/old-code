<?php

 class ColumnSpec
 {
  var $name;
  var $formName;
  var $canBeNull;
  var $type;
  var $length;
  var $validateOnClient;
  var $title;
  var $display;
  var $munge;
  
  function ColumnSpec($name,$formName,$canBeNull,$type,$length,$validateOnClient,$title,$display,$munge)
  {
   $this->name = $name || 'dummy';
   $this->formName = $formName || 'dummy';
   $this->canBeNull = $canBeNull;
   $this->type = $type || 'text';
   $this->length = $length || 1;
   $this->validateOnClient = $validateOnClient;
   $this->title = $title || 'dummy';
   $this->display = $display;
   $this->munge = $munge;
  }
 }
 
 class Tab
 {
  var $name;
  var $title;
  var $writeBody;
  var $acronym;
  
  function Tab($name,$title,$acronym,$writeBody)
  {
   $this->name = $name || 'dummy';
   $this->title = $title || 'Dummy';
   $this->writeBody = $writeBody || 0;
   $this->acronym = $acronym || 'D';
  }// Tab

 }
?>
