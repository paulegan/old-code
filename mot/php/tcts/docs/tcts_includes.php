<?php

  include('includes.php3');
  include('authenication.php3');
  
  $workingDir = "/www/php/lib/app/tcts";
  $patDir = "/www/docs/transactionCentre/tcts/pat";
  
  include("$workingDir/DB_tcts.php3");
  include("$workingDir/Admin.php3");
  include("$workingDir/Transaction.php3");
  include("$workingDir/CodeTables.php3");
  include("$workingDir/TransactionTable.php3");
  include("$workingDir/BatchTable.php3");
  include("$workingDir/new.php3");
  include("$workingDir/update.php3");
  include("$workingDir/logger.php3");
  
  $country = userToCountry();
?>
