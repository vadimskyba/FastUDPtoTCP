#include "stdafx.h"
#include "MySocketTCPClient.h"


CMySocketTCPClient::CMySocketTCPClient()
{
	m_bConnected = false;
}


CMySocketTCPClient::~CMySocketTCPClient()
{
}

bool CMySocketTCPClient::Init(const char *addr, int port)
{
	if (IsInitialized())
		return false;

	memset(&m_addr, NULL, sizeof(m_addr));
	m_addr.sin_family = AF_INET;
	m_addr.sin_addr.s_addr = ::inet_addr(addr);
    m_addr.sin_port = ::htons(port);

    m_type = TCP_CLIENT;
	return true;
}

bool CMySocketTCPClient::SetNoDelay(BOOL bEnable)
// Disabling Nagle algorithm to avoid delay between 2 sequential operations of sending the data
{
	if (GetType() != TCP_CLIENT)
		return false;

    int rc = ::setsockopt(m_s, IPPROTO_TCP, TCP_NODELAY, (LPCSTR)&bEnable, sizeof(bEnable));
	if (FSOCK_ISERROR(rc))
		return false;

	return true;
}

bool CMySocketTCPClient::Connect()
{
	if (GetType() != TCP_CLIENT)
		return false;

	if (IsConnected())
		return false;

	m_s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (FSOCK_ISERROR(m_s))
	{
		FSOCK_CLOSE(m_s);
		return false;
	}

	SetNoDelay(TRUE);

    int rc = connect(m_s, (SOCKADDR*)&m_addr, sizeof(m_addr));
	if (FSOCK_ISERROR(rc))
	{
		FSOCK_CLOSE(m_s);
		return false;
	}

	m_bConnected = true;
	return true;
}

bool CMySocketTCPClient::CloseSocket()
{
	bool brez = CMySocket::CloseSocket();
	m_bConnected = false;

	return brez;
}
