#pragma once

#include "MySocketUDPServer.h"
#include "MySocketTCPClient.h"
#include "MemAllocator.h"

#include <inttypes.h>
#include <vector>
#include <map>
#include <thread>
#include <mutex> 
#include <condition_variable>
#include <atomic>

struct MESSAGE
{
	uint16_t MessageSize;
	uint8_t MessageType;
	uint64_t MessageId;
	uint64_t MessageData;
};

class CUDP2TCPProxy
{
	//friend void UDPThread(SOCKET s);

public:
	CUDP2TCPProxy();
	virtual ~CUDP2TCPProxy();

	bool Initialize(const char *str_udp_address, const int udp_port, const char *str_tcp_address, const int tcp_port);
	bool inline IsInitialized() { return m_udpS.IsInitialized() & m_tcpS.IsInitialized(); };
	bool inline IsRunning() { return !m_aThreads.empty(); };
	bool Run(int nServerThreads);
	bool Terminate();

private:
	void UDPThread(CMySocketUDPServer *s, MemAllocator<MESSAGE> *pMsgAllocator);	// UDP server thread function
	bool TCPClient(CMySocketTCPClient &s);								// TCP client non-thread function
	std::vector < MemAllocator<MESSAGE>* > m_apAllocators;
	std::vector<std::thread> m_aThreads;

	CMySocketUDPServer m_udpS;
	CMySocketTCPClient m_tcpS;

	std::atomic<bool> m_bStopSig;

	std::map <uint64_t, MESSAGE*> map_messages;				// Map (MESSAGE::MessageId, MESSAGE*) of all received messages 
	std::vector <MESSAGE> m_aMessages1;						// For best performance m_aMessages1 and m_aMessages2 are switching every time:
	std::vector <MESSAGE> m_aMessages2;						// while one of them fills with the messages to transmit with UDPThread(),
															// another already contains messages, that tcpclient function is sending
	std::vector <MESSAGE> *m_apMessages = &m_aMessages1;	// Contains pointer to messages container for UDPThread()

	std::mutex m_mMessages;									// mutex for map_messages
	std::mutex m_aMessages;									// mutex for m_paMessages
	std::condition_variable cond_qMessages;					// condition for m_paMessages
	
};

