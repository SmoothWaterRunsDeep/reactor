#include"InetAddr.h"
#include<string.h>

InetAddr::InetAddr(const string &ip,unsigned short port){
    ::bzero(&_addr,sizeof(_addr));//这里使用限定符是明确告诉编译器，调用的是全局作用域中的bzero函数，防止因为命名冲突导致调用了其他函数
    _addr.sin_family=AF_INET;//设置网络协议
    _addr.sin_port=htons(port);//设置端口
    _addr.sin_addr.s_addr=inet_addr(ip.c_str());//设置ip
}

InetAddr::InetAddr(struct sockaddr_in &addr)
:_addr(addr){}

InetAddr::~InetAddr(){}//类中的数据成员不是new出来的，所以析构函数可以不做事


string InetAddr::getip()const{
    return string(inet_ntoa(_addr.sin_addr));//从网络字节序转换为本机字节序
}

unsigned short InetAddr::getport()const{
    return ntohs(_addr.sin_port);
}

const struct sockaddr_in* InetAddr::getAddr()const{//注意在 const 成员函数中，this 指针是 const 类型的，这意味着 _addr 实际上是 const struct sockaddr_in 类型的对象，那么 &_addr 就是 const struct sockaddr_in * 类型的指针。
    return &_addr;
}

socklen_t InetAddr::size(){
    return sizeof(_addr);
}

