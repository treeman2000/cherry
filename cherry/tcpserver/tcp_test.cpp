#include<thread>
#include<arpa/inet.h>
#include<cstring>
#include<sys/socket.h>
#include<unistd.h>
#include"tcpserver.h"
#include"tcp_connection.h"
#include"cherry/net/eventloop.h"
#include"cherry/utils/logging.h"
using namespace cherry;

EventLoop* g_loop = nullptr;
void createClientAndConnect(){
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    while(!g_loop){
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
    for(int i=0;i<3;i++){
        int n = send(sockfd, "hello", 5, 0);
        if(n < 0){
            LOG_ERROR<<"send failed "<<strerror(errno);
            exit(1);
        }else{
            LOG_INFO<<"send success, send "<<n<<" bytes";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
    close(sockfd);
}

void createServerAndListen(){
    EventLoop loop;
    TcpServer server(&loop, "0.0.0.0", 80);
    server.setConnectionCallback([](const TcpConnectionPtr& conn){
        LOG_INFO<<"new connection";
    });
    server.setMessageCallback([](const TcpConnectionPtr& conn, Buffer* buf, mytime::Time t){
        LOG_INFO<<"new message: "<<buf<<", time:"<<t.toString();
    });
    server.start();
    g_loop = &loop;
    loop.loop();
}

void TestTcpserver(){
    std::thread tServer(createServerAndListen);
    std::thread tCli(createClientAndConnect);

    tCli.join();
    g_loop->quit();
    tServer.join();
    LOG_INFO<<"TestTcpserver done ";
}

int main(){
    setLogLevel(LogLevel::INFO);
    TestTcpserver();
    return 0;
}