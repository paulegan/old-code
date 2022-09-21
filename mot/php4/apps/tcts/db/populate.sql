
INSERT INTO center SELECT
 name
FROM center_codes;

INSERT INTO type SELECT
 t.name,c.name
FROM transaction_codes t, center_codes c
WHERE t.country = c.code;

INSERT INTO reject_reason SELECT
 r.name,c.name
FROM rejection_codes r, center_codes c
WHERE r.country = c.code;

INSERT INTO submission_type SELECT
 s.name,c.name
FROM sending_codes s, center_codes c
WHERE s.country = c.code;

INSERT INTO status SELECT
 DISTINCT status,status,name
FROM trans, center;

UPDATE transactions SET
 trans_id = REPLACE(trans_id,'undefined','TN')
WHERE trans_id LIKE 'undefined%';

INSERT INTO trans SELECT 
 t.trans_id,
 NULL,
 t.prev_trans_id,
 t.emp_cid,
 NVL(type.name,'UNKNOWN'),
 DECODE(t.status,'C','Closed',
                 'D','Deleted',
                 'O','Open',
                 'R','Rejected',
                 'Deleted'),
 NVL(center.name,'UK and Ireland'),
 NVL(closed_by.email_id,'UNKNOWN'),
 NVL(hr_admin.email_id,t.hr_admin),
 t.trans_date,
 t.effective_date,
 t.approved_date,
 t.close_date,
 NULL,
 reject_reason.name
FROM transactions t,
     transaction_codes type,
     center_codes center,
     rejection_codes reject_reason,
     all_twin hr_admin,
     all_twin closed_by
WHERE t.trans_type = type.code (+) AND t.trans_center = type.country (+)
  AND t.reject_reason = reject_reason.code (+) AND t.trans_center = reject_reason.country (+)
  AND t.hr_admin = hr_admin.cid (+)
  AND t.closed_by = closed_by.cid (+)
  AND t.trans_center = center.code (+);

INSERT INTO trans SELECT 
 b.trans_id,
 b.batch_id,
 NULL,
 b.emp_cid,
 NVL(type.name,'UNKNOWN'),
 'Pending',
 NVL(center.name,'UK and Ireland'),
 'UNKNOWN',
 t.email_id,
 b.trans_date,
 b.effective_date,
 b.approved_date,
 NULL,
 NULL,
 NULL
FROM batches b,
     transaction_codes type,
     center_codes center,
     all_twin t
WHERE b.trans_type = type.code (+) AND b.trans_center = type.country (+)
  AND b.hr_admin = t.cid (+)
  AND b.trans_center = center.code (+);


INSERT INTO perm_operation VALUES ( 'view' );
INSERT INTO perm_operation VALUES ( 'create' );
INSERT INTO perm_operation VALUES ( 'delete' );
INSERT INTO perm_operation VALUES ( 'update' );
INSERT INTO perm_operation VALUES ( 'submit batch' );
INSERT INTO perm_operation VALUES ( 'manage users' );
INSERT INTO perm_operation VALUES ( 'manage lists' );

INSERT INTO perm
SELECT DISTINCT UPPER(p.userid), INITCAP(t.full_name),
       o.name, NVL(c.name,'UK and Ireland')
  FROM wwwauth.userid_to_group p, all_twin t, center_codes c, perm_operation o
 WHERE UPPER(p.userid) = t.email_id
   AND t.work_country = c.code (+)
   AND p.group_name LIKE 'TCTS_Admin'
   AND o.name IN ( 'create','view','update','delete' )
UNION
SELECT DISTINCT UPPER(p.userid), INITCAP(t.full_name),
       o.name, NVL(c.name,'UK and Ireland')
  FROM wwwauth.userid_to_group p, all_twin t, center_codes c, perm_operation o
 WHERE UPPER(p.userid) = t.email_id
   AND t.work_country = c.code (+)
   AND p.group_name LIKE 'TCTS'
   AND o.name IN ( 'create','view','update' )
UNION
SELECT DISTINCT NVL(t.email_id,h.cid), INITCAP(t.full_name),
       o.name, NVL(c.name,'UK and Ireland')
  FROM hr_admins h, all_twin t, center_codes c, perm_operation o
 WHERE h.cid = t.cid
   AND h.country = c.code
   AND o.name IN ( 'submit batch','view' );

INSERT INTO perm
SELECT t.email_id, INITCAP(t.full_name), o.name, d.name
 FROM all_twin t, perm_operation o, perm_dataset d
WHERE o.name LIKE 'manage%'
  AND t.email_id in ( 'QCOR358','R35644' );
