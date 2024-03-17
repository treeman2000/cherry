#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include<thread>
#include"cherry/utils/noncopyable.h"

namespace cherry{

class EventLoop: noncopyable{
public:
    EventLoop();
    ~EventLoop();

    void loop();
    // void runInLoop();

    bool isInLoopThread();
    void assertInLoopThread();

private:
    bool looping_;
    // 构造时记录下这个eventloop对应的线程id
    const std::thread::id tid_;
};

}

#endif