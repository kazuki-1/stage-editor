#pragma once
#include "GAMEOBJECT.h"
class GAMEOBJECT;
class OBJECT_DATA;
class GAMEOBJECT_2D : public GAMEOBJECT
{

public:
    GAMEOBJECT_2D(std::shared_ptr<OBJECT_DATA>d);
    HRESULT Initialize() override;
    void Execute() override;
    void Render() override;
    void RenderUI() override;
};