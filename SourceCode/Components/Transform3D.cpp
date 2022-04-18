#include "Base Classes/Component.h"
#include "Transform3D.h"

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------Transform3D_Data Class--------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------Transform3D_Data Constructor--------------------------------------------*/

Transform3D_Data::Transform3D_Data()
{
    type = COMPONENT_TYPE::Transform3D_Component;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------Transform3D_Component Class-------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------Transform3D_Component Constructor-------------------------------------------------*/

Transform3D_Component::Transform3D_Component(GameObject* t, ComponentData* data)
{
    parent = t;
    this->data = static_cast<Transform3D_Data*>(data);
    assert(data != nullptr);
}

/*---------------------------------------Transform3D_Component Initialize()-------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (Transform3D_Data) </para>
/// <para>コンポネント生成時呼び出せます。対応したコンポネントデータを用いてこのコンポネントを初期化する（TRANSFORM_3D_DATA）</para>
/// </summary>
/// <returns></returns>
HRESULT Transform3D_Component::Initialize()
{
    quaternion.Load(XMQuaternionRotationRollPitchYawFromVector(ToRadians(data->rotation).XMV()));
    // scale = data->scale;
    // translation = data->translation;
    return S_OK;
}

/*---------------------------------------Transform3D_Component Execute()-------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> フレームごっこに呼び出せる </para>
/// </summary>
void Transform3D_Component::Execute()
{
     data->translation += velocity;
    velocity *= 0.9f;
    if (velocity.Length() < 0.01f)
        velocity = {};


    // quaternion.Load(XMQuaternionRotationRollPitchYawFromVector(data->rotation.XMV()));
    // VECTOR3 r{ ToRadians(data->rotation) };
    // quaternion.Load(XMQuaternionRotationRollPitchYawFromVector(r.XMV()));
    XMMATRIX S, R, T;
    S = XMMatrixScalingFromVector(data->scale.XMV());
    R = XMMatrixRotationQuaternion(quaternion.XMV());
    T = XMMatrixTranslationFromVector(data->translation.XMV());
    XMMATRIX W{ S * R * T };

    XMStoreFloat4x4(&transform, S * R * T);



}

/*---------------------------------------Transform3D_Component ExecuteUI()-------------------------------------------------*/

void Transform3D_Component::ExecuteUI()
{
    XMMATRIX S, R, T;
    S = XMMatrixScalingFromVector(data->scale.XMV());
    quaternion.Load(XMQuaternionRotationRollPitchYawFromVector(ToRadians(data->rotation).XMV()) );
    R = XMMatrixRotationQuaternion(quaternion.XMV());
    T = XMMatrixTranslationFromVector(data->translation.XMV());
    XMStoreFloat4x4(&transform, S * R * T);
}

/*---------------------------------------Transform3D_Component Render()-------------------------------------------------*/
/// <summary>
/// <para>Called after Execute to perform any render functions </para>
/// <para>Execute実行後に描画用関数を呼び出す関数</para>
/// </summary>
void Transform3D_Component::Render()
{
}

/*---------------------------------------Transform3D_Component UI()-------------------------------------------------*/

/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画する関数 </para>
/// </summary>
void Transform3D_Component::UI()
{
    
    ImGui::SetNextItemOpen("true");
    if (ImGui::TreeNode("Transform"))
    {
        ImGui::DragFloat3("Scale", &data->scale.x, 0.05f);
        ImGui::DragFloat3("Rotation", &data->rotation.x, 0.05f);
        ImGui::DragFloat3("Translation", &data->translation.x, 0.05f);
        ImGui::TreePop();
    }
}

/*---------------------------------------Transform3D_Component Scale()-------------------------------------------------*/
/// <summary>
/// Returns the scale 
/// </summary>
/// <returns></returns>
Vector3 Transform3D_Component::Scale()
{
    return data->scale;
}

/*---------------------------------------Transform3D_Component Rotation()-------------------------------------------------*/
/// <summary>
/// Returns the rotation
/// </summary>
/// <returns></returns>
Vector3 Transform3D_Component::Rotation()
{
    return data->rotation;
}

/*---------------------------------------Transform3D_Component Translation-------------------------------------------------*/
/// <summary>
/// Returns the translation
/// </summary>
/// <returns></returns>
Vector3 Transform3D_Component::Translation()
{
    return data->translation;
}

/*---------------------------------------Transform3D_Component Velocity()-------------------------------------------------*/
/// <summary>
/// Returns the velocity
/// </summary>
/// <returns></returns>
Vector3 Transform3D_Component::Velocity()
{
    return velocity;
}

/*---------------------------------------Transform3D_Component Transform()-------------------------------------------------*/
/// <summary>
/// <para> Returns the transformation matrix in a XMFLOAT4X4 form </para>
/// <para> 変換行列をXMFLOAT4X4の形で戻る</para>
/// </summary>
/// <returns></returns>
XMFLOAT4X4 Transform3D_Component::Transform()
{
    return transform;
}

/*---------------------------------------Transform3D_Component TranformMatrix()-------------------------------------------------*/
/// <summary>
/// <para> Returns the transformation matrix in a XMMATRIX form </para>
/// <para> 変換行列をXMMATRIXの形で戻る</para>
/// </summary>
/// <returns></returns>
XMMATRIX Transform3D_Component::TransformMatrix()
{
    Vector4 q{};
    Vector3 r{ ToRadians(data->rotation) };
    q.Load(XMQuaternionRotationRollPitchYawFromVector(r.XMV()));
    return XMMatrixScalingFromVector(data->scale.XMV()) * XMMatrixRotationQuaternion(q.XMV()) * XMMatrixTranslationFromVector(data->translation.XMV());
}

/*---------------------------------------Transform3D_Component TranformMatrixQuaternion()-------------------------------------------------*/
/// <summary>
/// <para> Returns the transformation matrix in a XMMATRIX form </para>
/// <para> 変換行列をXMMATRIXの形で戻る</para>
/// </summary>
/// <returns></returns>
XMMATRIX Transform3D_Component::TransformMatrixQuaternion()
{
    return XMMatrixScalingFromVector(data->scale.XMV()) * XMMatrixRotationQuaternion(quaternion.XMV()) * XMMatrixTranslationFromVector(data->translation.XMV());
}



/*---------------------------------------Transform3D_Component Data()-------------------------------------------------*/
/// <summary>
/// <para> Returns the data class that stores all the data of this component </para>
/// <para> コンポネントをエキスパート用データを戻る</para>
/// </summary>
/// <returns></returns>
Transform3D_Data* Transform3D_Component::Data()
{
    return data;
}

/*---------------------------------------Transform3D_Component Right()-------------------------------------------------*/
/// <summary>
/// Returns the right vector of the object
/// </summary>
/// <returns></returns>
Vector3 Transform3D_Component::Right()
{
    XMMATRIX temp{ TransformMatrix() };
    Vector3 r{};
    r.Load(temp.r[0]);
    r.Normalize();
    return r;
}

/*---------------------------------------Transform3D_Component Forward()-------------------------------------------------*/
/// <summary>
/// Returns the forward vector of the object
/// </summary>
/// <returns></returns>
Vector3 Transform3D_Component::Forward()
{
    XMMATRIX temp{ TransformMatrixQuaternion() };
    Vector3 r{};
    r.Load(temp.r[2]);
    r.Normalize();
    return r;
}

/*---------------------------------------Transform3D_Component Up()-------------------------------------------------*/
/// <summary>
/// Returns the up vector of the object
/// </summary>
/// <returns></returns>
Vector3 Transform3D_Component::Up()
{
    XMMATRIX temp{ TransformMatrix() };
    Vector3 r{};
    r.Load(temp.r[1]);
    r.Normalize();
    return r;

}

/*---------------------------------------Transform3D_Component Quaternion()-------------------------------------------------*/
/// <summary>
/// Returns the quaternion of rotation
/// </summary>
/// <returns></returns>
Vector4 Transform3D_Component::Quaternion()
{
    return quaternion;
}

/*---------------------------------------Transform3D_Component SlerpRotation()-------------------------------------------------*/
/// <summary>
/// <para> Slerps the rotation to the target vector. Target is in radian</para>
/// <para> Targetに対して球面線形補間を行う. VECTOR3はラジアン型</para>
/// </summary>
void Transform3D_Component::SlerpRotation(Vector3 target)
{
    Vector4 q1;
    q1.Load(XMQuaternionRotationRollPitchYawFromVector(target.XMV()));
    quaternion.Load(XMQuaternionSlerp(quaternion.XMV(), q1.XMV(), 0.1f));

}
/// <summary>
/// <para> Slerps the rotation to the target quaternion </para>
/// <para> Targetに対して球面線形補間を行う</para>
/// </summary>
void Transform3D_Component::SlerpRotation(Vector4 target)
{
    quaternion.Load(XMQuaternionSlerp(quaternion.XMV(), target.XMV(), 0.1f));
}

/*---------------------------------------Transform3D_Component SetScale()-------------------------------------------------*/

void Transform3D_Component::SetScale(Vector3 s)
{
    data->scale = s;
}

/*---------------------------------------Transform3D_Component SetRotation()-------------------------------------------------*/

void Transform3D_Component::SetRotation(Vector3 r)
{
    data->rotation = r;
}

/*---------------------------------------Transform3D_Component SetTranslation-------------------------------------------------*/

void Transform3D_Component::SetTranslation(Vector3 t)
{
    data->translation = t;
}

/*---------------------------------------Transform3D_Component OffsetTranslation()-------------------------------------------------*/

void Transform3D_Component::OffsetTranslation(Vector3 off)
{
    data->translation += off;
}

/*---------------------------------------Transform3D_Component SetVelocity()-------------------------------------------------*/

void Transform3D_Component::SetVelocity(Vector3 v)
{
    velocity = v;
}

/*----------------------------------------------------------Transform3D_Component GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE Transform3D_Component::GetComponentType()
{
    return data->type;
}