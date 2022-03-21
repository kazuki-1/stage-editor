#include "COLLISION.h"
#include "../Components/Base Classes/COMPONENT_DATA.h"
#include "../Components/Headers/OBB_COLLIDER.h"
#include "../Components/Headers/SPHERE_COLLIDER.h"
#include "../Components/Headers/CAPSULE_COLLIDER.h"
using namespace COLLIDERS;

VECTOR3 COLLIDERS::PointLineClosest(VECTOR3 top, VECTOR3 bottom, VECTOR3 target)
{
    // Forming a line vector
    // 直線ベクター
    VECTOR3 line(top - bottom);
    line.Normalize();

    // Dot product to get the point
    // 座標点を計算
    VECTOR3 dist(target - bottom);
    float dot = dist.Dot(line);
    VECTOR3 point = bottom + line * dot;

    // Limiting point inside the line
    // 座標点を線内に制限
    float d1(VECTOR3(point - top).Dot(line)), d2(VECTOR3(point - bottom).Dot(line));
    if (d1 > 0)
        point = top;
    if (d2 < 0)
        point = bottom;
    return point;

}
VECTOR3 COLLIDERS::PointLineClosest(VECTOR3 origin, CAPSULE* target)
{
    // Forming a line vector
    // 直線ベクター
    VECTOR3 top, bottom;
    top = target->Top();
    bottom = target->Bottom();
    VECTOR3 line(top - bottom);
    //line = line.Normalize();
    line.Normalize();


    // Dot product to get the point
    // 座標点を計算
    VECTOR3 dist(origin - bottom);
    float dot = dist.Dot(line);
    VECTOR3 point = bottom + line * dot;
    float d1(VECTOR3(point - top).Dot(line)), d2(VECTOR3(point - bottom).Dot(line));

    // Limiting point inside the line
    // 座標点を線内に制限
    if (d1 > 0)
        point = top;
    if (d2 < 0)
        point = bottom;
    return point;

}
bool COLLIDERS::OBBCollision(OBB* ori, OBB* tar)
{
    if (!ori->Status() || !tar->Status())
        return false;

    VECTOR3 cb{ tar->Center() };
    VECTOR3 ca = ori->Center();
    if (Math::Length(ca, cb) > 1.0f)
    {
        return false;
    }

    VECTOR3 min1, max1, min2, max2;
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


    int count{};
    AxisCasting(min1, max1, min2, max2, ori->Rotation(), &count);
    AxisCasting(min1, max1, min2, max2, tar->Rotation(), &count);
    return count == 6;


}
void COLLIDERS::AxisCasting(VECTOR3 oriMin, VECTOR3 oriMax, VECTOR3 tarMin, VECTOR3 tarMax, VECTOR3 rotation, int* colCount)
{
    XMFLOAT3 temp{ rotation.x, rotation.y, rotation.z };
    XMMATRIX tempM{ XMMatrixIdentity() };
    tempM = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
    XMFLOAT3 r, t, f;
    XMStoreFloat3(&r, tempM.r[0]);
    XMStoreFloat3(&t, tempM.r[1]);
    XMStoreFloat3(&f, tempM.r[2]);

    VECTOR3 Right{ r }, Top{ t }, Front{ f };

    float ori_x1, ori_y1, ori_z1, ori_x2, ori_y2, ori_z2;
    float tar_x1, tar_y1, tar_z1, tar_x2, tar_y2, tar_z2;

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

    if (ori_x1 > tar_x2 || ori_x2 < tar_x1)
        return;
    if (ori_y1 > tar_y2 || ori_y2 < tar_y1)
        return;
    if (ori_z1 > tar_z2 || ori_z2 < tar_z1)
        return;
    *colCount += 3;




}
bool COLLIDERS::RayCast(VECTOR3& s, VECTOR3& e, MODEL* m, RAYCASTDATA& hr)
{
    
    XMVECTOR w_Start{ s.XMV() };                           // Ray World Start Position
    XMVECTOR w_End{ e.XMV() };                             // Ray World End Position
    XMVECTOR w_RayVector{ w_End - w_Start };                        // World Ray Vector 
    XMVECTOR w_RayLength = XMVector3Length(w_RayVector);            // World Ray Length
    XMStoreFloat(&hr.distance, w_RayLength);

    bool hit{};
    MODEL_RESOURCES::ANIMATION::KEYFRAME& kf = m->Resource()->Animations.at(m->CurrentTake()).Keyframes.at(m->CurrentFrame());
    for (auto& ms : m->Resource()->Meshes)
    {
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
                XMVECTOR A{ a.position.XMV() };
                XMVECTOR B{ b.position.XMV() };
                XMVECTOR C{ c.position.XMV() };


                //  Step 2: Vector of edges
                XMVECTOR AB{ B - A };
                XMVECTOR BC{ C - B };
                XMVECTOR CA{ A - C };


                //  Step 3: Normal retrieval
                XMVECTOR Normal{ XMVector3Cross(AB, BC) };


                //  Step 4: In front of or behind
                XMVECTOR Dot{ XMVector3Dot(Normal, Dir) };
                float f_Dot;
                XMStoreFloat(&f_Dot, Dot);
                if (f_Dot >= 0)
                    continue;
                //return false;
                // 
                // 
                // 
            //  Step 5: Point of intersection
                XMVECTOR distance{ A - S };
                XMVECTOR T = XMVector3Dot(Normal, distance) / Dot;


                float length = XMVectorGetX(T);
                if (length > min_Length || length < 0)
                    continue;


                XMVECTOR ContactPoint{ S + Dir * T };


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

                hit = true;
            }
        }
    }
    return hit;
}


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
XMMATRIX COLLIDER_BASE::MatrixOffset()
{
    return XMMatrixScaling(1, 1, 1) * XMMatrixRotationQuaternion(Math::Quaternion(rotation).XMV()) * XMMatrixTranslationFromVector(offset.XMV());
}

