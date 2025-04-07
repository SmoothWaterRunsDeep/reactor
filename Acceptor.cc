#include"Acceptor.h"


Acceptor::Acceptor(const string&ip,unsigned short port)
:_soc()
,_addr(ip,port)
{ }

Acceptor::~Acceptor(){}

void Acceptor::SetReuseAddr(){//设置地址复用
    int opt=1;
    setsockopt(_soc.getfd(),SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
}

void Acceptor::SetReusePort(){
    int opt=1;
    setsockopt(_soc.getfd(),SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt));
}

void Acceptor::Bind(){
    int ret=bind(_soc.getfd(),(struct sockaddr*)_addr.getAddr(),_addr.size());
    if(ret<0){
        perror("bind error");
        _exit(-1);
    }
}

void Acceptor::Listen(){
    int ret=listen(_soc.getfd(),128);
    if(ret<0){
        perror("listen error");
        _exit(-1);
    }
    else{
        printf("the server is listening\n");
    }
}

int Acceptor::Accept(){
    int connfd=accept(_soc.getfd(),nullptr,nullptr);
    if(connfd<0){
        perror("accept error");
        _exit(-1);
    }
}

void Acceptor::ready(){
    SetReuseAddr();
    SetReusePort();
    Bind();
    Listen();
}

int Acceptor::fd(){
    return _soc.getfd();
}



