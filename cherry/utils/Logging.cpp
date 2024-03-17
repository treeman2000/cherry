#include"logging.h"
#include<cstdio>
#include<stdlib.h>
#include<string>

#include<iostream>

namespace cherry{

constexpr int MaxNumericSize = 48;

LogStreamImpl& LogStreamImpl::operator<<(int a){
    if(buffer_.avail()>MaxNumericSize){
        char tmp[MaxNumericSize];
        memset(tmp,0,sizeof tmp);
        sprintf(tmp,"%d",a);
        buffer_.append(tmp,strlen(tmp));
    }
    return *this;
}

LogStreamImpl& LogStreamImpl::operator<<(unsigned long a){
    if(buffer_.avail()>MaxNumericSize){
        char tmp[MaxNumericSize];
        memset(tmp,0,sizeof tmp);
        sprintf(tmp,"%lu",a);
        buffer_.append(tmp,strlen(tmp));
    }
    return *this;
}

LogStreamImpl& LogStreamImpl::operator<<(long long a){
    if(buffer_.avail()>MaxNumericSize){
        char tmp[MaxNumericSize];
        memset(tmp,0,sizeof tmp);
        sprintf(tmp,"%lld",a);
        buffer_.append(tmp,strlen(tmp));
    }
    return *this;
}

LogStreamImpl& LogStreamImpl::operator<<(float a){
    if(buffer_.avail()>MaxNumericSize){
        char tmp[MaxNumericSize];
        memset(tmp,0,sizeof tmp);
        sprintf(tmp,"%f",a);
        buffer_.append(tmp,strlen(tmp));
    }
    return *this;
}

LogStreamImpl& LogStreamImpl::operator<<(double a){
    if(buffer_.avail()>MaxNumericSize){
        char tmp[MaxNumericSize];
        memset(tmp,0,sizeof tmp);
        sprintf(tmp,"%lf",a);
        buffer_.append(tmp,strlen(tmp));
    }
    return *this;
}

LogStreamImpl& LogStreamImpl::operator<<(const char* p){
    int len = strlen(p);
    if(buffer_.avail()>len){
        buffer_.append(p,len);
    }
    return *this;
}

LogStreamImpl& LogStreamImpl::operator<<(std::string s){
    if(buffer_.avail()>static_cast<int>(s.size())){
        buffer_.append(s.c_str(),s.size());
    }
    return *this;
}

LogStream& Logger::stream(){
    if(logLevel_ >= gLogLevel){
        return logStream_;
    }
    return emptyStream;
}

Logger::Logger(const char* curFileName, int lineNo, LogLevel logLevel):
            curFileName_(curFileName), lineNo_(lineNo), logLevel_(logLevel){
    isValid_ = logLevel >= gLogLevel;
    if(isValid_){
        t_ = mytime::now();
        std::string&& time_str = t_.toString();
        logStream_.buffer_.append(time_str.c_str(),time_str.size());
        logStream_.buffer_.append(" ",1);
        // thread id
        auto tid = std::this_thread::get_id();
        logStream_<<std::to_string(std::hash<std::thread::id>()(tid))<<" ";
        logStream_<<level2name[logLevel_]<<" ";
    }
}

Logger::~Logger(){
    if(logLevel_ >= gLogLevel){
        logStream_<<" "<<curFileName_<<": "<<lineNo_<<"\n";
        gBackend->append(logStream_.buffer_.data(),logStream_.buffer_.length());
    }
}


} //cherry