#include "fish_server.h"
#include "mayday/net/TcpServer.h"
#include "mayday/net/NetworkLoop.h"
#include "mayday/net/Buffer.h"
#include "mayday/net/TcpConnection.h"
#include "fish_user.h"

//系统缓存大小
const int32 fishServer::sysRecvBuffSize = 64 * 1024;
const int32 fishServer::sysSendBuffSize = 64 * 1024;
//BUFF的缓存大小
const int32 fishServer::cacheRecvBuffSize = 32 * 1024;
const int32 fishServer::cacheSendBuffSize = 32 * 1024;


fishServer::fishServer()
{

}

fishServer::~fishServer()
{

}

void fishServer::loop()
{
	loop_->loop();
}

void fishServer::startServer(const mayday::net::InetAddress &listenAddr)
{
	loop_ = new mayday::net::NetworkLoop();
	tcpServer_.reset(new mayday::net::TcpServer(
		loop_,
		"gatewayServer",
		listenAddr,
		fishServer::sysRecvBuffSize,
		fishServer::sysSendBuffSize,
		fishServer::cacheRecvBuffSize,
		fishServer::cacheSendBuffSize));

	tcpServer_->setConnectionCallback(std::bind(&fishServer::onClientConnection, this, std::placeholders::_1));
	tcpServer_->setMessageCallback(std::bind(&fishServer::onClientMessage, this, std::placeholders::_1, std::placeholders::_2));
	tcpServer_->setWriteCompleteCallback(std::bind(&fishServer::onClientWriteComplete, this, std::placeholders::_1));
	tcpServer_->start();
}


void fishServer::onClientConnection(const mayday::net::TcpConnectionPtr& conn)
{
	if (conn->connected())
	{
		assert(conn->getContext() == NULL);
		fishUser *user = new fishUser(conn);
		conn->setContext(user);
	}
	else if (conn->disconnected())
	{
		fishUser *user = (fishUser *)conn->getContext();
		user->onDisconnect();
		conn->setContext(NULL);
		delete user;
	}
}

void fishServer::onClientMessage(const mayday::net::TcpConnectionPtr& conn, mayday::net::Buffer* buf)
{
	fishUser *user = (fishUser *)conn->getContext();
	assert(user);
	user->onMessage(conn, buf);
}

void fishServer::onClientWriteComplete(const mayday::net::TcpConnectionPtr& conn)
{

}