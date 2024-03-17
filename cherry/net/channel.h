#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
#include <memory>

#include "cherry/utils/noncopyable.h"
#include "cherry/utils/time_my.h"

namespace cherry{

class EventLoop;

/* 
负责把某个fd上的读写事件分发到对应的回调函数。
Channel自己不持有fd。
*/
class Channel : noncopyable{
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(mytime::Time)> ReadEventCallback;

    Channel(EventLoop* loop, int fd);
    ~Channel();

    int fd() const { return fd_; }
    int events() const { return events_; }

    void handleEvent(mytime::Time receiveTime);
    void setReadCallback(ReadEventCallback cb) { readCallback_ = std::move(cb); }
    void setWriteCallback(EventCallback cb) { writeCallback_ = std::move(cb); }
    void setCloseCallback(EventCallback cb) { closeCallback_ = std::move(cb); }
    void setErrorCallback(EventCallback cb) { errorCallback_ = std::move(cb); }

    void enableReading() { events_ |= kReadEvent; update(); }
    void disableReading() { events_ &= ~kReadEvent; update(); }
    void enableWriting() { events_ |= kWriteEvent; update(); }
    void disableWriting() { events_ &= ~kWriteEvent; update(); }
    void disableAll() { events_ = kNoneEvent; update(); }
    bool isWriting() const { return events_ & kWriteEvent; }
    bool isReading() const { return events_ & kReadEvent; }

private:
    EventLoop* loop_;
    const int fd_;

    int events_; //关心的事件
    int revents_; // 实际接收到的事件。保证只会接收到关心的事件。

    static const int kNoneEvent;
    static const int kReadEvent;
    static const int kWriteEvent;

    // 告诉这个channel的持有者，更新关心的事件。持有者最终会调用epoll_ctl告诉内核。类似观察者模式。
    void update();

    ReadEventCallback readCallback_;
    EventCallback writeCallback_;
    EventCallback closeCallback_;
    EventCallback errorCallback_;
};

}

#endif //CHANNEL_H