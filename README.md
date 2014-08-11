FastUDPtoTCP
=========
C++11 Crossplatform Multithreaded Fast UDP to TCP Messages Retransmitter.
Contains project files for MS Visual Studio 2013 (*.sln) and QT Creator (*.pro).

No needs of any third-party libraries , only native C++11 code.

UDP2TCP - Multithreaded Fast UDP to TCP Messages Retransmitter;
UDPClient - simple udp sender to test UDP2TCP;
TCPServer - simple TCP server to receive messages from UDP2TCP;

All the apps are using to send and receive the messages in format as follows:

{
	uint16_t MessageSize
	uint8_t MessageType
	uint64_t MessageId
	uint64_t MessageData
}

If UDP2TCP receive the packet with MessageData == 10, it should be transmitted to TCPServer.