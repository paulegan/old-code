<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Class used to access permissions for an application.
 //			Permissions are defined by operations that can be performed
 //			on data sets.
 //
 // Created:		2001/01/16
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.2  2001/01/24 19:53:07  eganp
 // History:		Changed text for existing user error
 // History:
 // History:		Revision 1.1.1.1  2001/01/19 11:49:28  eganp
 // History:		Intial import
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////
 
 require_once("db.php");

 class AppPermissions
 {
  var $classname = 'AppPermissions';
  var $ops = array();
  var $datasets = array();
  var $all = array();
  var $eh;
  var $db;

  function AppPermissions(&$db)
  {
   $this->eh = new errorHandler();
   $this->db = &$db;  
  }
  
  function operations()
  {
   if ($this->ops) return ($this->ops);
   
   $this->db->query("SELECT name FROM perm_operation");

   while ($this->db->next_record())
     $this->ops[] = $this->db->f('name');
   
   return ($this->ops);
  }
  
  function datasets()
  {
   if ($this->datasets) return ($this->datasets);
   
   $this->db->query("SELECT name FROM perm_dataset");

   while ($this->db->next_record())
     $this->datasets[] = $this->db->f('name');
   
   return ($this->datasets);
  }
  
  function usersWithOp($op,$dataset)
  {
   $users = array();
   $datasetClause = $dataset?"AND dataset ='$dataset'":"";
   
   $this->db->query("SELECT DISTINCT user_id,user_name AS FROM perm WHERE operation = '$op' $datasetClause");
   while ($this->db->next_record())
     $users[$this->db->f('user_id')] = $this->db->f('user_name');

   return ($users);
  }
  
  function all($dataset=0)
  {
   $db = &$this->db;
   $all = &$this->all;
   
   if (is_array($dataset))
     $datasetClause = "WHERE dataset IN ( '".join("','",$dataset)."' )";
   else if ($dataset)
     $datasetClause = "WHERE dataset='$dataset'";
   else
     $datasetClause = "";
      
   $db->query("SELECT * FROM perm $datasetClause ORDER by user_name");

   while ($db->next_record()){
     if (!$all[$db->f('user_id')])
       $all[$db->f('user_id')] = array('name'=>$db->f('user_name'),'ops'=>array());
     $all[$db->f('user_id')]['ops'][$this->db->f('operation')][] = $this->db->f('dataset');
   }
   
   return ($all);
  }
  
  function updateUser($userID,$scope,$perms)
  {
   $db = &$this->db;
   
   $db->beginTransaction();
   
   $db->query("SELECT DISTINCT user_name FROM perm WHERE user_id = '$userID'");
   if (!$db->next_record() || !($userName = $db->f('user_name'))){
     $db->rollback();
     $this->eh->halt("Couldn't find $userID in database!");
   }
   
   $db->query("DELETE FROM perm WHERE user_id = '$userID' AND dataset IN ('".join("','",$scope)."')");
   if ($db->num_rows()<1){
     $db->rollback();
     $this->eh->halt("Error when deleted old permissions for $userID!");
   }
   
   foreach ($perms as $op => $datasets)
     foreach ($datasets as $dataset){
       $db->query("INSERT INTO perm VALUES ('$userID','$userName','$op','$dataset')");
       if ($db->num_rows()!=1){
         $db->rollback();
         $this->eh->halt("Unable to insert new permission for $userID, $userName - $op, $dataset !");
       }
     }
   
   return ($db->commit());
  }
  
  function userExists($userID,$scope=0)
  {
   $db = &$this->db;
   $datasetClause = $scope?"AND dataset IN ('".join("','",$scope)."')":"";
   $db->query("SELECT DISTINCT user_id FROM perm WHERE user_id = '$userID' $datasetClause");
   return ($db->next_record());
  }
 
  function id2name($id)
  {
   $db = &$this->db;
   $db->query("SELECT email_id,INITCAP(full_name) AS name FROM all_twin ".
   	"WHERE email_id = '$id' OR cid = '$id'");
   if ($db->next_record())
     return (array($db->f('email_id')?$db->f('email_id'):$id,$db->f('name')));
   else
     return (array());
  }
  
  function verifyNewUsers(&$userText,$scope)
  {
   $userIDs = array();
   $success = true;
   
   foreach (split("\n",$userText) as $line){
     
     if (preg_match("/^\s*$/",$line)) continue;
       
     if (!preg_match("/^([A-Za-z0-9]{3,8})(\s*)(.*)$/",$line,$m))
       return (false);

     list($userID,$name) = $this->id2name(strtoupper($m[1]));
     $userID = $userID?$userID:strtoupper($m[1]);
     $name = $name?$name:($m[3]?$m[3]:'Name Unknown');
     
     if ($this->userExists($userID,$scope)){
       $name .= "  !exists";	// Should probably be i18n'd
       $success = false;
     }
     
     $userText = preg_replace("/".$m[0]."/","$userID $name",$userText);

   }
   
   return ($success);
  }
  
  function addNewUsers(&$userText,$scope,$perms)
  {
   if (!$this->verifyNewUsers($userText,$scope)) return (false);
   
   $db = &$this->db;
   $db->beginTransaction();
   
   foreach (split("\n",$userText) as $line){
     
     if (preg_match("/^\s*$/",$line)) continue;
       
     if (!preg_match("/^([A-Za-z0-9]{3,8})(\s*)(.*)$/",$line,$m)){
       $db->rollback();
       return (false);
     }
     
     $userID = $m[1];
     $userName = $db->add_specialcharacters($m[3]);
     
     foreach ($perms as $op => $datasets)
       foreach ($datasets as $dataset){
         $db->query("INSERT INTO perm VALUES ('$userID','$userName','$op','$dataset')");
         if ($db->num_rows()!=1){
           $db->rollback();
           $this->eh->halt("Unable to insert new permission for $userID, $userName - $op, $dataset !");
         }
     }
   }
   
   $db->commit();
   
   return (true);
  }
  
 }// class AppPermissions

?>
