INSERT INTO jobs SELECT
	j.ref,
	j.author,
	j.contactCID,
	j.advertiseDate,
	j.expiryDate,
	j.contactPerson,
	j.contactAddress,
	j.contactEmail,
	j.contactTel,
	j.contactFax,
	grd.en_text,
	rlc.en_text,
	NULL,
	dep.en_text,
	loc.en_text,
	ctr.en_text,
	ctr2.region,
	fnc.en_text,
	j.en_title,
	j.en_description,
	j.en_experience,
	j.numOfPositions
FROM motRedep.jobs j,
     motRedep.grade grd,
     motRedep.relocation rlc,
     motRedep.sector dep,
     motRedep.function fnc,
     motRedep.location loc,
     motRedep.country ctr,
     country ctr2
WHERE j.gradeID = grd.ID
  AND j.relocationID = rlc.ID
  AND j.sectorID = dep.ID
  AND j.functionID = fnc.ID
  AND j.locationID = loc.ID
  AND loc.countryID = ctr.ID
  AND ctr.en_text = ctr2.name;

INSERT INTO applications SELECT
	a.cid,
	a.ref,
	a.time,
	a.email,
	a.passwd,
	a.name,
	a.position,
	a.monthsInPos,
	a.manager,
	a.grade,
	NULL,
	a.address,
	a.workTel,
	a.homeTel,
	a.education,
	a.career,
	a.textInfo,
	r.en_text,
	i.en_text,
	o.en_text,
	NVL(a.restricted,'N')
FROM motRedep.applications a,
     motRedep.review r,
     motRedep.iStatus i,
     motRedep.oStatus o
WHERE a.reviewID = r.ID
  AND a.iStatusID = i.ID
  AND a.oStatusID = o.ID;


EXIT;

