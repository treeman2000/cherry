#include<functional>
#include"tcpserver.h"
#include"tcp_connection.h"
#include"acceptor.h"
#include"cherry/utils/logging.h"

namespace cherry{

TcpServer::TcpServer(EventLoop* loop,const std::string& ipAddr, int port)
  : loop_(loop),
    acceptor_(std::make_unique<Acceptor>(loop, ipAddr, port)),
    started_(false),
    nextConnId_(1)
{
    acceptor_->setNewConnectionCallback(
        std::bind(&TcpServer::newConnection, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
}

TcpServer::~TcpServer(){
    // delete all connections
}

void TcpServer::start(){
    // 启动acceptor，具体是在listen中的enableReading的时候，把acceptor的channel加入到了poller中
    if(!started_){
        started_ = true;
        acceptor_->listen();
    }
}

void TcpServer::newConnection(int sockfd, const std::string& ipAddr, int port){
    LOG_INFO<<"TcpServer::newConnection ["<<ipAddr<<":"<<port<<"] - new connection ["<<sockfd<<"]";
    TcpConnectionPtr conn = std::make_shared<TcpConnection>(loop_, sockfd, ipAddr, port);
    conn->setConnectionCallback(connectionCallback_);
    conn->setMessageCallback(messageCallback_);
    conn->connectEstablished();
    // tcpserver控制所有它产生的tcpConn的shared_ptr
    connections_[nextConnId_++] = conn;
}

}