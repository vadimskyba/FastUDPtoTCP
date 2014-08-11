#pragma once
#include "MySocket.h"
class CMySocketUDPServer :	public CMySocket
{
public:
	CMySocketUDPServer();
	virtual ~CMySocketUDPServer();

	virtual bool Init(const char *addr, int port);

	int RecvFrom(char *buf, int len, SOCKADDR *pAddr);
};

