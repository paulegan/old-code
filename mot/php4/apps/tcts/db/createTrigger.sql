
/*
 *******************************************************************************
 *
 *  File:		$Source$
 *
 *  Description:	SQL script for creating trigger for use with transaction
 *			loads.
 *
 *  Created:		2001/01/25
 *  Created By:		Joel Pinckheard
 *
 *  Last Edited:	$Date$
 *  Edited By:		$Author$
 *
 *  ID:			$Id$
 *
 *  History:		$Log$
 *  History:		Revision 1.1  2001/01/25 20:39:17  eganp
 *  History:		Initial import
 *  History:
 *
 *******************************************************************************
 */

CREATE OR REPLACE TRIGGER set_trans_id
BEFORE INSERT OR UPDATE OF trans_id ON trans_tb
FOR EACH ROW
BEGIN
  IF (:new.trans_id IS NULL) THEN
     SELECT 'TJ'||trans_id_seq.nextVal INTO :new.trans_ID FROM dual;
  END IF;
END;
/
