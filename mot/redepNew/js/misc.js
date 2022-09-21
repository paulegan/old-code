
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19980929
//  TestedWith:
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  Description:        A few miscellaneous functions.
//  Keywords:            
//  Notes:


function error(message)
{
 if (client){
   client.errorMessage = message;
   client.errorPage = request.uri;
   redirect("error.html");
 }else{
   debug(message);
 }
}// error

function addToLog(msg)
{
 var logFile = new File(project.errorLogFile);
 var props = [ 'ip', 'auth_user', 'uri', 'agent' ];
 var now = new Date();

 if (!logFile.open("a")) return(false);
 logFile.write(now.getDate()+"/"+(now.getMonth()+1)+"/"+now.getYear()+"-"+now.getHours()+":"+now.getMinutes());
 logFile.write(" \""+msg.replace(/\n/,"")+"\" ");
 while ((p=props.pop())!=null)
   if (request[p])
      logFile.write("\""+request[p]+"\" ");
 logFile.writeln(" ");
 logFile.close();

 return true;
}// addToLog


// Prints the contents of a file. Returns false on error, else true.
function includeFile(filename)
{
 var f = new File(filename)

 if (!f.open("r")) return(false);
 while (!f.eof())
   write(f.readln()+"\n");
 f.close();

 return(true);
}// includeFile


function mailResume()
{
 var msg = new SendMail();

 if ( request.email != "" ){
   msg.Cc = request.email;
   msg["Reply-To"] = request.email;
 }
 msg.To = request.jobEmail;
 msg.From = project.systemEmail;
 msg.Subject = "Application for vacancy "+request.ref;
 msg.Body  = "\nApplication for \""+request.jobTitle+"\", "+request.ref+"\n\n";
 msg.Body += "NAME:\n"+request.name+"\n\n";
 msg.Body += "CID:\n"+request.cid+"\n\n";
 msg.Body += "EMAIL ADDRESS:\n"+request.email+"\n\n";
 msg.Body += "ADDRESS:\n"+request.address+"\n\n";
 msg.Body += "DAYTIME PHONE NUMBER:\n"+request.workTel+"\n\n";
 msg.Body += "HOME PHONE NUMBER:\n"+request.homeTel+"\n\n";
 msg.Body += "CURRENT POSITION:\n"+request.position+"\n\n";
 msg.Body += "MONTHS IN CURRENT POSITION:\n"+request.monthsInPos+"\n\n";
 msg.Body += "CURRENT GRADE:\n"+request.grade+"\n\n";
 msg.Body += "MOST RECENT MANAGER:\n"+request.manager+"\n\n";
 msg.Body += "CURRENTLY ON A PIP OR INCIDENT REPORT:\n"+((request.pip=="on")?"Yes":"No")+"\n\n";
 msg.Body += "EDUCATION & QUALIFICATIONS:\n"+request.education+"\n\n";
 msg.Body += "CAREER HISTORY:\n"+request.career+"\n\n";
 msg.Body += "SUPPORTING INFORMATION:\n"+request.supportingText+"\n\n--\n";
 msg.Body = msg.Body.replace(/\n\./g,".\n");            // Bug fix

 if (!msg.send()) addToLog("Couldn't mail "+request.jobEmail+" about "+request.ref+"/"+request.cid+".");

 return(true);
}// mailResume


