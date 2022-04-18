#include "DEBUG_MANAGER.h"
#include "../Components/Base Classes/Component.h"
#include "../Components/Transform3D.h"
#include "../GAMEOBJECT_2D.h"
#include "IMGUI.h"
#include "Input.h"
#include "Camera.h"
bool scaling{}, moving{ true }, rotating{};
bool start;
Vector2 clicked_pos, movement;

HRESULT DebugController::Initialize()
{
    arrows = std::make_shared<DEBUG_ARROWS>();
    scalars = std::make_shared<DEBUG_SCALARS>();
    discs = std::make_shared<DEBUG_DISCS>();
    arrows->Initialize();
    scalars->Initialize();
    discs->Initialize();
    return S_OK;

}
void DebugController::Execute()
{
    if (!hasTarget)
        return;
    if (dynamic_cast<GameObject_2D*>(target))
        return;
    Transform3D_Component* target = this->target->GetComponent<Transform3D_Component>();
    arrows->SetTarget(target->Translation(), target->Rotation());
    scalars->SetTarget(target->Translation(), target->Rotation());
    discs->SetTarget(target->Translation(), target->Rotation());
    arrows->Execute();
    scalars->Execute();
    discs->Execute();

    //if (moving)
    //    TranslateArrows();









    INPUTMANAGER::KEYBOARD* k{ INPUTMANAGER::Instance()->Keyboard().get() };

    if (k->Triggered('W'))
    {
        moving = true;
        scaling = false;
        rotating = false;
    }
    if (k->Triggered('E'))
    {
        moving = false;
        scaling = false;
        rotating = true;
    }

    if (k->Triggered('R'))
    {
        moving = false;
        scaling = true;
        rotating = false;
    }

}
void DebugController::Render()
{
    if (!hasTarget)
        return;
    if (scaling)
        scalars->Render();
    else if (rotating)
        discs->Render();
    else if (moving)
        arrows->Render();

}
void DebugController::SetTarget(GameObject* g)
{
    target = g;
    hasTarget = true;
}
void DebugController::ClearTarget()
{
    target = {};
    hasTarget = false;
}
void DebugController::MouseControl()
{
    Vector2 pos;
    INPUTMANAGER::MOUSE* m{ INPUTMANAGER::Instance()->Mouse().get() };
    if (m->LButton().Triggered())
    {
        clicked_pos += m->fPosition();
        start = true;
    }
    if (m->LButton().Held() && start)
    {
        pos = m->fPosition();
        Vector2 v{ pos - clicked_pos };
        v *= 0.1f;
        clicked_pos = pos;
    }
    if (m->LButton().Released())
    {
        clicked_pos = {};
        start = false;
    }

    Vector3 m_pos;
    Vector4 start, end;
    m_pos.x = m->fPosition().x;
    m_pos.y = m->fPosition().y;
    m_pos.z = 0.0f;
    D3D11_VIEWPORT vp;
    UINT num{ 1 };
    DirectX11::Instance()->DeviceContext()->RSGetViewports(&num, &vp);
}

bool x{}, y{}, z{};


void DebugController::TranslateArrows()
{
    INPUTMANAGER* in{ INPUTMANAGER::Instance() };
    Vector2 movement{};
    D3D11_VIEWPORT vp;
    UINT num{ 1 };
    DirectX11::Instance()->DeviceContext()->RSGetViewports(&num, &vp);
 
    if (dynamic_cast<GameObject_2D*>(target))
        return;
    Transform3D_Component* target = this->target->GetComponent<Transform3D_Component>();


    Vector3 horizontol, vertical, forward;
    horizontol.Load(target->TransformMatrix().r[0]);
    vertical.Load(target->TransformMatrix().r[1]);
    forward.Load(target->TransformMatrix().r[2]);

    static Vector2 clicked_pos{};
    static bool start;
    Vector2 pos;
    Vector2 v;

    if (in->Mouse()->LButton().Triggered())
    {
        clicked_pos = in->Mouse()->fPosition();
        start = true;
    }
    if (in->Mouse()->LButton().Held() && start)
    {
        pos = in->Mouse()->fPosition();
        v = { pos - clicked_pos };
        clicked_pos = pos;
    }
    if (in->Mouse()->LButton().Released())
    {
        clicked_pos = {};
        start = false;
    }





    movement = v;
    

    if (in->MouseRayCast(arrows->xAxis.get(), vp))
    {
        x = true;
    }
    else if (in->MouseRayCast(arrows->yAxis.get(), vp) )
    {
        y = true;
    }
    else if (in->MouseRayCast(arrows->zAxis.get(), vp) )
    {
        z = true;
    }
    if (in->Mouse()->LButton().Released())
        x = y = z = false;

    
    ImGui::Begin("S");
    ImGui::Checkbox("X", &x);
    ImGui::Checkbox("Y", &y);
    ImGui::Checkbox("Z", &z);
    ImGui::End();





}