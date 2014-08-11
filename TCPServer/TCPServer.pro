TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pthread
LIBS += -pthread

SOURCES += \
    TCPServer/CrossPlatform.cpp \
    TCPServer/stdafx.cpp \
    TCPServer/TCPServer.cpp

HEADERS += \
    TCPServer/CrossPlatform.h \
    TCPServer/stdafx.h

