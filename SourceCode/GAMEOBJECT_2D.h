#pragma once
#include "GameObject.h"
class GameObject;
class OBJECT_DATA;
class GameObject_2D : public GameObject
{

public:
    GameObject_2D(std::shared_ptr<OBJECT_DATA>d);
    HRESULT Initialize() override;
    void Execute() override;
    void ExecuteUI() override;
    void Render() override;
    void RenderUI() override;
};