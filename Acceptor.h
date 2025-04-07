#pragma once
#include<string>
#include"Socket.h"
#include"InetAddr.h"
using std::string;

class Acceptor
{
public:
    Acceptor(const string &,unsigned short);//初始化数据成员
    ~Acceptor();

    void SetReuseAddr();//设置地址复用的函数
    void SetReusePort();//设置端口复用的函数
    void Bind();//套接字绑定地址信息的函数
    void Listen();//将套接字设为被动状态（内核中会开辟出请求队列）
    int Accept();//进行建立连接的函数
    void ready();//集成了SetReuseAddr,SetReusePort,Bind,Listen这些函数，因为这些函数经常一起使用
    int fd();//Acceptor负责交互，因此它应该具备提供套接字的文件描述符的功能
private:
    Socket _soc;
    InetAddr _addr;
};

