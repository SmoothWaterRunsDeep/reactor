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

