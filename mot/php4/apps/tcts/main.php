<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	The TCTS application root class.
 // 
 // Created:		2001/01/09
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.3  2001/02/05 16:43:00  eganp
 // History:		Display informational message when verifying CIDs
 // History:
 // History:		Revision 1.2  2001/01/24 19:22:58  eganp
 // History:		Fixed error with results count
 // History:		Added email notification of batch submission
 // History:		Added check on employee CIDs
 // History:		Only try & update a user if one was selected
 // History:		
 // History:		Revision 1.1.1.1  2001/01/19 11:41:06  eganp
 // History:		Initial import of TCTS3.
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////

 require_once("config.php");
			
 require_once("db.php");
 require_once("errorHandler.php");
 require_once("user.php");
 require_once("userPermissions.php");
 require_once("appPermissions.php");

 require_once("transaction.php");
 require_once("miscFuncs.php");

 require_once("createPage.php");
 require_once("searchPage.php");
 require_once("resultsPage.php");
 require_once("detailsPage.php");
 require_once("listEditPage.php");
 require_once("userEditPage.php");
 require_once("userAddPage.php");

 class tcts
 {
  var $classname = 'tcts';
  var $context = array();
  
  function tcts()
  {
   global $HTTP_GET_VARS,$HTTP_POST_VARS;
   $this->context['db'] = new db(DB_USER,DB_PASSWD);
   $this->context['eh'] = new ErrorHandler(APPLICATION_NAME);
   $this->context['user'] = new User();
   $this->context['userPerms'] = new UserPermissions($this->context['user']->uid(),$this->context['db']);
   $this->context['appPerms'] = new AppPermissions($this->context['db']);
   $this->context['form'] = array_merge($HTTP_GET_VARS,$HTTP_POST_VARS);
   $this->process();
  }
  
  function center($op=0)
  {
   global $HTTP_COOKIE_VARS;
   $context = &$this->context;
   
   // center is set to (in order) either "_center" from the query string, the
   // tcts_center cookie or the first center in the user's permitted centers.
   
   $permittedCenters = $context['userPerms']->datasets($op);
   
   if ($context['form']['_center'] && in_array($context['form']['_center'],$permittedCenters))
     $center = $context['form']['_center'];
   elseif ($HTTP_COOKIE_VARS[CENTER_COOKIE_NAME] && in_array($HTTP_COOKIE_VARS[CENTER_COOKIE_NAME],$permittedCenters))
     $center = $HTTP_COOKIE_VARS[CENTER_COOKIE_NAME];
   else
     $center = $permittedCenters[0];

   SetCookie(CENTER_COOKIE_NAME, $center, time()+CENTER_COOKIE_EXPIRY, "/");
   
   return ($center);
  }

  function process()
  {
   global $DEFAULT_OP_ORDER, $UPDATE_OPS;
   
   // Choose the default op if none is given 
   if (!$this->context['form']['_op']){
     foreach ($DEFAULT_OP_ORDER as $o)
       if ($this->context['userPerms']->canDoOp($o)){
	 $this->context['form']['_op'] = $op = $o;
	 break;
       }
   }else{
     $op = $this->context['form']['_op'];
   }
   
   // Map update ops to 'update'
   if (in_array($op,$UPDATE_OPS)) $op = 'update';
   
   // Halt if we still havn't got anything
   if (!$op) $this->context['eh']->halt("User, ".$this->context['user']->uid().
		", hasn't permission to do anything!");
   
   // Deal with logout early - before permission checks
   if ($op=='logout') return ($this->context['user']->logoutUser());
   
   // The op must be one of those allowed for this user...
   if (!in_array($op,$this->context['userPerms']->operations()))
     $this->context['eh']->halt("Invalid op - $op !");
     
   // (needed to wait till op was decided before getting the center)
   $this->context['center'] = $this->center($op);
   
   // ... and allowed for the specified center   
   if (!$this->context['userPerms']->canDoOp($op,$this->context['center']))
     $this->context['eh']->halt("User, ".$this->context['user']->uid().
     		", isn't allowed access to ".$op.
		" for ".$this->context['center']."!");
   
   // Now finally select which op to perform -
   if ($op=='create' || $op=='submit batch')
       return ($this->doCreate());
   elseif ($op=='view' || $op=='delete' || $op=='update')
       return ($this->doShowTrans());
   elseif ($op=='manage lists')
       return ($this->doManageLists());
   elseif ($op=='manage users')
       return ($this->doManageUsers());
   
  }
  
  function doShowTrans()
  {
   switch ($this->context['form']['_stage']){
     case 'commit':
       return ($this->doUpdate());
     case 'details':
       return ($this->doDetails());
     case 'results':
       return ($this->doSearchResults());
     case 'search':
     default:
       $page = new SearchPage($this->context);
       $page->parsePage();
       return ($page->display());
   }
  }
  
  function doSearchResults()
  {
   $trans = new Transaction($this->context);
   $results = $trans->find($msg);
   $msg = 0;
   
   if (!$results){
     $page = new SearchPage($this->context);
     $page->parsePage($msg?$msg:'nothing_found');
   }elseif ($this->context['form']['total']==1){
     $page = new DetailsPage($this->context);
     $trans->getDetails($results[0]['trans_id']);
     $page->parsePage($trans);
   }else{
     $page = new ResultsPage($this->context);
     $page->parsePage($results);
   }
   
   return ($page->display());
  }
  
  function doDetails()
  {
   $trans = new Transaction($this->context,$this->context['form']['trans_id']);

   if (!$trans->details){
     $page = new SearchPage($this->context);
     $page->parsePage('invalid_trans_id');
   }else{
     $page = new DetailsPage($this->context);
     $page->parsePage($trans);
   }
   
   return ($page->display());
  }
  
  function doUpdate()
  {
   global $OP_2_STATUS;
   $f = &$this->context['form'];
   $trans = new Transaction($this->context);

   switch ($f['_op']){
     case 'reject':
     case 'close':
     case 'integrate':
     case 'delete':
       $trans->update($f['trans_id'],$OP_2_STATUS[$f['_op']],$f['reject_reason']);
       $page = new DetailsPage($this->context);
       $trans->getDetails($f['trans_id']);
       $page->parsePage($trans,$f['_op'].'_success');
       break;
     case 'reopen':
       $newID = $trans->reopen($f['trans_id']);
       $trans = new Transaction($this->context,$newID);
       $page = new DetailsPage($this->context);
       $page->parsePage($trans,$f['_op'].'_success');
       break;
     case 'reject batch':
     case 'integrate batch':
       $trans->update($f['batch_id'],$OP_2_STATUS[$f['_op']],$f['reject_reason']);
       $page = new ResultsPage($this->context);
       $page->parsePage($trans->find($msg),$f['_op'].'_success');
   }
   
   return ($page->display());
  }
  
  function doCreate()
  {
   $page = new CreatePage($this->context);
   $trans = new Transaction($this->context);
   $status = ($this->context['form']['_op']=='submit batch')?'Pending':'Open';
   
   if ($this->context['form']['_stage'] == 'commit'){
     $msgs = $trans->create($status);
     if (($msgs[0]=='create_success') && ($status=='Pending'))
       emailPendingNotification($page->root,$this->context);
   }elseif ($this->context['form']['_stage'] == 'check cids'){
     setNamesFromCIDs($this->context['form'],$this->context['db']);
     $msgs[] = 'check_cids';
   }
   
   $page->parsePage($msgs);

   return ($page->display());
  }
  
  function doManageLists()
  {
   $f = &$this->context['form'];
   $page = new ListEditPage($this->context);

   if ($f['_stage']=='commit'){
     updateList($f['listName'],$this->context['db'],$this->context['center'],$f['listItems']);
     $page->parsePage('commit_success');
   }else{
     $page->parsePage();
   }
   
   $page->display();

  }
  
  function doManageUsers()
  {
   $p = &$this->context['appPerms'];
   $scope = $this->context['userPerms']->datasets('manage users');
   
   switch ($this->context['form']['_stage']){
     case 'add':
       $page = new UserAddPage($this->context);
       $page->parsePage();
       break;
     case 'add confirm':
       $page = new UserAddPage($this->context);
       $usersOK = $p->verifyNewUsers($this->context['form']['users'], $scope);
       $page->parsePage($usersOK?'':'invalid_users');
       break;
     case 'add commit':
       if ($p->addNewUsers($this->context['form']['users'], $scope,
		     getPermsFromMatrix($this->context['form']))){
         $page = new UserEditPage($this->context);
         $page->parsePage('add_success');
       }else{
         $page = new UserAddPage($this->context);
         $page->parsePage('invalid_users');
       }
       break;
     case 'commit':
       if ($this->context['form']['user']){
         $p->updateUser($this->context['form']['user'], $scope,
		     getPermsFromMatrix($this->context['form']));
         $page = new UserEditPage($this->context);
         $page->parsePage('commit_success');
         break;
       }
     default:
       $page = new UserEditPage($this->context);
       $page->parsePage();
   }
   
   return ($page->display());
  }
  
 }// class tcts

?>
