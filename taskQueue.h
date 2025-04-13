#pragma once
#include"mutexLock.h"
#include"condition.h"
#include<queue>
#include<functional>
using namespace std;

using ElemType=function<void()>;

class taskQueue
{
public:
    taskQueue(size_t);
    ~taskQueue();
    bool isempty();
    bool isfull();

    void push(ElemType&&task);//存
    ElemType pop();//取
    void wakeup();//唤醒进程（没写阻塞函数是因为系统自动为我们实现阻塞，我们只需负责唤醒即可）

private:
size_t _maxsize;//缓冲区（这里指任务队列）的上限容量
queue<ElemType>_task;//任务队列

mutexLock _mut;//拥有互斥锁和条件变量应是缓冲区的固有属性，所以直接以数据成员的形式包含
condition _notempty;
condition _notfull;
bool _flag;//这个标志位就是为了避免死锁才设置的，是配合threadpool避免死锁才使用的
};

