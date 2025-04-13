#pragma once
#include<pthread.h>
#include"NonCopyable.h"

class mutexLock
:NonCopyable
{
public:
    mutexLock();
    ~mutexLock();

    void lock();//上锁
    void trylock();//尝试上锁
    void unlock();//解锁
    pthread_mutex_t* getmutex();//获取锁
private:
    pthread_mutex_t _mut;//互斥锁
};


class MutexLockGuard//这个类是一个辅助类，遵循RAII原则。因为互斥锁属于资源，为了确保锁的正确释放，防止死锁.
{
public:
    MutexLockGuard(mutexLock& Mut)
        :_Mut(Mut)
    {
        _Mut.lock();//在构造函数中获取资源
    }
    ~MutexLockGuard(){
        _Mut.unlock();//在析构函数中释放资源
    };

private:
    mutexLock &_Mut;//这里必须使用引用类型，因为我设置了禁止复制


};
