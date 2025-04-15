#include"TcpConnection.h"
#include<sstream>
#include<iostream>
#include<memory>
using namespace std;


TcpConnection::TcpConnection(int connfd)//构造函数
    :_fd(connfd)
    ,_sockfd(connfd)
     ,_localaddr(getLocaladdr())
     ,_peeraddr(getPeeraddr()){}

     TcpConnection::~TcpConnection(){}//析构函数

     void TcpConnection::send(const string&msg){//send函数
         _sockfd.writen(msg.c_str(),msg.size());
     }

string TcpConnection::recv(){//接收函数
    char buf[655535]={0};
    _sockfd.readLine(buf,sizeof(buf));
    return string(buf);
}

/*getsockname 函数用于获取与当前 TCP 连接所使用的套接字关联的本地地址信息。获取到的地址信息存储在 struct sockaddr_in 类型的变量addr中，
 * 然后通过 InetAddress(addr) 构造一个 InetAddress 对象*/
InetAddr TcpConnection::getLocaladdr(){
    struct sockaddr_in addr;
    socklen_t len=sizeof(addr);
    int ret=getsockname(_fd.getfd(),(struct sockaddr*)&addr,&len);
    if(ret<0){
        perror("getLocaladdr error");
    }
    return InetAddr(addr);
}

InetAddr TcpConnection::getPeeraddr(){
    struct sockaddr_in addr;
    socklen_t len=sizeof(addr);
    int ret=getpeername(_fd.getfd(),(struct sockaddr*)&addr,&len);
    if(ret<0){
        perror("getPeeraddr error");
        _exit(-1);
    }
    return InetAddr(addr);
}

//返回一个存储网络连接的两端的地址信息的字符串
string TcpConnection::toString(){
    stringstream ss;
    ss<<_localaddr.getip()<<":"<<_localaddr.getport()
        <<"----->"
        <<_peeraddr.getip()<<":"<<_peeraddr.getport();
    return ss.str();
}


bool TcpConnection::isClosed()const{
    char buf[20]={0};
    int ret=::recv(_fd.getfd(),buf,sizeof(buf),MSG_PEEK);//这里必须在recv前加上限定符表示调用全局上的recv函数，因为我前面好像有和recv重名的函数
    return (0==ret);
}

//回调函数的注册
/* void TcpConnection::SetNewConnectionCallback(TcpConnectionCallback&& cb){一定要注意，这么写是错的，因为在执行第70行的时候会调用移动构造函数，
 * 移动语义中会把cb的空间转交给_onConenction 但是这样会出现一个问题，那就是在第一个客户端连接后，EventLoop中的用于注册函数的数据成员会被置为nullptr，
 * 这样的话就会造成第一个客户端可以正常连接但是后续的客户端连接就不能正常通信了。这里使用const左值引用的话由于const属性，在第70行执行的将是复制的赋值函数*/
void TcpConnection::SetNewConnectionCallback(const TcpConnectionCallback& cb){
    _onConnection=move(cb);
}

/* void TcpConnection::SetMessageCallback(TcpConnectionCallback&&cb){ */
void TcpConnection::SetMessageCallback(const TcpConnectionCallback&cb){
    _onMessage=move(cb);
}

/* void TcpConnection::SetCloseCallback(TcpConnectionCallback &&cb){ */
void TcpConnection::SetCloseCallback(const TcpConnectionCallback &cb){
    _onClose=move(cb);
}


//回调函数的执行
void TcpConnection::handleNewConnectionCallback(){
    if(_onConnection){
        _onConnection(shared_from_this());
    }
    else{
        cout<<"no connection has been established"<<endl;
    }
}


void TcpConnection::handleMessageCallback(){
    if(_onMessage){
        _onMessage(shared_from_this());
    }
    else{
        cout<<"no message has been sent"<<endl;
    }
}

void TcpConnection::handleCloseCallback(){
    if(_onClose){
        _onClose(shared_from_this());
    }
    else{
        cout<<"no tcp can be closed"<<endl;
    }
}

void TcpConnection::sendLoop(const string&msg){
    if(_ev){
        _ev->run(bind(&TcpConnection::send,this,msg));
    } 
}

