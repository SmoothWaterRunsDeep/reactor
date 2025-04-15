#pragma once
#include"SockIo.h"
#include"Acceptor.h"
#include<functional>
#include<memory>//需要使用到智能指针托管空间
#include"EventLoop.h"

class SockIo;//前向声明SockIo类

class TcpConnection
:public enable_shared_from_this<TcpConnection>//shared_from_this() 返回的是指向当前对象的shared_ptr，并且共享引用计数，这样能避免多个指针独立管理同一片空间
{

    using TcpConnectionPtr=shared_ptr<TcpConnection>;//这个声明不能拿到前面去，因为你放到前面的话看不到TcpConnection的定义会发生报错
    using TcpConnectionCallback=function<void(const TcpConnectionPtr &)>;

public:
    TcpConnection(int connfd,EventLoop*ev);//这里参数名填写为connfd更直观体现出对Acceptor的依赖关系
    ~TcpConnection();

    void send(const string&msg);//提供发送数据的接口
    string recv();//提供接收数据的接口
    bool isClosed()const;

private:
    SockIo _sockfd;


    //其实到这里就已经足够功能的实现了。不过为了便于调试以及对用户更加友好
    //所以可以再扩展出一些数据成员和功能
#if 1
private:
    InetAddr getLocaladdr();//获取本地的地址信息
    InetAddr getPeeraddr();//获取对端的地址信息

private:
    Socket _fd;
    InetAddr _localaddr;
    InetAddr _peeraddr;

public:
    string toString();//返回一个存储地址信息的字符串
#endif


    //对Tcp的功能进一步扩展和分层
#if 1
private:
    TcpConnectionCallback _onConnection;//连接建立
    TcpConnectionCallback _onMessage;//数据收发
    TcpConnectionCallback _onClose;//断开连接

public:
    //回调函数的注册
    void SetNewConnectionCallback(const TcpConnectionCallback&);
    void SetMessageCallback(const TcpConnectionCallback&);
    void SetCloseCallback(const TcpConnectionCallback&);

    //回调函数的执行
    void handleNewConnectionCallback();
    void handleMessageCallback();
    void handleCloseCallback();
#endif

private:
    EventLoop* _ev;//TcpConnection需要能感知到EventLoop的存在

public:
    void sendLoop(const string&msg);
};
