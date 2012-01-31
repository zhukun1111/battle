#ifndef _H_PCH_H_
#define _H_PCH_H_

#include <vector>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <string>
#include <utility>
#include <assert.h>

#include <iostream>

#ifndef WIN32
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <error.h>
#include <errno.h>
#include <string.h>
#else
#include <io.h>
#endif
#include "wmdefine.h"
//#include <kernel/wmtime.h>
// #include <protocol/byteorder.h>
//#include <protocol/octets.h>
//#include <protocol/wmexception.h>

#define LOG printf
#define ASSERT assert

extern INT32	WMNET_MAX_PKG_LEN;
extern INT32	WMNET_EVT_PIPE_SIZE;
extern INT32	WMNET_REQ_PIPE_SIZE;
extern INT32	WMNET_TEMPBUF_QUEUE_LEN;
#endif
