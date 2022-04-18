#include "COLLISION.h"
#include "../Components/Base Classes/ComponentData.h"
#include "../Components/OBBCollider.h"
#include "../Components/SphereCollider.h"
#include "../Components/CapsuleCollider.h"
#include "../Components/Mesh.h"
#include "../Components/Transform3D.h"
using namespace COLLIDERS;

/*---------------------------------------------------PointLineClosest()---------------------------------------------------*/

/// <summary>
/// <para> Calculates a point on a line that is closest to the target point </para>
/// <para> 目標点に一番近い点を計算 </para>
/// </summary>
/// <param name="top"> : Starting point of vector</param>
/// <param name="bot"> : Ending point of vector</param>
/// <returns></returns>
Vector3 COLLIDERS::PointLineClosest(Vector3 top, Vector3 bottom, Vector3 target)
{
    // Forming a line vector
    // 直線ベクター
    Vector3 line(top - bottom);
    line.Normalize();

    // Dot product to get the point
    // 座標点を計算
    Vector3 dist(target - bottom);
    float dot = dist.Dot(line);
    Vector3 point = bottom + line * dot;

    // Limiting point inside the line
    // 座標点を線内に制限
    float d1(Vector3(point - top).Dot(line)), d2(Vector3(point - bottom).Dot(line));
    if (d1 > 0)
        point = top;
    if (d2 < 0)
        point = bottom;
    return point;

}
/// <summary>
/// <para> Calculates a point on a line that is closest to the target point </para>
/// <para> 目標点に一番近い点を計算 </para>
/// </summary>
/// <returns></returns>
Vector3 COLLIDERS::PointLineClosest(Vector3 origin, CAPSULE* target)
{
    // Forming a line vector
    // 直線ベクター
    Vector3 top, bottom;
    top = target->Top();
    bottom = target->Bottom();
    Vector3 line(top - bottom);
    //line = line.Normalize();
    line.Normalize();


    // Dot product to get the point
    // 座標点を計算
    Vector3 dist(origin - bottom);
    float dot = dist.Dot(line);
    Vector3 point = bottom + line * dot;
    float d1(Vector3(point - top).Dot(line)), d2(Vector3(point - bottom).Dot(line));

    // Limiting point inside the line
    // 座標点を線内に制限
    if (d1 > 0)
        point = top;
    if (d2 < 0)
        point = bottom;
    return point;

}


/*---------------------------------------------------AxisCasting()---------------------------------------------------*/
/// <summary>
/// <para> Performs axis casting, where each point is casted onto the axis, and is compared </para>
/// <para> 各点を軸にキャストして比較される </para>
/// </summary>
/// <param name="oriMin"> ： Minimum point of first collider</param>
/// <param name="oriMax"> ： Maximum point of first collider</param>
/// <param name="tarMin"> ： Minimum point of second collider</param>
/// <param name="tarMax"> ： Maximum point of second collider</param>
/// <param name="rotation"> : Rotation of first collider</param>
/// <param name="colCount"> : Output. Shows how many time it is a hit</param>
void COLLIDERS::AxisCasting(Vector3 oriMin, Vector3 oriMax, Vector3 tarMin, Vector3 tarMax, Vector3 rotation, int* colCount)
{
    // Extracing the axises from the rotation matrix 
    // 回転MatrixからXYZ軸を抽出
    XMFLOAT3 temp{ rotation.x, rotation.y, rotation.z };
    XMMATRIX tempM{ XMMatrixIdentity() };
    tempM = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    XMFLOAT3 r, t, f;
    XMStoreFloat3(&r, tempM.r[0]);
    XMStoreFloat3(&t, tempM.r[1]);
    XMStoreFloat3(&f, tempM.r[2]);
    Vector3 Right{ r }, Top{ t }, Front{ f };

    float ori_x1, ori_y1, ori_z1, ori_x2, ori_y2, ori_z2;
    float tar_x1, tar_y1, tar_z1, tar_x2, tar_y2, tar_z2;

    // Perform dot on each points to check if they are within range
    // 各座標を内積して範囲内チェック
    ori_x1 = Right.Dot(oriMin);
    ori_x2 = Right.Dot(oriMax);
    ori_y1 = Top.Dot(oriMin);
    ori_y2 = Top.Dot(oriMax);
    ori_z1 = Front.Dot(oriMin);
    ori_z2 = Front.Dot(oriMax);

    tar_x1 = Right.Dot(tarMin);
    tar_x2 = Right.Dot(tarMax);
    tar_y1 = Top.Dot(tarMin);
    tar_y2 = Top.Dot(tarMax);
    tar_z1 = Front.Dot(tarMin);
    tar_z2 = Front.Dot(tarMax);


    // Perform Comparison on each axis
    if (ori_x1 > tar_x2 || ori_x2 < tar_x1)
        return;
    if (ori_y1 > tar_y2 || ori_y2 < tar_y1)
        return;
    if (ori_z1 > tar_z2 || ori_z2 < tar_z1)
        return;
    *colCount += 3;




}

