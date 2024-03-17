#include<iostream>
#include"logging.h"
using namespace std;

void testLogStream(){
    cherry::LOG_INFO<<1<<"\taaa\t"<<2.5<<"abc\t"s;
}

void testLogger(){
    cherry::setBackend("sync");
    cherry::setLogFile("/workspaces/mycppwebserver/log");
    cherry::LOG_INFO<<2<<"\tbbb\t"<<6.6<<"ddd\t"s;
}

void testAsyncBackend(){
    cherry::setBackend("async");
    cherry::LOG_INFO << cherry::level2name[cherry::gLogLevel];
    constexpr int N = 100;
    for(int i=0;i<N;i++){
        cherry::LOG_INFO<<i<<"\tbvb\t"<<6.6<<"ddd\t"s;
    }
    // this_thread::sleep_for(std::chrono::seconds(4));
}

int main(){
    // testLogStream();
    // testLogger();
    testAsyncBackend();
    return 0;
}