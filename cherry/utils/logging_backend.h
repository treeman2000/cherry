#include<thread>
#include<string>
#include<cstring>
#include<mutex>
#include<stack>
#include<queue>
#include <memory>
#include <condition_variable>
#include"noncopyable.h"

namespace cherry{

constexpr int _4K = 4*1024;
// constexpr int _4M = _4K * 1024;
constexpr int _4M = 400;

template<int N>
class FixedBuffer: noncopyable{
public:
    // 用于前端写入到buffer
    int avail() const {return static_cast<int>(arr.size()) - cur_;}
    char* current() {return &arr[cur_];}
    void add(int len) { cur_ += len; }

    void append(const char* p, int len){
        int actual_size = std::min(len, avail());
        memcpy(current(), p, actual_size);
        cur_ += actual_size;
    }

    // 用于把buffer写入后端
    int length() const {return cur_;}
    char* data() {return &arr[0];}

    void clear(){cur_ = 0;}

private:
    std::array<char, N> arr;
    int cur_ = 0;
};

class Backend{
public:
    Backend() = default;
    virtual ~Backend() = default;
    virtual void append(const char* p, int len) = 0;
};

class DefaultBackend: public Backend{
public:
    void append(const char* p, int len) override;
};

class AsyncBackend: public Backend{
public:
    AsyncBackend();
    ~AsyncBackend();
    void append(const char* p, int len) override;
    static constexpr int NUM_BUFFERS = 10;
private:
    bool stop_;
    typedef std::unique_ptr<FixedBuffer<_4M>> BufferPtr;
    typedef std::stack<BufferPtr> BufferStack;
    typedef std::vector<BufferPtr> BufferVec;
    // mtx_保护下面的数据结构, mtx_backend_专门给条件变量用
    std::mutex mtx_, mtx_backend_;
    // 条件变量用的
    std::unique_lock<std::mutex> lock_;
    std::condition_variable cond_;
    std::thread t_;
    BufferStack emptyBufferStack_;
    BufferVec toWriteBufferVec_;
    BufferPtr cur_;
    void backendThreadFunc();
};

extern std::unique_ptr<Backend> gBackend;
extern FILE* gFile;

// sync or async
void setBackend(std::string mode = "sync");
void setLogFile(std::string fileName = "stdout");
}