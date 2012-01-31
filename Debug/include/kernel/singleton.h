#ifndef _H_SINGLETON_H_
#define _H_SINGLETON_H_

#include <stdio.h>

#define DECLARE_SINGLETON(cls) \
private:\
	static cls* sm_poInstance;\
public:\
	static bool CreateInstance()\
	{\
		if (NULL == sm_poInstance) sm_poInstance = new cls; \
			return sm_poInstance != NULL;\
	}\
	static cls* Instance(){ return sm_poInstance; }\
	static void DestroyInstance()\
	{\
		if(sm_poInstance != NULL)\
		{\
			delete sm_poInstance;\
			sm_poInstance = NULL;\
		}\
	}

#define INSTANCE_SINGLETON(cls) \
	cls* cls::sm_poInstance = NULL;
#endif
