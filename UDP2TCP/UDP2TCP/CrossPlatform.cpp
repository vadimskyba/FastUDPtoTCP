#include "stdafx.h"
#include "CrossPlatform.h"



#if defined _WIN32 || defined _WIN64 || __MINGW32__

void winsockinit()
{
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);
}

#else

#endif



