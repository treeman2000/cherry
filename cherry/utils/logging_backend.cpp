#include<cstdio>
#include<assert.h>
#include<iostream>
#include<functional>
#include<algorithm>
#include"logging_backend.h"

using std::cout;
using std::endl;

namespace cherry{

std::unique_ptr<Backend> gBackend = std::make_unique<DefaultBackend>();
FILE* gFile = stdout;

void DefaultBackend::append(const char* p, int len){
    fwrite(p, 1, len, gFile);
    fflush(gFile);
}

void AsyncBackend::backendThreadFunc(){
    // 当有写满的块 或者 每隔2秒，写入一次
    while(!stop_){
        cond_.wait_for(lock_,std::chrono::seconds(2));
        std::cout<<"do!"<<std::endl;
        BufferVec tmpVec;
        {
            // 只是取出待写的块,就释放锁
            std::lock_guard lock(mtx_);
            if(toWriteBufferVec_.empty()){
                tmpVec.emplace_back(std::move(cur_));
                cur_ = std::move(emptyBufferStack_.top());
                emptyBufferStack_.pop();
            }else{
                // swap一下就好
                std::swap(tmpVec,toWriteBufferVec_);
            }
        }

        // 写入目的地
        for(size_t i=0;i<tmpVec.size();i++){
            fwrite(tmpVec[i]->data(), 1, tmpVec[i]->length(), gFile);
            tmpVec[i]->clear();
        }

        {
            // 把写完的空块放回栈里
            std::lock_guard lock(mtx_);
            for(size_t i=0;i<tmpVec.size();i++){
                emptyBufferStack_.emplace(std::move(tmpVec[i]));
            }
        }
    }
    return;
}

AsyncBackend::AsyncBackend(): 
        stop_(false), mtx_(), mtx_backend_(), lock_(mtx_backend_){
    cout<<"construct async backend"<<endl;
    // 初始化NUM_BUFFERS个内存块
    cur_ = std::make_unique<FixedBuffer<_4M>>();
    for(int i=1;i<NUM_BUFFERS;i++){
        emptyBufferStack_.emplace(std::make_unique<FixedBuffer<_4M>>());
    }
    // std::cout<<emptyBufferStack_.size()<<std::endl;

    // 启动后端线程
    t_ = std::thread(&AsyncBackend::backendThreadFunc,this);
}

AsyncBackend::~AsyncBackend(){
    cout<<"destruct async backend start"<<endl;
    stop_ = true;
    t_.join();
    cout<<"destruct async backend ok"<<endl;
}

void AsyncBackend::append(const char* p, int len){
    std::lock_guard<std::mutex> lock_guard(mtx_);
    // 如果当前块剩余空间不够大，则把当前块放入待写队列，并取空闲块。
    if(cur_->avail() < len){
        toWriteBufferVec_.emplace_back(std::move(cur_));
        // 写满了就通知后台线程
        cond_.notify_one();

        if(!emptyBufferStack_.empty()){
            cur_ = std::move(emptyBufferStack_.top());
            emptyBufferStack_.pop();
        }else{
            cur_ = std::make_unique<FixedBuffer<_4M>>();
        }
    }
    cur_->append(p,len);
}


void setBackend(std::string mode){
    assert(mode == "sync" || mode == "async");
    if(mode == "sync"){
        gBackend = std::make_unique<DefaultBackend>();
    }else if(mode == "async"){
        gBackend = std::make_unique<AsyncBackend>();
    }
    return;
}

void setLogFile(std::string fileName){
    if(fileName == "stdout"){
        gFile = stdout;
        return;
    }
    gFile = fopen(fileName.c_str(), "w");
    assert(gFile != nullptr);
    // if(gFile==nullptr){
    //     std::cout<<"errno:"<<errno<<std::endl;
    // }
    return;
}

}