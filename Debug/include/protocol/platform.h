#ifndef  _H_PLATFORM_H_
#define	 _H_PLATFORM_H_

#ifdef WIN32

#ifdef _WIN64
#error "__i386__ only"
#endif

#include <windows.h>

#ifndef __i386__
#define __i386__
#endif

#else

#include <stdint.h>
#endif

#endif