/*---------------------------------------------------OBBCollision()---------------------------------------------------*/
/// <summary>
/// <para> Performs collision check between 2 OBBs </para>
/// <para> 2つのOBBに当たり判定を計算 </para>
/// </summary>
/// <returns></returns>
bool COLLIDERS::OBBCollision(OBB* ori, OBB* tar)
{
    // Check if colliders are activated
    // コライダーのステータスチェック
    if (!ori->Status() || !tar->Status())
        return false;

    // Check both OBB for their minimum and maximum points
    // 両方のOBBの最小と最大点をチェック
    Vector3 min1, max1, min2, max2;
    for (auto& v : ori->Points())
    {
        if (v == ori->Points().at(0))
        {
            min1 = v;
            max1 = v;
        }

        if (min1.x > v.x)
            min1.x = v.x;
        if (min1.y > v.y)
            min1.y = v.y;
        if (min1.z > v.z)
            min1.z = v.z;
        if (max1.x < v.x)
            max1.x = v.x;
        if (max1.y < v.y)
            max1.y = v.y;
        if (max1.z < v.z)
            max1.z = v.z;
    }
    for (auto& v : tar->Points())
    {
        if (v == tar->Points().at(0))
        {
            min2 = v;
            max2 = v;
        }
        if (min2.x > v.x)
            min2.x = v.x;
        if (min2.y > v.y)
            min2.y = v.y;
        if (min2.z > v.z)
            min2.z = v.z;
        if (max2.x < v.x)
            max2.x = v.x;
        if (max2.y < v.y)
            max2.y = v.y;
        if (max2.z < v.z)
            max2.z = v.z;
    }

    // Perform axis casting to see if all points are within range
    // Axis Castを使ってすべての座標は範囲内チェック
    int count{};
    AxisCasting(min1, max1, min2, max2, ori->Rotation(), &count);
    AxisCasting(min1, max1, min2, max2, tar->Rotation(), &count);
    return count == 6;


}

