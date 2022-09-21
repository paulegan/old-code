<?php

 ///////////////////////////////////////////////////////////////////////////////
 // 
 // File:		$Source$
 // 
 // Description:	Some configuration elements for the COAP app.
 // 
 // Created:		2001/01/29
 // Created By:		Paul Egan
 // 
 // Last Edited:	$Date$
 // Edited By:		$Author$
 // 
 // ID:			$Id$
 // 
 // History:		$Log$
 // History:		Revision 1.1  2001/02/01 16:30:54  eganp
 // History:		Simple systax conversion of js files from version 1
 // History:		  Yet to be even parsed.  To do -
 // History:		
 // History:		Display of correct template determined in main
 // History:		Move of code from original html files to main
 // History:		Convert db handling from cursors to PHP db.
 // History:		$form (context['form']) and $db (context['db']) need to be
 // History:		 passed around.  Originally SSJS database & request objects
 // History:		 were global.
 // History:		Client side funcs need to be stored in client js file.
 // History:		Move from prints to return of html string to be inserted in
 // History:		 to templates
 // History:
 // 
 ///////////////////////////////////////////////////////////////////////////////

 define('APPLICATION_NAME','coap');
 define('DB_USER','coap');
 define('DB_PASSWD','c0ap$d$');

 $TABS = array(
  //	    name			title			acronym	writeBody
  new Tab ( 'initiative_training',	'Initiative Training',	'IT',	'writeCoapTab' ),
  new Tab ( 'skill_sets',		'Skill Sets',		'SS',	'writeCoapTab' ),
  new Tab ( 'corrective_action',	'Corrective Action',	'CA',	'writeCoapTab' ),
  new Tab ( 'super_user',		'Admin',		'ADM',	'writeAdminTab' )
 );

 $ADMIN_TABLES = array(
   new AdminTable ( 'user_access',	'Access Control' ),
   new AdminTable ( 'it_course',	'Initiative Training Courses' ),
   new AdminTable ( 'ss_name',		'Skill Set Cert Names' ),
   new AdminTable ( 'ss_status',	'Skill Set Status' ),
   new AdminTable ( 'ss_recert',	'Skill Set Recertification' ),
   new AdminTable ( 'ca_reason',	'Corrective Action Reasons' ),
   new AdminTable ( 'ca_stage',		'Corrective Action Stages' ),
   new AdminTable ( 'ca_outcome',	'Corrective Action Outcomes' )
 );
 
 $TABLE_DEF = array();
 $TABLE_DEF['initiative_training'] = array(
  //		   col name	       form name       null?   type	       len     VOC     title	       display munge
  new ColumnSpec ( 'rec',	       'rec',	       false,  'number',       8,      false,  'Record Number',	false, 0 ),
  new ColumnSpec ( 'cid',	       'cid',	       false,  'text',         8,      true,   'CID',	       false,  0 ),
  new ColumnSpec ( 'start_date',       'startDate',    false,  'date',         11,     true,   'Start Date',   true,   'mungeDate' ),
  new ColumnSpec ( 'end_date',         'endDate',      true,   'date',         11,     true,   'End Date',     true,   'mungeDate' ),
  new ColumnSpec ( 'course_name',      'courseName',   false,  'list',         50,     false,  'Course Name',  true,   0 ),
  new ColumnSpec ( 'duration',         'duration',     false,  'number',       2,      true,   'Duration',     true,   'mungeDuration' )
 );
 $TABLE_DEF['skill_sets'] = array(
  //		   col name	       form name       null?   type	       len     VOC     title	       display munge
  new ColumnSpec ( 'rec',	       'rec',	       false,  'number',       8,      false,  'Record Number',	false, 0 ),
  new ColumnSpec ( 'cid',	       'cid',	       false,  'text',         8,      true,   'CID',	       false,  0 ),
  new ColumnSpec ( 'start_date',       'startDate',    false,  'date',         11,     true,   'Start Date',   true,   'mungeDate' ),
  new ColumnSpec ( 'end_date',         'endDate',      true,   'date',         11,     true,   'End Date',     true,   'mungeDate' ),
  new ColumnSpec ( 'cert_name',        'certName',     false,  'list',         50,     false,  'Cert Name',    true,   0 ),
  new ColumnSpec ( 'cert_status',      'certStatus',   false,  'list',         10,     false,  'Status',       true,   0 ),
  new ColumnSpec ( 'cert_date',        'certDate',     false,  'date',         11,     true,   'Cert Date',    true,   'mungeDate' ),
  new ColumnSpec ( 'recert',	       'recert',       false,  'list',         20,     false,  'Recert?',      true,   'mungeRecert' ),
  new ColumnSpec ( 'hours',	       'hours',        false,  'number',       2,      true,   'Duration',     true,   'mungeDuration' )
 );
 $TABLE_DEF['corrective_action'] = array(
  //		   col name	       form name       null?   type	       len     VOC     title	       display munge
  new ColumnSpec ( 'rec',	       'rec',	       false,  'number',       8,      false,  'Record Number',	false, 0 ),
  new ColumnSpec ( 'cid',	       'cid',	       false,  'text',         8,      true,   'CID',	       false,  0 ),
  new ColumnSpec ( 'start_date',       'startDate',    false,  'date',         11,     true,   'Start Date',   true,   'mungeDate' ),
  new ColumnSpec ( 'end_date',         'endDate',      true,   'date',         11,     true,   'End Date',     true,   'mungeDate' ),
  new ColumnSpec ( 'reason',	       'reason',       false,  'list',         20,     false,  'Reason',       true,   0 ),
  new ColumnSpec ( 'stage',	       'stage',        false,  'list',         10,     false,  'Stage',        true,   0 ),
  new ColumnSpec ( 'team_leader_cid',  'manager',      true,   'text',         8,      true,   'Team Leader',  true,   0 ),
  new ColumnSpec ( 'hr_cid',	       'hr',	       true,   'text',         8,      true,   'HR',	       true,   0 ),
  new ColumnSpec ( 'interview_date',   'interviewDate', false, 'date',         11,     true,   'Interview Date', true, 'mungeDate' ),
  new ColumnSpec ( 'review_date',      'reviewDate',   false,  'date',         11,     true,   'Review Date',  true,   'mungeDate' ),
  new ColumnSpec ( 'outcome',	       'outcome',      false,  'list',         20,     false,  'Outcome',      true,   0 )
 );
  //		   col name	       form name       null?   type	       len     VOC     title	       display munge
 $TABLE_DEF['it_course'] = array(
  new ColumnSpec ( 'name',	       'name',	       false,  'text',         50,     true,   'Course Name',  true,   0 ),
  new ColumnSpec ( 'duration',	       'duration',     true,   'number',       2,      false,  'Duration',     false,  0 )
 );
 $TABLE_DEF['ss_name'] = array(
  new ColumnSpec ( 'name',	       'name',	       false,  'text',         50,     true,   'Skill Set Name',  true, 0 )
 );
 $TABLE_DEF['ss_status'] = array(
  new ColumnSpec ( 'name',	       'name',	       false,  'text',         10,     true,   'Status',       true,   0 )
 );
 $TABLE_DEF['ss_recert'] = array(
  new ColumnSpec ( 'name',	       'name',	       false,  'text',         20,     true,   'Recert?',      true,   0 ),
  new ColumnSpec ( 'hours',	       'hours',        true,   'number',       2,      false,  'Duration',     false,  0 )
 );
 $TABLE_DEF['ca_reason'] = array(
  new ColumnSpec ( 'name',	       'name',	       false,  'text',         20,     true,   'Reason',       true,   0 )
 );
 $TABLE_DEF['ca_stage'] = array(
  new ColumnSpec ( 'name',	       'name',	       false,  'text',         10,     true,   'Stage',        true,   0 )
 );
 $TABLE_DEF['ca_outcome'] = array(
  new ColumnSpec ( 'name',	       'name',	       false,  'text',         20,     true,   'Outcome',      true,   0 )
 );
 $TABLE_DEF['user_access'] = array(
  new ColumnSpec ( 'user_id',		'user_id',      false,  'text',         8,      true,   'User ID',      true,   0 ),
  new ColumnSpec ( 'initiative_training','initiative_training',true,  'text',	1,      false,   'initiative_training',      true,   0 ),
  new ColumnSpec ( 'skill_sets',	'skill_sets',      true,  'text',	1,      false,   'skill_sets',      true,   0 ),
  new ColumnSpec ( 'corrective_action',	'corrective_action',true,  'text',	1,      false,   'corrective_action',      true,   0 ),
  new ColumnSpec ( 'super_user',	'super_user',      true,  'text',	1,      false,   'super_user',      true,   0 )
 );

 $COL_2_TABLE = array();
 $COL_2_TABLE['initiative_training.course_name']	= 'it_course';
 $COL_2_TABLE['skill_sets.cert_name']			= 'ss_name';
 $COL_2_TABLE['skill_sets.cert_status']			= 'ss_status';
 $COL_2_TABLE['skill_sets.recert']			= 'ss_recert';
 $COL_2_TABLE['corrective_action.reason']		= 'ca_reason';
 $COL_2_TABLE['corrective_action.stage']		= 'ca_stage';
 $COL_2_TABLE['corrective_action.outcome']		= 'ca_outcome';

?>
