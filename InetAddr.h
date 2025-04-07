#pragma once
#include<arpa/inet.h>
#include<string>
using namespace std;

class InetAddr
{
public:
    InetAddr(const string&,unsigned short);
    /* InetAddr(const InetAddr &rhs);//拷贝构造函数,在 C++ 里，拷贝构造函数的第一个参数必须是对同类型对象的引用（通常是 const 引用）。要是不按引用传递，在调用拷贝构造函数时，就会产生无限递归调用，进而导致编译错误。 */
    InetAddr(struct sockaddr_in&);
    ~InetAddr();
    string getip()const;
    unsigned short getport()const;
    const struct sockaddr_in* getAddr()const;
    socklen_t size();
private:
    struct sockaddr_in _addr;
};

