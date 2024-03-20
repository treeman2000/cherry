# pragma once

#include<memory>
#include<string>
#include"callbacks.h"

namespace cherry{

class Channel;
class EventLoop;

/*
表示一次tcp连接。
并不创建socketfd，但是析构时会关闭socketfd。
因为从自身返回的shared_ptr不是从别的shared_ptr拷贝过来的，而是新建的，所以不知道自己已经被几个shared_ptr引用了。
enable_shared_from_this使得这个类的方法能够返回自身的shared_ptr，并正确处理引用计数，防止二次析构。
*/
class TcpConnection : public std::enable_shared_from_this<TcpConnection>{
public:
    TcpConnection(EventLoop* loop, int sockfd, const std::string& ipAddr, int port);
    ~TcpConnection();

    void setConnectionCallback(const ConnectionCallback& cb){
        connectionCallback_ = cb;
    }
    void setMessageCallback(const MessageCallback& cb){
        messageCallback_ = cb;
    }
    void connectEstablished();
private:
    enum State {kConnecting, kConnected, kDisconnecting, kDisconnected};
    
    void handleRead(mytime::Time);
    void handleWrite();
    
    EventLoop* loop_;
    int sockfd_;
    std::string ipAddr_;
    int port_;
    ConnectionCallback connectionCallback_;
    MessageCallback messageCallback_;
    // Buffer inputBuffer_;
    // Buffer outputBuffer_;
    State state_;
    std::unique_ptr<Channel> channel_;
};

}