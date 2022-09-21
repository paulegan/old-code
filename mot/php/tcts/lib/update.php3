<?php
  
  /* This function creates a select box of Employees */
  function generateEmployeeSelect($center)
  {    
    $tt = new TransactionTable;
    $employees = $tt->getEmployeeList($center);
    $output = "";
    
    $counter = 0;
    $numOfElems = count($employees);    
    while ( $counter < $numOfElems )
    {
       $cid = $employees[$counter];
       $output .= "<OPTION VALUE=\"$cid\">$cid</OPTION>";
       $counter++;
    }

    return $output;  
  } // function generateEmployeeSelect
  
  function printUpdateForm($option,$center)
  {
    global $patDir;

    $output = hris_includeFile("$patDir/updateForm.pat");
    
    $output = str_replace("#option#",$option,$output);
    
    $adminCID = hris_emailID2CID(GetEnv('REMOTE_USER'));
    $output = str_replace("#adminSelect#",generateAdminSelect($center),$output);
    $output = str_replace("#empSelect#",generateEmployeeSelect($center),$output);
    $cc = new CenterCodes;
    $centerCodes = $cc->getCodes();
    $output = str_replace("#centerSelect#",generateSelect($center,$centerCodes),$output);
    
    hris_printOutput($output);
  }

  function printQueryResults($db,$selectStmt,$transID,$adminCID,$empCid,$center,$type,$day,$mth,$year,$option,$status,$start,$end,$file,$button)
  {
    global $PHP_SELF,$patDir;    
    
    $transTable = new TransactionTable;
    
    $numTransactions = $db->num_rows();    
    if ($start > $numTransactions)
       hris_internalError("Invalid value for the start of search ( $start <= $numTransactions");
    if ($end <= 0)   
       hris_internalError("Invalid value for the end of search (0 < $end");    
    if ($end > $numTransactions) $end = $numTransactions; 
    if ($start < 0) $start = 0;
    
    $output = hris_includeFile("$patDir/$file");    

    for ($i = 0; $i< $start; $i++)
        $db->next_record();
	
    for ($i = $start; $i < $end; $i++)
    {      
      $db->next_record();
      $dir = ereg_replace("/[^/]*$","",$PHP_SELF);
      $editPage = $dir."/".$option.".phtml?transID=".$db->f("trans_id");    
      
      if ( ($i % 2) == 0) $results .= "<TR bgcolor =\"#CCCCFF\">"; 
      else $results .= "<TR bgcolor =\"#FFFFCC\">"; 
      
      if (hris_looksLikeCID($db->f("emp_cid")))
         $empName = hris_CIDToName($db->f("emp_cid")); 
      else $empName = $db->f("emp_cid");
          
      $results .= "<TD><A HREF=\"".$editPage."\"> ".$db->f("trans_id")." </A></TD>";
      $results .= "<TD> ".$empName."</TD><TD>".hris_CIDToName($db->f("hr_admin"))." </TD> <TD> ".$db->f("trans_type_name")." </TD><TD>".$db->f("trans_date_text")." </TD></TR>\n";
      
    }
    
    $output = str_replace("#results#",$results,$output);
    
    // Now create the back and forward buttons
    $nextStart = $end;
    $prevStart = 
    $buttonText = hris_includeFile($patDir."/".$button);    
    if ($end == $numTransactions )
       $output = str_replace("#go_forward#","&nbsp;",$output);
    else {
           $forwardButton = str_replace("#submitValue#","Next ",$buttonText);
	   $forwardButton = str_replace("#next_start#",$end,$forwardButton);
	   $forwardButton = str_replace("#next_end#",$end+HRIS_APP_TCTS_SEARCHRESULTS,$forwardButton);
	   $output = str_replace("#go_forward#",$forwardButton,$output);
	 }
	 
    if ($start == 0 )
       $output = str_replace("#go_back#","&nbsp;",$output);
    else {
           $backButton = str_replace("#submitValue#","Previous",$buttonText);
	   $backButton = str_replace("#next_end#",$start,$backButton);
	   $backButton = str_replace("#next_start#",$start-HRIS_APP_TCTS_SEARCHRESULTS,$backButton);
	   $output = str_replace("#go_back#",$backButton,$output);
	 }	 
	
    $cc = new CenterCodes;
    $output = str_replace("#center_select#",generateSelect($center,$cc->getCodes()),$output);
    $output = str_replace("#status#",$status,$output);
    $output = str_replace("#start#",$start+1,$output);
    $output = str_replace("#end#",$end,$output);
    $output = str_replace("#total#",$numTransactions,$output);
    $output = str_replace("#transID#",$transID,$output);
    $output = str_replace("#adminCID#",$adminCID,$output);
    $output = str_replace("#empCID#",$empCid,$output);
    $output = str_replace("#trans_center#",$center,$output);
    $output = str_replace("#trans_type#",$type,$output);
    $output = str_replace("#day#",$day,$output);
    $output = str_replace("#mth#",$mth,$output);
    $output = str_replace("#year#",$year,$output);
    $output = str_replace("#option#",$option,$output);	 
    hris_printOutput($output);
  }
  
  function printSearchResults($transID,$adminCID,$empCID,$center,$type,$day,$mth,$year,$option,$status,$start=0,$end=HRIS_APP_TCTS_SEARCHRESULTS,$file="search.pat",$button="searchButton.pat",$noResults="noResults.pat")
  {
    global $PHP_SELF;    
    
    if ($option == "modify") $type = "I"; // If a modify they should only see the Integration transactions
    
    $tb = new TransactionTable;
    $ad = new Admin;
    $db = new DB_tcts;
    
    $selectStmt = "SELECT TRANS_ID,TO_CHAR(TRANS_DATE,'DD-MM-YYYY') AS TRANS_DATE_TEXT,HR_ADMIN,EMP_CID,TRANS_TYPE,tc.NAME as TRANS_TYPE_NAME,status from $tb->table t,transaction_codes tc where tc.code = t.trans_type AND tc.country = t.trans_center AND status = '$status' ";
    
    if ($transID != "")
    {
       if ($tb->isValidTransID($transID))
         $selectStmt .= "AND trans_id='$transID' ";
       else hris_printError("Invalid transaction ID : $transID");
    }
    
    if ($empCID != "")
    {
       if (hris_isCID($empCID) || (ereg("^[A-Za-z0-9]{3,8}$",$empCID)))
         $selectStmt .= "AND emp_cid = '$empCID' ";
       else hris_printError("Invalid employee identifier : $empCID");
    }
    
    if ($center != "")
    {
      $cc = new CenterCodes;
      if ($cc->isValidCode($center))
          $selectStmt .= "AND trans_center = '$center'";
      else hris_printError("Invalid transaction center : $center");
    }

    if ($type != "")
    {
      $tc = new TransactionCodes;
      if ($tc->isValidCode($type,$center))
          $selectStmt .= "AND trans_type = '$type'";
      else hris_printError("Invalid transaction type : $type");
    }

    if ($adminCID != "")
    {
       if ($ad->isValidAdmin($adminCID,$center))
         $selectStmt .= "AND HR_ADMIN = '$adminCID'";
       else hris_printError("Invalid Admin ID : $adminCID");
    }
    
    if ($day.$mth.$year != "")
    {
       if ($tb->isValidDate($day,$mth,$year))
         $selectStmt .= "AND TRANS_DATE >= TO_DATE('$day-$mth-$year','DD-MM-YYYY') ";
       else hris_printError("Invalid date : $day/$mth/$year");
    }       
    
    $db->query($selectStmt." ORDER BY TRANS_DATE DESC");
    $numOfTransactions = $db->num_rows();
    if ($numOfTransactions == 0)
       printNoResults($transID,$adminCID,$empCID,$center,$type,$day,$mth,$year,$option,$status,$noResults);
    elseif ($numOfTransactions == 1)
       {
         $db->next_record();
	 $transID = $db->f("trans_id");
	 $dir = ereg_replace("/[^/]*$","",$PHP_SELF);
	 hris_redirect($dir."/".$option.".phtml?transID=".$transID);
       }       
    else printQueryResults($db,$selectStmt,$transID,$adminCID,$empCid,$center,$type,$day,$mth,$year,$option,$status,$start,$end,$file,$button);
	   
       
  }
  
  function printDetailsForm($transID,$stage,$file)
  {
    global $patDir,$PHP_SELF;
    $td = new TransactionTable;
    
    $output = hris_includeFile("$patDir/$file");
    $details = $td->details($transID);

    $output = str_replace("#trans_id#",  $details["trans_id"],$output);
    $output = str_replace("#trans_date#",$details["trans_date"],$output);
    $output = str_replace("#approved_date#",$details["approved_date"],$output);
    $output = str_replace("#effective_date#",$details["effective_date"],$output);
    $output = str_replace("#hr_admin#",  $details["hr_admin"],$output);
    
    if (hris_looksLikeCID($details["emp_cid"]))
        $output = str_replace("#emp_cid#",   $details["emp_cid"]." - ".$details["emp_name"],$output);
    else $output = str_replace("#emp_cid#",   $details["emp_cid"],$output);
    
    $output = str_replace("#trans_type#",$details["trans_type"],$output);
    $output = str_replace("#trans_center#",$details["trans_center"],$output);
    $output = str_replace("#status#",    $details["status"],$output);
    $output = str_replace("#close_date#",$details["close_date"],$output);
    if ($details["closed_by_name"] !=  $details["closed_by"])
       $output = str_replace("#closed_by#", $details["closed_by"]." - ".$details["closed_by_name"],$output);
    else $output = str_replace("#closed_by#", $details["closed_by"],$output);
    $output = str_replace("#reject_reason#", $details["reject_reason"],$output);
    
    // Might need to print a select list here
    $center = $details["trans_center_code"];
    $rc = new RejectionCodes;
    $output = str_replace("#reject_reasons#",generateSelect("",$rc->getCodes($center)),$output);

    $output = str_replace("#reject_reason#",$details["reject_reason"],$output);
    
    $output = str_replace("#form_action#",$PHP_SELF,$output);
    $output = str_replace("#stage#",$stage,$output);

   hris_printOutput($output);

  } // function printDetailsForm

  
  /* These print the various success messages */
  
  function printSuccess($transID,$file,$furtherInfo="")
  {
    global $patDir;
    
    $output = hris_includeFile("$patDir/$file");
    
    $output = str_replace("#trans_id#",  $transID,$output);
    $output =  str_replace("#further_info#",$furtherInfo,$output);
    hris_printOutput($output); 
  }
  
  function printNoResults($transID,$adminCID,$empCID,$center,$type,$day,$mth,$year,$option,$status,$file)
  {
    global $patDir;
    
    $output = hris_includeFile("$patDir/$file");
    
    $output = str_replace("#trans_id#",  $transID,$output);
    $output = str_replace("#emp_cid#",  $empCID,$output);
    $output = str_replace("#trans_center#",  $center,$output);    
    $output = str_replace("#trans_type#",  $type,$output);    
    $output = str_replace("#hr_admin#",  $adminCID,$output);
    $output = str_replace("#trans_date#","$day-$mth-$year",$output);
    $output = str_replace("#trans_center#","$center",$output);
    $output = str_replace("#status#","$status",$output);
    $cc = new CenterCodes;
    $select = generateSelect($center,$cc->getCodes());
    $output = str_replace("#center_select#","$select",$output);
    
    hris_printOutput($output); 
  }  
  
  function isSuperuser($emailID)
  {
    $auth = new Auth;
    $db = new DB_Auth;
    
    return($auth->inGroup($db,$emailID,HRIS_APP_TCTS_ADMINGROUP));
    
  }
?>
