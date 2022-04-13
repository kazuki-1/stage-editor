#include "../Base Classes/COMPONENT.h"
#include "../Headers/TRANSFORM_3D.h"

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------TRANSFORM_3D_DATA Class--------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------TRANSFORM_3D_DATA Constructor--------------------------------------------*/

TRANSFORM_3D_DATA::TRANSFORM_3D_DATA()
{
    type = COMPONENT_TYPE::TRANSFORM_3D;
}

/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------TRANSFORM_3D Class-------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------TRANSFORM_3D Constructor-------------------------------------------------*/

TRANSFORM_3D::TRANSFORM_3D(GAMEOBJECT* t, COMPONENT_DATA* data)
{
    parent = t;
    this->data = static_cast<TRANSFORM_3D_DATA*>(data);
    assert(data != nullptr);
}

/*---------------------------------------TRANSFORM_3D Initialize()-------------------------------------------------*/
/// <summary>
/// <para>Called when component is created. Initializes the component with the component data of its matching type (TRANSFORM_3D_DATA) </para>
/// <para>コンポネント生成時呼び出せます。対応したコンポネントデータを用いてこのコンポネントを初期化する（TRANSFORM_3D_DATA）</para>
/// </summary>
/// <returns></returns>
HRESULT TRANSFORM_3D::Initialize()
{
    quaternion.Load(XMQuaternionRotationRollPitchYawFromVector(ToRadians(data->rotation).XMV()));
    // scale = data->scale;
    // translation = data->translation;
    return S_OK;
}

/*---------------------------------------TRANSFORM_3D Execute()-------------------------------------------------*/
/// <summary>
/// <para> Called each frame </para>
/// <para> フレームごっこに呼び出せる </para>
/// </summary>
void TRANSFORM_3D::Execute()
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

/*---------------------------------------TRANSFORM_3D Render()-------------------------------------------------*/
/// <summary>
/// <para>Called after Execute to perform any render functions </para>
/// <para>Execute実行後に描画用関数を呼び出す関数</para>
/// </summary>
void TRANSFORM_3D::Render()
{
}

/*---------------------------------------TRANSFORM_3D UI()-------------------------------------------------*/

/// <summary>
/// <para> Renders the UI for this component </para>
/// <para> UIを描画する関数 </para>
/// </summary>
void TRANSFORM_3D::UI()
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

/*---------------------------------------TRANSFORM_3D Scale()-------------------------------------------------*/
/// <summary>
/// Returns the scale 
/// </summary>
/// <returns></returns>
Vector3 TRANSFORM_3D::Scale()
{
    return data->scale;
}

/*---------------------------------------TRANSFORM_3D Rotation()-------------------------------------------------*/
/// <summary>
/// Returns the rotation
/// </summary>
/// <returns></returns>
Vector3 TRANSFORM_3D::Rotation()
{
    return data->rotation;
}

/*---------------------------------------TRANSFORM_3D Translation-------------------------------------------------*/
/// <summary>
/// Returns the translation
/// </summary>
/// <returns></returns>
Vector3 TRANSFORM_3D::Translation()
{
    return data->translation;
}

/*---------------------------------------TRANSFORM_3D Velocity()-------------------------------------------------*/
/// <summary>
/// Returns the velocity
/// </summary>
/// <returns></returns>
Vector3 TRANSFORM_3D::Velocity()
{
    return velocity;
}

/*---------------------------------------TRANSFORM_3D Transform()-------------------------------------------------*/
/// <summary>
/// <para> Returns the transformation matrix in a XMFLOAT4X4 form </para>
/// <para> 変換行列をXMFLOAT4X4の形で戻る</para>
/// </summary>
/// <returns></returns>
XMFLOAT4X4 TRANSFORM_3D::Transform()
{
    return transform;
}

/*---------------------------------------TRANSFORM_3D TranformMatrix()-------------------------------------------------*/
/// <summary>
/// <para> Returns the transformation matrix in a XMMATRIX form </para>
/// <para> 変換行列をXMMATRIXの形で戻る</para>
/// </summary>
/// <returns></returns>
XMMATRIX TRANSFORM_3D::TransformMatrix()
{
    Vector4 q{};
    Vector3 r{ ToRadians(data->rotation) };
    q.Load(XMQuaternionRotationRollPitchYawFromVector(r.XMV()));
    return XMMatrixScalingFromVector(data->scale.XMV()) * XMMatrixRotationQuaternion(q.XMV()) * XMMatrixTranslationFromVector(data->translation.XMV());
}

