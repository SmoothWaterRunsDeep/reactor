#include"TcpConnection.h"
#include<iostream>
using namespace std;

void test(){

//创建出服务器端
Acceptor acceptor("127.0.0.1",8888);//创建出服务器对应的套接字
acceptor.ready();//此时服务器处于监听状态

//建立出Tcp连接，从这里又一次体现出TcpConnection对Acceptor的依赖关系
TcpConnection _Tcp(acceptor.Accept());cout<<_Tcp.toString()<<"has connected"<<endl;;


while(1){
cout<<">>recv from client:"<<_Tcp.recv()<<endl;
_Tcp.send("hello\n");
}
}


int main(){
    test();
    return 0;
}
