// test_sel_s.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <WinSock2.h>
#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#pragma comment(lib, "ws2_32.lib")

#define NORM_SIO_UDP_CONNRESET _WSAIOW(IOC_VENDOR,12)

int _tmain(int argc, _TCHAR* argv[])
{
	WSADATA wd;
	if (NO_ERROR != WSAStartup(MAKEWORD(2,2), &wd)) {
		return -1;
	}

	SOCKET s = socket(AF_INET, SOCK_DGRAM, 0);

	if (1) {
		DWORD udp_opt = 0, udp_btr = 0;
		WSAIoctl(s, NORM_SIO_UDP_CONNRESET, &udp_opt, sizeof(udp_opt), NULL, 0, &udp_btr, NULL, NULL);
	}

	unsigned long val = 1;
	ioctlsocket(s, FIONBIO, &val);

	int opt;
	opt = 512*1024;
	setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char*)&opt, sizeof(opt));
	opt = 512*1024;
	setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char*)&opt, sizeof(opt));

	DWORD last_tm = GetTickCount();
	int total = 0;

	for(;;) {
		fd_set wr_set;
		memset(&wr_set, 0, sizeof(wr_set));
		FD_SET (s, &wr_set);

		struct timeval tv;
		tv.tv_sec = 0; tv.tv_usec = 0;
		int ready = select(0, NULL, &wr_set, NULL, &tv);

		if (ready == -1) {
			continue;
		} 
		else if (ready == 0) {
			continue;
		}
		
		if (!FD_ISSET(s, &wr_set))
			continue;

		int flag = 1;
		int n = 0;
		const int size = 1452;
		while(flag) {
			char* buf = new char[size];
			struct sockaddr_in so_addr;

			memset(&so_addr, 0, sizeof(so_addr));
			so_addr.sin_family = AF_INET;
			so_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
			so_addr.sin_port = htons(8888);

			int err = sendto(s, buf, size, 0, (struct sockaddr*)&so_addr, sizeof(so_addr));

			delete []buf;

			if (err == SOCKET_ERROR) {
				if (GetLastError() == WSAEWOULDBLOCK) {
					//break;
					continue;
				}
				else
					return 0;
			}

			total++;

			if (GetTickCount() - last_tm >= 1000) {
				printf("snd cnt=%d\r\n", total);
				total = 0;
				last_tm = GetTickCount();
			}
			//break;
		}
	}

	return 0;
}

