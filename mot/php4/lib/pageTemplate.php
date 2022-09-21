<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Class for managing template files with i18n support.
 //			Extends the phplib template class.
 // 
 // Created:		2001/01/02
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.4  2001/02/05 19:00:47  eganp
 // History:		No longer any need to load ROOT_FILE in each method
 // History:		ROOT_FILE can now be overridden with parent
 // History:		New method set_message that more closely matches set_block
 // History:
 // History:		Revision 1.3  2001/02/02 12:18:47  eganp
 // History:		Updated handling of site header & footer
 // History:		
 // History:		Revision 1.2  2001/01/24 19:53:46  eganp
 // History:		Now sets PHP_SELF
 // History:		
 // History:		Revision 1.1.1.1  2001/01/19 11:49:28  eganp
 // History:		Intial import
 // History:		
 // 
 ///////////////////////////////////////////////////////////////////////////////

 define('PHP_DIR','/www/php4');
 define('LOCAL_TEMPLATE_DIR',PHP_DIR.'/apps/'.APPLICATION_NAME.'/templates');
 define('SYSTEM_TEMPLATE_DIR',PHP_DIR.'/templates');
 define('LOCALE_DIR','locale');
 define('DEFAULT_LOCALE','C');
 define('SYSTEM_HEADER',SYSTEM_TEMPLATE_DIR.'/header');
 define('SYSTEM_FOOTER',SYSTEM_TEMPLATE_DIR.'/footer');
 define('ROOT_FILE','_root');
 
 require_once("template.inc");
 require_once("errorHandler.php");
 
 class PageTemplate extends Template
 {
  var $classname = 'PageTemplate';
  var $locale;
  
  function PageTemplate($page,$locale=0)
  {
   global $PHP_SELF;
   
   $this->locale = $locale?$locale:(getenv('LANG')?getenv('LANG'):DEFAULT_LOCALE);
   
   foreach (array(LOCAL_TEMPLATE_DIR,SYSTEM_TEMPLATE_DIR) as $dir)
     foreach (array($dir.'/'.LOCALE_DIR.'/'.$this->locale,$dir.'/'.LOCALE_DIR.'/'.DEFAULT_LOCALE,$dir) as $source)
       if (is_readable($source.'/'.$page)){
         $this->set_root($source);
	 break 2;
       }

   $this->set_var('PHP_SELF',$PHP_SELF);
   
   $this->set_file(ROOT_FILE,$page);
   $this->loadfile(ROOT_FILE);
   if (preg_match("/\{site_....er\}/", $this->get_var(ROOT_FILE))){
     $this->set_file('site_header',SYSTEM_HEADER);
     $this->loadfile('site_header');
     $this->set_file('site_footer',SYSTEM_FOOTER);
     $this->loadfile('site_footer');
   }
  }
  
  function display()
  {
   print $this->subst(ROOT_FILE);
  }
  
  function showMessage($name,$parent=ROOT_FILE)
  {
   $text = $this->get_var($parent);
   $text = preg_replace("/[<!-> \t]+MESSAGE $name [<!-> \t]+/", "", $text);
   $this->set_var($parent, $text);
  }

  function set_message($parent,$name,$handle=0)
  {
   $text = $this->get_var($parent);
   $reg = "/<!--\s+MESSAGE $name\s+-\s*(.*)\n\s*-\s+MESSAGE $name\s+-->/s";
   if (preg_match($reg, $text, $m)){
     $this->set_var($handle?$handle:$name, $m[1]);
     $this->set_var($parent, preg_replace($reg, "{".$name."}", $text));
   }
  }
  
  function remove_block($block,$parent=ROOT_FILE)
  {
   $text = $this->get_var($parent);
   $reg = "/\s*<!--\s+BEGIN $block\s+-->.*<!--\s+END $block\s+-->/s";
   $text = preg_replace($reg, "", $text);
   $this->set_var($parent, $text);
  }
  
  function parse_block($block,$parent=ROOT_FILE)
  {
   if (!isset($this->varkeys[$block]))
     $this->set_block($parent, $block,"_".$block."_block");
   $this->parse("_".$block."_block", $block, true);
  }
  
  function halt($msg)
  {
   $eh = new ErrorHandler();
   $eh->logError($msg,LOG_CRIT);      // Can't call eh->halt 'cause that uses this class!
   print $msg;
   exit();
  }

  function root()
  {
   return (ROOT_FILE);
  }
  
 }// class PageTemplate

?>
