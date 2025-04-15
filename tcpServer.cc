#include"tcpServer.h"

tcpServer::tcpServer(const string&ip,unsigned short port)
    :_acceptor(ip,port) //如果你调试的话会发现是先走到_evLoop的构造函数，因为它是按照你在类中的声明顺序来调用构造函数的而不会参考你在此处手动调用构造函数的顺序
     ,_evLoop(_acceptor)//虽然只是一个小小的细节，但是由于_evLoop的构造依赖于_acceptor，如果不注意声明顺序，此处就会产生运行时错误
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

