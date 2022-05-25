#include "MODEL.h"
#include "Rasterizer.h"

/*----------------------------------------------------------------------------------------------*/
/*--------------------------------MODEL Class---------------------------------------------------*/
/*----------------------------------------------------------------------------------------------*/
/*--------------------------------MODEL Initialize()--------------------------------------------*/

HRESULT MODEL::Initialize(std::string model_path)
{
    resource = ModelResourceManager::Instance()->Load(model_path);
    if (!resource)
        return E_FAIL;
    animationTakes.resize(resource->Animations.size());
    for (int a = 0; a < animationTakes.size(); ++a)
        animationTakes[a] = resource->Animations[a].Name;

    for (int ind = 0; ind < Resource()->Meshes.size(); ++ind)
    {
        std::shared_ptr<BOUNDING_BOX>& b = Boxes.emplace_back();
        b = std::make_shared<BOUNDING_BOX>();
        b->Min.x = resource->Meshes[ind].BOUNDING_BOX[0].x;
        b->Min.y = resource->Meshes[ind].BOUNDING_BOX[0].y;
        b->Min.z = resource->Meshes[ind].BOUNDING_BOX[0].z;
        b->Max.x = resource->Meshes[ind].BOUNDING_BOX[1].x;
        b->Max.y = resource->Meshes[ind].BOUNDING_BOX[1].y;
        b->Max.z = resource->Meshes[ind].BOUNDING_BOX[1].z;

    }
    cur_AnimationTake = 0;

    return S_OK;
}

/*--------------------------------MODEL UpdateTransform()--------------------------------------------*/

void MODEL::UpdateTransform()
{
    XMMATRIX S, R, T;
    //S = XMMatrixScaling(scale.x, scale.y, scale.z);
    //T = XMMatrixTranslation(translation.x, translation.y, translation.z);
    S = XMMatrixScalingFromVector(scale.XMV());
    R = XMMatrixRotationQuaternion(quaternion.XMV());
    T = XMMatrixTranslationFromVector(translation.XMV());
    m_Transform = S * R * T;
    XMStoreFloat4x4(&transform, m_Transform);
}

/*--------------------------------MODEL Render()--------------------------------------------*/

void MODEL::Render(float SamplingRate, XMFLOAT4 colour)
{
    static const int transition_delay{ 15 };
    
    AN::KEYFRAME* cur_kf, * next_kf, output;
    AN* cur_an, * next_an;

    if (cur_AnimationTake == -1)
        cur_AnimationTake = next_AnimationTake;
    if (cur_AnimationTake != next_AnimationTake && !isTransitioning)
    {
        transition_progress = next_Keyframe = 0;
        next_AnimTimer = 0.0f;
        isTransitioning = true;
    }
    else if (!isTransitioning)
        cur_AnimationTake = next_AnimationTake;
    cur_an = &Resource()->Animations.at(cur_AnimationTake);
    next_an = &Resource()->Animations.at(next_AnimationTake);
    SamplingRate = SamplingRate ? SamplingRate : cur_an->SamplingRate;
    if (isTransitioning)
    {
        ++transition_progress;
        transition_progress = (std::min)(transition_delay, transition_progress);
        factor = (float)transition_progress / (float)transition_delay;
    }
    else
        factor = 0.5f;
    if (!animPaused)
    {
        cur_Keyframe = (int)(cur_AnimTimer * SamplingRate);
        next_Keyframe = (int)(next_AnimTimer * SamplingRate);
    }
    if (cur_Keyframe > cur_an->Keyframes.size() - 1)
    {
        cur_Keyframe = 0;
        cur_AnimTimer = 0.0f;
    }
    else
        cur_AnimTimer += FRAMETIME;
    if (next_Keyframe > next_an->Keyframes.size() - 1)
    {
        next_Keyframe = 0;
        next_AnimTimer = 0.0f;
    }
    else
        next_AnimTimer += FRAMETIME;

    if (!isTransitioning)
        next_Keyframe = (std::min)(cur_Keyframe + 1, (int)next_an->Keyframes.size() - 1);

    cur_kf = &cur_an->Keyframes.at(cur_Keyframe);
    next_kf = &next_an->Keyframes.at(next_Keyframe);

    if (factor == 1)
    {
        isTransitioning = false;
        cur_AnimationTake = next_AnimationTake;
    }





    resource->BlendAnimation(cur_kf, next_kf, factor, &output);

    resource->UpdateAnimation(&output);
    resource->Render(DirectX11::Instance()->DeviceContext(), transform, colour, &output);



}

