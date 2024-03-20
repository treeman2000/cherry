#include<poll.h>
#include"channel.h"
#include"eventloop.h"
#include"cherry/utils/logging.h"

namespace cherry{

const int Channel::kNoneEvent = 0;
const int Channel::kReadEvent = POLLIN | POLLPRI;
const int Channel::kWriteEvent = POLLOUT;


Channel::Channel(EventLoop* loop, int fd)
    :loop_(loop), fd_(fd), events_(0), revents_(0){
}

Channel::~Channel(){
}

void Channel::handleEvent(mytime::Time receiveTime){
    LOG_DEBUG << "handleEvent, fd:" << fd_ << " revents:" << revents_;
    // POLLNVAL指明fd不是一个有效的socket的fd，POLLERR指明网络出问题了之类的。
    if(revents_ & (POLLNVAL | POLLERR) ){
        LOG_WARN << "handleEvent error";
        if(errorCallback_){
            errorCallback_();
        }
    }
    if(revents_ & POLLIN){
        if(readCallback_){
            readCallback_(receiveTime);
        }
    }
    if(revents_ & POLLOUT){
        if(writeCallback_){
            writeCallback_();
        }
    }
}

void Channel::update(){
    // 因为只考虑epoll，所以不用维护fds数组。只通知eventloop就好。
    loop_->update(this);
}

}