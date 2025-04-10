#include"EventLoop.h"
#include"Acceptor.h"
#include"TcpConnection.h"
#include<iostream>
#include<sys/epoll.h>



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
        _exit(-1);
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
    else{
        if(_evList[i].events&EPOLLIN){//这个判断语句写不写都行，为了预防任何修改的发生，这里还是判断一下
            handleMessage(fd);
        }
    }

    }






}






}
















