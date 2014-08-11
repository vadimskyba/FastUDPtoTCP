TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pthread
LIBS += -pthread

SOURCES += \
    UDP2TCP/CrossPlatform.cpp \
    UDP2TCP/main.cpp \
    UDP2TCP/MySocket.cpp \
    UDP2TCP/MySocketTCPClient.cpp \
    UDP2TCP/MySocketUDPServer.cpp \
    UDP2TCP/stdafx.cpp \
    UDP2TCP/UDP2TCPProxy.cpp

HEADERS += \
    UDP2TCP/CrossPlatform.h \
    UDP2TCP/MemAllocator.h \
    UDP2TCP/MySocket.h \
    UDP2TCP/MySocketTCPClient.h \
    UDP2TCP/MySocketUDPServer.h \
    UDP2TCP/stdafx.h \
    UDP2TCP/UDP2TCPProxy.h

