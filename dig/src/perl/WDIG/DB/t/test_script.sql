
/* a test script */

if exists (select name from sysobjects where name = 'lumpy_test' and type = 'U')
  drop table lumpy_test
go

create table [dbo].[lumpy_test] (
  id int not null, -- some inline comments
  name nvarchar(20)
)
go

insert into lumpy_test values ( 1, 'one' );
insert into lumpy_test values ( 2, 'two' );
go

if exists (select name from sysobjects where name = 'sp_lumpy_test' and type = 'P')
  drop procedure sp_lumpy_test
go

/*
 * this is a test stored proc
 */

create procedure dbo.sp_lumpy_test 
 @col_name sysname,
 @num_rows int output
as
begin

 select @num_rows = count(@col_name) from dbo.lumpy_test

end

go

declare @num_rows int
execute sp_lumpy_test 'name', @num_rows output

if ( @num_rows <> 2 )
  raiserror('row count not as expected!', 16, -1)

go

drop procedure sp_lumpy_test
drop table lumpy_test
go