function mailPositionNotification(contactEmail,mngrEmail)
{
 var msg = new SendMail();
 var guidelines = "";

 guidelines += "\n-------------------------------------------------------------------------\n\n";
 guidelines += "U.S. REDEPLOYMENT GUIDELINES\n";
 guidelines += "(Effective Date:  10/20/1998)\n\n";
 guidelines += "Key Terms\n\n";
 guidelines += "* \"Affected Employees\" are employees whom Motorola has notified of\n";
 guidelines += "  employment termination due to a workforce reduction or job elimination, or\n";
 guidelines += "  the closing of a facility or operation.\n\n";
 guidelines += "* \"Senior Service,\" as used in these guidelines and in U.S. H.R.\n";
 guidelines += "  Policy #7, refers to employees who have been continuously employed for ten\n";
 guidelines += "  (10) years or more since their Service Date (as defined in the U.S. H.R.\n";
 guidelines += "  policies).  \"Service Date\" is to be distinguished from \"Service Club Date\"\n";
 guidelines += "  (also defined in U.S. H.R. policies).\n\n";
 guidelines += "Transition Center\n\n";
 guidelines += "* The IOS process will be suspended until further notice, and jobs\n";
 guidelines += "  will be filled through the Transition Center.\n\n";
 guidelines += "* The Transition Center will collect and provide information on job\n";
 guidelines += "  openings to Affected Employees.  The same job openings will be made\n";
 guidelines += "  available to current Motorolans through the Transition Center, or otherwise.\n\n";
 guidelines += "Application Process\n\n";
 guidelines += "* It shall be the responsibility of Affected Employees and current\n";
 guidelines += "  Motorolans to review openings in the jobs pool and make application.\n\n";
 guidelines += "* No Affected Employee on a Performance Improvement Plan (PIP) or\n";
 guidelines += "  Class II or III warning will be permitted to apply.\n\n";
 guidelines += "* To ensure that the process is not overwhelmed by applications, and\n";
 guidelines += "  to promote the retention of employees in jobs for which they are qualified,\n";
 guidelines += "  the following guidelines apply:\n\n";
 guidelines += "* Counselors will help Affected Employees to apply only for jobs for\n";
 guidelines += "  which they are qualified.\n\n";
 guidelines += "* No Affected Employee may apply for a position more than three (3)\n";
 guidelines += "  grades lower than the last grade held by the Affected Employee.\n\n";
 guidelines += "* The system will require Affected Employees and current Motorolans to\n";
 guidelines += "  consult a Transition Center counselor or Staffing representative as a\n";
 guidelines += "  condition of applying for more than five, and, if appropriate, more than ten\n";
 guidelines += "  open positions. \n\n";
 guidelines += "* The Transition Center  must demonstrate in writing reasonable\n";
 guidelines += "  efforts to redeploy Affected Senior Service Employees.\n\n";
 guidelines += "  Application Window\n\n";
 guidelines += "* Affected Employees will be eligible to apply for jobs for up to\n";
 guidelines += "  thirty (30) days following notification of termination.\n\n";
 guidelines += "* Applications shall be deemed to have been rejected if the Affected\n";
 guidelines += "  Employee has not been selected at the end of the thirty-day period. \n\n";
 guidelines += "* If the applicant receives and accepts a job offer, he or she must\n";
 guidelines += "  advise the Transition Center, which will then cancel all other pending\n";
 guidelines += "  applications.\n\n";
 guidelines += "Selection Process\n\n";
 guidelines += "* Interviews will be conducted for the most qualified candidates, as\n";
 guidelines += "  determined by the hiring manager in consultation with HR.\n\n";
 guidelines += "* Selection criteria for available openings are as follows\n\n";
 guidelines += "* Affected Employees and current Motorolans may apply for the same\n";
 guidelines += "  positions.\n\n";
 guidelines += "* It is paramount that persons selected for openings are the best\n";
 guidelines += "  qualified to perform the jobs.\n\n";
 guidelines += "* When qualified, a Motorolan should be selected to fill positions\n";
 guidelines += "  listed in the jobs pool which are currently filled by contract labor if such\n";
 guidelines += "  positions are being converted to regular positions.\n\n";
 guidelines += "* Motorolans shall be given preference over equally or less qualified\n";
 guidelines += "  external candidates.\n\n";
 guidelines += "* Senior Service current Motorolans and Affected Employees shall be\n";
 guidelines += "  selected over equally or less qualified internal or external candidates.\n\n";
 guidelines += "Relocation\n\n";
 guidelines += "* Motorola will pay for relocations only where otherwise payable under\n";
 guidelines += "  company policy.\n\n";
 guidelines += "Outplacement\n\n";
 guidelines += "* The Transition Center will provide outplacement services as well as\n";
 guidelines += "  internal redeployment assistance.  Affected Employees will be permitted to\n";
 guidelines += "  explore outplacement opportunities at the same time that they are exploring\n";
 guidelines += "  alternative internal placements.\n\n\n";
 guidelines += "\n-------------------------------------------------------------------------\n\n";

 msg.To = contactEmail;
 msg.From = project.systemEmail;
 msg.Subject = "Redeployment Position Creation or Update";
 msg.Body  = "\nA position "+request.ref+", \""+request.title+"\" has been created\n";
 msg.Body += "or updated on the Redeployment System for which you are specified as the\n";
 msg.Body += "contact person.  Any application made to this position will be sent to\n";
 msg.Body += "you by email.\n";
 msg.Body += "\nYou may view the position at:\n";
 msg.Body += "   http://redeployment.mot.com/search/details.html?ref="+request.ref+"\n\n";
 msg.Body += guidelines;

 if (!msg.send()) addToLog("Couldn't mail "+request.contactEmail+" about "+request.ref+".");

 if (mngrEmail){
   msg.To = mngrEmail;
   msg.From = project.systemEmail;
   msg.Subject = "Redeployment Position Creation or Update";
   msg.Body  = "\nA position "+request.ref+", \""+request.title+"\" has been created\n";
   msg.Body += "or updated on the Redeployment System for which you are specified as the\n";
   msg.Body += "hiring manager.  You may review applications for this position by accessing\n";
   msg.Body += "this website - http://redeployment.mot.com/mngr/\n";
   msg.Body += "\nTo access this system you will need an X500 account. You can initialize\n";
   msg.Body += "your X500 password here - http://directory.mot.com/password.html\n\n";
   msg.Body += guidelines;

   if (!msg.send()) 
     addToLog("Couldn't mail "+mngrEmail+" about "+request.ref+".");
 }

 return (true);
}// mailPositionNotification


