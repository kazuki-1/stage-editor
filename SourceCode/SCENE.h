#pragma once
#include <d3d11.h>
class SCENE
{
public:
    virtual HRESULT Initialize() { return S_OK; };
    virtual void Execute() {};
    virtual void Render() {};
    virtual void Finalize() {};
};