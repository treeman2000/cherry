#include<thread>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include<iostream>
#include"eventloop.h"
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

int main(){
    cherry::setLogLevel(cherry::LogLevel::DEBUG);
    // testCreateEventLoop();
    testCreateEventLoopFail();
    return 0;
}
