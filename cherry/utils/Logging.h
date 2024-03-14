/*
logger功能：
设置log级别
定时写入目的地（每隔3s）



logger实现：
异步日志，分为前后端
    前端负责提供API，生成日志消息
    后端负责把日志写到目标地点
    设置空buffer池，让buffer循环利用
    前后端接口为：
    typedef void (*OutputFunc)(const char* msg, int len);
    用c++11写出来就是
    setOutput(OutputFunc);

与muduo的区别
使用了c++标准的线程实现。原书写的时候c++标准线程还没成为主流。
使用了buffer池，进一步提高极端场景下的性能。原书用的是4个buffer块。
函数指针定义改成了用functional
muduo用宏定义了LOG_INFO, LOG_ERROR等，不太安全。

其他：
strace command 检查command的系统调用情况，可以用来发现gettimeofday不是系统调用
直接拷贝性能会比每条日志都用一个string表示要快，因为日志很短，每次string申请内存的时间开销比拷贝要大。

AsycnLogging只提供了向文件写入的功能，
由于Logger默认输出位置是stdout，
所以需要像同步日志那样调用Logger::setOutput(OutputFunc out)接口，
把日志输出位置改为文件。
AsyncLogging* g_asyncLog = nullptr;
void asyncOutput(const char* msg, int len)
{
	g_asyncLog->append(msg, len);
}

int main()
{
    off_t kRollSize = 500*1000*1000;
    AsyncLogging log(“basename”, kRollSize);
    log.start();		// 启动异步日志的后台线程
    g_asyncLog = &log;
    Logger::setOutput(asyncOutput);
    LOG_INFO << "Hello 0123456789" << " abcdefghijklmnopqrstuvwxyz";
    return 0;
}

因为可能多线程同时写logger，所以logger每次用的时候新建一个对象就好，每个logger有自己的缓冲区，
即logstream对象。
改进：如果太长了，写不下，可以多加一条log说明。
Logger是一个非常轻量级的对象。在析构时，把logstream的数据调用outputFunc写入后端
*/
#ifndef LOGGING_H
#define LOGGING_H

#include<cstring>
#include<string>
#include<array>
#include<thread>
#include<functional>
#include"noncopyable.h"
#include"logging_backend.h"
#include"time_my.h"

namespace cherry{

enum LogLevel{
    DEBUG,
    INFO,
    WARN,
    ERROR,
    FATAL,
    NUM,
};

// 前端，负责处理<<格式输入，写入buffer
class LogStream{
public:
    virtual LogStream& operator<<(int) = 0;
    virtual LogStream& operator<<(long long) = 0;
    virtual LogStream& operator<<(float) = 0;
    virtual LogStream& operator<<(double) = 0;
    virtual LogStream& operator<<(const char*) = 0;
    virtual LogStream& operator<<(std::string) = 0;
};

class LogStreamImpl: public LogStream{
public:
    LogStreamImpl(){};
    LogStreamImpl& operator<<(int);
    LogStreamImpl& operator<<(long long);
    LogStreamImpl& operator<<(float);
    LogStreamImpl& operator<<(double);
    LogStreamImpl& operator<<(const char*);
    LogStreamImpl& operator<<(std::string);
    FixedBuffer<_4K> buffer_; 
};

class EmptyLogStream: public LogStream {
public:
    EmptyLogStream(){};
    EmptyLogStream& operator<<(int){return *this;}
    EmptyLogStream& operator<<(long long){return *this;}
    EmptyLogStream& operator<<(float){return *this;}
    EmptyLogStream& operator<<(double){return *this;}
    EmptyLogStream& operator<<(const char*){return *this;}
    EmptyLogStream& operator<<(std::string){return *this;}
};
EmptyLogStream emptyStream;

const char* level2name[LogLevel::NUM] =
{
  "DEBUG ",
  "INFO  ",
  "WARN  ",
  "ERROR ",
  "FATAL ",
};

// 每次调用LOG_INFO<<等函数时创建，调用完就析构，析构时把日志写入后端
class Logger:noncopyable{
public:
    Logger() = delete;
    Logger(const char* curFileName, int lineNo, LogLevel logLevel);
    ~Logger();
    LogStream& stream();
private:    
    bool isValid_;
    LogStreamImpl logStream_;
    const char* curFileName_;
    int lineNo_;
    LogLevel logLevel_;
    mytime::Time t_;
};

LogLevel gLogLevel = LogLevel::INFO;
void setLogLevel(LogLevel logLevel){
    gLogLevel = logLevel;
}

#define LOG_DEBUG Logger(__FILE__, __LINE__,cherry::LogLevel::DEBUG).stream()
#define LOG_INFO Logger(__FILE__, __LINE__,cherry::LogLevel::INFO).stream()
#define LOG_WARN Logger(__FILE__, __LINE__,cherry::LogLevel::WARN).stream()
#define LOG_ERROR Logger(__FILE__, __LINE__,cherry::LogLevel::ERROR).stream()
#define LOG_FATAL Logger(__FILE__, __LINE__,cherry::LogLevel::FATAL).stream()


} // cherry

#endif // LOGGING_H