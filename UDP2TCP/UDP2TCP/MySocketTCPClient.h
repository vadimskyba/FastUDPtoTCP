#pragma once
#include "MySocket.h"
class CMySocketTCPClient :	public CMySocket
{
public:
	CMySocketTCPClient();
	virtual ~CMySocketTCPClient();

	virtual bool Init(const char *addr, int port);
    bool SetNoDelay(BOOL bEnable);
	bool Connect();

	bool inline IsConnected() { return m_bConnected; };
	virtual bool CloseSocket();

private:
	bool m_bConnected;
};