/*--------------------------------MODEL RenderWireframe()--------------------------------------------*/

void MODEL::RenderWireframe(Vector4 colour)
{
    Vector3 old_scale{ scale };
    SetScale(scale * 1.01f);
    UpdateTransform();
    DirectX11::Instance()->DeviceContext()->RSSetState(RasterizerManager::Instance()->Retrieve(RasterizerTypes::Wireframe)->GetRasterizer().Get());
    Render(0.0f, colour.XMF4());
    SetScale(old_scale);

}

/*--------------------------------MODEL ResetTimer()--------------------------------------------*/

void MODEL::ResetTimer()
{
    cur_AnimTimer = 0;
}

/*--------------------------------MODEL FinishedAnim()--------------------------------------------*/

bool MODEL::FinishedAnim()
{
    return cur_Keyframe >= Resource()->Animations.at(cur_AnimationTake).Keyframes.size() - 1;
}

/*--------------------------------MODEL InAnim()--------------------------------------------*/

bool MODEL::InAnim(int start, int end)
{
    return cur_Keyframe >= start && cur_Keyframe <= end;
}

/*--------------------------------MODEL PauseAnim()--------------------------------------------*/

void MODEL::PauseAnim()
{
    animPaused = true;
}

/*--------------------------------MODEL ResumeAnim()--------------------------------------------*/

void MODEL::ResumeAnim()
{
    animPaused = false;
}

/*--------------------------------MODEL SetTranslation()--------------------------------------------*/

void MODEL::SetTranslation(Vector3 t)
{
    translation = t;
}

/*--------------------------------MODEL SetRotation()--------------------------------------------*/

//void MODEL::SetQuaternion(VECTOR4 q)
//{
//    quaternion = q;
//}
void MODEL::SetRotation(Vector3 r)
{
    rotation = r;
}

/*--------------------------------MODEL SetScale()--------------------------------------------*/

void MODEL::SetScale(Vector3 s)
{
    scale = s;
}

/*--------------------------------MODEL SetTransformation()--------------------------------------------*/

void MODEL::SetTransformation(Vector3 s, Vector3 r, Vector3 t)
{
    scale = s;
    rotation = Vector3::ToRadians(r);
    quaternion.Load(XMQuaternionRotationRollPitchYawFromVector(rotation.XMV()));
    translation = t;
}
void MODEL::SetTransformation(Vector3 s, Vector4 q, Vector3 t)
{
    scale = s;
    quaternion = q;
    translation = t;
}
//void MODEL::SetTransformationQ(VECTOR3 s, VECTOR4 q, VECTOR3 t)
//{
//    scale = s.XMF3();
//    quaternion = q;
//    translation = t.XMF3();
//}

/*--------------------------------MODEL OffsetTransformation()--------------------------------------------*/

void MODEL::OffsetTransform(XMMATRIX mat)
{
    XMMATRIX cur{ XMLoadFloat4x4(&transform) };
    XMStoreFloat4x4(&transform, cur * mat);
}

/*--------------------------------MODEL SetTake()--------------------------------------------*/

void MODEL::SetTake(int take)
{
    next_AnimationTake = take;
}

/*--------------------------------MODEL SetFrame()--------------------------------------------*/

void MODEL::SetFrame(int frame)
{
    cur_Keyframe = frame;
}

