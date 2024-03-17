#include<sys/epoll.h>
#include<unistd.h>
#include"poller.h"
#include"channel.h"
#include"cherry/utils/logging.h"

namespace cherry{

EPoller::EPoller(EventLoop* loop)
    : loop_(loop), epollfd_(epoll_create1(EPOLL_CLOEXEC)), events_(kInitEventListSize){
    if (epollfd_ < 0){
        LOG_FATAL << "EPoller construct failed, epoll_create1 error";   
    }
    LOG_DEBUG << channels_.size();
}

EPoller::~EPoller(){
    close(epollfd_);
}

mytime::Time EPoller::poll(int timeoutMs, ChannelList& activeChannels){
    loop_->assertInLoopThread();
    int numEvents = epoll_wait(epollfd_, events_.data(), events_.size(), timeoutMs);
    mytime::Time now = mytime::now();
    if(numEvents > 0){
        LOG_DEBUG << numEvents << " events happened";
        fillActiveChannels(numEvents, activeChannels);
        if(numEvents == static_cast<int>(events_.size())){
            events_.resize(events_.size() * 2);
        }
    }else if(numEvents == 0){
        LOG_DEBUG << "nothing happened";
    }else{
        LOG_ERROR << "EPoller::poll() failed, errno = " << errno << ", error message = " << strerror(errno);
    }
    return now;
}

void EPoller::fillActiveChannels(int numEvents ,ChannelList& activeChannels){
    activeChannels.clear();
    for(int i=0;i<numEvents;i++){
        auto ch = static_cast<Channel*>(events_[i].data.ptr);
        ch->set_revents(events_[i].events);
        activeChannels.push_back(ch);
    }
}

void EPoller::updateChannel(Channel* channel){
    LOG_DEBUG << "fd = " << channel->fd() << " events = " << channel->events();
    LOG_DEBUG << channels_.size();
    LOG_DEBUG << channels_.count(channel->fd());
    loop_->assertInLoopThread();
    // todo: 考虑删除channel
    if(channels_.count(channel->fd()) == 0){
        // insert into map if not exits
        channels_[channel->fd()] = channel;
        
        update(EPOLL_CTL_ADD, channel);
    }else{
        // update
        update(EPOLL_CTL_MOD, channel);
    }
}

void EPoller::update(int operation, Channel* channel){
    LOG_DEBUG << "fd = " << channel->fd() << " events = " << channel->events();
    epoll_event event;
    event.events = channel->events();
    // 存放数据用的，也可以不用。放了channel的指针，这样在事件发生时，可以通过event.data.ptr拿到channel，不用遍历map。
    event.data.ptr = channel;
    int fd = channel->fd();
    if(epoll_ctl(epollfd_, operation, fd, &event) < 0){
        LOG_ERROR << "epoll_ctl error, operation = " << operation << ", fd = " << fd;
    }
}

}