#include<string>
namespace time{

constexpr long long Second = 1000 * 1000;

class Time{
public:
    Time(){};
    Time(long long timestamp):timestamp_(timestamp){};
    ~Time(){};
    // 返回Time的微秒表示，而不是微秒的那部分
    long long UnixMicro() const {return timestamp_;}
    // yyyy-mm-dd hh:mm:ss ms
    std::string toString() const;
private:
    long long timestamp_;
};

bool operator<(const Time& t1, const Time& t2){
    return t1.UnixMicro() < t2.UnixMicro();
}

bool operator==(const Time& t1, const Time& t2){
    return t1.UnixMicro() == t2.UnixMicro();
}

Time now();
// timeval结构体里面一个秒，一个微秒，加起来就是当前时间
}
