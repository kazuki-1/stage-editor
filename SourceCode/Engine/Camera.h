#pragma once
#include <Windows.h>
#include <x3daudio.h>
#include <DirectXMath.h>
#include "Audio.h"
#include <Library/Singleton.h>
#include <Library/Math.h>
class AudioListener;

using namespace DirectX;
using namespace Math;
class Camera : public Singleton<Camera>
{
    //Camera() {};
    AudioListener audioListener;
    float range{};
    Vector3 position{}, rotation{}, velocity{};
    Vector3 cameraPosition{};
    Vector3 target{};
    Vector3 next_target{};
    XMMATRIX viewMatrix{};
    XMFLOAT4X4 view{};
    bool reset{};
public:
    void Initialize(XMFLOAT3 Default_Eye_Position, XMFLOAT3 Target);
    void Execute();
    void Render();
    void ResetCamera();
    void ResetToTarget(Vector3 t);



    void SetPosition(XMFLOAT3 pos);
    void SetVelocity(XMFLOAT3 vel);
    void SetRotation(XMFLOAT3 rot);
    void SetTarget(XMFLOAT3 t);
    void SetTarget(Vector3 t);
    void SetLookAt();
    void SetRange(float r);

    float Range();
    Vector3 Position();
    Vector3 Velocity();
    Vector3 Rotation();
    Vector3 EyePosition();
    XMMATRIX ViewMatrix();


};
