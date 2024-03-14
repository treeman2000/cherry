#include<string>
namespace mytime{

// 微秒数
constexpr long long Second = 1000 * 1000;

class Time{
public:
    Time(){};
    Time(long long timestampMicro):timestampMicro_(timestampMicro){};
    ~Time(){};
    // 返回Time的微秒表示，而不是微秒的那部分
    long long UnixMicro() const {return timestampMicro_;}
    // yyyy-mm-dd hh:mm:ss ms
    std::string toString(bool showMicroseconds=false) const;
private:
    long long timestampMicro_;
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
