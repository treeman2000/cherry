#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include<thread>
#include"cherry/utils/noncopyable.h"
#include"channel.h"

namespace cherry{

class EventLoop: noncopyable{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    // void runInLoop();

    bool isInLoopThread();
    void assertInLoopThread();

    // 当修改channel关心的事件时，eventloop负责调用poller把更新告诉内核
    void update(Channel* ch);

private:
    bool looping_;
    // 构造时记录下这个eventloop对应的线程id
    const std::thread::id tid_;
};

}

#endif