/*--------------------------------MODEL CurrentTake()--------------------------------------------*/

int MODEL::CurrentTake()
{
    return cur_AnimationTake;
}

/*--------------------------------MODEL CurrentFrame()--------------------------------------------*/

int MODEL::CurrentFrame()
{
    return cur_Keyframe;
}

/*--------------------------------MODEL Scale()--------------------------------------------*/

Vector3 MODEL::Scale()
{
    return scale;
}

/*--------------------------------MODEL Rotation()--------------------------------------------*/

Vector3 MODEL::Rotation()
{
    return rotation;
}

/*--------------------------------MODEL Translation()--------------------------------------------*/

Vector3 MODEL::Translation()
{
    return translation;
}
//VECTOR4 MODEL::Quaternion()
//{
//    return quaternion;
//}

/*--------------------------------MODEL Transform()--------------------------------------------*/

XMFLOAT4X4 MODEL::Transform()
{
    return transform;
}

/*--------------------------------MODEL TransformMatrix()--------------------------------------------*/

XMMATRIX MODEL::TransformMatrix()
{
    //return XMMatrixScaling(scale.x, scale.y, scale.z) * XMMatrixRotationQuaternion(quaternion.XMV()) * XMMatrixTranslation(translation.x, translation.y, translation.z);
    Vector4 q;
    Vector3 r{ ToRadians(rotation.x), ToRadians(rotation.y), ToRadians(rotation.z) };
    q.Load(XMQuaternionRotationRollPitchYawFromVector(rotation.XMV()));

    return XMMatrixScalingFromVector(scale.XMV()) * XMMatrixRotationQuaternion(q.XMV()) * XMMatrixTranslationFromVector(translation.XMV());
}

/*--------------------------------MODEL AnimationTakes()--------------------------------------------*/

std::vector<std::string>MODEL::AnimationTakes()
{
    return animationTakes;
}

/*--------------------------------MODEL Resource()--------------------------------------------*/

std::shared_ptr<MODEL_RESOURCES>MODEL::Resource()
{
    return resource;
}

/*--------------------------------MODEL GetBB()--------------------------------------------*/

std::vector<std::shared_ptr<MODEL::BOUNDING_BOX>>MODEL::GetBB()
{
    return Boxes;
}

/*--------------------------------MODEL Right()--------------------------------------------*/

Vector3 MODEL::Right()
{
    Vector3 temp;
    temp.Load(TransformMatrix().r[0]);
    return temp;
}

/*--------------------------------MODEL Up()--------------------------------------------*/

Vector3 MODEL::Up()
{
    Vector3 temp;
    temp.Load(TransformMatrix().r[1]);
    return temp;
}

/*--------------------------------MODEL Forward()--------------------------------------------*/

Vector3 MODEL::Forward()
{
    Vector3 temp;
    temp.Load(TransformMatrix().r[2]);
    return temp;
}

/*--------------------------------MODEL RetrieveAxises()--------------------------------------------*/

void MODEL::RetrieveAxisesQ(Vector3* r, Vector3* u, Vector3* f)
{
    UpdateTransform();
    XMMATRIX temp{ TransformMatrix() };
    if (r)
    {
        XMFLOAT3 t;
        XMStoreFloat3(&t, temp.r[0]);
        *r = t;
    }
    if (u)
    {
        XMFLOAT3 t;
        XMStoreFloat3(&t, temp.r[1]);
        *u = t;
    }
    if (f)
    {
        XMFLOAT3 t;
        XMStoreFloat3(&t, temp.r[2]);
        *f = t;
    }

}

/*--------------------------------MODEL PerformUVScrolling()--------------------------------------------*/

void MODEL::PerformUVScrolling()
{
    //resource->performUVScroll = true;
}

/*--------------------------------MODEL StopUVScrolling()--------------------------------------------*/

void MODEL::StopUVScrolling()
{
    //resource->performUVScroll = false;
}