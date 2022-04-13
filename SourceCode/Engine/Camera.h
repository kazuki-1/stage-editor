#pragma once
#include "Singleton.h"
#include "DXMath.h"
#include <DirectXMath.h>
using namespace DirectX;
using namespace Math;
class Camera : public Singleton<Camera>
{
    //Camera() {};
    float range{};
    Vector3 position{}, rotation{}, velocity{};
    Vector3 Eye{};
    Vector3 target{};
    Vector3 next_target{};
    XMMATRIX viewMatrix{};
    XMFLOAT4X4 view{};
    bool reset{};
public:
    //static Camera* Instance()
    //{
    //    static Camera c;
    //    return &c;
    //}
    void SetPosition(XMFLOAT3 pos)
    {
        position = pos;
    }
    void SetVelocity(XMFLOAT3 vel)
    {
        velocity = vel;
    }
    void SetRotation(XMFLOAT3 rot)
    {
        rotation = rot;
    }
    void SetTarget(XMFLOAT3 t)
    {
        target = t;
    }
    void SetTarget(Vector3 t)
    {
        target = t;
    }
    void SetLookAt()
    {
        Vector3 u{ 0, 1, 0 };
        viewMatrix = XMMatrixLookAtLH(Eye.XMV(), target.XMV(), u.XMV());
    }
    void SetRange(float r)
    {
        range = r;
    }
    void ResetCamera();
    void ResetToTarget(Vector3 t);

    float Range()
    {
        return range;
    }
    XMFLOAT3 Position()
    {
        return position.XMF3();
    }
    XMFLOAT3 Velocity()
    {
        return velocity.XMF3();
    }
    XMFLOAT3 Rotation()
    {
        return rotation.XMF3();
    }
    XMFLOAT3 EyePosition()
    {
        return Eye.XMF3();
    }
    XMMATRIX ViewMatrix()
    {
        return viewMatrix;
    }


    void Initialize(XMFLOAT3 Default_Eye_Position, XMFLOAT3 Target);
    void Execute();
    void Render();
};