/*---------------------------------------------------RayCast()---------------------------------------------------*/
/// <summary>
/// <para> Perform Raycasting </para>
/// <para> レイーキャストを計算 </para>
/// </summary>
/// <param name="s"> : Starting point of ray</param>
/// <param name="e"> : Direction of ray</param>
/// <param name="m"> : Target model</param>
/// <param name="hr"> : Output. RayCastData is stored here. Create a new and put it here</param>
/// <returns></returns>
bool COLLIDERS::RayCast(Vector3& s, Vector3& e, MODEL* m, RAYCASTDATA& hr, int mesh_index)
{

    XMVECTOR w_Start{ s.XMV() };                           // Ray World Start Position
    XMVECTOR w_End{ e.XMV() };                             // Ray World End Position
    XMVECTOR w_RayVector{ w_End - w_Start };                        // World Ray Vector 
    XMVECTOR w_RayLength = XMVector3Length(w_RayVector);            // World Ray Length
    XMStoreFloat(&hr.distance, w_RayLength);

    // Retrieve current keyframe 
    // 現在のキーフレームを抽出
    bool hit{};
    MODEL_RESOURCES::ANIMATION::KEYFRAME& kf = m->Resource()->Animations.at(m->CurrentTake()).Keyframes.at(m->CurrentFrame());
    int cur_index{};
    for (auto& ms : m->Resource()->Meshes)
    {
        bool onTarget{};
        if (mesh_index != -1)
        {
            if (mesh_index != cur_index)
            {
                ++cur_index;
                continue;
            }
            else
                onTarget = true;
        }

        if (!onTarget && mesh_index != -1)
            continue;

        // Retrieve the current mesh node and transform matrix to Local Transformation
        // げんざいMESHNODEを抽出し、ローカル変換行列に変換
        MODEL_RESOURCES::ANIMATION::KEYFRAME::NODE& n = m->Resource()->Animations.at(m->CurrentTake()).Keyframes.at(m->CurrentFrame()).Nodes.at(ms.n_Index);

        XMMATRIX w_Transform{ XMLoadFloat4x4(&n.g_Transform) };
        w_Transform *= m->TransformMatrix();
        XMMATRIX inv_w_Transform{ XMMatrixInverse(nullptr, w_Transform) };

        XMVECTOR S{ XMVector3TransformCoord(w_Start, inv_w_Transform) };
        XMVECTOR E{ XMVector3TransformCoord(w_End, inv_w_Transform) };

        XMVECTOR V{ E - S };
        XMVECTOR Dir{ XMVector3Normalize(V) };
        XMVECTOR L{ XMVector3Length(V) };
        float min_Length{};
        XMStoreFloat(&min_Length, L);


        std::vector<MODEL_RESOURCES::VERTEX>& v{ ms.Vertices };
        const std::vector<int>i{ ms.Indices };

        int m_Index{ -1 };
        XMVECTOR h_Pos, h_Norm;
        for (auto& sub : ms.Subsets)
        {
            for (int in = 0; in < sub.indices.size(); in += 3)
            {
                //UINT index{ sub.first_index + in };
                // if (index > sub.indexCount)
                //     continue;

                MODEL_RESOURCES::VERTEX& a{ v.at(sub.indices[in]) };
                MODEL_RESOURCES::VERTEX& b{ v.at(sub.indices[in + 1]) };
                MODEL_RESOURCES::VERTEX& c{ v.at(sub.indices[in + 2]) };


                //  Step 1: Triangle Vertex Retrieval
                // 三角の生成
                XMVECTOR A{ a.position.XMV() };
                XMVECTOR B{ b.position.XMV() };
                XMVECTOR C{ c.position.XMV() };

                // Skip if not near
                Vector3 point{};
                point.Load(A);
                //if ((point - s).Length() > 10.0f)
                //    continue;

                //  Step 2: Vector of edges
                // 三角のベクタ
                XMVECTOR AB{ B - A };
                XMVECTOR BC{ C - B };
                XMVECTOR CA{ A - C };


                //  Step 3: Normal retrieval
                // 法線抽出
                XMVECTOR Normal{ XMVector3Cross(AB, BC) };


                //  Step 4: In front of or behind
                // ターゲットは三角以外か以内
                XMVECTOR Dot{ XMVector3Dot(Normal, Dir) };
                float f_Dot;
                XMStoreFloat(&f_Dot, Dot);
                if (f_Dot >= 0)
                    continue;
                // Step 5: Point of intersection
                // 交差点
                XMVECTOR distance{ A - S };
                XMVECTOR T = XMVector3Dot(Normal, distance) / Dot;
                float length = XMVectorGetX(T);
                if (length > min_Length || length < 0)
                    continue;
                XMVECTOR ContactPoint{ S + Dir * T };

                // Perform dot check on each point on the triangle 
                // 各点に内積チェック
                XMVECTOR PA{ A - ContactPoint };
                XMVECTOR PB{ B - ContactPoint };
                XMVECTOR PC{ C - ContactPoint };


                XMVECTOR C_PAB{ XMVector3Cross(PA, AB) };
                XMVECTOR C_PAC{ XMVector3Cross(PB, BC) };
                XMVECTOR C_PBC{ XMVector3Cross(PC, CA) };

                XMVECTOR PAB_DOT{ XMVector3Dot(Normal, C_PAB) };
                XMVECTOR PAC_DOT{ XMVector3Dot(Normal, C_PAC) };
                XMVECTOR PBC_DOT{ XMVector3Dot(Normal, C_PBC) };


                float f_pabdot{ XMVectorGetX(PAB_DOT) }, f_pacdot{ XMVectorGetX(PAC_DOT) }, f_pbcdot{ XMVectorGetX(PBC_DOT) };
                if (f_pabdot < 0 || f_pacdot < 0 || f_pbcdot < 0)
                    continue;
                h_Pos = ContactPoint;
                h_Norm = Normal;
                m_Index = (int)sub.m_UID;
            }
        }
        if (m_Index >= 0)
        {
            // RAYCASTDATA storing
            // RAYCASTDATA 保存

            XMVECTOR w_Position{ XMVector3TransformCoord(h_Pos, w_Transform) };
            XMVECTOR w_CrossVector{ w_Position - w_Start };
            XMVECTOR w_CrossLength{ XMVector3Length(w_CrossVector) };
            float dist;
            XMStoreFloat(&dist, w_CrossLength);
            if (hr.distance > dist)
            {
                XMVECTOR w_Normal = XMVector3TransformCoord(h_Norm, w_Transform);
                hr.distance = dist;
                hr.m_Index = m_Index;
                // XMStoreFloat3(&hr.position, w_Position);
                // XMStoreFloat3(&hr.normal, XMVector3Normalize(w_Normal));
                hr.position.Load(w_Position);
                hr.normal.Load(w_Normal);
                hr.normal.Normalize();
                hit = true;
            }
        }
    }
    return hit;
}


