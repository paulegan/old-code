
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


// Takes an array of words and returns a string
// of the words in "English syntax". eg:
// Given list = split("word1 word2 word3"," ");
// englishList(list,"and") would return "word1, word2 and word3"
function englishList(wordList,conj)
{
 var str = "";

 if (wordList.length==1)
   str += "&quot;"+wordList[0]+"&quot;";
 else if (wordList.length>1){
   for(var i=0; i<(wordList.length-2); i++)
     str += "&quot;"+wordList[i]+"&quot;, ";
   str += "&quot;"+wordList[wordList.length-2]+"&quot; "+conj
   str += " &quot;"+wordList[wordList.length-1]+"&quot;";
 }

 return str;
}// englishList


// Returns true is an object with a name beginning
// with "str" was selected.
function selectionMadeFor(str)
{
 for ( prop in request )
   if ((prop.substring(0,str.length)==str) && (request[prop]!=""))
     return true;

 return false;
}// selectionMadeFor


// Returns an array of names corresponding to all objects
// with name beginning with "str" that were selected.
// Suitable for a field of checkboxes.
function listOfCheckedItemsFor(str)
{
 var i=0;

 for ( prop in request )
   if (prop.substring(0,str.length) == str)
     this[i++]=request[prop];
 this.length=i;

 return this;
}// listOfCheckedItemsFor


// This returns a string explaining what the search was
// and how many vacancies were found.
function resultString(numFound)
{
 var str = "";
 var functionCBF = new CheckboxField("function");
 var fCBFs = functionCBF.selected();
 var functionMS = new MultiSelect("function");
 var fMSs = functionMS.selected();
 var functionsSelected = (fCBFs.length>0)?fCBFs:fMSs;

 str += "Found "+(numFound==0?"no":numFound);
 str += " position"+(numFound==1?"":"s");

 if (functionsSelected.length>0){
   str += " for function"+(functionsSelected.length>1?"s ":" ");
   str += englishList(functionsSelected,"or");
 }
 if (selectionMadeFor("keywords")){
   keywordList = request.keywords.split(" ");
   str += " containing the word"+(keywordList.length>1?"s ":" ");
   str += englishList(keywordList,request.logicOp.toLowerCase());
 }
 if (request.timeOfPosting>0){
   str += " posted within the last "+(request.timeOfPosting>1?(request.timeOfPosting+" days"):("day"));
 }
 str += '.';

 return(str);
}// resultString
