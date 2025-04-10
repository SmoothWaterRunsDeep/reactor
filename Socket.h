#pragma once
#include"NonCopyable.h"
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>


/*在网络编程里，像 Socket类应实现禁止复制*/
/*1.避免资源重复释放*/
/*2.保证一对一的关系，如果允许 Socket 类对象被复制，就会打破这种一对一的关系，使得程序逻辑变得混乱，难以管理和维护。*/
/*3.线程安全方面避免竞争条件。如果多个 Socket 对象共享同一个套接字描述符，
 * 在多线程环境下对该套接字进行读写操作时，就可能会出现竞争条件。
 * 例如，一个线程正在发送数据，而另一个线程同时关闭了套接字，这会导致数据传输错误或者程序崩溃。*/

/*4.遵循 RAII 原则：Socket 类通常遵循资源获取即初始化（RAII）原则，也就是在对象构造时获取资源，在对象析构时释放资源。
 * 禁止复制可以确保资源的生命周期和对象的生命周期保持一致，提高代码的可维护性和复用性。*/
class Socket
:NonCopyable
{
public:
    Socket();
    Socket(int);
    ~Socket(); 
    int getfd()const;
    void shut_write();
private:
int _fd;
};

