#include <stdio.h>
#include <string.h>
#include "kernel/mempipe.h"


MemPipe::MemPipe( void )
{
	m_pBuffer	= NULL;
	m_pHead		= NULL;
	m_pTail		= NULL;
	m_nSize		= 0;
}


MemPipe::~MemPipe( void )
{
	if (m_pBuffer != NULL)
	{
		delete []m_pBuffer;
	}
}

bool MemPipe::Init( INT32 nSize )
{
	m_pBuffer = new char[nSize];
	if (NULL == m_pBuffer)
	{
		return false;
	}

	m_pHead = m_pBuffer;
	m_pTail = m_pBuffer;
	m_nSize = nSize;

	return true;
}

bool MemPipe::Write( const char* pData, INT32 nLen )
{
	INT32 nDist = (INT32)(m_pTail + (UINT32)m_nSize - m_pHead);
	INT32 nUsed = nDist >= m_nSize ? (nDist - m_nSize) : nDist;

	if (nLen + nUsed + 1 > m_nSize)
	{
		return false;
	}

	if (m_pTail + nLen >= m_pBuffer + m_nSize)
	{
		INT32 nSeg1 = (INT32)(m_pBuffer + m_nSize - m_pTail);
		INT32 nSeg2 = nLen - nSeg1;
		memcpy(m_pTail, pData, nSeg1);
		memcpy(m_pBuffer, pData + nSeg1, nSeg2);
		m_pTail = m_pBuffer + nSeg2;
	}
	else
	{
		memcpy(m_pTail, pData, nLen);
		m_pTail += nLen;
	}

	return true;
}

bool MemPipe::Read( char* pBuf, INT32 nLen )
{
	INT32 nDist = (INT32)(m_pTail + (UINT32)m_nSize - m_pHead);
	INT32 nUsed = nDist >= m_nSize ? (nDist - m_nSize) : nDist;

	if (nLen >nUsed)
	{
		return false;
	}

	if (m_pHead + nLen >= m_pBuffer + m_nSize)
	{
		INT32 nSeg1 = (INT32)(m_pBuffer + m_nSize - m_pHead);
		INT32 nSeg2 = nLen - nSeg1;
		memcpy(pBuf, m_pHead, nSeg1);
		memcpy(pBuf+nSeg1, m_pBuffer, nSeg2);
		m_pHead = m_pBuffer + nSeg2;
	}
	else
	{
		memcpy(pBuf, m_pHead, nLen);
		m_pHead += nLen;
	}

	return true;
}

void MemPipe::Discard( INT32 nLen )
{
	INT32 nDist = (INT32)(m_pTail + (UINT32)m_nSize - m_pHead);
	INT32 nUsed = nDist >= m_nSize ? (nDist - m_nSize) : nDist;

	if (nLen > nUsed)
	{
		return;
	}

	if (m_pHead + nLen >= m_pBuffer + m_nSize)
	{
		INT32 nSeg1 = (INT32)(m_pBuffer + m_nSize - m_pHead);
		INT32 nSeg2 = nLen - nSeg1;
		m_pHead = m_pBuffer + nSeg2;
	}
	else
	{
		m_pHead += nLen;
	}
}

