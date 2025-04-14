#include"tcpServer.h"

tcpServer::tcpServer(const string&ip,unsigned short port)
    :_acceptor(ip,port)
     ,_evLoop(_acceptor)
{}

tcpServer::~tcpServer(){};

void tcpServer::start(){
    _acceptor.ready();//服务器进行监听连接
    _evLoop.Loop();//epoll开始监听就绪事件
}

void tcpServer::stop(){
    _evLoop.unLoop();
}

void tcpServer::setAllCallback(TcpConnectionCallback &&cb_1  //注册回调函数
                               ,TcpConnectionCallback&&cb_2
                               ,TcpConnectionCallback&&cb_3)
{
    _evLoop.SetNewConnectionCallback(move(cb_1));
    _evLoop.SetNewMessageCallback(move(cb_2));
    _evLoop.SetCloseCallback(move(cb_3));
}

