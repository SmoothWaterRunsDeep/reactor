#include"thread.h"
#include<iostream>
using namespace std;

thread::thread(ThreadCallback&&cb)
:_cb(move(cb))
,_isrunning(false)
,_thread(0){
    
}

thread::~thread(){}

void thread::start(){
int ret=pthread_create(&_thread,nullptr,threadFunc,this);//之所以传入this指针是因为线程的入口函数的参数必须是void*类型的，所以编译器默认传入的this指针的类型是不符合要求的
//因此做法是先把threadFunc设为静态消除编译器传入的this指针的影响，然后再手动传入this指针。这也是解决线程创建时需要访问对象成员的常用方法
    if(ret){
perror("pthread_create error");
return;
}
_isrunning=true;
}

void thread::stop(){
    if(_isrunning){
        int ret=pthread_join(_thread,nullptr);//阻塞等待_thread对应的线程的退出
        if(ret){
            perror("pthread_join error");
            return;
        }
    }
    _isrunning=false;
}

void* thread::threadFunc(void*arg){
    thread* pth=static_cast<thread*>(arg);
    if(pth){
        pth->_cb();
        cout<<"threadFunc"<<endl;
    }
    else{
    perror("pth is nullptr");
    }
}


