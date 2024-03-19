#include<thread>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<iostream>
#include<sys/timerfd.h>

#include"eventloop_thread.h"
#include"eventloop.h"
#include"channel.h"
#include"cherry/utils/logging.h"

using namespace cherry;
using namespace std;

void testEventLoopThread(){
    LOG_DEBUG << "in testEventLoopThread()";
    EventLoopThread loopThread;
    EventLoop* loop = loopThread.startLoop();
    LOG_DEBUG<<"call runInLoop() in main thread"<< hash<std::thread::id>()(this_thread::get_id());
    loop->runInLoop([loop]{
        LOG_DEBUG<<"runInLoop(): tid = "<<hash<std::thread::id>()(this_thread::get_id());
    });
    

    return;
}

int main(){
    cout<<"start test"<<endl;
    cherry::setLogLevel(cherry::LogLevel::DEBUG);
    testEventLoopThread();
    return 0;
}
