#pragma once

#include<functional>
#include<string>
#include<vector>
#include<memory>

using std::string;
using std::vector;

namespace cherry{

class EventLoop;
class EventLoopThread;

class EventLoopThreadPool{
public:
    typedef std::function<void(EventLoop*)> ThreadInitCallback;

    EventLoopThreadPool(EventLoop* baseLoop, const string& nameArg);
    ~EventLoopThreadPool();
    void setThreadNum(int numThreads) { numThreads_ = numThreads; }
    void start(const ThreadInitCallback& cb = ThreadInitCallback());

    EventLoop* getNextLoop();

    // 每次相同参数的调用都返回同一个EventLoop
    EventLoop* getLoopForHash(size_t hashCode);

    vector<EventLoop*> getAllLoops();

    bool started() const { return started_; }

    const string& name() const { return name_; }

private:
    EventLoop* baseLoop_;
    string name_;
    bool started_;
    int numThreads_;
    int next_;
    vector<std::unique_ptr<EventLoopThread>> threads_;
    vector<EventLoop*> loops_;
};

} // namespace cherry
