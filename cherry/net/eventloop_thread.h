#pragma once

#include<thread>
#include<memory>
#include<mutex>
#include<condition_variable>
#include"cherry/utils/noncopyable.h"

namespace cherry{

class EventLoop;

class EventLoopThread{
public:
    EventLoopThread(/* args */);
    ~EventLoopThread();
    // 启动线程，返回loop指针
    EventLoop* startLoop();
private:
    void threadFunc();
    std::mutex mutex_;
    // 必须传一个mutex进去，不然是空的
    std::unique_lock<std::mutex> lock_;
    std::condition_variable cv_;
    std::unique_ptr<std::thread> t_;
    EventLoop* loop_;
};



}