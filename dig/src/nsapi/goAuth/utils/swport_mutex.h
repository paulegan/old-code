/******************************************************************************
 * swport_mutex.h
 * 
 * a simple wrapper class to allow WINNT and SOLARIS apps to use the same
 * interface for synchronization of critical regions.
 *
 * Uses NT CriticalSections and Solaris mutexes.
 * 
 * @author John Mollman
 * 10/98
 *
 *****************************************************************************/

#ifndef _SWPORT_MUTEX_H_
#define _SWPORT_MUTEX_H_


#if defined __OS_WINNT__
	#include <windows.h>
#elif defined __OS_SOLARIS__
	#include <thread.h>
#endif

#include "platform_defines.h"


// Return code defines.  These are same as swport_waitable_lock, 
// should go in platform_defines.h
#define EALREADY_INITIALIZED -1
#define ENOT_INITIALIZED -2
#define ETIMED_OUT_WAITING -3


class swport_mutex {

	public:
		swport_mutex();
		~swport_mutex();

		int 	Init();
		int		Destroy(); 
		int		Lock();
		int		Unlock();

		// returns address of mutex_t member for Solaris, else NULL pointer
		// This allows use of this class with Solaris condition variables
		void* 	GetAddr();

	private:
		bool bInitialized;	

	#if defined __OS_WINNT__
		CRITICAL_SECTION	m_mutex;
	#elif defined __OS_SOLARIS__
		mutex_t				m_mutex;
	#endif
};


#endif	
