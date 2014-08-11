TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11 -pthread
LIBS += -pthread

SOURCES += \
    UDPClient/CrossPlatform.cpp \
    UDPClient/stdafx.cpp \
    UDPClient/UDPClient.cpp

HEADERS += \
    UDPClient/CrossPlatform.h \
    UDPClient/stdafx.h

