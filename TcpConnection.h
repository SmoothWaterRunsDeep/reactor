#pragma once
#include"SockIo.h"
#include"Acceptor.h"

class SockIo;//前向声明SockIo类

class TcpConnection
{
public:
    TcpConnection(int connfd);//这里参数名填写为connfd更直观体现出对Acceptor的依赖关系
    ~TcpConnection();

    void send(const string&msg);//提供发送数据的接口
    string recv();//提供接收数据的接口
    
private:
SockIo _sockfd;
//其实到这里就已经足够功能的实现了。不过为了便于调试以及对用户更加友好
//所以可以再扩展出一些数据成员和功能

private:
InetAddr getLocaladdr();//获取本地的地址信息
InetAddr getPeeraddr();//获取对端的地址信息


private:
Socket _fd;
InetAddr _localaddr;
InetAddr _peeraddr;

public:
string toString();//返回一个存储地址信息的字符串



};

