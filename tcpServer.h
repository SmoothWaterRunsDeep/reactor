#pragma once
#include"EventLoop.h"
#include"Acceptor.h"

class tcpServer
{
public:
    tcpServer(const string&ip,unsigned short port);
    ~tcpServer();

    void start();//服务器运行
    void stop();//服务器停止
    void setAllCallback(TcpConnectionCallback &&cb_1 //回调函数的注册
                        ,TcpConnectionCallback &&cb_2
                        ,TcpConnectionCallback&& cb_3);
    
private:
EventLoop _evLoop;
Acceptor _acceptor;
};

