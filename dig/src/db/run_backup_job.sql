
use master

if exists (select * from sysobjects where id = object_id('sp_run_backup_job')
 and objectproperty(id, 'IsProcedure') = 1)
drop procedure sp_run_backup_job
go

/*
 * sp_run_backup_job - executes the backup job for the given database.
 * The procedure sleeps for a bit before checking the job result.  Its 
 * possible that the job will not have completed at this time and the
 * result of the previous job will be used!  Returns 0 on success and
 * 1 on failure.
 *
 * paul.x.egan.-nd@email.disney.com 20030822
 *
 */

create procedure dbo.sp_run_backup_job 
 @db_name sysname = null
as
begin

 declare @job uniqueidentifier
 declare @max_runtime int
 declare @message nvarchar(1024)
 declare @status int

 if ( @db_name is null )
   set @db_name = db_name()

 select @job = msdb..sysjobs.job_id from msdb..sysjobs, msdb..sysdbmaintplan_databases, msdb..sysdbmaintplan_jobs
  where database_name = @db_name
    and msdb..sysdbmaintplan_jobs.plan_id = msdb..sysdbmaintplan_databases.plan_id
    and msdb..sysjobs.job_id = msdb..sysdbmaintplan_jobs.job_id
    and msdb..sysjobs.name like 'DB Backup%'

 if ( @job is null ) begin
   raiserror('No backup job found for "%s"', 16, -1, @db_name)
   return (1)
 end

 select @max_runtime = max(run_duration)+1 from msdb..sysjobhistory
  where job_id = @job
    and step_id = 0

 execute @status = msdb..sp_start_job @job_id = @job, @output_flag = 0

 if ( @status <> 0 )
   return (@status)

 waitfor delay @max_runtime

 select top 1 @status = run_status, @message = message from msdb..sysjobhistory
  where job_id = @job
    and step_id = 0
  order by run_date desc, run_time desc

 if ( @status <> 1 ) begin
   raiserror(@message, 16, -1)
   return (1)
 end
 else begin
   raiserror(@message, -1, -1)
   return (0)
 end

end
go

if not exists ( select name from sysusers where name = 'run_backup_job' and issqlrole = 1 )
  execute sp_addrole 'run_backup_job'

grant execute on dbo.sp_run_backup_job to run_backup_job
grant execute on dbo.xp_sqlmaint to run_backup_job

go
