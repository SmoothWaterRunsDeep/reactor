#pragma once

class SockIo
{
public:
    SockIo(int);
    ~SockIo();
    int readn(char*,int);
    int writen(const char*,int);
    int readLine(char*,int);

private:
    int _fd;
};

