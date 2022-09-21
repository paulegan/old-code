//--file: CTime.h----------------------------------------------------
//
// Generalized,  portable date/time class used to generate local and 
// GMT time now,  in the past,  and in the future.  The past and 
// future date/times are generated as deltas from now of n TimeUnits,  
// where n is an unsigned integer and TineUnits is one of:
//
// SECOND, SECONDS, MINUTE, MINUTES, HOUR, HOURS, DAY, DAYS, 
// WEEK, WEEKS, MONTH, MONTHS, YEAR, YEARS} timeUnit
//
// See the 2 test functions at the bottom for usage.
//
#include "CTime.h"
#include <time.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

static const char aDayOfWeek[7][4]  = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char aMonOfYear[12][4] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

static const char aFullDayOfWeek[7][10]  = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };
static const char aFullMonOfYear[12][10] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };

static const int  cDaysInMon[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }; 
static const char szTimeUnits[14][5] = { "Sec", "Min", "Hour", "Day", "Week", "Mon", "Year" };


// int tm_sec;     /* seconds after the minute - [0,59] */
// int tm_min;     /* minutes after the hour - [0,59]   */
// int tm_hour;    /* hours since midnight - [0,23]     */
// int tm_mday;    /* day of the month - [1,31]         */
// int tm_mon;     /* months since January - [0,11]     */
// int tm_year;    /* years since 1900                  */
				   /* Note that this is a 3 digit number Y2K and after */

// int tm_wday;    /* days since Sunday - [0,6]         */
// int tm_yday;    /* days since January 1 - [0,365]    */
// int tm_isdst;   /* daylight savings time flag        */


CTime::CTime()
{
	m_cSec=99;
	m_cMin=99;
	m_cHour=99;
	m_cDay=99;
	m_cMonth=99;
	m_cYear=99;

	memset( m_szDay, '\0', 16 );
	memset( m_szMonth, '\0', 16 );
	memset( m_szFullDay, '\0', 16 );
	memset( m_szFullMonth, '\0', 16 );
	
	return;
}


/** *****************************************************************
 *
 * CTime::getLocalDateTime      Get the local time now.
 *
 **/
bool CTime::getLocalDateTime( char * psz, unsigned int cSize )
{
	return ( getLocalDateTime( 0L, psz, cSize ) );
}

/** *****************************************************************
 *
 * CTime::getLocalDateTimePlus      Get the local time in the future.
 *
 **/
bool CTime::getLocalDateTimePlus( unsigned int n, timeUnit tu, char * psz, unsigned int cSize ) 
{
	return ( getLocalDateTime( getSeconds(n,tu), psz, cSize ) );
}

/** *****************************************************************
 *
 * CTime::getLocalDateTimeMinus     Get the local time in the past.
 *
 **/
bool CTime::getLocalDateTimeMinus( unsigned int n, timeUnit tu, char * psz, unsigned int cSize )
{
	return ( getLocalDateTime( (-1*getSeconds(n,tu)), psz, cSize ) );
}


/** *****************************************************************
 *
 * CTime::getLocalDateTime      Generate a local time given an offset 
 *                          (positive offset indicates the time will be 
 *                          in the future,  negative offset indicates 
 *                          the time will be in the past).
 **/

// Y2K OK
// Literal 1900 is OK, tm_year is years since 1900. 
// format %02u is OK as well (no overflow)
// -jam	(8/2/99)
 
