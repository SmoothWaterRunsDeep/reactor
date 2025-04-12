#include"condition.h"
#include"mutexLock.h"
#include<stdio.h>

condition::condition(mutexLock&mutex)
    :_mutex(mutex){
        int ret=pthread_cond_init(&_cond,nullptr);
        if(ret){
            perror("pthread_cond_init error");
            return;
        }
    }

condition::~condition(){
    int ret=pthread_cond_destroy(&_cond);
    if(ret){
        perror("pthread_cond_destroy error");
        return;
    }
}

void  condition::wait(){
    int ret=pthread_cond_wait(&_cond,_mutex.getmutex());//释放对应的锁，并将_cond加入等待队列阻塞等待
    if(ret){
        perror("wait error");
        return;
    }
}


void condition::notify(){
    int ret=pthread_cond_signal(&_cond);
    if(ret){
        perror("notify error");
        return;
    }
}

void condition::notifyAll(){
    int ret=pthread_cond_broadcast(&_cond);
    if(ret){
        perror("notifyAll error");
        return;
    }
}










