#pragma once
#include "DEBUG_PRIMITIVE.h"
#include "DXMath.h"
#include "Singleton.h"
#include <d3d11.h>
using namespace Math;

class GameObject;
class Transform3D_Component;
/// <summary>
/// Use this to call out the translation arrow, rotation circles and the scaling scalars
/// </summary>
class DebugController : public Singleton<DebugController>
{
    //XMMATRIX target;
    //Transform3D_Component* target;
    GameObject* target;
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
    //void SetTarget(Transform3D_Component* t);
    void SetTarget(GameObject* g);
    void ClearTarget();
    void MouseControl();
    bool HasTarget() { return hasTarget; }
};