bool CTime::getLocalDateTime( long l, char * psz, unsigned int cSize )
{
	char sz1[64] = {'\0'};
	struct tm * pt = 0;
	long ltime;
	unsigned int dIdx, mIdx;


	/** Get the time */
	time( &ltime );
	ltime += l;
	
	#ifdef __OS_SOLARIS__
	
		tm _tm;
		pt = localtime_r( &ltime, &_tm );	// multithreaded version
	
	#elif defined __OS_WINNT__
	
		pt = localtime( &ltime );
	
	#endif


	/** Set member variables */
	if ( 0 != pt )
	{
		m_cSec = pt->tm_sec;	m_cMin = pt->tm_min;	m_cHour = pt->tm_hour;
		m_cDay = pt->tm_mday;	m_cMonth = pt->tm_mon;	m_cYear = pt->tm_year+1900;

		dIdx = ( pt->tm_wday>=0 && pt->tm_wday<7) ? (pt->tm_wday) : (pt->tm_wday%7);
		mIdx = ( pt->tm_mon>=0  && pt->tm_mon<12) ? (pt->tm_mon)  : (pt->tm_mon%12);

		sprintf ( m_szDay,       "%s", aDayOfWeek[dIdx] );
		sprintf ( m_szFullDay,   "%s", aFullDayOfWeek[dIdx] );
		
		sprintf ( m_szMonth,     "%s", aMonOfYear[mIdx] );
		sprintf ( m_szFullMonth, "%s", aFullMonOfYear[mIdx] );

		if ( 0 != psz )
		{
			sprintf( sz1, "%s, %02u-%s-%02u %02u:%02u:%02u", 	
				aDayOfWeek[pt->tm_wday], 
				pt->tm_mday, aMonOfYear[pt->tm_mon], pt->tm_year+1900, 
				pt->tm_hour, pt->tm_min, pt->tm_sec );

			sprintf( psz, "%s", sz1 );
		}

		return true;
	}
	else
	{
		if ( 0 != psz )
		{
			sprintf( psz, "N/A, 99-N/A-99 99:99:99" );
		}

		return false;
	}

}







/** *****************************************************************
 *
 * CTime::getGMTDateTime      Get the GMT time now.
 *
 **/
bool CTime::getGMTDateTime( char * psz, unsigned int cSize )
{
	return ( getGMTDateTime( 0L, psz, cSize ) );
}

/** *****************************************************************
 *
 * CTime::getGMTDateTime      Get the GMT time in the future.
 *
 **/
bool CTime::getGMTDateTimePlus( unsigned int n, timeUnit tu, char * psz, unsigned int cSize ) 
{
	return ( getGMTDateTime( getSeconds(n,tu), psz, cSize ) );
}

/** *****************************************************************
 *
 * CTime::getGMDateTTime      Get the GMT time in the past.
 *
 **/
bool CTime::getGMTDateTimeMinus( unsigned int n, timeUnit tu, char * psz, unsigned int cSize ) 
{
	return ( getGMTDateTime( (-1*getSeconds(n,tu)), psz, cSize ) );
}

/** *****************************************************************
 *
 * CTime::getGMTDateTime      Generate a GMT time given an offset 
 *                        (positive offset indicates the time will be 
 *                        in the future,  negative offset indicates 
 *                        the time will be in the past).
 **/

// Y2K OK
// Literal 1900 is OK, tm_year is years since 1900. 
// format %02u is OK as well (no overflow)
// -jam	(8/2/99)
 
