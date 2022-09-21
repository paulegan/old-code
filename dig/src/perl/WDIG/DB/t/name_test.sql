
declare @server nvarchar(40)
select @server = convert(nvarchar(40), serverproperty('servername'))
raiserror(@server, 16, -1)

