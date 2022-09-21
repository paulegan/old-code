#include "swport_mutex.h"

swport_mutex::swport_mutex() {
	bInitialized = false;
}


swport_mutex::~swport_mutex() {
	if(bInitialized) {
		Destroy();	
	}
}



int swport_mutex::Init() {
	int iRetVal = 0;

	#if defined __OS_WINNT__
		InitializeCriticalSection(&m_mutex );
	#elif defined __OS_SOLARIS__
		iRetVal = mutex_init( &m_mutex, NULL, NULL );
	#endif

	bInitialized = true;
	return iRetVal;
}


int swport_mutex::Destroy() {
	if(!bInitialized) return ENOT_INITIALIZED;	
	
	int iRetVal = 0;

	#if defined __OS_WINNT__
		DeleteCriticalSection(&m_mutex );
	#elif defined __OS_SOLARIS__
		iRetVal = mutex_destroy( &m_mutex );
	#endif

	bInitialized = false;
	return iRetVal;
}


int swport_mutex::Lock() {
	int iRetVal = 0;

	// If we're not initialized, nobody should be calling this!
	if(!bInitialized) return ENOT_INITIALIZED;

	#if defined __OS_WINNT__
		EnterCriticalSection( &m_mutex );
	#elif defined __OS_SOLARIS__
		iRetVal = mutex_lock ( &m_mutex );
	#endif

	return iRetVal;
}


int swport_mutex::Unlock() {
	int iRetVal = 0;

	// If we're not initialized, nobody should be calling this!
	if(!bInitialized) return ENOT_INITIALIZED;
	
	#if defined __OS_WINNT__
		LeaveCriticalSection( &m_mutex );
	#elif defined __OS_SOLARIS__
		iRetVal = mutex_unlock( &m_mutex );
	#endif

	return iRetVal;
}



void* swport_mutex::GetAddr() {

	// make sure we're initialized
	if(!bInitialized) Init();

	#if defined __OS_SOLARIS__
		return &m_mutex;
	#else
		// unsupported for WINNT and others, return NULL
		return 0;
	#endif

}



