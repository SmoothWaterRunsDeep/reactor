#pragma once
#include<pthread.h>
#include<functional>
using std::function;

using ThreadCallback=function<void()>;

class thread
{
public:
    thread(ThreadCallback&&);
    ~thread();
    void start();//创建线程并进入创建线程时的入口函数
    void stop();

static void*threadFunc(void *);//threadFunc 可能是一个用于线程内部处理的辅助函数，不需要被其他模块调用。
//将其声明为static可以防止外部模块意外调用该函数，从而保持模块的封装性和独立性。

private:
pthread_t _thread;//唯一标识一个线程id
bool _isrunning;//标识线程是否运行
ThreadCallback _cb;//一个多态函数包装器，用于存储指向线程的具体业务逻辑的函数
};

