#pragma once
#include "DEBUG_PRIMITIVE.h"
#include "DXMath.h"
#include "Singleton.h"
#include <d3d11.h>
using namespace Math;

class GAMEOBJECT;
class TRANSFORM_3D;
/// <summary>
/// Use this to call out the translation arrow, rotation circles and the scaling scalars
/// </summary>
class DEBUG_MANAGER : public SINGLETON<DEBUG_MANAGER>
{
    //XMMATRIX target;
    //TRANSFORM_3D* target;
    GAMEOBJECT* target;
    bool hasTarget;
    std::shared_ptr<DEBUG_ARROWS>arrows;
    std::shared_ptr<DEBUG_DISCS>discs;
    std::shared_ptr<DEBUG_SCALARS>scalars;
    void TranslateArrows();
    void ScaleScalars();
    void RotateDiscs();
public:
    HRESULT Initialize();
    void Execute();
    void Render();
    //void SetTarget(TRANSFORM_3D* t);
    void SetTarget(GAMEOBJECT* g);
    void ClearTarget();
    void MouseControl();
    bool HasTarget() { return hasTarget; }
};