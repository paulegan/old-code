------------------------------------------------------------------------------------------
-- Filename    :  MS-OLE_SPs.sql
-- Description :  Creates the two "OLE Automation Return Codes and Error Information"
--                srored procedures referenced under Books On-Line
--                Please note this script has been produced directly from Microsoft SQL 
--                Server Books Online (v7). All rights Acknowledged. 
------------------------------------------------------------------------------------------

-- Downloaded from http://www.swynk.com/friends/green


/****** Object:  Stored Procedure dbo.sp_hexadecimal ******/
if exists (select * from sysobjects where id = object_id(N'[dbo].[sp_hexadecimal]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[sp_hexadecimal]
GO

/****** Object:  Stored Procedure dbo.sp_displayoaerrorinfo ******/
if exists (select * from sysobjects where id = object_id(N'[dbo].[sp_displayoaerrorinfo]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[sp_displayoaerrorinfo]
GO

SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

/****** Object:  Stored Procedure dbo.sp_hexadecimal ******/

CREATE PROCEDURE sp_hexadecimal

    @binvalue varbinary(255),

    @hexvalue varchar(255) OUTPUT

AS

DECLARE @charvalue varchar(255)

DECLARE @i int

DECLARE @length int

DECLARE @hexstring char(16)

SELECT @charvalue = '0x'

SELECT @i = 1

SELECT @length = DATALENGTH(@binvalue)

SELECT @hexstring = '0123456789abcdef'

WHILE (@i <= @length)

BEGIN

DECLARE @tempint int

DECLARE @firstint int

DECLARE @secondint int

SELECT @tempint = CONVERT(int, SUBSTRING(@binvalue,@i,1))

SELECT @firstint = FLOOR(@tempint/16)

SELECT @secondint = @tempint - (@firstint*16)

SELECT @charvalue = @charvalue +

SUBSTRING(@hexstring, @firstint+1, 1) +

SUBSTRING(@hexstring, @secondint+1, 1)

SELECT @i = @i + 1

END

SELECT @hexvalue = @charvalue


GO

SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO

/****** Object:  Stored Procedure dbo.sp_displayoaerrorinfo ******/

CREATE PROCEDURE sp_displayoaerrorinfo

    @object int,

    @hresult int

AS

DECLARE @output varchar(255)

DECLARE @hrhex char(10)

DECLARE @hr int

DECLARE @source varchar(255)

DECLARE @description varchar(255)

PRINT 'OLE Automation Error Information'

EXEC sp_hexadecimal @hresult, @hrhex OUT

SELECT @output = '  HRESULT: ' + @hrhex

PRINT @output

EXEC @hr = sp_OAGetErrorInfo @object, @source OUT, @description OUT

IF @hr = 0

BEGIN

SELECT @output = '  Source: ' + @source

PRINT @output

SELECT @output = '  Description: ' + @description

PRINT @output

END

ELSE

BEGIN

    PRINT "  sp_OAGetErrorInfo failed."

    RETURN

END


GO

SET QUOTED_IDENTIFIER  OFF    SET ANSI_NULLS  ON 
GO



/****************************************************************************************/
/* Filename: 		spDisplayPKGErrors.sql						*/
/* Description:		Evauluate Steps for Pkg and get results. 			*/
/*			(Original by Bill Hoghead, via Neil Pike's FAQ)			*/
/* Created:		Darren Green 200006013						*/
/****************************************************************************************/

if exists (SELECT * from dbo.sysobjects where id = object_id(N'[dbo].[sp_displayPKGErrors]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
drop procedure [dbo].[sp_displayPKGErrors]
GO

CREATE PROC sp_displayPKGErrors
	@oPkg As integer
AS
SET NOCOUNT ON

DECLARE @StepCount int
DECLARE @Steps int
DECLARE @Step int
DECLARE @StepResult int
DECLARE @oPkgResult int
DECLARE @hr int

DECLARE @StepName varchar(255)
DECLARE @StepDescription varchar(255)

	IF OBJECT_ID('tempdb..#PkgResult') IS NOT NULL
	    DROP TABLE #PkgResult 

	CREATE TABLE #PkgResult
	(
	  StepName varchar(255) NOT NULL,
	  StepDescription varchar(255) NOT NULL,
	  Result bit NOT NULL
	)

	SELECT @oPkgResult = 0

	EXEC @hr = sp_OAGetProperty @oPkg, 'Steps', @Steps OUTPUT
	IF @hr <> 0
	BEGIN
	    PRINT '***  Unable to get steps'
	    EXEC sp_displayoaerrorinfo @oPkg , @hr
	    RETURN 1
	END

	EXEC @hr = sp_OAGetProperty @Steps, 'Count', @StepCount OUTPUT
	IF @hr <> 0
	BEGIN
	    PRINT '***  Unable to get number of steps'
	    EXEC sp_displayoaerrorinfo @Steps , @hr
	    RETURN 1
	END
	
	WHILE @StepCount > 0 
	BEGIN
		EXEC @hr = sp_OAGetProperty @Steps, 'Item', @Step OUTPUT, @StepCount
		IF @hr <> 0
		BEGIN
		    PRINT '***  Unable to get step'
		    EXEC sp_displayoaerrorinfo @Steps , @hr
		    RETURN 1
		END
	
		EXEC @hr = sp_OAGetProperty @Step, 'ExecutionResult', @StepResult OUTPUT
		IF @hr <> 0
		BEGIN
		    PRINT '***  Unable to get ExecutionResult'
		    EXEC sp_displayoaerrorinfo @Step , @hr
		    RETURN 1
		END


		EXEC @hr = sp_OAGetProperty @Step, 'Name', @StepName OUTPUT
		IF @hr <> 0
		BEGIN
		    PRINT '***  Unable to get step Name'
		    EXEC sp_displayoaerrorinfo @Step , @hr
		    RETURN 1
		END

		EXEC @hr = sp_OAGetProperty @Step, 'Description', @StepDescription OUTPUT
		IF @hr <> 0
		BEGIN
		    PRINT '***  Unable to get step Description'
		    EXEC sp_displayoaerrorinfo @Step , @hr
		    RETURN 1
		END

		INSERT #PkgResult VALUES(@StepName, @StepDescription, @StepResult)
		PRINT 'Step ' + @StepName + ' (' + @StepDescription + ') ' + CASE WHEN @StepResult = 0 THEN 'Succeeded' ELSE 'Failed' END

		SELECT @StepCount = @StepCount - 1
		SELECT @oPkgResult = @oPkgResult + @StepResult
	END

	SELECT * FROM #PkgResult
	
	IF @oPkgResult > 0
	BEGIN
		PRINT 'Package had ' + CAST(@oPkgResult as varchar) + ' failed step(s)'
		RETURN 9
	END
	ELSE 
	BEGIN
		PRINT 'Packge Succeeded'
		RETURN 0
	END



GO

/****************************************************************************************/
/* Filename: 		spExecutePKG.sql						*/
/* Description:		Execute DTS Packages via sp_OAxxxx				*/
/* Created:		Darren Green 200006013						*/
/****************************************************************************************/

if exists (select * from sysobjects where id = object_id(N'[dbo].[sp_execDTS]') and OBJECTPROPERTY(id, N'IsProcedure') = 1)
    drop procedure [dbo].[sp_execDTS]
GO

CREATE PROC sp_execDTS
	@Server varchar(255),
	@PkgName varchar(255), 			-- Package Name (Defaults to most recent version)
	@ServerPWD varchar(255) = Null,		-- Server Password if using SQL Security to load Package (UID is SUSER_NAME())
	@IntSecurity bit = 0,			-- 0 = SQL Server Security, 1 = Integrated Security
	@PkgPWD varchar(255) = ''		-- Package Password
AS
SET NOCOUNT ON
/*
  Return Values
  - 0 Successfull execution of Package
  - 1 OLE Error
  - 9 Failure of Package
*/
DECLARE @hr int, @ret int, @oPKG int, @Cmd varchar(1000)

	-- Create a Pkg Object
	EXEC @hr = sp_OACreate 'DTS.Package', @oPKG OUTPUT 
	IF @hr <> 0
	BEGIN
		PRINT '***  Create Package object failed'
		EXEC sp_displayoaerrorinfo @oPKG, @hr
		RETURN 1
	END	

	-- Evaluate Security and Build LoadFromSQLServer Statement
	IF @IntSecurity = 0 
		SET @Cmd = 'LoadFromSQLServer("' + @Server +'", "' + SUSER_SNAME() + '", "' + @ServerPWD + '", 0, "' + @PkgPWD + '", , , "' + @PkgName + '")'
	ELSE
		SET @Cmd = 'LoadFromSQLServer("' + @Server +'", "", "", 256, "' + @PkgPWD + '", , , "' + @PkgName + '")'
	
	EXEC @hr = sp_OAMethod @oPKG, @Cmd, NULL

	IF @hr <> 0
	BEGIN
	    PRINT '***  LoadFromSQLServer failed'
	    EXEC sp_displayoaerrorinfo @oPKG , @hr
	    RETURN 1
	END

	-- Execute Pkg
	EXEC @hr = sp_OAMethod @oPKG, 'Execute'
	IF @hr <> 0
	BEGIN
	    PRINT '***  Execute failed'
	    EXEC sp_displayoaerrorinfo @oPKG , @hr
	    RETURN 1
	END

	-- Check Pkg Errors
	EXEC @ret=sp_displayPkgErrors @oPKG

	-- Unitialize the Pkg
	EXEC @hr = sp_OAMethod @oPKG, 'UnInitialize'
	IF @hr <> 0
	BEGIN
	    PRINT '***  UnInitialize failed'
	    EXEC sp_displayoaerrorinfo @oPKG , @hr
	    RETURN 1
	END

	-- Clean Up
	EXEC @hr = sp_OADestroy @oPKG
	IF @hr <> 0
	BEGIN
		EXEC sp_displayoaerrorinfo @oPKG , @hr
		RETURN 1
	END

	RETURN @ret
GO