#pragma region SPHERE
SPHERE::SPHERE(VECTOR3 pos, float rad)
{
    center = pos;
    radius = rad;

}
//HRESULT SPHERE::Initialize(VECTOR3* pos, float rad)
//{
//    return S_OK;
//}
void SPHERE::Execute(XMMATRIX mat)
{
    center.Load(XMVector3TransformCoord({}, mat));
}
void SPHERE::Render()
{

}
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
bool SPHERE::Collide(VECTOR3 p)
{
    return (Center() - p).Length() < radius;
}
VECTOR3 SPHERE::Center()
{
    return center;
}
float SPHERE::Radius()
{
    return radius;
}
void SPHERE::SetCenter(VECTOR3 v)
{
    center = v;
}
void SPHERE::SetRadius(float rad)
{
    radius = rad;
}
void SPHERE::SetData(COMPONENT_DATA* data)
{
    SPHERE_COLLIDER_DATA* d{ CastData<SPHERE_COLLIDER_DATA>(data) };
    SetCenter(d->center);
    SetRadius(d->radius);
}

#pragma endregion
#pragma region OBB

OBB::OBB()
{
    *this = OBB({}, {});
}
OBB::OBB(VECTOR3 vMin, VECTOR3 vMax)
{
    oriMin = vMin;
    oriMax = vMax;


    VECTOR3& p = points.emplace_back();
    p = vMin;
    VECTOR3& p2 = points.emplace_back();
    p2 = { vMax.x, vMin.y, vMin.z };
    VECTOR3& p3 = points.emplace_back();
    p3 = { vMin.x, vMax.y, vMin.z };
    VECTOR3& p4 = points.emplace_back();
    p4 = { vMax.x, vMax.y, vMin.z };
    VECTOR3& p5 = points.emplace_back();
    p5 = { vMin.x, vMin.y, vMax.z };
    VECTOR3& p6 = points.emplace_back();
    p6 = { vMax.x, vMin.y, vMax.z };
    VECTOR3& p7 = points.emplace_back();
    p7 = { vMin.x, vMax.y, vMax.z };
    VECTOR3& p8 = points.emplace_back();
    p8 = vMax;
    Initialize();

}
HRESULT OBB::Initialize()
{
    return S_OK;
//    testCubes.resize(points.size());
//
//    for (auto& t : testCubes)
//    {
//        t = std::make_shared<MODEL>();
//        t->Initialize("Data/Model/Debug Primitives/cube.mrs");
//        t->SetTake(0);
//        t->SetScale({ 0.1f, 0.1f, 0.1f });
//    }
//
}
void OBB::UpdatePosition(XMMATRIX mat)
{
    std::vector<VECTOR3>& ps(points);

    ps[0] = oriMin;
    ps[1] = VECTOR3{ oriMax.x, oriMin.y, oriMin.z };
    ps[2] = VECTOR3{ oriMin.x, oriMax.y, oriMin.z };
    ps[3] = VECTOR3{ oriMax.x, oriMax.y, oriMin.z };
    ps[4] = VECTOR3{ oriMin.x, oriMin.y, oriMax.z };
    ps[5] = VECTOR3{ oriMax.x, oriMin.y, oriMax.z };
    ps[6] = VECTOR3{ oriMin.x, oriMax.y, oriMax.z };
    ps[7] = oriMax;


    //XMMATRIX temp{ PLAYERMANAGER::Instance()->Get()->Model()->TransformMatrix() };
    XMMATRIX world{ XMMatrixScaling(1, 1, 1) * XMMatrixTranslationFromVector(offset.XMV()) };
    for (auto& p : points)
    {
        XMVECTOR pss = XMVector3TransformCoord(p.XMV(), world);
        pss = XMVector3TransformCoord(pss, mat);
        p.Load(pss);
    }
    //XMVECTOR p1 = XMVector3TransformCoord(ps[1].XMV(), PLAYERMANAGER::Instance()->Get()->Model()->TransformMatrix());
    //ps[1].Load(p1);
    //((DEBUG_CUBIC*)(d_Primitive.get()))->UpdateVertices(points);
    //for (int a = 0; a < points.size(); ++a)
    //{
    //    testCubes[a]->SetTransformation({ 0.1f, 0.1f, 0.1f }, {}, { points[a].x, points[a].y, points[a].z });
    //    testCubes[a]->SetTake(0);
    //    testCubes[a]->UpdateTransform();
    //}
}
void OBB::Update(VECTOR3 pos, VECTOR3 rot)
{
    XMMATRIX T{ XMMatrixTranslationFromVector(pos.XMV()) };
    XMVECTOR Q{ XMQuaternionRotationRollPitchYawFromVector(rot.XMV()) };
    XMMATRIX R{ XMMatrixRotationQuaternion(Q) };
    XMMATRIX W{ R * T };
    UpdatePosition(W);

    //for (int a = 0; a < points.size(); ++a)
    //{
    //    testCubes[a]->SetTranslation(points[a].XMF3());
    //    testCubes[a]->UpdateTransform();
    //}
}
void OBB::Execute(XMMATRIX mat)
{
    UpdatePosition(mat);
    //for (int a = 0; a < points.size(); ++a)
    //{
    //    testCubes[a]->SetTransformation({ 0.1f, 0.1f, 0.1f }, { 0, 0 ,0 }, { points[a].x, points[a].y, points[a].z });
    //    testCubes[a]->SetTake(0);
    //    testCubes[a]->UpdateTransform();

    //}
    //for (int a = 0; a < points.size(); ++a)
    //{
    //}
}
void OBB::Render()
{

}
bool OBB::Collide(COLLIDER_BASE* other)
{
    return OBBCollision(this, static_cast<OBB*>(other));
}
bool OBB::Collide(VECTOR3 p)
{
    float size = (points[0] - points[8]).Length();
    return (Center() - p).Length() < size;
}
std::vector<VECTOR3>OBB::Points()
{
    return points;
}
VECTOR3 OBB::Rotation()
{
    return rotation;
}
VECTOR3 OBB::Center()
{
    VECTOR3 temp;
    temp = points[7] - points[0];
    temp *= .5;
    return *points.begin() + temp;
}
float OBB::Size()
{
    VECTOR3 center{ Center() };
    return (points[0] - center).Length();
}
bool OBB::Status()
{
    return isActive;
}
void OBB::SetMin(VECTOR3 min)
{
    oriMin = min;
}
void OBB::SetMax(VECTOR3 max)
{
    oriMax = max;
}
void OBB::SetData(COMPONENT_DATA* d)
{
    OBB_COLLIDER_DATA* od{ static_cast<OBB_COLLIDER_DATA*>(d) };
    SetMin(od->min);
    SetMax(od->max);
}

