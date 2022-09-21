<?php



/*
   This class provides the methods required to insert 
   data into the transactions table.
 */
class TransactionTable extends Transaction
{
  
   function insert($cid,$adminCID,$type,$day,$mth,$year,$approved_day,$approved_mth,$approved_year,$effective_day,$effective_mth,$effective_year,$center,$prevTransID="")
   {
     $db = new DB_tcts;          
     
     $db->query("SELECT Transaction_ID_SEQ.nextVal as next_ref from DUAL");
     if (!$db->next_record())
        hris_internalError("No value returned for the next value in the sequence");
	
     $transID = HRIS_APP_TCTS_PREFIX . $db->f("next_ref");
     $status  = HRIS_APP_TCTS_OPEN;
     if ($prevTransID == "")
        $db->query("INSERT into $this->table values ('$transID',TO_DATE('$day-$mth-$year','DD-MM-YYYY'),'$cid','$adminCID',NULL,NULL,'$type','$center','$status',NULL,NULL,NULL,NULL,          TO_DATE('$approved_day-$approved_mth-$approved_year','DD-MM-YYYY'),TO_DATE('$effective_day-$effective_mth-$effective_year','DD-MM-YYYY'))");
     else
        $db->query("INSERT into $this->table values ('$transID',TO_DATE('$day-$mth-$year','DD-MM-YYYY'),'$cid','$adminCID',NULL,NULL,'$type','$center','$status',NULL,NULL,NULL,'$prevTransID',TO_DATE('$approved_day-$approved_mth-$approved_year','DD-MM-YYYY'),TO_DATE('$effective_day-$effective_mth-$effective_year','DD-MM-YYYY'))");

     return $transID;
     
   } // function insert

   /*
      Check if the date given is valid
    */
   function isValidDate($day,$mth,$year)
   {
     if (!checkdate($mth,$day,$year))
       return false;
     else {
            if ($year < HRIS_APP_TCTS_START_YEAR)
	       return false;
	    
	    $givenDate = mktime(0,0,0,$mth,$day,$year);
	    $today = mktime(0,0,0);
	    
	    if ($givenDate > $today)
	       return false;
          }
	  
     return true;  
   } // function isValidDate
    
    function reopen($prevTransID)
    {
      $details = $this->details($prevTransID);
      $transID = $this->insert($details["emp_cid"],$details["adminCID"],$details["trans_type_code"],$details["day"],$details["mth"],$details["year"],$details["approved_day"],$details["approved_mth"],$details["approved_year"],$details["effective_day"],$details["effective_mth"],$details["effective_year"],$details["trans_center_code"],$prevTransID);
      
      return $transID;
    }
      
  function getEmployeeList($center)
  {
    $db = new DB_tcts;
    $status = HRIS_APP_TCTS_OPEN;
    $db->query("select distinct(emp_cid) as cid from $this->table where status='$status' and trans_center='$center' ORDER BY CID");
    $counter = 0;
    $codes;
    
    while ($db->next_record())
    {
      $codes[$counter] = $db->f("cid");
      $counter++; 
    }
    
    if ($counter == 0)
       hris_internalError("No employees in the $this->table table");
       
    return $codes;
  } // function getCodes
} // class TransactionTable 