/*--------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------COLLIDER_BASE Class--------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------COLLIDER_BASE FitToBone()--------------------------------------------------------------*/
/// <summary>
/// <para> virtual Function. Called to fit the collider to the bone of the model</para>
/// <para> 仮想関数。モデルにしてされたボーンにコライダーをセット</para>
/// </summary>
/// <param name="name"> : Name of bone</param>
/// <param name="m"> : Pointer of model</param>
void COLLIDER_BASE::FitToBone(std::string bone_name, MODEL* m)
{
    int64_t index{ -1 };
    XMMATRIX bone, global;
    for (auto& m : m->Resource()->Meshes)
    {
        for (auto& b : m.Bind_Pose.Bones)
        {
            if (b.Name == bone_name)
            {
                index = b.n_Index;
                break;
            }
        }
    }
    if (index != -1)
    {
        int kf{ m->NextFrame() };
        MODEL_RESOURCES::ANIMATION& an{ m->Resource()->Animations[m->CurrentTake()] };

        int size{ (int)an.Keyframes.size() - 1 };
        int take{ m->CurrentFrame() };
        kf = (std::min)((std::max)(kf, 0), (int)an.Keyframes.size() - 1);
        XMFLOAT4X4 temp = m->Resource()->Animations[m->CurrentTake()].Keyframes[kf].Nodes.at(index).g_Transform;
        bone = XMLoadFloat4x4(&temp);
    }
    global = m->TransformMatrix();
    XMFLOAT4X4 temp{ m->Resource()->Axises.AxisCoords };

    bone *= XMLoadFloat4x4(&temp);
    bone *= MatrixOffset() * global;
    bone_World = bone;
    Execute(bone);

}

/*-----------------------------------------------------COLLIDER_BASE MatrixOffset()--------------------------------------------------------------*/


XMMATRIX COLLIDER_BASE::MatrixOffset()
{
    return XMMatrixScaling(1, 1, 1) * XMMatrixRotationQuaternion(Math::Quaternion(rotation).XMV()) * XMMatrixTranslationFromVector(offset.XMV());
}

/*--------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------SPHERE Class--------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------SPHERE Constructor--------------------------------------------------------------*/

#pragma region SPHERE
SPHERE::SPHERE(Vector3 pos, float rad)
{
    center = pos;
    radius = rad;

}

