#include<thread>
#include<assert.h>
#include<stdio.h>
#include<unistd.h>
#include<iostream>
#include<sys/timerfd.h>

#include"eventloop.h"
#include"channel.h"
#include"cherry/utils/logging.h"

using namespace cherry;
using namespace std;

EventLoop* g_loop;

void callback()
{
  cout<<"callback(): tid = "<<hash<std::thread::id>()(this_thread::get_id())<<endl;
  EventLoop anotherLoop;
}

void threadFunc()
{
  cout<<"threadFunc(): tid = "<< hash<std::thread::id>()(this_thread::get_id())<<endl;
  EventLoop loop;

  loop.loop();
}

void testCreateEventLoop(){
    cout<<"main():tid ="<<hash<std::thread::id>()(this_thread::get_id())<<endl;
    EventLoop loop;

    thread t(threadFunc);

    loop.loop();
    t.join();
    return;
}

void testCreateEventLoopFail(){
    cout<<"main():tid ="<<hash<std::thread::id>()(this_thread::get_id())<<endl;
    EventLoop loop;
    EventLoop loop2;
    loop.loop();

    return;
}

void testChannelPollerEventloop(){
    LOG_DEBUG << "in testChannelPollerEventloop()";
    EventLoop* loop = new EventLoop();
    auto timeOut = [loop](mytime::Time t){
        cout<<"time out! current time: "<<t.toString()<<endl;
        loop->quit();
    };

    int timerfd = timerfd_create(CLOCK_MONOTONIC, TFD_NONBLOCK | TFD_CLOEXEC);
    Channel ch(loop, timerfd);

    ch.setReadCallback(timeOut);
    // 此时会告诉poller更新监听的事件
    ch.enableReading();


    itimerspec howlong;
    bzero(&howlong, sizeof howlong);
    howlong.it_value.tv_sec = 3;
    timerfd_settime(timerfd, 0, &howlong, NULL);

    loop->loop();

    close(timerfd);
    delete loop;
}

int main(){
    cout<<"start test"<<endl;
    cherry::setLogLevel(cherry::LogLevel::DEBUG);
    // testCreateEventLoop();
    // testCreateEventLoopFail();
    testChannelPollerEventloop();
    return 0;
}
