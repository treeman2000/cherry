#include<cstring>
#include<sys/socket.h>
#include<unistd.h>
#include"acceptor.h"
#include"cherry/net/eventloop.h"
#include"cherry/utils/logging.h"

namespace cherry{

int createNonblockingOrDie(){
    int sockfd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK | SOCK_CLOEXEC, IPPROTO_TCP);
    if (sockfd < 0){
        LOG_ERROR << "sockets::createNonblockingOrDie";
        exit(1);
    }
    return sockfd;
}

Acceptor::Acceptor(EventLoop* loop, const std::string& ipAddr, int port, bool reuseport)
  : loop_(loop), 
    socketFd_(createNonblockingOrDie()),
    acceptChannel_(loop, socketFd_),
    listening_(false)
{
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    // fix: use ipAddr instead of INADDR_ANY
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(static_cast<short>(port));
    if (bind(socketFd_, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) == -1) {
        LOG_ERROR << "Failed to bind socket";
        close(socketFd_);
        exit(1);
    }
    LOG_DEBUG<<"bind success";
    acceptChannel_.setReadCallback(std::bind(&Acceptor::handleRead, this, std::placeholders::_1));
}

Acceptor::~Acceptor(){
    close(socketFd_);
}

void Acceptor::listen(){
    if(::listen(socketFd_,SOMAXCONN) == -1){
        LOG_ERROR<<"listen failed";
        exit(1);
    }
    LOG_DEBUG<<"Acceptor::listen success";
    listening_ = true;
    acceptChannel_.enableReading();
}

std::pair<std::string,int> Acceptor::convertSockaddrToIpPort(const sockaddr_in& addr){
    char ip[INET_ADDRSTRLEN];
    ::inet_ntop(AF_INET, &addr.sin_addr, ip, sizeof ip);
    int port = ntohs(addr.sin_port);
    return std::make_pair(ip, port);
}

void Acceptor::handleRead(mytime::Time t){
    loop_->assertInLoopThread();
    sockaddr_in clientAddr;
    // 新连接的文件描述符 accept( socket文件描述符, 客户端地址, 写入的地址长度（比如ipv4和v6的地址长度就不一样）)
    socklen_t len = sizeof clientAddr;
    int connfd = ::accept(socketFd_ , reinterpret_cast<sockaddr*>(&clientAddr), &len);
    if (connfd >= 0){
        if (newConnectionCallback_){
            LOG_DEBUG<<"Acceptor::handleRead, new connection";
            auto [ipAddr, port] = convertSockaddrToIpPort(clientAddr);
            newConnectionCallback_(connfd, ipAddr, port);
        }else{
            LOG_DEBUG<<"Acceptor::handleRead, no newConnectionCallback_";
            close(connfd);
        }
  }
}

}