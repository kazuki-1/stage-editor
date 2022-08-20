#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Library/Singleton.h>
#include <string>

class DROPMANAGER : public Singleton<DROPMANAGER>
{
    std::string path{};
    std::wstring wpath{};
    bool loaded{};
public:
    void Load(TCHAR* t);
    bool Loaded();

    std::string Path();
    std::wstring WPath();

};