<?php
  define('HRIS_APP_TCTS_PREFIX'  ,"TC");
  define('HRIS_APP_TCTS_BATCH_PREFIX'  ,"BT");  
  define('HRIS_APP_TCTS_OPEN'    ,"O");
  define('HRIS_APP_TCTS_CLOSED'  ,"C");
  define('HRIS_APP_TCTS_DELETED' ,"D");
  define('HRIS_APP_TCTS_REJECTED',"R");
  define('HRIS_APP_TCTS_START_YEAR',"1999");
  define('HRIS_APP_TCTS_INTEGRATION',"I");
  define('HRIS_APP_TCTS_SEARCHRESULTS',"20");
  define('HRIS_APP_TCTS_ADMINGROUP',"TCTS_Admin");
  define('HRIS_APP_TCTS_DEFAULT_CENTER','GBR');
  define('HRIS_APP_TCTS_DEFAULT_SENDING','F');
  define('HRIS_APP_TCTS_URL',"http://".GetEnv("HTTP_HOST")."/transactionCentre/tcts");
  
  /* This class represents one transacation and provides 
     methods to change that transaction.
   */
  class Transaction
  {
    var $table = "transactions_tb";
    
    function isValidTransID($id)
    {
      $db = new DB_tcts;
      $prefix = HRIS_APP_TCTS_PREFIX;
      
      if (ereg("^[A-Za-z]+[0-9]+$",$id))
      {
        $db->query("SELECT trans_id from $this->table where trans_ID='$id'");
	return $db->next_record();
      }
      else return false;     
    } // function isValidTransID

    function close($transID,$closedBy)
    {
      if ($this->isValidTransID($transID) && hris_isCID($closedBy))
      {
        $db = new DB_tcts;
	$status = HRIS_APP_TCTS_CLOSED;
	$date = date("d-m-Y-H-i");
	$db->query("UPDATE $this->table set CLOSED_BY='$closedBy', status = '$status', close_date = TO_DATE('$date','DD-MM-YYYY-HH24-MI') where trans_Id = '$transID'");
	
	return true;
      }
      else return false;
    } // function close
     
    function modifyCID($transID,$cid)
    {
      $db = new DB_tcts;          
     
      $db->query("UPDATE $this->table set emp_cid = '$cid' where trans_id='$transID'");     
    } // function modifyCID      
           
    function reject($transID,$closedBy,$reason)
    {
      if ($this->isValidTransID($transID) && hris_isCID($closedBy))
      {
        $rc = new RejectionCodes;
	$center = $this->getCenter($transID);
        if ($rc->isValidCode($reason,$center))
	{
          $db = new DB_tcts;
	  $status = HRIS_APP_TCTS_REJECTED;
	  $date = date("d-m-Y-H-i");
	  $db->query("UPDATE $this->table set CLOSED_BY='$closedBy', status = '$status', close_date = TO_DATE('$date','DD-MM-YYYY-HH24-MI'),REJECT_REASON='$reason' where trans_Id = '$transID'");
	  return true;
	}
	else return false;
      }
      else return false;
    } // function close
   
    function delete($transID,$deletedBy)
    {
      if ($this->isValidTransID($transID) && hris_isCID($deletedBy))
      {
         $db = new DB_tcts;
	 $status = HRIS_APP_TCTS_DELETED;
	 $date = date("d-m-Y-H-i");
	 $db->query("UPDATE $this->table set CLOSED_BY='$deletedBy', status = '$status', close_date = TO_DATE('$date','DD-MM-YYYY-HH24-MI') where trans_Id = '$transID'");
	 return true;
      }
      else return false;
    } // function delete
    
    function details($transID)
    {
      if ($this->isValidTransID($transID))
      {
        $center = $this->getCenter($transID);
        $db = new DB_tcts;
	$ad = new Admin;
	$tc = new TransactionCodes;
	$cc = new CenterCodes;
	$rc = new RejectionCodes;
	$details;

	$db->query("SELECT TRANS_ID,TO_CHAR(TRANS_DATE,'DD') AS DAY,TO_CHAR(TRANS_DATE,'MM') AS MTH,TO_CHAR(TRANS_DATE,'YYYY') AS YEAR,TO_CHAR(TRANS_DATE,'DD-MM-YYYY') AS TRANS_DATE,HR_ADMIN,EMP_CID,STATUS,TRANS_TYPE,TRANS_CENTER,TO_CHAR(CLOSE_DATE,'DD-MM-YYYY') AS CLOSE_DATE,CLOSED_BY,REJECT_REASON, TO_CHAR(APPROVED_DATE,'DD') AS APPROVED_DAY,TO_CHAR(APPROVED_DATE,'MM') AS APPROVED_MTH,TO_CHAR(APPROVED_DATE,'YYYY') AS APPROVED_YEAR,TO_CHAR(APPROVED_DATE,'DD-MM-YYYY') AS APPROVED_DATE, TO_CHAR(EFFECTIVE_DATE,'DD') AS EFFECTIVE_DAY,TO_CHAR(EFFECTIVE_DATE,'MM') AS EFFECTIVE_MTH,TO_CHAR(EFFECTIVE_DATE,'YYYY') AS EFFECTIVE_YEAR,TO_CHAR(EFFECTIVE_DATE,'DD-MM-YYYY') AS EFFECTIVE_DATE from $this->table where trans_id='$transID'");
	if ($db->next_record())
	{ 
	  $status = $db->f("status");
	  if ($status == "O") $status = "Open"; 
	  if ($status == "C") $status = "Closed"; 
	  if ($status == "R") $status = "Rejected"; 
	  if ($status == "D") $status = "Deleted"; 

	  $closedby = "";
          if (hris_isCID($db->f("closed_by")))
             $closedby = hris_CIDToName($db->f("closed_by"));

          $center = $db->f("trans_center");
	  if (hris_looksLikeCID($db->f("emp_cid")))
	  {
	    $details = array (
	                "trans_id"	   => $db->f("trans_id"),
			"day"		   => $db->f("day"),
			"mth"		   => $db->f("mth"),
			"year"		   => $db->f("year"),						
			"trans_date"	   => $db->f("trans_date"),
			"adminCID"	   => $db->f("hr_admin"),
			"hr_admin"	   => $db->f("hr_admin")." - ".$ad->getName($db->f("hr_admin"),$center),			
			"emp_cid"	   => $db->f("emp_cid"),
			"emp_name" 	   => hris_CIDToName($db->f("emp_cid")),  
	  		"trans_type"	   => $tc->getName($db->f("trans_type"),$center),
			"trans_type_code"  => $db->f("trans_type"),
			"trans_center"	   => $cc->getName($db->f("trans_center"),$center),
			"trans_center_code"=> $db->f("trans_center"),
	  		"status"	   => $status,
	  		"close_date"	   => $db->f("close_date"),
	  		"closed_by"	   => $db->f("closed_by"),
			"closed_by_name"   => $closedby,
	  		"reject_reason"    => $rc->getName($db->f("reject_reason"),$center),
			"approved_day"	   => $db->f("approved_day"),
			"approved_mth"	   => $db->f("approved_mth"),
			"approved_year"	   => $db->f("approved_year"),						
			"approved_date"	   => $db->f("approved_date"),
			"effective_day"	   => $db->f("effective_day"),
			"effective_mth"	   => $db->f("effective_mth"),
			"effective_year"   => $db->f("effective_year"),						
			"effective_date"   => $db->f("effective_date")
			   );
	  }
	  else {
  	     $details = array (
		        "trans_id"	   => $db->f("trans_id"),
			"day"		   => $db->f("day"),
			"mth"		   => $db->f("mth"),
			"year"		   => $db->f("year"),						
			"trans_date"	   => $db->f("trans_date"),
			"trans_center"     => $cc->getName($db->f("trans_center"),$center),
			"trans_center_code"=> $db->f("trans_center"),				
			"adminCID"	   => $db->f("hr_admin"),
			"hr_admin"	   => $db->f("hr_admin")." - ".$ad->getName($db->f("hr_admin"),$center),			
			"emp_cid"	   => $db->f("emp_cid"),
			"emp_name" 	   => "",  
		  	"trans_type"	   => $tc->getName($db->f("trans_type"),$center),
		  	"status"	   => $status,
		  	"close_date"	   => $db->f("close_date"),
		  	"closed_by"	   => $db->f("closed_by"),
			"closed_by_name"   => $closedby,
		  	"reject_reason"    => $rc->getName($db->f("reject_reason"),$center),
			"approved_day"	   => $db->f("approved_day"),
			"approved_mth"	   => $db->f("approved_mth"),
			"approved_year"	   => $db->f("approved_year"),						
			"approved_date"	   => $db->f("approved_date"),
			"effective_day"	   => $db->f("effective_day"),
			"effective_mth"	   => $db->f("effective_mth"),
			"effective_year"   => $db->f("effective_year"),						
			"effective_date"   => $db->f("effective_date")
			   );
	       }	  
	  #$db->disconnect();		   
	   return($details);
	}
	else hris_internalError("Trans ID $transID not found in $this->table");		
      }
      else hris_internalError("Invalid Trans ID $transID"); 
     
    }
    
    function getCenter($transID)
    {
      $db = new DB_tcts;
      $db->query("select trans_center from transactions where trans_id = '$transID'");
      $db->next_record();
      return $db->f("trans_center");
    }
  } // class Transaction

?>