/*---------------------------------------TRANSFORM_3D TranformMatrixQuaternion()-------------------------------------------------*/
/// <summary>
/// <para> Returns the transformation matrix in a XMMATRIX form </para>
/// <para> 変換行列をXMMATRIXの形で戻る</para>
/// </summary>
/// <returns></returns>
XMMATRIX TRANSFORM_3D::TransformMatrixQuaternion()
{
    return XMMatrixScalingFromVector(data->scale.XMV()) * XMMatrixRotationQuaternion(quaternion.XMV()) * XMMatrixTranslationFromVector(data->translation.XMV());
}



/*---------------------------------------TRANSFORM_3D Data()-------------------------------------------------*/
/// <summary>
/// <para> Returns the data class that stores all the data of this component </para>
/// <para> コンポネントをエキスパート用データを戻る</para>
/// </summary>
/// <returns></returns>
TRANSFORM_3D_DATA* TRANSFORM_3D::Data()
{
    return data;
}

/*---------------------------------------TRANSFORM_3D Right()-------------------------------------------------*/
/// <summary>
/// Returns the right vector of the object
/// </summary>
/// <returns></returns>
Vector3 TRANSFORM_3D::Right()
{
    XMMATRIX temp{ TransformMatrix() };
    Vector3 r{};
    r.Load(temp.r[0]);
    r.Normalize();
    return r;
}

/*---------------------------------------TRANSFORM_3D Forward()-------------------------------------------------*/
/// <summary>
/// Returns the forward vector of the object
/// </summary>
/// <returns></returns>
Vector3 TRANSFORM_3D::Forward()
{
    XMMATRIX temp{ TransformMatrixQuaternion() };
    Vector3 r{};
    r.Load(temp.r[2]);
    r.Normalize();
    return r;
}

/*---------------------------------------TRANSFORM_3D Up()-------------------------------------------------*/
/// <summary>
/// Returns the up vector of the object
/// </summary>
/// <returns></returns>
Vector3 TRANSFORM_3D::Up()
{
    XMMATRIX temp{ TransformMatrix() };
    Vector3 r{};
    r.Load(temp.r[1]);
    r.Normalize();
    return r;

}

/*---------------------------------------TRANSFORM_3D Quaternion()-------------------------------------------------*/
/// <summary>
/// Returns the quaternion of rotation
/// </summary>
/// <returns></returns>
Vector4 TRANSFORM_3D::Quaternion()
{
    return quaternion;
}

/*---------------------------------------TRANSFORM_3D SlerpRotation()-------------------------------------------------*/
/// <summary>
/// <para> Slerps the rotation to the target vector. Target is in radian</para>
/// <para> Targetに対して球面線形補間を行う. VECTOR3はラジアン型</para>
/// </summary>
void TRANSFORM_3D::SlerpRotation(Vector3 target)
{
    Vector4 q1;
    q1.Load(XMQuaternionRotationRollPitchYawFromVector(target.XMV()));
    quaternion.Load(XMQuaternionSlerp(quaternion.XMV(), q1.XMV(), 0.1f));

}
/// <summary>
/// <para> Slerps the rotation to the target quaternion </para>
/// <para> Targetに対して球面線形補間を行う</para>
/// </summary>
void TRANSFORM_3D::SlerpRotation(Vector4 target)
{
    quaternion.Load(XMQuaternionSlerp(quaternion.XMV(), target.XMV(), 0.1f));
}

/*---------------------------------------TRANSFORM_3D SetScale()-------------------------------------------------*/

void TRANSFORM_3D::SetScale(Vector3 s)
{
    data->scale = s;
}

/*---------------------------------------TRANSFORM_3D SetRotation()-------------------------------------------------*/

void TRANSFORM_3D::SetRotation(Vector3 r)
{
    data->rotation = r;
}

/*---------------------------------------TRANSFORM_3D SetTranslation-------------------------------------------------*/

void TRANSFORM_3D::SetTranslation(Vector3 t)
{
    data->translation = t;
}

/*---------------------------------------TRANSFORM_3D OffsetTranslation()-------------------------------------------------*/

void TRANSFORM_3D::OffsetTranslation(Vector3 off)
{
    data->translation += off;
}

/*---------------------------------------TRANSFORM_3D SetVelocity()-------------------------------------------------*/

void TRANSFORM_3D::SetVelocity(Vector3 v)
{
    velocity = v;
}

/*----------------------------------------------------------TRANSFORM_3D GetComponentType()-----------------------------------------------------------*/

COMPONENT_TYPE TRANSFORM_3D::GetComponentType()
{
    return data->type;
}