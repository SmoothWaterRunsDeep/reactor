#pragma once

class NonCopyable
{
public:
    NonCopyable()=default;
    ~NonCopyable()=default;

    NonCopyable(const NonCopyable&rhs)=delete;
    NonCopyable& operator=(const NonCopyable&rhs)=delete;
};
