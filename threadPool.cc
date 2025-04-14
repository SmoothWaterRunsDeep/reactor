#include"threadPool.h"
#include"thread.h"
#include<unistd.h>

threadPool::threadPool(size_t capacity,size_t maxsize)
    :_maxsize(maxsize)
    ,_quesize(capacity)
    ,_taskQueue(capacity)
     ,_exit(false)
{
    _threads.resize(_maxsize);//这里不适用这句话也没关系，因为即使vector发生扩容，使用的也是unique_ptr的移动构造函数 
}

threadPool::~threadPool(){}

void threadPool::start(){
    for(int i=0;i<_maxsize;++i){
        unique_ptr<thread>tmp(new thread(bind(&threadPool::doTask,this)));//std::bind(&threadPool::doTask, this) 返回的是一个未指定具体类型的可调用对象，
        //它能隐式转换为 std::function<void()> 类型
        _threads.push_back(move(tmp));//unique_ptr独占所有权，不能被复制，所以这里必须要进行右值转换实现移动语义（vector的移动构造函数）转交所有权
    }

    //for(auto th:_threads)//这样是错的，因为这样会发生拷贝构造函数
    for(auto &th:_threads)//引用类型不会创建新的对象，所以这样是正确的
    {
        th->start();//让所有线程都启动
    }
}

void threadPool::stop(){
    while(!_taskQueue.isempty()){
        sleep(1);       //只要任务队列中还有任务就不能stop线程池
    }
    _exit=true;//标识线程池退出

    _taskQueue.wakeup();//唤醒所有阻塞等待notempty的线程，让他们顺利运行完（因为此时_exit为true了，各个线程不会因为任务队列还有任务而阻塞等待）

    for(auto &th:_threads){
        th->stop();
    }
}

void threadPool::addTask(task&&cb){//使用右值引用避免不必要的拷贝
    if(cb){
        _taskQueue.push(move(cb));
    }
}


task threadPool::getTask(){
    return _taskQueue.pop();
}

void threadPool::doTask(){
    while(!_exit){
        task Task=getTask();
        if(Task){
            Task();
        }
        else{
            perror("current Task is nullptr");
        }
    }
}

