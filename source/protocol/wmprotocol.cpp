#include "pch.h"

#include "wmsystem.h"
#include "octetsstream.h"
#include "wminterface.h"
#include "wmprotocol.h"

bool CProtocol::Encode( OctetsStream& os, char* pcPkg, INT32* pnLength ) const
{
	WMASSERT(NULL != pcPkg && NULL != pnLength && *pnLength > 0);

	os.clear();
	os << *this;

	if (*pnLength < static_cast<INT32>(os.size()))
	{
		LOG("Encode Package Length is %d, shorter than %d", *pnLength, os.size());
		*pnLength = -1;
		return false;
	}

	*pnLength = static_cast<INT32>(os.size());
	os.pop_front(pcPkg, *pnLength);

	return true;
}

bool CProtocol::Decode( OctetsStream& os, const char* pcPkg, INT32 nLength )
{
	WMASSERT(NULL != pcPkg && nLength > 0);

	os.assign(pcPkg, nLength);
	os >> * this;
	return true;
}

CProtocol* CProtocol::GetProtocol( UINT32 dwType )
{
	CMapProtocol::iterator it = _ProtocolMap().find(dwType);
	if (_ProtocolMap().end() == it)
	{
		return NULL
	}
	else
	{
		CProtocol* poPtrotocol = it->second;
		poPtrotocol->Reset();
		return poPtrotocol;
	}
}
