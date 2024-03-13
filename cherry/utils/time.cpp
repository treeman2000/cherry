#include"time.h"
#include<sys/time.h>

namespace mytime{

Time now(){
    timeval tv;
    gettimeofday(&tv,NULL);
    return Time(tv.tv_sec * Second + tv.tv_usec);
}    

std::string Time::toString(bool showMicroseconds) const{
    char buf[64];
    tm tm_time;
    time_t seconds = timestampMicro_ / Second;
    gmtime_r(&seconds, &tm_time);
    if(showMicroseconds){
        int microseconds = static_cast<int>(timestampMicro_ % Second);
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
             microseconds);
    }else{
        snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }
    return buf;
}

}
