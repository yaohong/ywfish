// ywfish.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "fish_server.h"
#include "mayday\net\InetAddress.h"
int main(int argc, char* argv[])
{

	fishServer server;
	server.startServer(mayday::net::InetAddress("192.168.1.181", 7777));
	server.loop();
	return 0;
}

