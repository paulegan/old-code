
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19990909
//  TestedWith:		ES 362
//  TestDate:
//  ReviewedBy:         
//  ReviewDate:         
//  ApprovedBy:
//  ProdDate:
//  History:            
//  Description:        Object encapsulating operations on corrective_action table.
//  Keywords:            
//  Notes:	
	
function CorrectiveAction()
{
 // extends CoapTable
 this.parent = CoapTable;
 this.parent("corrective_action");

 // methods
 this.getRows = CorrectiveAction_getRows;

 // This is just too cool not to do!!
 eval(StandardTable_writeUpdateForm.toString().replace(/StandardTable/,"CoapTable").replace(/this\.columns\[c\]\.title/,"(this.columns[c].name.match(/_cid/)?(\"<A HREF=\\\"javascript:null;\\\" onClick=\\\"window.open('empSearch.html?targetField=\"+this.columns[c].formName+\"&initial=\"+this.columns[c].formName+\"+is+not+null','empSearch','width=250,height=350,resizable=1,scrollbars=yes'); return false;\\\">\"+this.columns[c].title+\"</A>\"):this.columns[c].title)"));
 this.writeUpdateForm = CoapTable_writeUpdateForm;

}// CorrectiveAction


// Overrides the standard getRows() to do a join to the emp_ident table
// in order to print names instead of CIDs in the record table.
function CorrectiveAction_getRows(whereClause,order)
{
 if (order)
   return (database.superCursor("SELECT NVL(tl.name,team_leader_cid) AS team_leader_cid,NVL(hr.name,hr_cid) AS hr_cid,ca.rec,ca.cid,ca.start_date,end_date,ca.reason,ca.stage,ca.interview_date,ca.review_date,ca.outcome FROM corrective_action ca, emp_ident tl, emp_ident hr WHERE ca.team_leader_cid = tl.cid(+) AND ca.hr_cid = hr.cid(+) AND ca.cid = '"+request.cid+"' ORDER BY ca.start_date"));
 else
   return (database.superCursor("SELECT * FROM "+this.tableName+(whereClause?(" WHERE "+whereClause):"")+(order?(" ORDER BY "+order):"")));
}// CorrectiveAction_getRows
