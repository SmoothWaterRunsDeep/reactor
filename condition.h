#pragma once
#include"NonCopyable.h"
#include<pthread.h>


class mutexLock;//前向声明

class condition
{
public:
    condition(mutexLock&);
    ~condition();
    void wait();
    void notify();
    void notifyAll();

private:
    mutexLock &_mutex;//对互斥锁对象的引用
    pthread_cond_t _cond;//条件变量
};



