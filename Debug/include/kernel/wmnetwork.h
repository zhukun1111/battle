#ifndef _H_WMNETWORK_H_
#define _H_WMNETWORK_H_

#include "wmdefine.h"

const UINT32 WM_NETWORK_VERSION	= 2;
const UINT32 WM_DEFAULT_SENDBUF_SIZE = 65535;
const UINT32 WM_DEFAULT_RECVBUF_SIZE = 65535;

class IWMNetworkProcessor
{
public:
	virtual void WMAPI OnConnect(WMHANDLE hNetHandle, UINT32 dwConnID, EWMErrCode nWMErrCode, const SWMUserData& rstHandledata) = 0;
	//virtual void WMAPI OnClose(UINT32 dwConnID, EWMErrCode nWMErrCode, const SWMUserData& rstConnData) = 0;
	virtual void WMAPI OnRecv(UINT32 dwConnID, const char* pcPkg, INT32 nLen, const SWMUserData& rstConnData) = 0;
};

class IWMNetWork
{
	virtual WMHANDLE WMAPI Connect(UINT32 dwRemoteIP, UINT16 wRemotePort, IWMNetworkProcessor* poNetProcessor, UINT32 dwSendBufSize = WM_DEFAULT_SENDBUF_SIZE, UINT32 dwRecvBufSize = WM_DEFAULT_RECVBUF_SIZE) = 0;
	virtual WMHANDLE WMAPI Listen(UINT32 dwLocalIP, UINT16 wLocalPort, IWMNetworkProcessor* poNetProcessor, bool bReuseAddr, UINT32 dwSendBufSize = WM_DEFAULT_SENDBUF_SIZE, UINT32 dwRecvBufSize = WM_DEFAULT_RECVBUF_SIZE) = 0;
	virtual bool WMAPI Send(UINT32 dwConnID, const char* pcData, INT32 nLen) = 0;
	virtual bool WMAPI ProcessMsg(INT32 nCount) = 0;

	//virtual bool WMAPI Close(UINT32 dwConnID) = 0;
	//virtual bool WMAPI CloseHandle(WMHANDLE hNetHandle) = 0;

	//virtual bool WMAPI SetHandleUserData(WMHANDLE hNetHandle, const SWMUserData& rstUserData) = 0;
	//virtual bool WMAPI GetHandleUserData(WMHANDLE hNetHandle, SWMUserData& rstUserData) = 0;
	//virtual bool WMAPI SetConnUserData(UINT32 dwConnID, const SWMUserData& rstUserData) = 0;
	//virtual bool WMAPI GetConnUserData(UINT32 dwConnID, SWMUserData& rstUserData) = 0;
	//virtual bool WMAPI GetConnLocalAddr(UINT32 dwConnID, UINT32& rdwIP, UINT16& rwPort) = 0;
	//virtual bool WMAPI GetConnRemoteAddr(UINT32 dwConnID, UINT32& rdWIP, UINT16& rwPort) = 0;

};

#endif
