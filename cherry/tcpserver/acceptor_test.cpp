#include<unistd.h>
#include<thread>
#include"acceptor.h"
#include"cherry/net/eventloop.h"
#include"cherry/utils/logging.h"

using namespace cherry;

void TestConvertSockaddrToIpPort(){
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(80);
    std::pair<std::string,int> result = cherry::Acceptor::convertSockaddrToIpPort(addr);
    LOG_INFO<<"ip:"<<result.first<<" port:"<<result.second;
}

volatile bool serverOK = false;
void createClientAndConnect(){
    while(!serverOK){
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    // create a client and connect to the server
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(80);
    if(connect(sockfd, reinterpret_cast<sockaddr*>(&serverAddr), sizeof(serverAddr)) < 0){
        LOG_ERROR<<"connect failed "<<strerror(errno);
        exit(1);
    }
    LOG_INFO<<"connect success, connFd:"<<sockfd;
    close(sockfd);
}

EventLoop* g_loop;
void createServerAndListen(){
    EventLoop loop;
    Acceptor acceptor(&loop,"0.0.0.0", 80);
    acceptor.setNewConnectionCallback([](int sockfd, const std::string& ipAddr, int port){
        LOG_INFO<<"new connection, sockfd:"<<sockfd<<" ipAddr:"<<ipAddr<<" port:"<<port;
    });
    acceptor.listen();
    g_loop = &loop;
    LOG_INFO<<"start listening...";
    serverOK = true;
    loop.loop();
}

void TestAcceptor(){
    std::thread tServer(createServerAndListen);
    std::thread tCli(createClientAndConnect);

    tCli.join();
    g_loop->quit();
    tServer.join();
    LOG_INFO<<"TestAcceptor done";
}

int main(){
    // TestConvertSockaddrToIpPort();
    TestAcceptor();
    return 0;
}