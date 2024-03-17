#include<thread>
#include<cstdlib>
#include<assert.h>
#include"eventloop.h"
#include"poller.h"
#include"channel.h"
#include"cherry/utils/logging.h"

namespace cherry{
thread_local EventLoop* loopInThisThread = nullptr;

const int kPollTimeMs = 10000;

EventLoop::EventLoop()
    : looping_(false), quit_(false), tid_(std::this_thread::get_id()), poller_(std::make_unique<EPoller>(this)){
    LOG_DEBUG << "creat loop in thread "<< std::hash<std::thread::id>()(tid_);
    // 检查这个线程是否已经创建了EventLoop
    if(loopInThisThread != nullptr){
        LOG_ERROR << "loop already created in thread " << std::hash<std::thread::id>()(tid_);
        std::exit(0);
    }else{
        loopInThisThread = this;
    }

    return;
}

EventLoop::~EventLoop(){
    assert(!looping_);
    loopInThisThread = nullptr;
}

void EventLoop::loop(){
    assert(!looping_);
    looping_ = true;
    
    while(!quit_){
        // poll
        mytime::Time t = poller_->poll(timeoutMs,activeChannels_);
        // handle events
        for(auto ch: activeChannels_){
            ch->handleEvent(t);
        }
    }
    // LOG_DEBUG << "looping in thread "<<std::hash<std::thread::id>()(tid_);
    // std::this_thread::sleep_for(std::chrono::seconds(3));
    
    looping_ = false;
    return; 
}

void EventLoop::quit(){
    quit_ = true;
}

bool EventLoop::isInLoopThread(){
    return std::this_thread::get_id() == tid_;
}

void EventLoop::assertInLoopThread(){
    if(!isInLoopThread()){
        LOG_FATAL << "EventLoop::assertInLoopThread, loop created in threadId_ = " 
            << std::hash<std::thread::id>()(tid_) 
            << ", current thread id = " << std::hash<std::thread::id>()(std::this_thread::get_id());
        std::exit(0);
    }
}

void EventLoop::update(Channel* ch){
    assertInLoopThread();
    poller_->updateChannel(ch);
}

}