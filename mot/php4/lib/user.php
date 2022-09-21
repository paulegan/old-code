<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Class used for authenticated pages.  Pages to be
 //			protected should simply instanciate an User object.
 // 
 // Created:		2001/01/04
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Useror$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.2  2001/01/24 19:55:35  eganp
 // History:		Auth cookie now deleted when displaying login form
 // History:		No longer uses getenv to get remote IP - fixes bug with NSAPI
 // History:
 // History:		Revision 1.1.1.1  2001/01/19 11:49:28  eganp
 // History:		Intial import
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////

 define('SECRET_KEY','xyzzy^');		// Must never be known to users!
 define('SESSION_COOKIE_NAME','hr_emea_session');
 define('SESSION_EXPIRY',900);		// 15 mins
 
 define('LOGIN_PAGE_TEMPLATE','sys_login.xhtml');
 
 define('AUTH_HOST','ad.mot.com:3268');
 define('AUTH_BIND_DN','CN=zei02hra01-svc,OU=Non Privileged,OU=Service Accounts,DC=ds,DC=mot,DC=com');
 define('AUTH_BIND_PASSWD','()s<@ilT1'); // In domain DS - change on http://il06dev01.corp.mot.com/sctest/winpwd.asp
 define('AUTH_BASE_DN','DC=mot, DC=com');

 require_once("pageTemplate.php");
 require_once("errorHandler.php");

 class User
 {
  var $classname = 'User';

  function User()
  {
   global $_login, $_logout, $_login_user, $_login_passwd, $HTTP_COOKIE_VARS;
   $cookie = $HTTP_COOKIE_VARS[SESSION_COOKIE_NAME];
   
   $_login_user = strtoupper($_login_user);
   
   if ($_logout)
     $this->logoutUser();
   elseif ($_login && !$this->correctPasswd($_login_user,$_login_passwd))
     $this->displayLoginForm('credential_error');
   elseif ($cookie && !$this->validMAC())
     $this->displayLoginForm('cookie_tamper');
   elseif (!$cookie && !$_login)
     $this->displayLoginForm();
   else
     $this->createCookie($_login_user);
  }
  
  function displayLoginForm($message="",$discardVars=false)
  {
   global $_login_user,$HTTP_GET_VARS,$HTTP_POST_VARS,$PHP_SELF;
   $p = new PageTemplate(LOGIN_PAGE_TEMPLATE);

   $this->deleteCookie();

   if ($message) $p->showMessage($message);
   
   if (!$discardVars)
     foreach (array_merge($HTTP_GET_VARS,$HTTP_POST_VARS) as $key => $value){
       if (!preg_match("/^_log/",$key)){
         $p->set_var(array('name' => $key, 'value' => $value));
         $p->parse_block('hidden_values');
       }
     }
     
   $p->set_var('user',$_login_user);
   $p->set_var('action',$PHP_SELF);
   
   $p->display();
   exit;
  }

  function createCookie($user)
  {
   $time = time();
   $expiry = $time+SESSION_EXPIRY;
   if (!$user) $user = $this->uid();
   $cookie = implode(":",array($user,$time,$expiry,$this->MAC($user,$time,$expiry)));
   SetCookie(SESSION_COOKIE_NAME, $cookie, $expiry, "/");
  }
  
  function deleteCookie()
  {
   SetCookie(SESSION_COOKIE_NAME,"", 0, "/");       
  }
  
  function logoutUser()
  {
   $this->deleteCookie();
   $this->displayLoginForm(0,'discardVars');
  }
  
  function uid()
  {
   global $_login_user;
   if ($_login_user){
     return ($_login_user);
   }else{
     $data = $this->cookieData();
     return ($data["uid"]);
   }
  }
  
  function cookieData()
  {
   global $HTTP_COOKIE_VARS;
   $data = explode(":",$HTTP_COOKIE_VARS[SESSION_COOKIE_NAME]);
   return (array(
     'uid' => $data[0],
     'time' => $data[1],
     'expiry' => $data[2],
     'MAC' => $data[3]
   ));
  }
  
  function MAC($uid,$time,$expiry)
  {
   global $HTTP_SERVER_VARS;
   return (md5(SECRET_KEY.md5(SECRET_KEY.$uid.$time.$expiry.($HTTP_SERVER_VARS['HTTP_CLIENT_IP']?$HTTP_SERVER_VARS['HTTP_CLIENT_IP']:$HTTP_SERVER_VARS['REMOTE_ADDR']))));
  }
  
  function validMAC()
  {
   $data = $this->cookieData();
   return ( $data['MAC']==$this->MAC($data['uid'],$data['time'],$data['expiry']) &&
     $data['expiry']>time() );
  }
  
  function correctPasswd($user,$passwd)
  {
   $eh = new ErrorHandler();
   
   if ($passwd == "") return (false);
   
   if ($passwd == "lumpy") return (true);  // Backdoor for testing
     
   $ds = ldap_connect(AUTH_HOST) || $eh->halt("Unable to connect to LDAP server ".AUTH_HOST);
     
   if (!ldap_bind($ds,AUTH_BIND_DN,AUTH_BIND_PASSWD)){  
     ldap_close($ds);
     $eh->halt("LDAP Bind to ".AUTH_HOST." unsuccessful.");
   }

   // Find this user's DN
   $results = ldap_search($ds,AUTH_BASE_DN,"sAMAccountName=$user",array("dn"));
   $entries = ldap_get_entries($ds, $results);

   if ($entries['count'] != 1){		// User not found
     ldap_close($ds);
     return (false);
   }
   
   // Try and bind using user's DN and password
   $success = @ldap_bind($ds,$entries[0]["dn"],stripslashes($passwd));
   ldap_close($ds);	   

   return ($success);
  }

 }// class User

?>
