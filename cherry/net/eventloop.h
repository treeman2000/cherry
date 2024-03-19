#ifndef EVENTLOOP_H
#define EVENTLOOP_H

#include<thread>
#include<vector>
#include<mutex>
#include<functional>
#include"cherry/utils/noncopyable.h"

namespace cherry{
// 前向声明，避免头文件的循环依赖
class EPoller;
class Channel;
constexpr int timeoutMs = 1000;

class EventLoop: noncopyable{
public:
    typedef std::function<void()> Func;

    EventLoop();
    ~EventLoop();

    void loop();
    void runInLoop(Func callback);
    void quit();

    bool isInLoopThread();
    void assertInLoopThread();

    // 当修改channel关心的事件时，eventloop负责调用poller把更新告诉内核
    void update(Channel* ch);

private:
    typedef std::vector<Channel*> ChannelList;

    bool looping_;
    bool quit_;
    // 构造时记录下这个eventloop对应的线程id
    const std::thread::id tid_;

    std::unique_ptr<EPoller> poller_;
    ChannelList activeChannels_;

    // for runInLoop
    void doPendingFuncs();
    std::mutex mutex_;
    std::vector<Func> pendingFuncs_;
    // 为了在调用runInLoop后能立即执行，而不要阻塞在poll上
    int wakeupFd_;
    std::unique_ptr<Channel> wakeupChannel_;
    void wakeup();
    bool isCallingPendingFuncs_;
};

}

#endif