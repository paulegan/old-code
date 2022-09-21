
// config.h
// Last update:		10.5.1997
//
// Contains values for things that might be usefull to change.


#ifndef PFMConfigHeader
#define PFMConfigHeader


// PFM Host File Stuff
#define PFMFileEnvVar		"PFM_HOSTS_FILE"
#define HOMEEnvVar		"HOME"
#define PFMFileName		"/.pfm_hosts"
#define MaxFileLineWidth	80
#define Delimiters		" \t\n"
#define Comment			'#'


// Default & initial values
#define DefaultLevel		10	//default percentage to change by.
#define FirstSlaveGroupID	1
#define DefaultRPP		1000
#define InitialSuitability	0
#define nReceiveTimeout		{1,0}

// Used to slave suitability calc. [ s = (sv*s) + ( (1-sv)*deltaT ) ]
#define SuitabilityVariance	0.5F


// Defines encoding method for data - if any!
#define DataEncodingMethod	PvmDataRaw

#endif