// Returns true is "str" is a valid word.
function isWord(str)
{
 var digits = "0123456789";
 var lowerCaseLetters = "abcdefghijklmnopqrstuvwxyz";
 var upperCaseLetters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
 var otherChars = "-._";
 var validChars = lowerCaseLetters+upperCaseLetters+digits+otherChars;
 var ch;

 if (str.length<1) return false;
 for(var c=0; c<str.length; c++)
   if (validChars.indexOf(str.charAt(c))==-1)
     return false;

 return true;
}// isWord


// Checks if "str" is a valid email address. Displays an
// error message if not. An email address is taken to
// be a string with no spaces that contains an "@" and
// at least one ".", where the first "." comes after
// the "@".  The characters between the start & "@",
// "@" & "." and "." & end must also form a valid word.
function isValidEmailAddress(str)
{
 var isEmail = false;

 if ((str!=null) || (str!="") || (str.indexOf(" ")==-1)){
   atLoc = str.indexOf("@");
   dotLoc = str.indexOf(".");
   if ((atLoc>0) && (dotLoc>0) && (dotLoc>atLoc)){
     if (isWord(str.substring(0,atLoc)) &&
	 isWord(str.substring(atLoc+1,dotLoc)) &&
	 isWord(str.substring(dotLoc+1,str.length)))
       isEmail = true;
   }
 }

 return isEmail;
}// isValidEmailAddress


// Returns true is the string is a Motorola email ID.
// This is taken to be a one or more letters followed
// by numbers - and maybe more letters!.
function isMotEmailID(str)
{
 var isEmailID = false;
 var digits = "0123456789";
 var letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
 var i,c=0;

 if ((str.length>2)){
   str = str.toUpperCase()
   while((c<str.length) && (letters.indexOf(str.charAt(c))!=-1))
     c++;
   if ((c>0) && (c<str.length) && (digits.indexOf(str.charAt(c))!=-1)){
     while((c<str.length) && (digits.indexOf(str.charAt(c))!=-1))
       c++;
     if (c==str.length){
       isEmailID=true;
     }else{
       while((c<str.length) && (letters.indexOf(str.charAt(c))!=-1))
     	 c++;
       if (c==str.length)
     	 isEmailID=true;
     }
   }
 }

 return isEmailID;
}// isMotEmailID


function looksLikeCID(cid)
{
 if (cid.length != 8) return false;

 for (i=0; i<8; i++)
   if (isNaN(cid.charAt(i)))
     return false;

 return true;
}// looksLikeCID


function location2country(location)
{
 var temp = database.superCursor("SELECT c.name,c.region FROM country c, location l WHERE c.name = l.country AND l.name = '"+location+"'");
 temp.next();
 return (temp);
}// location2country
