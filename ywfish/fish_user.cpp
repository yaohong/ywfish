#include "fish_user.h"
#include "mayday/net/Buffer.h"
#include "mayday/net/Endian.h"
#include "mayday/net/TcpConnection.h"
#include "fish.pb.h"
#define CLIENT_PACKET_HEAD_LEN 4


fishUser::fishUser(const TcpConnectionPtr &conn)
: connection_(conn)
, isClose_(false)
{

}

fishUser::~fishUser()
{

}

void fishUser::onDisconnect()
{
	if (isClose_)
	{
		return;
	}
	connection_.reset();
	isClose_ = true;
}

void fishUser::onMessage(const mayday::net::TcpConnectionPtr& conn, mayday::net::Buffer* buf)
{
	assert(connection_ == conn);

	while (true)
	{
		int readableBbyte = buf->readableBytes();
		if (readableBbyte < CLIENT_PACKET_HEAD_LEN)
		{
			//不够一个包头
			return;
		}

		const char *bufOffset = buf->peek();
		unsigned int packetSize = sockets::networkToHost32(*(unsigned int *)bufOffset);
		int totalLen = static_cast<int>(packetSize) + CLIENT_PACKET_HEAD_LEN;
		if (readableBbyte < totalLen)
		{
			return;
		}
		onPackage(bufOffset + CLIENT_PACKET_HEAD_LEN, packetSize);
		buf->retrieve(totalLen);
	}
}

void fishUser::onPackage(const char *buff, int32 buffLen)
{
	fish::packet root;
	if (!root.ParseFromArray(buff, buffLen))
	{
		connection_->forceClose();
		return;
	}

	int32 cmd = root.cmd();
	const std::string &body = root.body();
	switch (cmd)
	{
		case fish::CMD_PING_REQ:
		{
		   fish::ping_req req;
		   req.ParseFromString(body);
		}
			break;
		case fish::CMD_LOGIN_REQ:
		{
			fish::login_req req;
			req.ParseFromString(body);
		}
			break;
		case fish::CMD_ENTER_ROOM_REQ:
		{
			fish::enter_room_req req;
			req.ParseFromString(body);
		}
			break;
		case fish::CMD_FIRE_REQ:
		{
			fish::fire_req req;
			req.ParseFromString(body);
		}
			break;
		default:
			break;
	}

}