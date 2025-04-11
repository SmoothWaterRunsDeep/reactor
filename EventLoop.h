#pragma once
//#include"Acceptor.h"
//#include"TcpConnection.h"
#include<vector>
#include<map>
#include<memory>
#include<functional>

using std::vector;
using std::map;
using std::shared_ptr;
using std::function;

//前向声明
class Acceptor;
class TcpConnection;


using TcpConnectionPtr=shared_ptr<TcpConnection>;
using TcpConnectionCallback=function<void(const TcpConnectionPtr&)>;//既防止更改，也防止拷贝
class EventLoop
{

public:
    EventLoop(Acceptor&);
    ~EventLoop();
    void Loop();//在epoll事件中，这个循环几乎占据了所有主要的篇幅
    void unLoop();

    //注册函数
    void SetNewConnectionCallback(TcpConnectionCallback&&);
    void SetNewMessageCallback(TcpConnectionCallback&&);
    void SetCloseCallback(TcpConnectionCallback&&);


    //下面这些成员函数之所以设为私有是因为他们是为上面这些共有的成员函数服务的，没有必要暴露在外面
private:
    void waitEpoll();
    void handleNewConnection();//这里的是正常封装的处理逻辑，并非回调函数
    void handleMessage(int fd);
    int createfd();//创建文件描述符。其实可以不写这个函数，但是把任何一种可能影响到程序的行为封装起来可以方便修改代码。而且更符合面向对象的特点
    void addEpollfd(int fd);//将文件描述符添加到红黑树上
    void delEpollfd(int fd);//将文件描述符从红黑树上移除


private:
    int ep_fd;
    bool isLooping;
    vector<struct epoll_event>_evList;//存储的是已就绪的存储描述符信息的结构体
    Acceptor &_acceptor;//原本_初始时创立的acceptor就是为了充当服务器的，并且获取表示监听的描述符时要用到Acceptor中的成员函数，因此创建一个Acceptor类型的对象
    map<int,TcpConnectionPtr>has;//建立起tcp连接与文件描述符的对应关系

    //这些数据成员的作用是将外部传入的回调函数保存到 EventLoop对象中。当有新的连接建立时，EventLoop 会创建一个 TcpConnection 对象，并将这些回调函数注册到该对象中
    TcpConnectionCallback _onConnection;
    TcpConnectionCallback _onMessage;
    TcpConnectionCallback _onClose;

};
