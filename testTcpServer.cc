#include"tcpServer.h"
#include<TcpConnection.h>

class myTask{
public:
    myTask(const string&msg,TcpConnectionPtr&&ptr)
    :_msg(msg)
    ,_con(ptr){}
    ~myTask(){}
    void procrss(){
    }

private:
        string _msg;
        TcpConnectionPtr _con;
};








void test(){
    tcpServer Tcp("127.0.0.1",8888);
    Tcp.start();
    
    

}

