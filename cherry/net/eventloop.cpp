#include<thread>
#include<cstdlib>
#include<assert.h>
#include<sys/eventfd.h>
#include<unistd.h>
#include"eventloop.h"
#include"poller.h"
#include"channel.h"
#include"cherry/utils/logging.h"

namespace cherry{
thread_local EventLoop* loopInThisThread = nullptr;

const int kPollTimeMs = 10000;

int createEventfd(){
  int evtfd = eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evtfd < 0)
  {
    LOG_ERROR << "Failed in eventfd";
    abort();
  }
  return evtfd;
}

EventLoop::EventLoop()
  : looping_(false), 
    quit_(false), 
    tid_(std::this_thread::get_id()), 
    poller_(std::make_unique<EPoller>(this)),
    wakeupFd_(createEventfd()),
    wakeupChannel_(std::make_unique<Channel>(this, wakeupFd_))
    {
    LOG_DEBUG << "creat loop in thread "<< std::hash<std::thread::id>()(tid_);
    // 检查这个线程是否已经创建了EventLoop
    if(loopInThisThread != nullptr){
        LOG_ERROR << "loop already created in thread " << std::hash<std::thread::id>()(tid_);
        std::exit(0);
    }else{
        loopInThisThread = this;
    }

    // 初始化wakeup channel
    wakeupChannel_->setReadCallback([this](mytime::Time t){
        LOG_DEBUG << "EventLoop::wakeupChannel_ read callback "<< t.toString();
        uint64_t one = 1;
        ssize_t n = read(wakeupFd_, &one, sizeof one);
        if(n != sizeof one){
            LOG_ERROR << "EventLoop::wakeupChannel_ read error";
        }
    });
    wakeupChannel_->enableReading();

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

        doPendingFuncs();
    }
    // LOG_DEBUG << "looping in thread "<<std::hash<std::thread::id>()(tid_);
    // std::this_thread::sleep_for(std::chrono::seconds(3));
    
    looping_ = false;
    return; 
}

void EventLoop::runInLoop(Func callback){
    if(isInLoopThread()){
        callback();
    }else{
        std::lock_guard<std::mutex> lock(mutex_);
        pendingFuncs_.push_back(callback);
    }

    // 不在IO线程 或者 正在IO线程里运行doPendingFuncs,其中的func又调用了一次runInLoop
    if(!isInLoopThread() || isCallingPendingFuncs_){
        wakeup();
    }
}

void EventLoop::wakeup(){
    uint64_t one = 1;
    ssize_t n = write(wakeupFd_, &one, sizeof one);
    if(n != sizeof one){
        LOG_ERROR << "EventLoop::wakeup() write error ";
    }
}

void EventLoop::doPendingFuncs(){
    std::vector<Func> funcs;
    isCallingPendingFuncs_ = true;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        pendingFuncs_.swap(funcs);
    }
    for(auto& func: funcs){
        func();
    }

    isCallingPendingFuncs_ = false;
}

void EventLoop::quit(){
    quit_ = true;
    // 如果在IO线程调用就不用wake up了，因为会在poll完，处理完PendingFuncs，之后再退出
    // 否则要wakeup一下，把func全执行完再退出
    if(!isInLoopThread()){
        wakeup();
    }
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