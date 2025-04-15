#include"tcpServer.h"
#include"EventLoop.h"
#include<string>
#include"TcpConnection.h"
#include<iostream>
#include"threadPool.h"
using namespace std;

threadPool* pool=nullptr;

class myTask{
public:
    myTask(const string&msg,const TcpConnectionPtr&con)
        :_msg(msg)
         ,_con(con){}

    void process(){
        //处理任务的逻辑
        //...
        //
        _con->sendLoop(_msg);
    }

private:
    string _msg;
    TcpConnectionPtr _con;
};

void onConnection(const TcpConnectionPtr&con){
    cout<<con->toString()<<"has connected"<<endl;
}

void onMeessage(const TcpConnectionPtr&con){
    string msg=con->recv();
    cout<<"<<recv from client:"<<msg<<endl;
    myTask task(msg,con);
    pool->addTask(bind(&myTask::process,task)); 
}

void onClose(const TcpConnectionPtr&con){
    cout<<con->toString()<<"has closed"<<endl;
}

void test(){
    threadPool _pool(4,10);
    _pool.start();
    pool=&_pool;

    tcpServer _server("127.0.0.1",8888);
    _server.setAllCallback(move(onConnection)
                           ,move(onMeessage)
                           ,move(onClose));

    _server.start();

}

int main(){
    test();
    return 0;
}

