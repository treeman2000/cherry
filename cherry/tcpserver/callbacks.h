#pragma once

#include<memory>
#include<functional>
#include"cherry/utils/time_my.h"

namespace cherry{

// class Buffer;
typedef char Buffer;
class TcpConnection;

typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef std::function<void (const TcpConnectionPtr&, Buffer*, mytime::Time)> MessageCallback;

}