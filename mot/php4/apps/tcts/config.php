<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Some configuration elements for the TCTS app.
 // 
 // Created:		2001/01/17
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.3  2001/02/02 14:21:57  eganp
 // History:		Ireland no longer worthy of mention! 8-)
 // History:
 // History:		Revision 1.2  2001/01/24 19:17:12  eganp
 // History:		Added results limit & email addr for submit notification
 // History:		
 // History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 // History:		Initial import of TCTS3.
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////

 define('APPLICATION_NAME','tcts');
 define('DB_USER','tcts');
 define('DB_PASSWD','trali');

 define('CENTER_COOKIE_NAME','tcts_center');
 define('CENTER_COOKIE_EXPIRY',3000000);

 define('TRANS_ID_PREFIX','T');
 define('BATCH_ID_PREFIX','B');

 define('RESULTS_PER_PAGE',20);
 define('RESULTS_LIMIT',400);

 define('TXC_EMAIL_ADDR','paul@hr.emea.mot.com');
 
 $DEFAULT_OP_ORDER = array(	'create',
 				'submit batch',
				'view',
				'manage users',
				'manage lists');

 $UPDATE_OPS = array(		'close',
 				'reopen',
				'integrate',
				'integrate batch',
				'reject',
				'reject batch');
				
 $OP_2_STATUS = array(		'create' => 'Open',
				'submit batch' => 'Pending',
				'delete' => 'Deleted',
				'close' => 'Closed',
				'reopen' => 'Open',
				'integrate' => 'Open',
				'integrate batch' => 'Open',
				'reject' => 'Rejected',
				'reject batch' => 'Rejected');
 
 $CENTER_2_LOCALE = array(	'UK' => 'en_UK',
 				'France' => 'fr_FR',
 				'Germany' => 'de_DE',
 				'Israel' => 'iw_IL');

?>
