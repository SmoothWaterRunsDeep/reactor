#include"EventLoop.h"
#include"TcpConnection.h"
#include<iostream>
using namespace std;

//tcp连接的三个半事件的具体方法在测试文件中实现。
//无论写出什么不同的行为，都可以无差别地执行

void onNewConnection(const TcpConnectionPtr&con){
    cout<<con->toString()<<"has connected"<<endl;
}

void onMessage(const TcpConnectionPtr&con){
    string msg=con->recv();
    cout<<"recv from client:"<<msg<<endl;
    con->send(msg);//这里只是简单的一些行为，事实上不会这么做，还需要完善
}

void onClose(const TcpConnectionPtr&con){
    cout<<con->toString()<<"has closed"<<endl;
}

void test(){
    Acceptor acceptor("127.0.0.1",8888);//建立一个服务器
    acceptor.ready();//服务器处于监听状态

    EventLoop ep(acceptor);
    //向ep注册三个半事件，由他转交给Tcp
    ep.SetNewConnectionCallback(::move(onNewConnection));
    ep.SetNewMessageCallback(::move(onMessage));
    ep.SetCloseCallback(::move(onClose));
    ep.Loop();//把ep的主要事务启动起来
}


int main(){
    test();
    return 0;
}