/*-----------------------------------------------------SPHERE Execute()--------------------------------------------------------------*/

void SPHERE::Execute(XMMATRIX mat)
{
    center.Load(XMVector3TransformCoord({}, mat));
}

/*-----------------------------------------------------SPHERE Render()--------------------------------------------------------------*/

void SPHERE::Render()
{

}

/*-----------------------------------------------------SPHERE Collide()--------------------------------------------------------------*/

bool SPHERE::Collide(COLLIDER_BASE* other)
{
    SPHERE* target{ dynamic_cast<SPHERE*>(other) };

    if (target)
    {
        float min_dist{ radius + target->radius };
        float distance{ (center - target->Center()).Length() };
        if (distance < min_dist)
            return true;
    }
    return false;
}
bool SPHERE::Collide(Vector3 p)
{
    return (Center() - p).Length() < radius;
}

/*-----------------------------------------------------SPHERE Center()--------------------------------------------------------------*/

Vector3 SPHERE::Center()
{
    return center;
}

/*-----------------------------------------------------SPHERE Radius()--------------------------------------------------------------*/

float SPHERE::Radius()
{
    return radius;
}

/*-----------------------------------------------------SPHERE SetCenter()--------------------------------------------------------------*/

void SPHERE::SetCenter(Vector3 v)
{
    center = v;
}

/*-----------------------------------------------------SPHERE SetRadius()--------------------------------------------------------------*/

void SPHERE::SetRadius(float rad)
{
    radius = rad;
}

/*-----------------------------------------------------SPHERE SetData()--------------------------------------------------------------*/

void SPHERE::SetData(ComponentData* data)
{
    SphereCollider_Data* d{ CastData<SphereCollider_Data>(data) };
    SetCenter(d->center);
    SetRadius(d->radius);
}

#pragma endregion
#pragma region OBB
/*--------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------OBB Class--------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------OBB Constructor--------------------------------------------------------------*/

OBB::OBB()
{
    *this = OBB({}, {});
}
OBB::OBB(Vector3 vMin, Vector3 vMax)
{
    oriMin = vMin;
    oriMax = vMax;


    Vector3& p = points.emplace_back();
    p = vMin;
    Vector3& p2 = points.emplace_back();
    p2 = { vMax.x, vMin.y, vMin.z };
    Vector3& p3 = points.emplace_back();
    p3 = { vMin.x, vMax.y, vMin.z };
    Vector3& p4 = points.emplace_back();
    p4 = { vMax.x, vMax.y, vMin.z };
    Vector3& p5 = points.emplace_back();
    p5 = { vMin.x, vMin.y, vMax.z };
    Vector3& p6 = points.emplace_back();
    p6 = { vMax.x, vMin.y, vMax.z };
    Vector3& p7 = points.emplace_back();
    p7 = { vMin.x, vMax.y, vMax.z };
    Vector3& p8 = points.emplace_back();
    p8 = vMax;
    Initialize();

}

/*-----------------------------------------------------OBB Initialize()--------------------------------------------------------------*/\

HRESULT OBB::Initialize()
{
    return S_OK;
}

/*-----------------------------------------------------OBB UpdatePosition()--------------------------------------------------------------*/\

void OBB::UpdatePosition(XMMATRIX mat)
{
    std::vector<Vector3>& ps(points);

    ps[0] = oriMin;
    ps[1] = Vector3{ oriMax.x, oriMin.y, oriMin.z };
    ps[2] = Vector3{ oriMin.x, oriMax.y, oriMin.z };
    ps[3] = Vector3{ oriMax.x, oriMax.y, oriMin.z };
    ps[4] = Vector3{ oriMin.x, oriMin.y, oriMax.z };
    ps[5] = Vector3{ oriMax.x, oriMin.y, oriMax.z };
    ps[6] = Vector3{ oriMin.x, oriMax.y, oriMax.z };
    ps[7] = oriMax;

    // Changing points to global transfrom
    XMMATRIX world{ XMMatrixScaling(1, 1, 1) * XMMatrixTranslationFromVector(offset.XMV()) };
    for (auto& p : points)
    {
        XMVECTOR pss = XMVector3TransformCoord(p.XMV(), world);
        pss = XMVector3TransformCoord(pss, mat);
        p.Load(pss);
    }
}

