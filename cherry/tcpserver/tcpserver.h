# pragma once

#include<memory>
#include<functional>
#include<map>
#include"acceptor.h"
#include"callbacks.h"

namespace cherry{

class EventLoop;

class TcpServer{
public:
    
    TcpServer(EventLoop* loop,const std::string& ipAddr, int port);
    ~TcpServer();
    
    void start();
    void setConnectionCallback(const ConnectionCallback& cb){
        connectionCallback_ = cb;
    }
    void setMessageCallback(const MessageCallback& cb){
        messageCallback_ = cb;
    }
private:
    typedef std::map<int, TcpConnectionPtr> ConnectionMap;
    void newConnection(int sockfd, const std::string& ipAddr, int port);
    
    EventLoop* loop_;
    std::unique_ptr<Acceptor> acceptor_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    bool started_;
    int nextConnId_;
    ConnectionMap connections_;
};

} // namespace cherry
