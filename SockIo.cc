#include"SockIo.h"
#include<unistd.h>
#include<errno.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<iostream>
using namespace std;


SockIo::SockIo(int fd)
:_fd(fd)
{}

SockIo::~SockIo(){
    close(_fd);
}

/*在网络编程中，read 函数并不保证每次都能读取到你所要求的全部字节数。
 * 因为网络是不可靠的，数据可能会分多次到达，或者由于各种原因（如信号中断、网络拥塞等）
 * 导致读取操作提前返回。因此，readn 函数的主要作用就是通过循环调用read 函数
 * ，确保能读取到指定长度的数据。*/
int SockIo::readn(char*buf,int len){
    int left=len;//用left记录还能够读取的字节数，即缓冲区的剩余空间大小
    char*ptr=buf;//用buf记录当前读取到的位置
    int ret=0;

    while(left>0){
        ret=read(_fd,ptr,left);
        if(ret<0&&errno==EINTR){//返回 -1 且 errno 为 EINTR，表示读取操作被信号中断，继续循环重试。
            continue;
        }
        else if(ret<0){//如果 read 返回 -1 且 errno 不为 EINTR，表示发生了真正的错误
            perror("read error");
            _exit(-1);
        }

        else if(ret==0){//当读取到的文件长度是0，表示客户端关闭了连接
            break;
        }

        else{//这种情况表示可以继续正常读取
            ptr+=ret;//当前读取位置前移ret个字节
            left-=ret;//剩余待读取字节数减少ret个字节

        }

    }
    return len-left;
}

/*readn 函数主要用于读取指定长度的数据，不关注数据是否为字符串，因此不需要预留 '\0' 的空间；
 * 而 readLine 函数用于读取一行数据并将其作为 C 风格字符串处理，所以需
 * 要预留一个字节的空间来存储 '\0'。*/
//其实readn一般情况下还是为readLine函数服务的，因为这种按照读取更符合用户的操作习惯
int SockIo::readLine(char*buf,int len){
    int left=len-1;//注意在readLine函数中。len是指明缓冲区大小的。而这里是为了预留一个字节存储\0
    char*ptr=buf;//指向缓冲区中待写入数据的位置
    int total=0;//已读取的字节数是0
    int ret=0;//y用于记录recv函数的返回值

    //使用 recv 函数的 MSG_PEEK 标志，我们可以先查看内核缓冲区中的数据，判断是否包含换行符，然后再决定读取多少数据。    
    while(left>0){
        ret=recv(_fd,ptr,left,MSG_PEEK);//注意这里必须是拷贝，不能直接把数据从内核移走
        if(ret<0&&errno==EINTR){    //如果是发生了中断，则继续进行循环
            continue;
        }
        else if(ret==-1){
            perror("recv error");   //如果真的发生错误，则报错并退出
            _exit(-1);
        }

        else if(ret==0){
            break;  //如果客户端断开连接，则退出循环
        }
        else{
            for(int i=0;i<ret;++i){
                if(ptr[i]=='\n'){   //如果读到了换行符
                    readn(buf,i+1);
                    total=i+1;
                    ptr[i+1]='\0';
                    return total;
                }
            }
            readn(ptr,ret);//如果在这一次读取中没有读到换行符
            total+=ret;
            ptr+=ret;
            left-=ret;
        }
    }
    *ptr='\0';
    return total;
}

int SockIo::writen(const char*buf,int len){
    int left=len;//这里的left表示的是剩余需要发送的字节数
    const char*ptr=buf;
    int ret=0;
    cout<<"writen ok"<<endl;

    while(left>0){
        ret=write(_fd,ptr,left);
        if(ret<0&&errno==EINTR){
            continue;
        }
        else if(ret<0){
            perror("write error");
            _exit(-1);
        }
        else if(!ret){//如果ret为0
            break;
        }

        else{
            ptr+=ret;
            left-=ret;
        }
    }
    return len-left;
}



