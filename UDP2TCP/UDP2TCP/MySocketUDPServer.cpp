#include "stdafx.h"
#include "MySocketUDPServer.h"


CMySocketUDPServer::CMySocketUDPServer()
{
}


CMySocketUDPServer::~CMySocketUDPServer()
{
}

bool CMySocketUDPServer::Init(const char *addr, int port)
{
	if (IsInitialized())
		return false;

	memset(&m_addr, NULL, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = ::inet_addr(addr);
    m_addr.sin_port = ::htons(port);

	m_s = ::socket(AF_INET, SOCK_DGRAM, 0);
	if (FSOCK_ISERROR(m_s))
	{
		FSOCK_CLOSE(m_s);
		return false;
	}

	const BOOL on = 1;
    int rc = ::setsockopt(m_s, SOL_SOCKET, SO_REUSEADDR, (char*)on, sizeof(on));

	rc = ::bind(m_s, (SOCKADDR*)&m_addr, sizeof(m_addr));
	if (FSOCK_ISERROR(rc))
	{
		FSOCK_CLOSE(m_s);
		return false;
	}

	m_type = UDP_SERVER;
	return true;
}

int CMySocketUDPServer::RecvFrom(char *buf, int len, SOCKADDR *pAddr)
{
	if (!IsInitialized())
		return -1;

    socklen_t peerSz = sizeof(sockaddr_in);

    return ::recvfrom(m_s, (char*)buf, len, 0, (SOCKADDR*)pAddr, &peerSz);
	
}
