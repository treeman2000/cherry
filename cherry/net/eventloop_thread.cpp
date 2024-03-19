#include<functional>
#include"eventloop_thread.h"
#include"eventloop.h"
#include"cherry/utils/logging.h"

namespace cherry{


EventLoopThread::EventLoopThread()
  : mutex_(), lock_(mutex_), cv_(), loop_(nullptr)
{
}

EventLoopThread::~EventLoopThread()
{
    if(loop_ != nullptr){
        loop_->quit();
        t_->join();
    }
}

EventLoop* EventLoopThread::startLoop(){
    t_ = std::make_unique<std::thread>(&EventLoopThread::threadFunc,this);
    cv_.wait(lock_,[this]{return loop_!=nullptr;});
    return loop_;
}

void EventLoopThread::threadFunc(){
    EventLoop loop;
    loop_ = &loop;
    cv_.notify_one();
    loop.loop();
    LOG_DEBUG<<"EventLoopThread::threadFunc() exits";
}

}