# pragma once

#include<functional>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<string>
#include"cherry/utils/noncopyable.h"
#include"cherry/utils/time_my.h"
#include"cherry/net/channel.h"

namespace cherry{

class EventLoop;
class Channel;


// 负责持有socket fd，以及bind和listen
class Acceptor : noncopyable{
public:
    // 便于新连接来的时候，可以获取到连接的地址
    typedef std::function<void (int sockfd, const std::string& ipAddr, int port)> NewConnectionCallback;
    Acceptor(EventLoop* loop, const std::string& ipAddr, int port, bool reuseport = false);
    ~Acceptor();

    void setNewConnectionCallback(const NewConnectionCallback& cb)
    { newConnectionCallback_ = cb; }

    void listen();
    bool listening() const { return listening_; }

    static std::pair<std::string,int> convertSockaddrToIpPort(const sockaddr_in& addr);
private:
    void handleRead(mytime::Time);

    EventLoop* loop_;
    int socketFd_;
    sockaddr_in serverAddr;
    Channel acceptChannel_;
    NewConnectionCallback newConnectionCallback_;
    bool listening_;
    int idleFd_;
};

}