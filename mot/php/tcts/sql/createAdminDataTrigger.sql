-- This trigger makes sure that a transaction is inserted with 
-- the correct values for the HR_ADMINs location and country.
create or replace trigger set_admin_data_BIR_TRG
before insert 
on transactions_tb
for each row
BEGIN
  if ( :new.hr_admin_location IS NULL ) then
     select location into :new.hr_admin_location 
     from all_twin where cid=:new.hr_admin;
  end if;

  if ( :new.hr_admin_country IS NULL ) then
     select work_country into :new.hr_admin_country
     from all_twin where cid=:new.hr_admin;
  end if;

end;
/
create or replace trigger set_batches_admin_BIR_TRG
before insert
on batches_tb
for each row
BEGIN
  if ( :new.hr_admin_location IS NULL ) then
     select location into :new.hr_admin_location
     from all_twin where cid=:new.hr_admin;
  end if;

  if ( :new.hr_admin_country IS NULL ) then
     select work_country into :new.hr_admin_country
     from all_twin where cid=:new.hr_admin;
  end if;

end;

