#include"taskQueue.h"


taskQueue::taskQueue(size_t num)
    :_maxsize(num)
     ,_notempty(_mut)
     ,_notfull(_mut){
     }

taskQueue::~taskQueue(){//没有new出来的资源，所以析构函数作用不大
}

bool taskQueue::isempty(){
    return 0==_task.size();
}

bool taskQueue::isfull(){
    return _maxsize==_task.size();
}

void taskQueue::push(ElemType&&task){
    /*MutexLockGuard(_mut);这是我忘了写对象名写出的错误情况*/
    /* MutexLockGuard{_mut}; 如果更改成这样的话编译器是没有提示我报错的，所以MutexLockGuard(_mut); 可能被编译器解析为 ‌函数声明‌（即声明一个名为 _mut、返回 MutexLockGuard 的函数），而非对象构造
     * 是一种语法歧义（Most Vexing Parse）*/
    MutexLockGuard guard(_mut);//辅助类，创建时获取资源（上锁）
    while(isfull()){
        _notfull.wait();
    }
    _task.push(task);
    _notempty.notify();
}

ElemType taskQueue::pop(){
    MutexLockGuard guard(_mut);
    while(isempty()&&_flag){
        _notempty.wait();
    }

    if(_flag){
        ElemType tmp=_task.front();
        _task.pop();
        _notfull.notify();
        return tmp;
    }
    else{
        return nullptr;
    }
}

void taskQueue::wakeup(){
    _flag=false;
    _notempty.notifyAll();   
}


