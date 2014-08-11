#include "stdafx.h"
#include "MySocket.h"


CMySocket::CMySocket()
{
	m_s = NULL;
	m_type = SOCKET_NULL;
}


CMySocket::~CMySocket()
{
	CloseSocket();
}

bool CMySocket::CloseSocket()
{
	if (m_type == SOCKET_NULL)
		return true;

	int rc = FSOCK_CLOSE(m_s);
	if (FSOCK_ISERROR(rc))
		return false;

	m_s = NULL;
	return true;
}
