#pragma once
template <class T>
class SINGLETON
{
public:
    static T* Instance()
    {
        static T t;
        return &t;
    }
};