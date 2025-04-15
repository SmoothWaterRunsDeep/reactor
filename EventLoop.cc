#include"EventLoop.h"
#include"Acceptor.h"
#include"TcpConnection.h"
#include<iostream>
#include<sys/epoll.h>
#include"sys/eventfd.h"

using namespace std;

EventLoop::EventLoop(Acceptor&acceptor)
    :ep_fd(createfd())
    ,isLooping(false)
    ,_evList(1024)
     ,_acceptor(acceptor)
{
    int listenfd=_acceptor.fd();
    addEpollfd(listenfd);//初始的第一个fd应该进行监听
}

EventLoop::~EventLoop(){
    close(ep_fd);//析构函数只要关闭epoll实例就行了，毕竟这才是epoll的核心
}

void EventLoop::Loop(){
    isLooping=true;
    while(isLooping){
        waitEpoll();
    }
}


void EventLoop::unLoop(){
    isLooping=false;//标志上退出循环即可，不必进行实际操作
}

void EventLoop::waitEpoll(){
    int nready=0;
    do{
        nready=epoll_wait(ep_fd,&*_evList.begin(),_evList.size(),3000);
    }while(nready==-1&&errno==EINTR);


    if(-1==nready){
        perror("epoll_wait error");
        return;
    }

    else  if(0==nready){
        cout<<"there are no ready events"<<endl;
    }

    else{
        if(nready==_evList.size()){
            _evList.resize(2*nready);//如果就绪事件以及达到上限，则应该扩容
        }

        //下面就是正常的进行数据的读取，针对新旧连接有不同的表现
        for(int i=0;i<nready;++i){
            int fd=_evList[i].data.fd;
            if(fd==_acceptor.fd()){ 
                handleNewConnection();//如果是新连接，则使用handleNewConnection方法
            }

            else if(fd==_evtfd){
                if(_evList[i].events&EPOLLIN){
                    handleRead();
                    dopendingtask();
                }
            }

            else{
                if(_evList[i].events&EPOLLIN){//这个判断语句写不写都行，为了预防任何修改的发生，这里还是判断一下
                    handleMessage(fd);
                }
            }

        }
    }
}



void EventLoop::handleNewConnection(){
    int connfd=_acceptor.Accept();
    if(connfd<0){
        perror("find the error of connection");
        return;
    }

    addEpollfd(connfd);//将connfd放入红黑树中监听是否有数据发来
    TcpConnectionPtr con(new TcpConnection(connfd));//每当有connfd成功返回就意味着有tcp连接成功建立

    //has[connfd]=con;//如果键不存在，会先创建一个默认构造的对象，然后再进行赋值操作,会带来额外的开销。
    /*每当有连接建立就应该注册tcp的三个半事件*/
    con->SetNewConnectionCallback(move(_onConnection));
    con->SetMessageCallback(move(_onMessage));
    con->SetCloseCallback(move(_onClose));

    //has[connfd]=con;//如果键不存在，会先创建一个默认构造的对象，然后再进行赋值操作,会带来额外的开销。
    has.insert(make_pair(connfd,con));//不会进行不必要的查找和可能的覆盖操作
    if(con)
        con->handleNewConnectionCallback();//执行连接建立事件的回调函数
    else{
        cout<<"no con "<<endl;
        _exit(-1);
    }
}


void EventLoop::handleMessage(int fd){
    //auto it=has[fd];//尽量不要使用[] 运算符,因为如果键不存在，会有插入操作，涉及到内存分配和对象构造，可能会带来额外的性能开销。
    auto it=has.find(fd);
    if(it!=has.end()){
        bool flag=it->second->isClosed();
        if(flag){//如果是断开连接

            //执行断开连接的回调函数
            it->second->handleCloseCallback();

            //从红黑树中移除文件描述符
            delEpollfd(fd);

            //从哈希表中删除对应的键值对
            has.erase(fd);

        }
        else{//如果不是断开连接，那就是正常的收发数据
            it->second->handleMessageCallback();
        }
    }
    else{
        perror("not find this connection");
        return;
    }
}

//创建epoll实例
int EventLoop::createfd(){
    int ep=epoll_create(1);//对于这个函数，它只要参数大于等于0就行
    if(ep<0){
        perror("the creeation of ep_fd is error");
        return -1;
    }
    return ep;
}

//将文件描述符添加到红黑树上
void EventLoop::addEpollfd(int fd){
    struct epoll_event evt;
    evt.events=EPOLLIN;
    evt.data.fd=fd;
    int ret=epoll_ctl(ep_fd,EPOLL_CTL_ADD,fd,&evt);
    if(ret<0){
        perror("addEPpollfd is error");
        return;
    }
}


//将文件描述符从红黑树上移除
void EventLoop::delEpollfd(int fd){
    int ret=epoll_ctl(ep_fd,EPOLL_CTL_DEL,fd,nullptr);
    if(ret<0){
        perror("delEpollfd is error");
        return;
    }
}


//注册三个半事件(为TcpConnection充当中介，因为外部事物并不直接与TcpConnection交互，所以先注册给EventLoop，再由EventLoop注册给TcpConnection)
void EventLoop::SetNewConnectionCallback(TcpConnectionCallback&&cb){
    _onConnection=move(cb);
}

void EventLoop::SetNewMessageCallback(TcpConnectionCallback&&cb){
    _onMessage=move(cb);
}

void EventLoop::SetCloseCallback(TcpConnectionCallback&&cb){
    _onClose=move(cb);
}


void EventLoop::wakeup(){
    uint64_t one=1;
    ssize_t ret=write(_evtfd,&one,sizeof(one));//写入_ectfd中的内核计数器
    if(ret!=sizeof(uint64_t)){//检查操作是否成功
        perror("EventLoop::wakeup is error");
        return ;
    }
}

void EventLoop::run(task&&cb){
    {//使用块作用域，之所以使用块作用域是为了缩短上锁的范围（细粒度）
        MutexLockGuard autoLock(_mut);//上锁
        _task.push_back(move(cb));  //把任务(向用户反馈数据)存储到vector中
    }

    wakeup();//这里唤醒的其实就是EventLoop，因为这个run方法是被传递给线程池运行的
}

int EventLoop::createEventFd(){
    int fd=eventfd(10,0);
    if(fd<0){
        perror("EventLoop::createEventFd error");
        return -1;
    }
    return fd;
}

void EventLoop::handleRead(){//把eventfd对应的内核计数器的数据读走
    uint64_t opt=0;
    ssize_t ret=read(_evtfd,&opt,sizeof(uint64_t));
    if(ret!=sizeof(uint64_t)){
        perror("EventLoop::handleRead error");
        return;
    }
}

void EventLoop::dopendingtask(){
    vector<task>tmp;
    {
        MutexLockGuard guard(_mut);
        tmp.swap(_task);//将 _pengdings 中存储的待执行任务转移到 tmp 中，这样执行任务时不需要再持有锁，这样可以避免锁的开销，提高任务执行的效率。在无锁的状态下遍历 tmp 执行任务，可以充分利用多核处理器的并行计算能力，加快任务的执行速度。
    }

    for(auto&cb:tmp){//执行完vector里面的所有任务
        cb();
    }
}


