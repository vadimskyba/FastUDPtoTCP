#pragma once

#include "CrossPlatform.h"

class CMySocket
{

public:
	CMySocket();
	virtual ~CMySocket();
	inline int GetType() { return m_type; };
	inline bool IsInitialized() { return m_type == SOCKET_NULL ? false : true; };

	virtual bool Init(const char *addr, int port) = 0;
	virtual bool CloseSocket();

//protected:
	SOCKET m_s;
	struct sockaddr_in m_addr;
	enum { SOCKET_NULL, TCP_SERVER, TCP_CLIENT, UDP_SERVER, UDP_CLIENT } m_type;
	
};