/*-----------------------------------------------------OBB Update()--------------------------------------------------------------*/\

void OBB::Update(Vector3 pos, Vector3 rot)
{
    XMMATRIX T{ XMMatrixTranslationFromVector(pos.XMV()) };
    XMVECTOR Q{ XMQuaternionRotationRollPitchYawFromVector(rot.XMV()) };
    XMMATRIX R{ XMMatrixRotationQuaternion(Q) };
    XMMATRIX W{ R * T };
    UpdatePosition(W);

}

/*-----------------------------------------------------OBB Execute()--------------------------------------------------------------*/\

void OBB::Execute(XMMATRIX mat)
{
    UpdatePosition(mat);
}

/*-----------------------------------------------------OBB Render()--------------------------------------------------------------*/\

void OBB::Render()
{

}

/*-----------------------------------------------------OBB Collide()--------------------------------------------------------------*/\

bool OBB::Collide(COLLIDER_BASE* other)
{
    return OBBCollision(this, static_cast<OBB*>(other));
}
bool OBB::Collide(Vector3 p)
{
    float size = (points[0] - points[8]).Length();
    return (Center() - p).Length() < size;
}

/*-----------------------------------------------------OBB Points()--------------------------------------------------------------*/\

std::vector<Vector3>OBB::Points()
{
    return points;
}

/*-----------------------------------------------------OBB Rotation()--------------------------------------------------------------*/\

Vector3 OBB::Rotation()
{
    return rotation;
}

/*-----------------------------------------------------OBB Center()--------------------------------------------------------------*/\

Vector3 OBB::Center()
{
    Vector3 temp;
    temp = points[7] - points[0];
    temp *= .5;
    return *points.begin() + temp;
}

/*-----------------------------------------------------OBB Size()--------------------------------------------------------------*/\

float OBB::Size()
{
    Vector3 center{ Center() };
    return (points[0] - center).Length();
}

/*-----------------------------------------------------OBB Status()--------------------------------------------------------------*/\

bool OBB::Status()
{
    return isActive;
}

/*-----------------------------------------------------OBB SetMin()--------------------------------------------------------------*/\

void OBB::SetMin(Vector3 min)
{
    oriMin = min;
}

/*-----------------------------------------------------OBB SetMax()--------------------------------------------------------------*/\

void OBB::SetMax(Vector3 max)
{
    oriMax = max;
}

/*-----------------------------------------------------OBB SetData()--------------------------------------------------------------*/\

void OBB::SetData(ComponentData* d)
{
    OBBCollider_Data* od{ static_cast<OBBCollider_Data*>(d) };
    SetMin(od->min);
    SetMax(od->max);
}

#pragma endregion
#pragma region CYLINDER_UNUSED


CYLINDER::CYLINDER()
{

}
CYLINDER::CYLINDER(Vector3 tp, Vector3 bot, float rad) : top(tp), bottom(bot), radius(rad)
{

}
HRESULT CYLINDER::Initialize()
{
    height = (top - bottom).Length();
    return S_OK;
}
void CYLINDER::Execute(XMMATRIX mat)
{
    top.Load(XMVector3TransformCoord(top.XMV(), mat));
    bottom.Load(XMVector3TransformCoord(bottom.XMV(), mat));
}
void CYLINDER::Render()
{

}
bool CYLINDER::Collide(COLLIDER_BASE* other)
{
    CYLINDER* target = (CYLINDER*)other;

    Vector3 ori_closest_point{ top }, tar_closest_point{ target->Top() };

    tar_closest_point = PointLineClosest(target->top, target->bottom, ori_closest_point);
    ori_closest_point = PointLineClosest(top, bottom, tar_closest_point);


    Vector3 v1, v2;
    v1 = top - bottom;
    v2 = bottom - top;
    if (v1.Dot(tar_closest_point) < 0 || v2.Dot(tar_closest_point) < 0)
        return false;
    return (tar_closest_point - ori_closest_point).Length() < radius + target->Radius();

}
void CYLINDER::SetData(Vector3 tp, Vector3 bot, float rad)
{
    top = tp;
    bottom = bot;
    radius = rad;
}