bool CTime::getGMTDateTime( long l, char * psz, unsigned int cSize ) 
{
	char sz1[64] = {'\0'};
	struct tm * pt = 0;
	long ltime;
	unsigned int dIdx, mIdx;

		
	/** Get the time */
	time( &ltime );
	ltime += l;
	
	#ifdef __OS_SOLARIS__
	
		tm _tm;
		pt = gmtime_r( &ltime, &_tm );	// multithreaded version
	
	#elif defined__OS_WINNT__
	
		pt = gmtime( &ltime ); 
	
	#endif



	/** Set member variables */
	if ( 0 != pt )
	{
		m_cSec = pt->tm_sec;	m_cMin = pt->tm_min;	m_cHour = pt->tm_hour;
		m_cDay = pt->tm_mday;	m_cMonth = pt->tm_mon;	m_cYear = pt->tm_year+1900;

		dIdx = ( pt->tm_wday>=0 && pt->tm_wday<7) ? (pt->tm_wday) : (pt->tm_wday%7);
		mIdx = ( pt->tm_mon>=0  && pt->tm_mon<12) ? (pt->tm_mon)  : (pt->tm_mon%12);

		sprintf ( m_szDay,       "%s", aDayOfWeek[dIdx] );
		sprintf ( m_szFullDay,   "%s", aFullDayOfWeek[dIdx] );

		sprintf ( m_szMonth,     "%s", aMonOfYear[mIdx] );
		sprintf ( m_szFullMonth, "%s", aFullMonOfYear[mIdx] );

		/** Format the return string? */
		if ( 0 != psz )
		{
			sprintf( sz1, "%s, %02u-%s-%02u %02u:%02u:%02u", 	
				aDayOfWeek[pt->tm_wday], 
				pt->tm_mday, aMonOfYear[pt->tm_mon], pt->tm_year+1900, 
				pt->tm_hour, pt->tm_min, pt->tm_sec );

			sprintf( psz, "%s", sz1 );
		}

		return true;
	}
	else
	{
		if ( 0 != psz )
		{
			sprintf( psz, "N/A, 99-N/A-99 99:99:99" );
		}

		return false;	
	}
}




/** *****************************************************************
 * 
 * CTime::getSeconds
 *
 * Given a timeunit and the number of these timeunits,  determine 
 * the equivalent number of seconds.  The date may be off by as much 
 * as 2 days when using Plus/Minus with MONTHS or YEARS.
 *
 **/
long CTime::getSeconds( unsigned int n, timeUnit tu ) const
{
	long l=0;

				long ltime;
				struct tm * pt;
				int cMonth=0, cDay=0, cDays=0, cNextMonth=0, cThisMonth=0, cYear=0;
				bool isLeap = false;
				unsigned int i;


	switch( tu )
	{
		case SECOND:
		case SECONDS: l = n;  break;
		case MINUTE:
		case MINUTES: l = n*60;  break;
		case HOUR:   
		case HOURS:   l = n*3600;  break;
		case DAY:    
		case DAYS:    l = n*24*3600;  break;
		case WEEK:
		case WEEKS:   l = n*7*24*3600;  break;

		/** Add 365 days/year for non-leap-year years...366 otherwise */
		case YEAR:    
		case YEARS:   

				for ( i=0; i<n; i++ )
				{
					time( &ltime );
					ltime += (n*365);
					pt = localtime( &ltime );
					cYear = pt->tm_year+1900;

					isLeap = ( (0==cYear%4 && 0!=cYear%100) || ( 0==cYear%400 && 0==cYear%100 ) ) ? (true) : (false);
					
					if ( true == isLeap )
						l += 366*24*3600; 
					else
						l += 365*24*3600; 
				}

				break;


		/** Convert down to days using array cDaysInMonth and the number of months to add */
		case MONTH:   
		case MONTHS: 

				time( &ltime );
				pt = localtime( &ltime );
				cMonth = pt->tm_mon;      /** months since January - [0,11] */
				cDay = pt->tm_mday;       /** day of the month - [1,31]    */
 
				for ( i=0; i<n; i++ )
				{
					if ( 0==i )
					{
						/** Add days according to percentage of this and next month */
						cThisMonth = cDaysInMon[(i+cMonth)%12];
						cNextMonth = cDaysInMon[(i+cMonth+1)%12];

						//long c1 = (long)( (cDay/(float)cThisMonth) * cThisMonth );
						//long c2 = (long)( (((cThisMonth-cDay)/(float)cThisMonth) ) * cThisMonth );
						//cDays += (long)( c1 + c2 );
					
						cDays += ( cDaysInMon[ (i+cMonth) % 12 ] );
					}
					else
					{
						cDays += ( cDaysInMon[ (i+cMonth) % 12 ] );
					}	
				}

				l = (long)( cDays * 24 * 3600 );  break;

				break;


	}


	return l;
}





