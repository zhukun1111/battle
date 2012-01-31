#ifndef _H_MEMPIPE_H_
#define _H_MEMPIPE_H_

#include "wmdefine.h"

class MemPipe
{
public:
	MemPipe(void);
	virtual ~MemPipe(void);

	bool Init(INT32 nSize);

	bool Write(const char* pData, INT32 nLen);

	bool Read(char* pBuf, INT32 nLen);

	void Discard(INT32 nLen);

protected:
	char*	m_pBuffer;
	char*	m_pHead;
	char*	m_pTail;
	INT32	m_nSize;
};

#endif
