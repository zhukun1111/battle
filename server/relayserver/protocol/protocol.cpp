#include "protocol/protocolheader.h"

void CProtocol::Register()
{
	static bool bOnce = true;

	if (true == bOnce)
	{
		_ProtocolMap().insert(std::make_pair(TYPE_PTCC2RREQ_LOGIN, new PtcC2RReq_Login));

		bOnce = false;
	}
}