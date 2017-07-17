#ifndef _fish_server_h__
#define _fish_server_h__

#include "mayday/net/InetAddress.h"
#include "mayday/net/Callbacks.h"
namespace mayday {
	namespace net {
		class NetworkLoop;
		class TcpServer;
		class Buffer;
	}
	
}

class fishUser;
class fishServer {
public:
	fishServer();
	~fishServer();
public:
	static const int32 sysRecvBuffSize;
	static const int32 sysSendBuffSize;
	static const int32 cacheRecvBuffSize;
	static const int32 cacheSendBuffSize;
public:
	void loop();
	void startServer(const mayday::net::InetAddress &listenAddr);
private:
	void onClientConnection(const mayday::net::TcpConnectionPtr& conn);
	void onClientMessage(const mayday::net::TcpConnectionPtr& conn, mayday::net::Buffer* buf);
	void onClientWriteComplete(const mayday::net::TcpConnectionPtr& conn);
private:
	mayday::net::NetworkLoop *loop_;
	std::shared_ptr<mayday::net::TcpServer> tcpServer_;
};





#endif