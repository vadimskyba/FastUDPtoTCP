#include "stdafx.h"
#include "UDP2TCPProxy.h"
#include <iostream>
#include "assert.h"

#define INIT_VECTORSIZERESERVE 100					// Initial vector capacity for best performance

using namespace std;

CUDP2TCPProxy::CUDP2TCPProxy()
{
	m_aMessages1.reserve(INIT_VECTORSIZERESERVE);	// Initial allocate of memory for better performance
	m_aMessages2.reserve(INIT_VECTORSIZERESERVE);
}

CUDP2TCPProxy::~CUDP2TCPProxy()
{
	assert(m_aThreads.empty());

	for (auto pAllocator : m_apAllocators)
		delete pAllocator;

	m_apAllocators.clear();
}

bool CUDP2TCPProxy::Initialize(const char *str_udp_address, const int udp_port, const char *str_tcp_address, const int tcp_port)
{
	if (!m_tcpS.Init(str_tcp_address, tcp_port))
		return false;

	if (!m_udpS.Init(str_udp_address, udp_port))
	{
		m_tcpS.CloseSocket();
		return false;
	}

	return true;
}

bool CUDP2TCPProxy::Run(int nServerThreads)
{
	if (!IsInitialized() || IsRunning())
		return false;
	
	assert(m_aThreads.empty());
	m_bStopSig = false;
	
	// Creating memore allocators and running threads
	for (int i = 0; i < nServerThreads; i++)
	{
		MemAllocator<MESSAGE> *pMsgAllocator;

		if (i < m_apAllocators.size())	// Using of previous thread session allocators 
		{
			pMsgAllocator = m_apAllocators[i];
		}
		else							// or create new ones
		{
			pMsgAllocator = new MemAllocator <MESSAGE>;
			m_apAllocators.push_back(pMsgAllocator);
		}

		m_aThreads.push_back(std::thread(&CUDP2TCPProxy::UDPThread, this, &m_udpS, pMsgAllocator));
	}

	return TCPClient(m_tcpS);
}

bool CUDP2TCPProxy::Terminate()
{
	if (!IsRunning())
		return true;

	m_bStopSig = true;

	// Closing sockets
	m_tcpS.CloseSocket();
	m_udpS.CloseSocket();

	// Waiting for ending of thread functions
	for (int i = 0; i < m_aThreads.size(); i++)
	{
		if (m_aThreads[i].joinable())
			m_aThreads[i].join();
	}

	m_aThreads.clear();
	return true;
}

void CUDP2TCPProxy::UDPThread(CMySocketUDPServer *s, MemAllocator<MESSAGE> *pMsgAllocator)
{
	cout << "Starting thread: " << std::this_thread::get_id() << endl;
	assert(s->IsInitialized());
	struct sockaddr_in peer;

	MESSAGE *pMsg = pMsgAllocator->Alloc();	// Allocate memory for the first UDP_MESSAGE element 
	const int msgSz = sizeof(MESSAGE);

	while (!m_bStopSig)
	{
		int recvSz = s->RecvFrom((char*)pMsg, msgSz, (SOCKADDR*)&peer);

		if (recvSz < 0)				// Error
		{
			// Here must be special code to inform main thread about error...
			cout << "Terminating thread ID: " << std::this_thread::get_id() << ". Socket error." << endl;
			return; 
		}
		else if (recvSz == msgSz)	// Received message with correct size
		{
			cout << "Thread: " << std::this_thread::get_id() << " Received packet: " << pMsg->MessageId << endl;

			MESSAGE *pNewMsg = NULL;
			m_mMessages.lock(); //	<------------------------------------------ locking map_messages
				auto ret = map_messages.emplace(pMsg->MessageId, pMsg);	// trying to insert the new message pointer to map
				if (ret.second) // The value of pMsg->MessageId is new to mMessages
				{
					pNewMsg = pMsg;
				}
				else // Insertion failed: element with key value pMsg->MessageId already exists in mMessages
				{
					auto pr = *ret.first;		// pr.first - existing MessageId
												// pr.second - pointer to existing UDP_MESSAGE struct

					pNewMsg = (MESSAGE*)pr.second;
					assert(pNewMsg->MessageId == pMsg->MessageId);

					memcpy(pNewMsg, pMsg, msgSz); // rewriting existing UDP_MESSAGE
					cout << "Duplicated message id: " << pMsg->MessageId << endl;
				}
				m_mMessages.unlock(); // <------------------------------------------ unlocking map_messages

			if (pNewMsg->MessageData == 10)
			{
				std::unique_lock<std::mutex> lock(m_aMessages);	// <--------------- locking m_apMessages
				m_apMessages->push_back(*pNewMsg);
				cond_qMessages.notify_one();					// <--------------- unlocking m_apMessages on lock destruction
			}

			if (pNewMsg == pMsg)
				pMsg = pMsgAllocator->Alloc();	// Allocate new message for next iteration
		}
		else
		{
			cout << "Received BAD packet." << endl;
		}
	}
}

bool CUDP2TCPProxy::TCPClient(CMySocketTCPClient &s)
{
	while (!m_bStopSig)
	{
		cout << "Trying to connecting the tcp server..." << endl;

		if (!s.Connect())
		{
			cout << "Can't connect to tcp server." << endl;
			std::this_thread::sleep_for(std::chrono::seconds(5));
			continue;
		}

		cout << "Connected." << endl;

		while (!m_bStopSig)
		{
			std::unique_lock<std::mutex> lock(m_aMessages);
			cond_qMessages.wait(lock, [&](){ return !m_apMessages->empty(); });	// <-------- locking m_apMessages

			assert(!m_apMessages->empty());
			vector <MESSAGE> *paSendMessages = m_apMessages;
			m_apMessages = (m_apMessages == &m_aMessages1) ? &m_aMessages2 : &m_aMessages1;	// swap messages vectors
			
			lock.unlock();														// <-------- unlocking m_apMessages

			int messagesSize = paSendMessages->size() * sizeof(MESSAGE);
			int rc = send(s.m_s, (LPCSTR)paSendMessages->data(), messagesSize, 0);
			if (FSOCK_ISERROR(rc))
			{
				cout << "Connection to tcp server lost..." << endl;
				s.CloseSocket();
				break;
			}

			paSendMessages->clear();	// The trick is that capacity still remains unchanged
		}
	}

	return true;
}