/** *****************************************************************
 *
 * CTime::test
 *
 * Run a test of the full functionality of the CTime class 
 * by running the local time test and the GMT time test.
 *
 **/
void CTime::test() 
{
	localTest();
	gmtTest();
}




/** *****************************************************************
 *
 * CTime::localTest
 *
 * Run a test of the full functionality of the CTime classes 
 * local time generation functions.
 *
 **/
void CTime::localTest()
{
	char sz[129] = {'\0'};
	timeUnit tu;

	getLocalDateTime( sz, 128 );                 
	printf ("Local time:        [%20s]  [%s, %02u-%s-%02u %02u:%02u:%02u]\r\n\r\n", 
		sz, m_szDay, m_cDay, m_szMonth, m_cYear, m_cHour, m_cMin, m_cSec );

	
	for ( int j=0; j<7; j++ )
	{
		switch (j)
		{
			case 0: tu = SECOND; break;
			case 1: tu = MINUTE; break;
			case 2: tu = HOUR;   break;
			case 3: tu = DAY;    break;
			case 4: tu = WEEK;   break;
			case 5: tu = MONTH;  break;
			case 6: tu = YEAR;   break;
		}

		for ( int i=0 ;i<3; i++ )
		{
		 	getLocalDateTimePlus (  i*2+1, tu, sz, 128 );  
		
			printf ("Local time + %d %s [%20s]  [%s, %02u-%s-%02u %02u:%02u:%02u]\r\n", 
				i*2+1, szTimeUnits[tu/2], sz,
				m_szDay, m_cDay, m_szMonth, m_cYear, m_cHour, m_cMin, m_cSec ); 
			
			getLocalDateTimeMinus(  i*2+1, tu, sz, 128 );

			printf ("Local time - %d %s [%20s]  [%s, %02u-%s-%02u %02u:%02u:%02u]\r\n", 
				i*2+1, szTimeUnits[tu/2], sz,
				m_szDay, m_cDay, m_szMonth, m_cYear, m_cHour, m_cMin, m_cSec ); 
		}

		printf( "\r\n" );
	}
}



/** *****************************************************************
 *
 * CTime::gmtTest
 *
 * Run a test of the full functionality of the CTime classes 
 * GMT time generation functions.
 *
 **/
void CTime::gmtTest()
{
	char sz[129] = {'\0'};
	timeUnit tu;

	getGMTDateTime( sz, 128 );
	printf ("GMT time:        [%20s]  [%s, %02u-%s-%02u %02u:%02u:%02u]\r\n\r\n", 
		sz, m_szDay, m_cDay, m_szMonth, m_cYear, m_cHour, m_cMin, m_cSec );
	
	for ( int j=0; j<7; j++ )
	{
		switch (j)
		{
			case 0: tu = SECOND; break;
			case 1: tu = MINUTE; break;
			case 2: tu = HOUR;   break;
			case 3: tu = DAY;    break;
			case 4: tu = WEEK;   break;
			case 5: tu = MONTH;  break;
			case 6: tu = YEAR;   break;
		}

		for ( int i=0 ;i<3; i++ )
		{
		 	getGMTDateTimePlus ( i*2+1, tu, sz, 128 );  
		
			printf ("GMT time + %d %s [%20s]  [%s, %02u-%s-%02u %02u:%02u:%02u]\r\n", 
				i*2+1, szTimeUnits[tu/2], sz,
				m_szDay, m_cDay, m_szMonth, m_cYear, m_cHour, m_cMin, m_cSec ); 
			
			getGMTDateTimeMinus( i*2+1, tu, sz, 128 );  

			printf ("GMT time - %d %s [%20s]  [%s, %02u-%s-%02u %02u:%02u:%02u]\r\n", 
				i*2+1, szTimeUnits[tu/2], sz,
				m_szDay, m_cDay, m_szMonth, m_cYear, m_cHour, m_cMin, m_cSec ); 
		}

		printf( "\r\n" );
	}
}
