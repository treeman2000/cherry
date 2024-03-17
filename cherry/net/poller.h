// 并发连接数较大而活跃比例不高时，epoll的效率优于poll和select
#ifndef EPOLL_H
#define EPOLL_H

#include<vector>
#include<map>
#include"cherry/utils/time_my.h"
#include"eventloop.h"

namespace cherry{

class Channel;

class EPoller{
public:
    static constexpr int kInitEventListSize = 16;
    typedef std::vector<Channel*> ChannelList;
    
    EPoller(EventLoop* loop);
    ~EPoller();

    // 获取活跃的channels
    mytime::Time poll(int timeoutMs, ChannelList& activeChannels);
    void fillActiveChannels(int numEvents, ChannelList& activeChannels);
    
    // 更新关注的channel
    void updateChannel(Channel* channel);
    void update(int operation, Channel* channel);

private:
    typedef std::map<int, Channel*> ChannelMap;
    typedef std::vector<struct epoll_event> EventList;

    EventLoop* loop_;
    int epollfd_;
    // 每次epoll_wait返回的活跃事件都会填入这个vector
    EventList events_;
    ChannelMap channels_;
};


}

#endif //EPOLL_H