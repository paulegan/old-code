
CREATE SNAPSHOT ors_reqs AS SELECT
  req.req_num,
  req.title,
  DECODE(LENGTH(NVL(req.sales_pitch,' ')),0,'0',1,'0','1') AS has_sales_pitch,
  DECODE(LENGTH(NVL(req.sales_pitch,' ')),0,SUBSTR(req.description,0,400),1,SUBSTR(req.description,0,400),req.sales_pitch) AS sales_pitch,
  req.description,
  req.skills_required,
  req.motorola_info,
  req.number_of_positions,
  req.low_grade,
  req.high_grade,
  req.sector,
  req.secondary_org,
  req.tertiary_org,
  req.department,
  req.full_or_part_time,
  req.hours_per_week,
  req.shift,
  req.labor_type,
  eeo.description AS eeo,
  rel.description AS relocation,
  loc.description AS location,
  cty.description AS country,
  reg.description AS region,
  NVL(fam.description,'Other') AS job_family,
  req.hiring_mgr_phone,
  req.hiring_mgr_fax,
  DECODE(req.hiring_mgr_core_id,NULL,NULL,req.hiring_mgr_core_id||'@email.mot.com') AS hiring_mgr_email,
  req.hiring_mgr_first_name||' '||
  DECODE(hiring_mgr_mid_init,NULL,'',hiring_mgr_mid_init||' ')||
  req.hiring_mgr_last_name AS hiring_mgr_name,
  NVL(req.advertise_date,NVL(req.approval_date,req.create_date)) AS advertise_date,
  NVL(req.advertise_code,'B') AS advertise_code,
  req.advertise_template
FROM oasis_reqs req, oasis_relocations rel, oasis_locations loc, 
     oasis_countries cty, oasis_regions reg, oasis_job_families fam,
     oasis_eeo eeo, oasis_jobs job
WHERE (
      req.relocation_code = rel.code(+)
  AND req.location = loc.code(+)
  AND loc.country = cty.code(+)
  AND cty.region = reg.code(+)
  AND req.job_code = job.code(+)
  AND job.job_family = fam.code(+)
  AND job.eeo = eeo.code(+)
  AND NVL(req.advertise_date,NVL(req.approval_date,req.create_date)) < sysdate
  AND NVL(req.advertise_code,'B') IN ( 'I','B' )
  AND req.status = 'O'
);


CREATE TABLE ors_resumes (
        cid			VARCHAR2(8) NOT NULL PRIMARY KEY,
	last_update_date	DATE NOT NULL,
        passwd			VARCHAR2(200),
        email			VARCHAR2(200),
        name			VARCHAR2(200) NOT NULL,
        position		VARCHAR2(200) NOT NULL,
        months_in_pos		VARCHAR2(200) NOT NULL,
        manager			VARCHAR2(200) NOT NULL,
        grade			VARCHAR2(200) NOT NULL,
        address         	VARCHAR2(200) NOT NULL,
        work_tel		VARCHAR2(200) NOT NULL,
        home_tel		VARCHAR2(200) NOT NULL,
        education		VARCHAR2(2000) NOT NULL,
        career			VARCHAR2(2000) NOT NULL,
        additional_info		VARCHAR2(2000),
	application_count	NUMBER NOT NULL,
	can_continue		CHAR(1) NOT NULL
);


CREATE TABLE ors_applications (
	cid			VARCHAR2(8) NOT NULL,
	req_num			VARCHAR2(10) NOT NULL REFERENCES oasis_reqs(req_num),
	mngr_review_status	VARCHAR2(100) NOT NULL,
	interview_status	VARCHAR2(100) NOT NULL,
	offer_status		VARCHAR2(100) NOT NULL,
        PRIMARY KEY ( cid, req_num )
);
