#include"TcpConnection.h"
#include<sstream>

TcpConnection::TcpConnection(int connfd)
    :_fd(connfd)
    ,_sockfd(connfd)
     ,_localaddr(getLocaladdr())
     ,_peeraddr(getPeeraddr()){}

     TcpConnection::~TcpConnection(){}

     void TcpConnection::send(const string&msg){
         _sockfd.writen(msg.c_str(),msg.size());
     }

string TcpConnection::recv(){
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
        _exit(-1);
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
        <<_peeraddr.getip()<<":"<<_peeraddr.getport()<<endl;
    return ss.str();
}
