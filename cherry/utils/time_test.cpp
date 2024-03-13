#include"time.h"
#include<iostream>

int main(){
    auto t = mytime::now();
    std::cout<< t.toString() << std::endl;
    std::cout<< t.toString(true) << std::endl;
    return 0;
}