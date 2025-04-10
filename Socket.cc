#include"Socket.h"

Socket::Socket(){
    _fd=socket(AF_INET,SOCK_STREAM,0);
    if(_fd<0){
        perror("socket creation error");
        _exit(-1);
    }
}

Socket::Socket(int fd)
    :_fd(fd){}


    Socket::~Socket(){
        close(_fd);
    }


/*用于获取套接字的文件描述符*/
int Socket::getfd()const{
    return _fd;
}

/*用于关闭已基于该套接字进行连接的写端*/
void Socket::shut_write(){
    int ret=shutdown(_fd,SHUT_WR);
    if(ret<0){
        perror("shutdown error");
        _exit(-1);
    }
}

