<?php

  $errorString = "";

  /*
   * This function calculates the correct country code for the current user
   */
  function userToCountry()
  {
    global $center;
    
    $country = "";
    
    $cc = new CenterCodes;
    if (($center) && ($cc->isValidCode($center)))
       $country = $center; 
    else {
           $user = GetEnv('REMOTE_USER');
    	   $db = new DB_TWIN;
	   $db->query("select work_country from all_twin where email_ID = UPPER('$user')");
           if ($db->next_record())
	   {
	      $country = $db->f("work_country");
	      if (!$cc->isValidCode($country))
	         $country = HRIS_APP_TCTS_DEFAULT_CENTER;	      
	   } 
	   else $country = HRIS_APP_TCTS_DEFAULT_CENTER;
         }
    return $country;
  }
  
  /* This function creates a select box of HR Admins */
  function generateAdminSelect($country)
  {
    global $adminCID;
    
    $ad = new Admin;
    $admins = $ad->getAdminList($country);
    $output = "";
    
    reset($admins);
    while ( list($cid,$name) = each($admins) )
          if ($cid == $adminCID)
             $output .= "<OPTION SELECTED VALUE=\"$cid\"> $cid - $name </OPTION>";
          else $output .= "<OPTION VALUE=\"$cid\"> $cid - $name </OPTION>";

    return $output;  
  } // function generateAdminSelect
  
  /* This creates a code select box */
  function generateSelect($current="",$values)
  {
    $output = "";
    
    reset($values);
    while ( list($code,$name) = each($values) )
          if ($code == $current)
             $output .= "<OPTION SELECTED VALUE=\"$code\"> $name </OPTION>";
          else $output .= "<OPTION VALUE=\"$code\"> $name </OPTION>";

    return $output;      
  }
  
  function printCreateForm($country,$batch = false)
  { 
    global $patDir,$day,$mth,$year,$approved_day,$approved_mth,$approved_year,$effective_day,$effective_mth,$effective_year,$numOfLines,$HTTP_POST_VARS;
    
    if ($batch)
        $file = "createBatch.pat";
    else $file="createForm.pat";
    
    $output = hris_includeFile("$patDir/$file");
    // Fix the values for the date box
    if (!isset($day)) $day = date("d");
    if (!isset($mth)) $mth = date("m");
    if (!isset($year)) $year = date("Y");  
    if (!isset($approved_day)) $approved_day = date("d");
    if (!isset($approved_mth)) $approved_mth = date("m");
    if (!isset($approved_year)) $approved_year = date("Y");  
    if (!isset($effective_day)) $effective_day = date("d");
    if (!isset($effective_mth)) $effective_mth = date("m");
    if (!isset($effective_year)) $effective_year = date("Y");  
    
    $output = str_replace("#day#", $day , $output);
    $output = str_replace("#mth#", $mth , $output);
    $output = str_replace("#year#", $year , $output);
    $output = str_replace("#approved_year#",$approved_year,$output);
    $output = str_replace("#approved_mth#",$approved_mth,$output);
    $output = str_replace("#approved_day#",$approved_day,$output);
    $output = str_replace("#effective_year#",$effective_year,$output);
    $output = str_replace("#effective_mth#",$effective_mth,$output);
    $output = str_replace("#effective_day#",$effective_day,$output);
        
    $adminList = generateAdminSelect($country);
    $output = str_replace("#select_admin#", $adminList , $output);
    
    $user = hris_emailID2CID(GetEnv("REMOTE_USER"));
    $output = str_replace("#user#",$user,$output);
       
    $admin = new Admin;
    $cCodes = new CenterCodes;
    $centerCodes;
    if ($batch)
       $centerCodes = $admin->getCenterList($user);
    else $centerCodes = $cCodes->getCodes();
    $centerSelect = generateSelect($country,$centerCodes);
    $output = str_replace("#select_center#", $centerSelect , $output);

    $sCodes = new SendingCodes;
    $sendingCodes = $sCodes->getCodes($country);
    $sendingSelect = generateSelect(HRIS_APP_TCTS_DEFAULT_SENDING,$sendingCodes);
    $output = str_replace("#select_sending#", $sendingSelect , $output);
        
    // Print the employee box(es)
    $employeeTemplate = hris_includeFile("$patDir/createEmployeeLine.pat");
    $employeeForm = "";
    $tc = new TransactionCodes;
    $transactionCodes = $tc->getCodes($country);
    if (!isset($numOfLines)) $numOfLines = 1;
    if (!ereg("^[0-9]+$",$numOfLines)) hris_internalError("Num of lines ($numOfLines) not a number");
    $selectedType = "";
    for($i=1; $i<=$numOfLines; $i++)
    {
      if ($HTTP_POST_VARS["type".$i] != "")
         $selectedType = $HTTP_POST_VARS["type".$i];
	 
      $transactionSelect = generateSelect($selectedType,$transactionCodes);
      $employeeForm .= str_replace("#emp_cid#", $HTTP_POST_VARS["empCID".$i] , str_replace("#trans_select#", $transactionSelect, str_replace("#num#",$i,$employeeTemplate)));
      
    } // for each line
       
    $output = str_replace("#employee_form#", $employeeForm , $output);
    $output = str_replace("#numOfLines#",$numOfLines,$output);
    
    // Now the remove line button
    if ($numOfLines > 1)
       $output = str_replace("#remove_button#",hris_includeFile("$patDir/removeButton.pat"),$output);
    else $output = str_replace("#remove_button#","&nbsp;",$output);
    
    hris_printOutput($output);
  } // function printCreateForm
  
  
  function confirmBatch()
  {
    global $patDir,$day,$mth,$year,$approved_day,$approved_mth,$approved_year,$effective_day,$effective_mth,$effective_year,$numOfLines,$sending,$center,$HTTP_POST_VARS;
    $tcodes = new TransactionCodes;
    $ccodes = new CenterCodes;
    $scodes = new SendingCodes;
    $admin = new Admin;
    $tt = new TransactionTable;        
    $bt = new BatchTable;    
    
    $adminCID = hris_emailID2CID(GetEnv("REMOTE_USER"));    
    $errorString = validateInsertion($adminCID,$day,$mth,$year,$approved_day,$approved_mth,$approved_year,$effective_day,$effective_mth,$effective_year,$numOfLines,$sending,$center,$HTTP_POST_VARS);    
    // Print out the confirmation
    if ($errorString != "")
       hris_printError("<UL>".$errorString."</UL>");
    else {
           $batchID = $bt->generateBatchID();
	   $confirmLines = "";    
           $confirmTemplate = hris_includeFile("$patDir/createBatchConfirmLine.pat");
	   
	   $mailLines = "";
	   $mailTemplate = hris_includeFile("$patDir/mailConfirmLine.pat");
           for ($i=1;$i<=$numOfLines;$i++)
           {
	     $empCID = $HTTP_POST_VARS["empCID".$i];
	     $type = $HTTP_POST_VARS["type".$i];
	     $transID = $bt->insert($batchID,$empCID,$adminCID,$type,$day,$mth,$year,$approved_day,$approved_mth,$approved_year,$effective_day,$effective_mth,$effective_year,$center);
	     if ($type != HRIS_APP_TCTS_INTEGRATION)
	        $empCID .= " - ".hris_CIDToName($empCID);
	     $confirmLines .= str_replace("#transID#",$transID,str_replace("#empName#",$empCID,str_replace("#typeName#",$tcodes->getName($type,$center),$confirmTemplate)));
	     $mailLines .= str_replace("#transID#",$transID,str_replace("#empCID#",$empCID,str_replace("#type#",$tcodes->getName($type,$center),$mailTemplate)));
	   }
	   
	   $mailMsg = hris_includeFile("$patDir/mailCreateConfirmation.pat");
	   $mailMsg = str_replace("#batch_id#",$batchID,$mailMsg);
	   $mailMsg = str_replace("#adminCID#",$adminCID." - ".$admin->getName($adminCID,$center),str_replace("#date#","$day-$mth-$year",$mailMsg));
           $mailMsg = str_replace("#edit_url#",HRIS_APP_TCTS_URL."/move.phtml?batchID=".$batchID,$mailMsg);
	   $mailMsg = str_replace("#sending#",$scodes->getName($sending,$center),$mailMsg);
	   $mailMsg = str_replace("#center#",$ccodes->getName($center),$mailMsg);
	   $mailMsg = str_replace("#confirmLines#",$mailLines,$mailMsg);
	   mail_msg($mailMsg);
	   
	   $output = hris_includeFile("$patDir/createBatchConfirmation.pat");
	   $output = str_replace("#batch_id#",$batchID,$output);
	   $output = str_replace("#adminCID#",$adminCID." - ".$admin->getName($adminCID,$center),$output);
	   $output = str_replace("#date#","$day-$mth-$year",$output);
	   $output = str_replace("#approved_date#","$approved_day-$approved_mth-$approved_year",$output);
	   $output = str_replace("#effective_date#","$effective_day-$effective_mth-$effective_year",$output);
	   $output = str_replace("#sending#",$scodes->getName($sending,$center),$output);	   
	   $output = str_replace("#center#",$ccodes->getName($center),$output);
	   $output = str_replace("#confirmLines#",$confirmLines,$output);
	   
	   hris_printOutput($output);
	 }
 
    
  } // function confirm batch 
  
  function mail_msg($msg)
  {
    $mailFrom = "From: TCTS@hr.emea.mot.com";
    $mailTo = "TCTS@hr.emea.mot.com";
    $mailSubject = "Batch Submitted To TCTS";
    
    mail($mailTo,$mailSubject,$msg,$mailFrom);   
  }
  
  function validateInsertion($adminCID,$day,$mth,$year,$approved_day,$approved_mth,$approved_year,$effective_day,$effective_mth,$effective_year,$numOfLines,$sending,$center,$HTTP_POST_VARS)
  {
    global $errorString;
    $tt = new TransactionTable;
    $tcodes = new TransactionCodes;
    $ccodes = new CenterCodes;
    $scodes = new SendingCodes;
    $admin = new Admin;

    if (!$ccodes->isValidCode($center))
    {
      hris_printError("The Center selected is invalid");
    }
    
    if (!$tt->isValidDate($day,$mth,$year))
    {
       hris_printError("The date ($day-$mth-$year) must be in the format (DD-MM-YYYY). Also the date cannot be later than today");
    } 

    if (!checkdate($approved_mth,$approved_day,$approved_year))
      hris_printError("The approved date ($approved_day-$approved_mth-$approved_year) is invalid.");
    if (!checkdate($effective_mth,$effective_day,$effective_year))
      hris_printError("The effective date ($effective_day-$effective_mth-$effective_year) is invalid.");
 
    if (!$admin->isValidAdmin($adminCID,$center))
    {
      hris_printError("The CID ($adminCID) for the HR Admin is invalid");
    }
    
    if (!$scodes->isValidCode($sending,$center) && $sending != "")
    {
      hris_printError("The transmission method ($sending) selected is invalid");
    }  
    
    $errorString = "";
    for($i=1;$i<=$numOfLines;$i++)
    {
      if ($HTTP_POST_VARS["type".$i] != HRIS_APP_TCTS_INTEGRATION) // New employees don't have commerece IDs
      {
         if (!hris_isCID($HTTP_POST_VARS["empCID".$i]))
	 {
	   $errorString .= "<LI> Line $i : Invalid Commerce ID : ".$HTTP_POST_VARS["empCID".$i]."</LI>";
	 }
      } 
      else {
             if (!ereg("^[A-Za-z0-9]{3,8}$",$HTTP_POST_VARS["empCID".$i]))
	     {
	       $errorString .= "<LI> Line $i : Invalid Employee Identifier (only 3-8 alphanumeric characters allowed) : ".$HTTP_POST_VARS["empCID".$i]."</LI>";	     
	     }
	   
           }
	      
      if ( !($tcodes->isValidCode($HTTP_POST_VARS["type".$i],$center)) )
      {
	   $errorString .= "<LI> Line $i : Invalid Transaction Type ".$HTTP_POST_VARS["type".$i]."</LI>";      
      }
    } // for each line

    return $errorString;
  }  
  
  
  function confirmTransaction()
  {
    global $patDir,$adminCID,$day,$mth,$year,$approved_day,$approved_mth,$approved_year,$effective_day,$effective_mth,$effective_year,$numOfLines,$sending,$center,$HTTP_POST_VARS;
    $tcodes = new TransactionCodes;
    $ccodes = new CenterCodes;
    $admin = new Admin;
    $tt = new TransactionTable;    

    $errorString = validateInsertion($adminCID,$day,$mth,$year,$approved_day,$approved_mth,$approved_year,$effective_day,$effective_mth,$effective_year,$numOfLines,$sending,$center,$HTTP_POST_VARS);
    // Print out the confirmation
    if ($errorString != "")
       hris_printError($errorString);
    else {
	   $confirmLines = "";    
           $confirmTemplate = hris_includeFile("$patDir/createConfirmLine.pat");
           for ($i=1;$i<=$numOfLines;$i++)
           {
	     $empCID = $HTTP_POST_VARS["empCID".$i];
	     $type = $HTTP_POST_VARS["type".$i];	     
	     if ($type != HRIS_APP_TCTS_INTEGRATION)
	        $thisLine = str_replace("#empName#"," - ".hris_CIDToName($empCID),$confirmTemplate);
	     else $thisLine = str_replace("#empName#"," - ".hris_CIDToName($empCID),$confirmTemplate);
	     
	     $thisLine = str_replace("#empNumber#","empCID".$i,str_replace("#empCID#",$empCID,$thisLine));
	     $thisLine = str_replace("#typeNumber#","type".$i,str_replace("#type#",$type,str_replace("#typeName#",$tcodes->getName($type,$center),$thisLine)));
	     $confirmLines .= $thisLine;
	   }
	   
	   $output = hris_includeFile("$patDir/createConfirmation.pat");
  
	   $bt = new BatchTable;
	   $output = str_replace("#batchID#",$bt->generateBatchID(),$output);
	   $output = str_replace("#adminCID#",$adminCID,str_replace("#adminName#",$admin->getName($adminCID,$center),$output));
	   $output = str_replace("##",$admin->getName($adminCID,$center),$output);
	   $output = str_replace("#center#",$center,str_replace("#centerName#",$ccodes->getName($center),$output));
	   $output = str_replace("#year#",$year,$output);
	   $output = str_replace("#mth#",$mth,$output);
	   $output = str_replace("#day#",$day,$output);
	   $output = str_replace("#approved_year#",$approved_year,$output);
	   $output = str_replace("#approved_mth#",$approved_mth,$output);
	   $output = str_replace("#approved_day#",$approved_day,$output);
	   $output = str_replace("#effective_year#",$effective_year,$output);
	   $output = str_replace("#effective_mth#",$effective_mth,$output);
	   $output = str_replace("#effective_day#",$effective_day,$output);
	   $output = str_replace("#numOfLines#",$numOfLines,$output);
	   $output = str_replace("#sending#",$sending,$output);	   	   
	   $output = str_replace("#confirmLines#",$confirmLines,$output);
	   
	   hris_printOutput($output);
	 }
 
    
  } // function confirm transaction 
  
    
  function insertTransaction()
  {
    global $patDir,$adminCID,$batchID,$day,$mth,$year,$approved_day,$approved_mth,$approved_year,$effective_day,$effective_mth,$effective_year,$numOfLines,$sending,$center,$HTTP_POST_VARS;
    $tcodes = new TransactionCodes;
    $ccodes = new CenterCodes;
    $admin = new Admin;
    $tt = new TransactionTable;    
    
    $errorString = validateInsertion($adminCID,$day,$mth,$year,$approved_day,$approved_mth,$approved_year,$effective_day,$effective_mth,$effective_year,$numOfLines,$sending,$center,$HTTP_POST_VARS);
    // Print out the confirmation
    if ($errorString != "")
       hris_printError($errorString);
    else {
           // Log this insertion
	   log_insertion($batchID,$numOfLines,"$day-$mth-$year",$adminCID,$center);
	   
	   $confirmLines = "";    
           $confirmTemplate = hris_includeFile("$patDir/createSuccessLine.pat");
           for ($i=1;$i<=$numOfLines;$i++)
           {
	     $empCID = $HTTP_POST_VARS["empCID".$i];
	     $type = $HTTP_POST_VARS["type".$i];
	     $transID = $tt->insert($empCID,$adminCID,$type,$day,$mth,$year,$approved_day,$approved_mth,$approved_year,$effective_day,$effective_mth,$effective_year,$center);
	     if ($type != HRIS_APP_TCTS_INTEGRATION)
	        $empCID .= " - ".hris_CIDToName($empCID);
	     $confirmLines .= str_replace("#transID#",$transID,str_replace("#empCID#",$empCID,str_replace("#type#",$tcodes->getName($type,$center),$confirmTemplate)));
	   }
	   
	   $output = hris_includeFile("$patDir/createSuccess.pat");
	   $output = str_replace("#batch_id#",$batchID,$output);
	   $output = str_replace("#adminCID#",$adminCID." - ".$admin->getName($adminCID,$center),$output);
	   $output = str_replace("#date#","$day-$mth-$year",$output);
	   $output = str_replace("#approved_date#","$approved_day-$approved_mth-$approved_year",$output);
	   $output = str_replace("#effective_date#","$effective_day-$effective_mth-$effective_year",$output);
	   $output = str_replace("#center#",$ccodes->getName($center),$output);
	   $output = str_replace("#confirmLines#",$confirmLines,$output);
	   
	   hris_printOutput($output);
	 }
 
    
  } // function insert transaction 
?>
