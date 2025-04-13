#pragma once
#include"taskQueue.h"
#include<vector>
#include<memory>


class thread;//前向声明thread类

using task=ElemType;

class threadpool
{
public:
    threadpool(size_t,size_t);
    ~threadpool();

    void start();
    void stop();
    void addTask(task&&);
    task getTask();
    void doTask();


private:
    size_t _maxsize;//线程池的最大容量
    size_t _quesize;//任务池的最大容量
    vector<unique_ptr<thread>>_threads;//存储工作线程
    taskQueue _taskQueue;//任务队列
    bool _exit;//标志线程池是否退出
};

