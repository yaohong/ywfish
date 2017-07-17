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
	MDLog("new fishUser");
}

fishUser::~fishUser()
{

}

void fishUser::onDisconnect()
{
	MDLog("fishUser::onDisconnect");
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
	MDLog("fishUser::onMessage");
	while (true)
	{
		int readableBbyte = buf->readableBytes();
		if (readableBbyte < CLIENT_PACKET_HEAD_LEN)
		{
			//不够一个包头
			MDLog("fishUser::onMessage 1, %d", readableBbyte);
			return;
		}

		const char *bufOffset = buf->peek();
		unsigned int packetSize = sockets::networkToHost32(*(unsigned int *)bufOffset);
		int totalLen = static_cast<int>(packetSize) + CLIENT_PACKET_HEAD_LEN;
		if (readableBbyte < totalLen)
		{
			MDLog("fishUser::onMessage 2, %d %d", readableBbyte, totalLen);
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
		MDLog("protobuff ParseFromArray failed close");
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

void fishUser::handlePingReq(const fish::ping_req &req)
{

}

void fishUser::handleLoginReq(const fish::login_req &req)
{
	const std::string &acc = req.acc();
	const std::string &pwd = req.pwd();
	MDLog("login_req %s, %s", acc.c_str(), pwd.c_str());

	{
		fish::login_rsp rsp;
		rsp.set_state(0);
		rsp.set_game_id("test_fish");
		rsp.add_room_type(1);


		sendPacket(&rsp, fish::CMD_LOGIN_RSP);
	}

}

void fishUser::handleEnterRoomReq(const fish::enter_room_req &req)
{

}

void fishUser::handleFireReq(const fish::fire_req &req)
{

}

#define DEFAULT_BUFF_SIZE   (1*1024*1024)
void fishUser::sendPacket(const google::protobuf::Message* msg, int cmdId)
{
	if (isClose_)
	{
		return;
	}
	static  char msgBuf[DEFAULT_BUFF_SIZE];
	int needSize = msg->ByteSize();
	msg->SerializeToArray(msgBuf, DEFAULT_BUFF_SIZE);

	fish::packet root;
	root.set_cmd(cmdId);
	root.set_body(msgBuf, needSize);

	int rootSize = root.ByteSize();
	root.SerializeToArray(msgBuf + 4, DEFAULT_BUFF_SIZE);
	*((unsigned int *)msgBuf) = sockets::hostToNetwork32(rootSize);


	connection_->send(msgBuf, rootSize + 4);
}