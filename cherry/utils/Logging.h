/*
logger功能：
设置log级别：DEBUG, INFO, ERROR
定时写入目的地（每隔3s）



logger实现：
分为前后端
    前端负责提供API，生成日志消息
    后端负责把日志写到目标地点


其他：
strace command 检查command的系统调用情况，可以用来发现gettimeofday不是系统调用
*/