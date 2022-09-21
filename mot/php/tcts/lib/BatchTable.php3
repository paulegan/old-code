<?php

/*
   This class provides the methods required to insert 
   data into the batch table.
 */
class BatchTable extends Transaction
{
   var $table = "batches_tb";
   var $transactionTable = "transactions_tb";
        
   function generateBatchID()
   {
     $db = new DB_tcts;
     $db->query("SELECT Batch_ID_SEQ.nextVal as next_ref from DUAL");
     if (!$db->next_record())
        hris_internalError("No value returned for the next value in the sequence of Batch IDs");
     $batchID = HRIS_APP_TCTS_BATCH_PREFIX . $db->f("next_ref");

     return $batchID;
   }
   
   function isValidBatchID($id)
   {
      $db = new DB_tcts;
      
      if (ereg("^[A-Za-z]+[0-9]+$",$id))
      {
        $db->query("SELECT batch_id from $this->table where batch_ID='$id'");
	return $db->next_record();
      }
      else return false;     
   }
   
   function insert($batchID,$cid,$adminCID,$type,$day,$mth,$year,$approved_day,$approved_mth,$approved_year,$effective_day,$effective_mth,$effective_year,$center)
   {
     $db = new DB_tcts;          
     
     $db->query("SELECT Transaction_ID_SEQ.nextVal as next_ref from DUAL");
     if (!$db->next_record())
        hris_internalError("No value returned for the next value in the sequence of Batch IDs");

     $transID = HRIS_APP_TCTS_PREFIX . $db->f("next_ref");
                    
     $status  = HRIS_APP_TCTS_OPEN;
     $db->query("INSERT into $this->table values ('$batchID','$transID',TO_DATE('$day-$mth-$year','DD-MM-YYYY'),'$cid','$adminCID',NULL,NULL,'$type','$center',TO_DATE('$approved_day-$approved_mth-$approved_year','DD-MM-YYYY'),TO_DATE('$effective_day-$effective_mth-$effective_year','DD-MM-YYYY'))");

     return $transID;
     
   } // function insert

   /*
     This function moves the transactions in the batch from the
     batches table to the transactions table.
   */
   function move($batchID)
   {
     $db = new DB_tcts;
     $status = HRIS_APP_TCTS_OPEN;
     // Insert the data into the transactions table
     $db->query("insert into $this->transactionTable select trans_id,trans_date,emp_cid,hr_admin,hr_admin_location,hr_admin_country,trans_type,trans_center,'$status',NULL,NULL,NULL,NULL,approved_date,effective_date from $this->table where batch_id = '$batchID'");
     // Delete the data from the batch table
     $db->query("delete from $this->table where batch_id='$batchID'");              
   } // function move
 
   function reject($batchID,$closedBy,$reason)
   {
     $db = new DB_tcts;
     $status = HRIS_APP_TCTS_REJECTED;

     $date = date("d-m-Y-H-i");
     // Insert the data into the transactions table
     $db->query("insert into $this->transactionTable select trans_id,trans_date,emp_cid,hr_admin,hr_admin_location,hr_admin_country,trans_type,trans_center,'$status',TO_DATE('$date','DD-MM-YYYY-HH24-MI'),'$reason','$closedBy',NULL,approved_date,effective_date from $this->table where batch_id = '$batchID'");
     // Delete the data from the batch table
     $db->query("delete from $this->table where batch_id='$batchID'");
   }
  
   function printBatchDetails($batchID,$file="batchDetails.pat")
   {
     global $patDir;
     $ccodes = new CenterCodes;
     $tcodes = new TransactionCodes;
     $admin  = new Admin;
     $db = new DB_tcts;
          
     $db->query("select batch_id,TRANS_ID,TO_CHAR(TRANS_DATE,'DD') AS DAY,TO_CHAR(TRANS_DATE,'MM') AS MTH,TO_CHAR(TRANS_DATE,'YYYY') AS YEAR,TO_CHAR(TRANS_DATE,'DD-MM-YYYY') AS TRANS_DATE,HR_ADMIN,EMP_CID,TRANS_TYPE,TRANS_CENTER, TO_CHAR(APPROVED_DATE,'DD') AS APPROVED_DAY,TO_CHAR(APPROVED_DATE,'MM') AS APPROVED_MTH,TO_CHAR(APPROVED_DATE,'YYYY') AS APPROVED_YEAR,TO_CHAR(APPROVED_DATE,'DD-MM-YYYY') AS APPROVED_DATE, TO_CHAR(EFFECTIVE_DATE,'DD') AS EFFECTIVE_DAY,TO_CHAR(EFFECTIVE_DATE,'MM') AS EFFECTIVE_MTH,TO_CHAR(EFFECTIVE_DATE,'YYYY') AS EFFECTIVE_YEAR,TO_CHAR(EFFECTIVE_DATE,'DD-MM-YYYY') AS EFFECTIVE_DATE  from $this->table where batch_id = '$batchID'");
     
     if (!$db->next_record()) hris_printError("No values found for batch ID $batchID");
     
     $center = $db->f("trans_center");
     $output = hris_includeFile("$patDir/$file");
     $output = str_replace("#batch_id#",$db->f("batch_id"),$output);
     $output = str_replace("#adminCID#",$db->f("hr_admin")." - ".$admin->getName($db->f("hr_admin"),$center),$output);
     $output = str_replace("#date#",$db->f("trans_date"),$output);
     $output = str_replace("#approved_date#",$db->f("approved_date"),$output);
     $output = str_replace("#effective_date#",$db->f("effective_date"),$output);
     $output = str_replace("#center#",$ccodes->getName($center),$output);
     $output = str_replace("#center_code#",$center,$output);     
     $employeeLine = hris_includeFile("$patDir/createBatchConfirmLine.pat");
     $confirmLines = "";
     do {
          $type = $db->f("trans_type");
	  $transID = $db->f("trans_id");
	  $empCID = $db->f("emp_cid");
	  if ($type != HRIS_APP_TCTS_INTEGRATION)
	        $empCID .= " - ".hris_CIDToName($empCID);
          $confirmLines .= str_replace("#transID#",$transID,str_replace("#empName#",$empCID,str_replace("#typeName#",$tcodes->getName($type,$center),$employeeLine)));
        } while ($db->next_record());
     $output = str_replace("#confirmLines#",$confirmLines,$output);
 
    $rc = new RejectionCodes;
    $output = str_replace("#reject_reasons#",generateSelect("",$rc->getCodes($center)),$output);
    
     hris_printOutput($output);
   } 
   
} // class BatchTable 
