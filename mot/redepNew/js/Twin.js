
//  Author:             paul@hr.emea.mot.com
//  DevDate:            19971208
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


function Twin()
{
 this.twinTable = "all_twin";
 Twin.prototype.name2cids = Twin_name2cids;
 Twin.prototype.name2emailIDs = Twin_name2emailIDs;
 Twin.prototype.cid2record = Twin_cid2record;
 Twin.prototype.emailID2record = Twin_emailID2record;
}// Twin


function Twin_name2cids(name)
{
 var cursor;
 var fullName = name.toUpperCase().replace(/[' ]/g,"%");
 var selectStmt = "SELECT cid,INITCAP(full_name) AS name FROM "+this.twinTable+" WHERE full_name LIKE ";

 cursor = database.superCursor(selectStmt+"'"+fullName+"' ORDER BY name");
 if (!cursor.next()){
   cursor.close();
   cursor = database.superCursor(selectStmt+"'%"+fullName+"%' ORDER BY name");
   cursor.next();
 }

 return cursor;
}// Twin_name2cids


function Twin_name2emailIDs(name)
{
 var cursor;
 var fullName = name.toUpperCase().replace(/[' ]/g,"%");
 var selectStmt = "SELECT email_id,INITCAP(full_name) AS name FROM "+this.twinTable+" WHERE full_name LIKE ";

 cursor = database.superCursor(selectStmt+"'"+fullName+"' ORDER BY name");
 if (!cursor.next()){
   cursor.close();
   cursor = database.superCursor(selectStmt+"'%"+fullName+"%' ORDER BY name");
   cursor.next();
 }

 return cursor;
}// Twin_name2emailIDs


// Function that returns an object with some TWIN data for
// a CID.  Returns NULL is there's no TWIN record for the CID given.
function Twin_cid2record(cid)
{
 var temp = database.superCursor("SELECT twin.cid, INITCAP(twin.full_name) AS name, twin.email_id AS email, twin.phone_num AS work_tel, CEIL((sysDate-twin.start_date)/30) AS months_in_pos, INITCAP(twinS.full_name)||' - '||twinS.cid AS manager FROM "+this.twinTable+" twin, "+this.twinTable+" twinS WHERE twin.scid = twinS.cid(+) AND twin.cid ='"+cid+"'");

 if (!temp.next()){
    temp.close();
    return (null);
 }

 return (temp);
}// Twin_cid2record


// Function that returns an object with some TWIN data for
// a CID.  Returns NULL is there's no TWIN record for the CID given.
function Twin_emailID2record(emailID)
{
 var temp = database.superCursor("SELECT twin.cid, INITCAP(twin.full_name) AS name, twin.email_id AS email, twin.phone_num AS work_tel, CEIL((sysDate-twin.start_date)/30) AS months_in_pos, INITCAP(twinS.full_name)||' - '||twinS.cid AS manager FROM "+this.twinTable+" twin, "+this.twinTable+" twinS WHERE twin.scid = twinS.cid(+) AND twin.email_id ='"+emailID.toUpperCase()+"'");

 if (!temp.next()){
    temp.close();
    return (null);
 }

 return (temp);
}// Twin_emailID2record
