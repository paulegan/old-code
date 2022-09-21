<?
  
  /*
	This logs the insertion of a transaction, NO validation is
	none on the values passed to this function as this function
	is only to be run directly before inserting a transaction.
	
	However batchID is checked to see if this is an attempt to
	insert a previously existing set of transactions.
   */
  function log_insertion($batchID,$numOfLines,$transDate,$hrAdmin,$center)
  {
     $db = new DB_tcts;
     
     if (!ereg(HRIS_APP_TCTS_BATCH_PREFIX."[0-9]+",$batchID))
        hris_internalError("The batch ID ($batchID)supplied is not valid");
     
     $db->query("select batch_id from transactions_audit_tb where batch_id = '$batchID'");
     
     if ($db->next_record())
        hris_printError("This appears to be an attempt to duplicate an existing set of transactions (set $batchID)");
	
     $user = GetEnv("REMOTE_USER");
     $ip = GetEnv("REMOTE_ADDR");        
     $insertionDate = date("d-m-Y-H:i:s");
     
     $db->query("insert into transactions_audit_tb values ('$batchID','$user','$ip',TO_DATE('$insertionDate','DD-MM-YYYY-HH24:MI:SS'),'$numOfLines',TO_DATE('$transDate','DD-MM-YYYY'),'$hrAdmin','$center')");     
  } // log_insertion
?>