Vector3 CYLINDER::Top()
{
    return top;
}
Vector3 CYLINDER::Bottom()
{
    return bottom;
}
float CYLINDER::Height()
{
    return height;
}
float CYLINDER::Radius()
{
    return radius;
}


#pragma endregion
#pragma region CAPSULE

/*--------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------CAPSULE Class--------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------CAPSULE Constructor--------------------------------------------------------------*/

CAPSULE::CAPSULE() : center(), radius()
{
    Initialize();
}
CAPSULE::CAPSULE(float rad) : center(), radius(rad)
{
    Initialize();
}
CAPSULE::CAPSULE(Vector3 cent, float rad, float ht) : center(cent), radius(rad), height(ht)
{
    Initialize();
}

/*-----------------------------------------------------CAPSULE Initialize()--------------------------------------------------------------*/

HRESULT CAPSULE::Initialize()
{
    return S_OK;
}

/*-----------------------------------------------------CAPSULE Execute()--------------------------------------------------------------*/

void CAPSULE::Execute(XMMATRIX mat)
{
    world = MatrixOffset() * mat;
}

/*-----------------------------------------------------CAPSULE Render()--------------------------------------------------------------*/

void CAPSULE::Render()
{
}

/*-----------------------------------------------------CAPSULE Collide()--------------------------------------------------------------*/

bool CAPSULE::Collide(COLLIDER_BASE* other)
{
    CAPSULE* target{ static_cast<CAPSULE*>(other) };

    // Check for point closest to each other 
    //一番近い点を検索
    Vector3 p0, p1;
    Vector3 top{ Top() }, bottom{ Bottom() };
    p0 = top;
    p1 = PointLineClosest(target->Top(), target->Bottom(), p0);
    p0 = PointLineClosest(top, bottom, p1);


    float minDist{ radius + target->radius };
    if (Length(p0, p1) < minDist)
        return true;
    return false;

}
bool CAPSULE::Collide(Vector3 p)
{
    Vector3 point = PointLineClosest(Top(), Bottom(), p);
    float distance{ (p - point).Length() };
    return (p - point).Length() < radius;

}

/*-----------------------------------------------------CAPSULE Top()--------------------------------------------------------------*/

Vector3 CAPSULE::Top()
{
    Vector3 center_point{};
    center_point.Load(XMVector3TransformCoord(center.XMV(), world));
    Vector3 top = { center.x, center.y + height / 2, center.z };
    top.Load(XMVector3TransformCoord(top.XMV(), world));
    return top;
}

/*-----------------------------------------------------CAPSULE Bottom()--------------------------------------------------------------*/

Vector3 CAPSULE::Bottom()
{
    Vector3 center_point{};
    center_point.Load(XMVector3TransformCoord(center.XMV(), world));

    Vector3 bottom = { center.x, center.y - height / 2, center.z };
    bottom.Load(XMVector3TransformCoord(bottom.XMV(), world));
    return bottom;

}

/*-----------------------------------------------------CAPSULE Center()--------------------------------------------------------------*/

Vector3 CAPSULE::Center()
{
    return center;
}

/*-----------------------------------------------------CAPSULE Radius()--------------------------------------------------------------*/

float CAPSULE::Radius()
{
    return radius;
}

/*-----------------------------------------------------CAPSULE Size()--------------------------------------------------------------*/

float CAPSULE::Size()
{
    return radius;
}

/*-----------------------------------------------------CAPSULE OffsetCenter()--------------------------------------------------------------*/

void CAPSULE::OffsetCenter(XMMATRIX world)
{
}

/*-----------------------------------------------------CAPSULE Center()--------------------------------------------------------------*/

void CAPSULE::SetCenter(Vector3 c)
{
    center = c;
}

