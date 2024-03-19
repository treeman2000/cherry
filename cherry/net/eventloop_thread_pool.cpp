// Copyright 2010, Shuo Chen.  All rights reserved.
// http://code.google.com/p/muduo/
//
// Use of this source code is governed by a BSD-style license
// that can be found in the License file.

// Author: Shuo Chen (chenshuo at chenshuo dot com)

#include<stdio.h>
#include<assert.h>
#include "eventloop_thread_pool.h"
#include "eventloop.h"
#include "eventloop_thread.h"


namespace cherry{

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop, const string& nameArg)
  : baseLoop_(baseLoop),
    name_(nameArg),
    started_(false),
    numThreads_(0),
    next_(0)
{
}

EventLoopThreadPool::~EventLoopThreadPool(){
}

void EventLoopThreadPool::start(const ThreadInitCallback& cb){
    assert(!started_);
    baseLoop_->assertInLoopThread();

    started_ = true;

    for (int i = 0; i < numThreads_; ++i){
        auto t = std::make_unique<EventLoopThread>();
        threads_.push_back(t);
        loops_.push_back(t->startLoop());
    }
    if (numThreads_ == 0 && cb){
        cb(baseLoop_);
    }
}

EventLoop* EventLoopThreadPool::getNextLoop(){
    baseLoop_->assertInLoopThread();
    assert(started_);
    EventLoop* loop = baseLoop_;

    if (!loops_.empty()){
        // round-robin
        loop = loops_[next_];
        next_ = (next_ + 1) % loops_.size();
    }
    return loop;
}


EventLoop* EventLoopThreadPool::getLoopForHash(size_t hashCode){
    baseLoop_->assertInLoopThread();
    EventLoop* loop = baseLoop_;

    if (!loops_.empty()){
        loop = loops_[hashCode % loops_.size()];
    }
  return loop;
}

std::vector<EventLoop*> EventLoopThreadPool::getAllLoops(){
    baseLoop_->assertInLoopThread();
    assert(started_);
    if (loops_.empty()){
        return std::vector<EventLoop*>(1, baseLoop_);
    }
    else{
        return loops_;
    }
}


} // namespace cherry