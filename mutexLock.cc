#include"mutexLock.h"
#include<stdio.h>
mutexLock::mutexLock(){
    int ret=pthread_mutex_init(&_mut,nullptr);
    if(ret){//如果初始化失败返回的是非0值
        perror("mutexLock error");
        return;
    }
}

mutexLock::~mutexLock(){
    int ret=pthread_mutex_destroy(&_mut);
    if(ret){
        perror("~mutexLock error");
        return;
    }
}

void mutexLock::lock(){
    int ret=pthread_mutex_lock(&_mut);
    if(ret){
        perror("lock error");
        return;
    }
}


void mutexLock::trylock(){
    int ret=pthread_mutex_trylock(&_mut);
    if(ret){
        perror("trylock error");
        return;
    }
}

void mutexLock::unlock(){
    int ret=pthread_mutex_unlock(&_mut);
    if(ret){
        perror("unlock error");
        return;
    }
}

pthread_mutex_t* mutexLock::getmutex(){
    return &_mut;
}
