//--file: CTime.h----------------------------------------------------
//
#ifndef _CTime_h_
#define _CTime_h_

#include "platform_defines.h"

typedef enum {SECOND, SECONDS, MINUTE, MINUTES, HOUR, HOURS, DAY, DAYS, WEEK, WEEKS, MONTH, MONTHS, YEAR, YEARS} timeUnit;

class CTime 
{
	public:
		 CTime();

	     bool getLocalDateTime( char * psz=0, unsigned int cSize=0 );
		 bool getLocalDateTimePlus( unsigned int, timeUnit, char * psz=0, unsigned int cSize=0 );
		 bool getLocalDateTimeMinus( unsigned int, timeUnit, char * psz=0, unsigned int cSize=0 );

		 bool getGMTDateTime( char * psz=0, unsigned int cSize=0  );
		 bool getGMTDateTimePlus( unsigned int, timeUnit, char * psz=0, unsigned int cSize=0 );
		 bool getGMTDateTimeMinus( unsigned int, timeUnit, char * psz=0, unsigned int cSize=0 );


		/** Accessors */
		unsigned int getSec() const { return m_cSec;  }
		unsigned int getMin() const { return m_cMin;  }
		unsigned int getHr()  const { return m_cHour; }

		unsigned int getDay() const { return m_cDay;   }
		unsigned int getMn()  const { return m_cMonth; }
		unsigned int getYr()  const { return m_cYear;  }

		const char * getDayStr() const { return m_szDay;   }
		const char * getMnStr()  const { return m_szMonth; }

		const char * getFullDayStr() const { return m_szFullDay;   }
		const char * getFullMnStr()  const { return m_szFullMonth; }

		void test();


	private:
		long getSeconds( unsigned int n, timeUnit tu ) const;
		bool getLocalDateTime( long, char *, unsigned int  );
		bool getGMTDateTime( long, char *, unsigned int );
		void localTest();
		void gmtTest();


		unsigned int m_cSec, m_cMin, m_cHour;
		unsigned int m_cDay, m_cMonth, m_cYear;
		char m_szDay[16];
		char m_szMonth[16];
		char m_szFullDay[16];
		char m_szFullMonth[16];


// int tm_sec;     /* seconds after the minute - [0,59] */
// int tm_min;     /* minutes after the hour - [0,59]   */
// int tm_hour;    /* hours since midnight - [0,23]     */
// int tm_mday;    /* day of the month - [1,31]         */
// int tm_mon;     /* months since January - [0,11]     */
// int tm_year;    /* years since 1900                  */
// int tm_wday;    /* days since Sunday - [0,6]         */
// int tm_yday;    /* days since January 1 - [0,365]    */
// int tm_isdst;   /* daylight savings time flag        */

};

#endif