#pragma endregion
#pragma region CYLINDER
CYLINDER::CYLINDER()
{

}
CYLINDER::CYLINDER(VECTOR3 tp, VECTOR3 bot, float rad) : top(tp), bottom(bot), radius(rad)
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

    VECTOR3 ori_closest_point{ top }, tar_closest_point{ target->Top() };

    tar_closest_point = PointLineClosest(target->top, target->bottom, ori_closest_point);
    ori_closest_point = PointLineClosest(top, bottom, tar_closest_point);


    VECTOR3 v1, v2;
    v1 = top - bottom;
    v2 = bottom - top;
    if (v1.Dot(tar_closest_point) < 0 || v2.Dot(tar_closest_point) < 0)
        return false;
    return (tar_closest_point - ori_closest_point).Length() < radius + target->Radius();

}
void CYLINDER::SetData(VECTOR3 tp, VECTOR3 bot, float rad)
{
    top = tp;
    bottom = bot;
    radius = rad;
}

VECTOR3 CYLINDER::Top()
{
    return top;
}
VECTOR3 CYLINDER::Bottom()
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

CAPSULE::CAPSULE() : center(), radius()
{
    Initialize();
}
CAPSULE::CAPSULE(float rad) : center(), radius(rad)
{
    Initialize();
}
CAPSULE::CAPSULE(VECTOR3 cent, float rad, float ht) : center(cent), radius(rad), height(ht)
{
    Initialize();
}
HRESULT CAPSULE::Initialize()
{
    return S_OK;
}
void CAPSULE::Execute(XMMATRIX mat)
{
    world = MatrixOffset() * mat;
}
void CAPSULE::Render()
{
}
bool CAPSULE::Collide(COLLIDER_BASE* other)
{
    CAPSULE* target{ static_cast<CAPSULE*>(other) };
    VECTOR3 p0, p1;
    VECTOR3 top{ Top() }, bottom{ Bottom() };
    p0 = top;
    p1 = PointLineClosest(target->Top() , target->Bottom(), p0);
    p0 = PointLineClosest(top, bottom, p1);
    float minDist{ radius + target->radius };
    if (Length(p0, p1) < minDist)
        return true;
    return false;

}
bool CAPSULE::Collide(VECTOR3 p)
{
    VECTOR3 point = PointLineClosest(Top(), Bottom(), p);
    float distance{ (p - point).Length() };
    return (p - point).Length() < radius;

}
VECTOR3 CAPSULE::Top()
{
    VECTOR3 center_point{};
    center_point.Load(XMVector3TransformCoord(center.XMV(), world));
    VECTOR3 top = { center.x, center.y + height / 2, center.z };
    top.Load(XMVector3TransformCoord(top.XMV(), world));
    return top;
}
VECTOR3 CAPSULE::Bottom()
{
    VECTOR3 center_point{};
    center_point.Load(XMVector3TransformCoord(center.XMV(), world));

    VECTOR3 bottom = { center.x, center.y - height / 2, center.z };
    bottom.Load(XMVector3TransformCoord(bottom.XMV(), world));
    return bottom;

}
VECTOR3 CAPSULE::Center()
{
    return center;
}
float CAPSULE::Radius()
{
    return radius;
}
float CAPSULE::Size()
{
    return radius;
}

void CAPSULE::OffsetCenter(XMMATRIX world)
{
}
void CAPSULE::SetCenter(VECTOR3 c)
{
    center = c;
}
void CAPSULE::SetRadius(float r)
{
    radius = r;
}
void CAPSULE::SetHeight(float h)
{
    height = h;
}
void CAPSULE::SetData(COMPONENT_DATA* d)
{
    CAPSULE_COLLIDER_DATA* cd = (static_cast<CAPSULE_COLLIDER_DATA*>(d));
    SetRadius(cd->radius);
}


#pragma endregion