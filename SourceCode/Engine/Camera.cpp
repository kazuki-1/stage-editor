#include "Camera.h"
#include "Input.h"
#include "DEBUG_PRIMITIVE.h"
#include "AudioEngine.h"
#include "IMGUI.h"
using namespace DirectX;

/*-------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------Camera Class--------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------Camera Initialize()-------------------------------------------------*/

void Camera::Initialize(XMFLOAT3 Default_Eye_Position, XMFLOAT3 Target)
{
    
    cameraPosition = Default_Eye_Position;
    target = Target;
    XMVECTOR UP{ 0.0f, 1.0f, 0.0f, 0.0f };
    viewMatrix = XMMatrixLookAtLH(cameraPosition.XMV(), target.XMV(), UP);

    // Targets the Audiolistener to the camera
    AudioEngine::Instance()->SetAudioListener(&audioListener);


}

/*-----------------------------------------------------------Camera Execute()-------------------------------------------------*/

void Camera::Execute()
{
    // Camera controls
    InputManager* i = InputManager::Instance();
    InputManager::MOUSE* m = InputManager::Instance()->Mouse().get();
    float wheel{};
    if (m->Wheel().Up().Held())
        wheel = 5.0f;
    if (m->Wheel().Down().Held())
        wheel = -5.0f;
    static Vector2 clicked_pos{}, move_pos{};
    static Vector2 movement;
    Vector2 pos, pos2, drag_pos;
    static bool start{};

    if (i->AltKeys()->State().Held())
    {
        if (m->LButton().Triggered()) {
            clicked_pos += m->fPosition();
            start = true;
        }       
        if (m->LButton().Held() && start)
        {
            pos = m->fPosition();
            Vector2 vector{ pos - clicked_pos };
            vector *= 0.1f;
            rotation.y += ToRadians(vector.x);
            rotation.x += -ToRadians(vector.y);
            clicked_pos = pos;
        }              
        if (m->LButton().Released()) {
            clicked_pos = {};
            start = false;
        }
        if (m->RButton().Triggered()) {
            move_pos += m->fPosition();
            start = true;
        }
        if (m->RButton().Held() && start)
        {
            pos2 = m->fPosition();
            movement.x += -(pos2.x - move_pos.x);
            movement.y += pos2.y - move_pos.y;
            movement *= 0.03f;
            move_pos = pos2;
        }
        if (m->RButton().Released())
        {
            move_pos = {};
            movement = {};
            start = false;
        }
    }


    // Update the camera position 
    XMMATRIX transform{ XMMatrixRotationRollPitchYawFromVector(rotation.XMV()) };
    Vector3 horizontol, vertical, forward;
    horizontol.Load(transform.r[0]);
    vertical.Load(transform.r[1]);
    forward.Load(transform.r[2]);
    horizontol.Normalize();
    vertical.Normalize();
    forward.Normalize();
    target += horizontol * movement.x + vertical * movement.y ;
    //rotation.x = Math::Clamp(rotation.x, ToRadians(-89), ToRadians(89));
    range += -forward.Length() * wheel * 0.5f;
    range = (std::max)(1.0f, range);

    
    cameraPosition.x = target.x + (forward.x * -range);
    cameraPosition.y = target.y + (forward.y * range);
    cameraPosition.z = target.z + (forward.z * -range);
    
    SetLookAt();

    // Updates AudioListener for 3D Audio
    forward.Normalize();
    Vector3 last_pos{ audioListener.position}, cur_pos{ cameraPosition };
    Vector3 velocity = cur_pos - last_pos;

    audioListener.position = cur_pos;
    audioListener.vFrontVector = forward;
    audioListener.vTopVector = vertical;
    audioListener.velocity = velocity;


    if (reset)
    {
        /*target.Load(XMVectorLerp(target.XMV(), next_target.XMV(), 0.1f));*/
        target = Vector3::Lerp(target, next_target, 0.1f);
        range = Lerp(range, 10.0f, 0.1f);
        if ((target - next_target).Length() < 0.001f && range - 10.0f < 0.001f)
        {
            reset = false;
            target = next_target;
            range = 10.0f;
        }
    }

}

/*-----------------------------------------------------------Camera Render()-------------------------------------------------*/
// For Debug
void Camera::Render()
{
}

/*-----------------------------------------------------------Camera ResetCamera()-------------------------------------------------*/

void Camera::ResetCamera()
{
    target = {};
    cameraPosition = {};
    rotation = {};
}

/*-----------------------------------------------------------Camera ResetToTarget()-------------------------------------------------*/

void Camera::ResetToTarget(Vector3 t)
{
    next_target = t;
    reset = true;
}

/*-----------------------------------------------------------Camera SetPosition()-------------------------------------------------*/

void Camera::SetPosition(XMFLOAT3 pos)
{
    position = pos;
}

/*-----------------------------------------------------------Camera SetVelocity()-------------------------------------------------*/

void Camera::SetVelocity(XMFLOAT3 vel)
{
    velocity = vel;
}

/*-----------------------------------------------------------Camera SetRotation()-------------------------------------------------*/

void Camera::SetRotation(XMFLOAT3 rot)
{
    rotation = rot;
}

/*-----------------------------------------------------------Camera SetTarget()-------------------------------------------------*/

void Camera::SetTarget(XMFLOAT3 t)
{
    target = t;
}

void Camera::SetTarget(Vector3 t)
{
    target = t;
}

/*-----------------------------------------------------------Camera SetLookAt()-------------------------------------------------*/

void Camera::SetLookAt()
{
    Vector3 u{ 0, 1, 0 };
    viewMatrix = XMMatrixLookAtLH(cameraPosition.XMV(), target.XMV(), u.XMV());
}

/*-----------------------------------------------------------Camera SetRange()-------------------------------------------------*/

void Camera::SetRange(float r)
{
    range = r;
}

/*-----------------------------------------------------------Camera Range()-------------------------------------------------*/

float Camera::Range()
{
    return range;
}

/*-----------------------------------------------------------Camera Position()-------------------------------------------------*/

Vector3 Camera::Position()
{
    return position;
}

/*-----------------------------------------------------------Camera Velocity()-------------------------------------------------*/

Vector3 Camera::Velocity()
{
    return velocity;
}

/*-----------------------------------------------------------Camera Rotation()-------------------------------------------------*/

Vector3 Camera::Rotation()
{
    return rotation;
}

/*-----------------------------------------------------------Camera EyePosition()-------------------------------------------------*/

Vector3 Camera::EyePosition()
{
    return cameraPosition;
}

/*-----------------------------------------------------------Camera ViewMatrix()-------------------------------------------------*/

XMMATRIX Camera::ViewMatrix()
{
    return viewMatrix;
}
