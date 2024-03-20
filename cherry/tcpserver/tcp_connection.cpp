#include<unistd.h>
#include"tcp_connection.h"
#include"cherry/net/eventloop.h"
#include"cherry/net/channel.h"
#include"cherry/utils/logging.h"

namespace cherry{

TcpConnection::TcpConnection(EventLoop* loop, int sockfd, const std::string& ipAddr, int port)
  : loop_(loop),
    sockfd_(sockfd),
    ipAddr_(ipAddr),
    port_(port),
    state_(State::kConnecting),
    channel_(std::make_unique<Channel>(loop, sockfd))
{
    channel_->setReadCallback(std::bind(&TcpConnection::handleRead, this, std::placeholders::_1));
}

TcpConnection::~TcpConnection(){
    close(sockfd_);
}

void TcpConnection::handleRead(mytime::Time t){
    LOG_DEBUG<<"TcpConnection::handleRead";
    char buf[1024];
    int n = read(sockfd_, buf, 1024);
    if(n<0){
        LOG_ERROR<<"read error "<<strerror(errno);
        return;
    }
    messageCallback_(shared_from_this(), buf, t);
    return;
}

void TcpConnection::handleWrite(){
    // todo
}

void TcpConnection::connectEstablished(){
    loop_->assertInLoopThread();
    channel_->enableReading();
    LOG_DEBUG<<"TcpConnection::connectEstablished and enable reading ok, connFd:"<<sockfd_;
    state_ = State::kConnected;
    connectionCallback_(shared_from_this());
}



}