/*-----------------------------------------------------CAPSULE SetRadius()--------------------------------------------------------------*/

void CAPSULE::SetRadius(float r)
{
    radius = r;
}

/*-----------------------------------------------------CAPSULE SetHeight()--------------------------------------------------------------*/

void CAPSULE::SetHeight(float h)
{
    height = h;
}

/*-----------------------------------------------------CAPSULE SetData()--------------------------------------------------------------*/

void CAPSULE::SetData(ComponentData* d)
{
    CapsuleCollider_Data* cd = (static_cast<CapsuleCollider_Data*>(d));
    SetRadius(cd->radius);
}


#pragma endregion
#pragma region RAYCAST_MANAGER

/*--------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------RAYCAST_MANAGER Class--------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------RAYCAST_MANAGER Insert()--------------------------------------------------------------*/
/// <summary>
/// <para> Inserts the model into the map and allow it to perform collision check </para>
/// <para> モデルをマップに登録し、MESH COLLIDERの対象内になる</para>
/// </summary>
/// <param name="name"> : Name of model</param>
/// <param name="m"> : Model pointer</param>
void RAYCAST_MANAGER::Insert(Mesh_Component* m)
{
    meshes.push_back(m);
}

/*-----------------------------------------------------RAYCAST_MANAGER Finalize()--------------------------------------------------------------*/
/// <summary>
/// Called at the end of the program or when switching scenes
/// </summary>
void RAYCAST_MANAGER::Finalize()
{
    meshes.clear();
}

/*-----------------------------------------------------RAYCAST_MANAGER Collide()--------------------------------------------------------------*/
/// <summary>
/// <para> Perform ray casting collision check </para>
/// <para> レイーキャストを利用して当たり判定を計算 </para>
/// </summary>
/// <param name="name"> : Name of current model. Collision check will not be performed onto this model</param>
/// <param name="startOfRay"> : Starting point of object</param>
/// <param name="direction_vector"> : Direction of movement</param>
/// <param name="rcd"> : Output. RayCastData is stored here. Create a new and put it here</param>
/// <returns></returns>
bool RAYCAST_MANAGER::Collide(Vector3 startOfRay, Vector3 endOfRay, Mesh_Component* cur_mesh, RAYCASTDATA& rcd)
{
    bool output{};
    for (auto& m : meshes)
    {
        Vector3 target{ m->Parent()->GetComponent<Transform3D_Component>()->Translation() };
        //if ((target - startOfRay).Length() > 0.3f)
        //    continue;
        if (m == cur_mesh)
            continue;
        output = RayCast(startOfRay, endOfRay, m->Model().get(), rcd);
        if (output)
            break;
    }
    return output;
}

/*-----------------------------------------------------RAYCAST_MANAGER Collide()--------------------------------------------------------------*/

bool RAYCAST_MANAGER::Collide(Vector3 startofRay, Vector3 endOfRay, Mesh_Component* target_mesh, int target_mesh_index, RAYCASTDATA& rcd)
{
    bool output{};
    for (auto& m : meshes)
    {
        if (m != target_mesh)
            continue;
        output = RayCast(startofRay, endOfRay, m->Model().get(), rcd);
        if (output)
            break;
    }
    return output;
}


/*-----------------------------------------------------RAYCAST_MANAGER Collide()--------------------------------------------------------------*/

void RAYCAST_MANAGER::GetListOfCollided(Mesh_Component* cur_Mesh, Vector3 startOfRay, Vector3 directionVector, std::vector<RAYCASTDATA>& rcd)
{
    for (auto& m : meshes)
    {
        RAYCASTDATA& cur_rcd{ rcd.emplace_back() };
        if (RayCast(startOfRay, directionVector, m->Model().get(), cur_rcd))
        {
            cur_rcd.model_name = m->Parent()->Data()->Name();
        }
    }
}

/*-----------------------------------------------------RAYCAST_MANAGER ModelMap()--------------------------------------------------------------*/

std::vector<Mesh_Component*>RAYCAST_MANAGER::Meshes()
{
    return meshes;
}
#pragma endregion