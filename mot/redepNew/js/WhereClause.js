
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19971122
//  TestedWith:		ES 351
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        
//  Keywords:            
//  Notes:		


function WhereClause(sql)
{
 this.sql = (arguments.length<1)?(""):(" WHERE "+sql);
 WhereClause.prototype.addColumnMatch = WhereClause_addColumnMatch;
 WhereClause.prototype.addTextSearch = WhereClause_addTextSearch;
 WhereClause.prototype.addPostDate = WhereClause_addPostDate;
}// WhereClause


function WhereClause_addColumnMatch(columnName,values)
{
 var list;

 if (!values || values.length<1) return(false);
 list = values.join("','");
 if (list=="") return(false);

 this.sql += (this.sql.length>0?" AND ":" WHERE ");
 this.sql += columnName+" IN ( '"+list+"' ) ";

 return(true);
}// WhereClause_addColumnMatch


function WhereClause_addTextSearch(str,op)
{
 var columns = project.textSearchCols;
 var keywords;

 if ( (str==null) || (str=="") || (columns.length<1) )
   return(false);

 keywords = str.replace(/'/g,"''").split(" ");

 this.sql += (this.sql.length>0?" AND ( ":" WHERE ( ");
 for (var k=0; k<keywords.length; k++){
   this.sql += ((k==0)?"( ":" "+op+" ( ");
   for (var c=0; c<columns.length; c++){
     this.sql += ((c==0)?" ":" OR ");
     this.sql += "upper("+columns[c]+") LIKE '%"+keywords[k].toUpperCase()+"%'";
   }
   this.sql += " )";
 }
 this.sql += " ) ";

 return(true);
}// WhereClause_addTextSearch


function WhereClause_addPostDate(val)
{
 if (val > 0){
   this.sql += (this.sql.length>0?" AND ":" WHERE ");
   this.sql += "(sysdate-advertise_date) < "+val+" ";
 }

 return(true);
}// WhereClause_addPostDate
