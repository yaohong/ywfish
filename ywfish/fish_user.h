#ifndef _fish_user_h__
#define _fish_user_h__

#include "./mayday/net/Callbacks.h"
#include "fish.pb.h"
using namespace mayday;
using namespace mayday::net;

class fishUser {
public:
	fishUser(const TcpConnectionPtr &conn);
	~fishUser();
public:
	//客户端断开连接了
	void onDisconnect();
	void onMessage(const mayday::net::TcpConnectionPtr& conn, mayday::net::Buffer* buf);
private:
	void onPackage(const char *buff, int32 buffLen);
private:
	void handlePingReq(const fish::ping_req &req);
	void handleLoginReq(const fish::login_req &req);
	void handleEnterRoomReq(const fish::enter_room_req &req);
	void handleFireReq(const fish::fire_req &req);
private:
	void sendPacket(const google::protobuf::Message* msg, int cmdId);
private:
	TcpConnectionPtr connection_;

	bool isClose_;
};











#endif