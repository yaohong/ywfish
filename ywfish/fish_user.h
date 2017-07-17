#ifndef _fish_user_h__
#define _fish_user_h__

#include "./mayday/net/Callbacks.h"

using namespace mayday;
using namespace mayday::net;

class fishUser {
public:
	fishUser(const TcpConnectionPtr &conn);
	~fishUser();
public:
	//�ͻ��˶Ͽ�������
	void onDisconnect();
	void onMessage(const mayday::net::TcpConnectionPtr& conn, mayday::net::Buffer* buf);
private:
	void onPackage(const char *buff, int32 buffLen);
private:
	TcpConnectionPtr connection_;

	bool isClose_;
};











#endif