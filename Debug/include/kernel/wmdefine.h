#ifndef _H_WMDEFINE_H
#define _H_WMDEFINE_H

typedef signed char INT8;
typedef unsigned char UINT8;
typedef signed short INT16;
typedef unsigned short UINT16;
typedef signed int INT32;
typedef unsigned int UINT32;

#ifdef WIN32
typedef __int64 INT64;
typedef unsigned __int64 UINT64;
#else
typedef signed long INT64;
typedef unsigned long UINT64;
#endif

typedef UINT32 WMHANDLE;

#define INVALID_WMHANDLE 0xFFFFFFFF
#define INVALID_BUSID    0xFFFFFFFF
#define INVALID_STATEID  -1

#ifdef WIN32
#define WMAPI __stdcall
#include<winsock2.h>
#else
#define WMAPI
#endif

#define WMASSERT assert

struct SWMUserData
{
	SWMUserData() : m_dwUserData(0), m_pUserPtr(0){}

	SWMUserData(UINT32 dwUserData, void* pUserPtr) : m_dwUserData(dwUserData), m_pUserPtr(pUserPtr)
	{}

	UINT32 m_dwUserData;
	void*  m_pUserPtr;
};

enum EWMErrCode
{
	WM_SUCCESS					= 0,	//³É¹¦
	WM_UNKNOWN_ERR				= 1,	//Î´Öª´íÎó
};

#endif