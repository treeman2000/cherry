#include"time.h"
#include<sys/time.h>

namespace time{

Time now(){
    timeval tv;
    gettimeofday(&tv,NULL);
    return Time(tv.tv_sec * Second + tv.tv_usec);
}    

}
