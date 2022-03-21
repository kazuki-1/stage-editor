#pragma once
#include <DirectXMath.h>
#include <vector>
#include "MODEL.h"
#include "DXMath.h"
#include "CerealHeaders.h"
using namespace Math;

using namespace DirectX;
//using namespace Math;

class COMPONENT_DATA;
class OBB_COLLIDER_DATA;
class CAPSULE_COLLIDER_DATA;

namespace COLLIDERS
{

#pragma region BASE CLASSES

    struct RAYCASTDATA
    {
        VECTOR3 position{ 0, 0, 0 };
        VECTOR3 normal{ 0, 0, 0 };
        float distance;
        int m_Index{ -1 };
    };
    struct OBB_DIRECTION
    {
        bool horizontol;
        bool vertical;
    };

    class COLLIDER_BASE
    {
    protected:
        bool isActive{ true };
        VECTOR3 offset;
        VECTOR3 rotation;
        XMMATRIX world;
        XMMATRIX bone_World{ XMMatrixIdentity() };
    public:
        COLLIDER_BASE() {};
        virtual HRESULT Initialize() { return S_OK; }
        virtual void Execute(XMMATRIX mat) {}
        virtual void Render() {}
        virtual bool Collide(COLLIDER_BASE* o) { return false; }
        virtual bool Collide(VECTOR3 p) { return false; }
        virtual void FitToBone(std::string name, MODEL* m);
        virtual void SetData(COMPONENT_DATA* data) {}
        /// <summary>
        /// Offsets the collider 
        /// </summary>
        /// <param name="off"> : User declared offset</param>
        virtual void OffsetCollider(VECTOR3 off) { offset = off; }
        virtual void RotateCollider(VECTOR3 rot) { rotation = rot; }

        /// <summary>
        /// Activates the collider
        /// </summary>
        virtual void Activate() { isActive = true; }
        /// <summary>
        /// Deactivates the collider
        /// </summary>
        virtual void Deactivate() { isActive = false; }
        /// <summary>
        /// Returns true if collider is active
        /// </summary>
        /// <returns></returns>
        virtual bool IsActive() { return isActive; }
        /// <summary>
        /// Cast the data into the desired type
        /// </summary>
        /// <returns></returns>
        template <class T>
        T* CastData(COMPONENT_DATA* d)
        {
            T* t = dynamic_cast<T*>(d);
            return t;
                
        }
        /// <summary>
        /// Returns the offset of the collider in Vector3 Form
        /// </summary>
        /// <returns></returns>
        VECTOR3 Offset() { return offset; }
        /// <summary>
        /// Generates a XMMATRIX from the offset vector
        /// </summary>
        /// <returns></returns>
        XMMATRIX MatrixOffset();
        /// <summary>
        /// Returns the world matrix of the collider after being fitted to a bone. Will return identity if unfitted
        /// </summary>
        /// <returns></returns>
        XMMATRIX WorldMatrix() { return bone_World; }
        virtual float Size() { return 0; };
    };



    class BOUNDING_BOX : public COLLIDER_BASE
    {
    public:
        BOUNDING_BOX() {};
        virtual void Collide(BOUNDING_BOX& tar) {};
    };

#pragma endregion
#pragma region SPHERE

    class SPHERE : public COLLIDER_BASE
    {
        VECTOR3 center;
        float radius;
        std::shared_ptr<MODEL>cube;
    public:
        SPHERE(VECTOR3 pos, float rad);
        //HRESULT Initialize(VECTOR3* pos, float rad);
        void Execute(XMMATRIX mat) override;
        void Render() override;
        bool Collide(COLLIDER_BASE* other) override;
        bool Collide(VECTOR3 p) override;
        VECTOR3 Center();
        float Radius();
        float Size() override { return radius; }

        void SetCenter(VECTOR3 v);
        void SetRadius(float rad);
        void SetData(COMPONENT_DATA* data) override;

    };

#pragma endregion
#pragma region OBB

    class OBB : public BOUNDING_BOX
    {
        std::vector<VECTOR3>points;
        VECTOR3 oriMin, oriMax;
        VECTOR3 rotation;

        /// <summary>
        /// Update the coordinates of each of the points
        /// </summary>
        /// <param name="mat"></param>
        void UpdatePosition(XMMATRIX mat);
    public:

        // Main Functions
        OBB();
        OBB(VECTOR3 vMin, VECTOR3 vMax);
        /// <summary>
        /// Initializes the collider. Call after creating 
        /// </summary>
        /// <returns></returns>
        HRESULT Initialize() override;
        void Update(VECTOR3 pos, VECTOR3 rot);
        void Execute(XMMATRIX mat) override;
        void Render() override;
        bool Collide(COLLIDER_BASE* other) override;
        bool Collide(VECTOR3 p) override;

        // Sub Functions

        void SetMin(VECTOR3 min);
        void SetMax(VECTOR3 max);
        void SetData(COMPONENT_DATA* d)override;

        std::vector<VECTOR3>Points();
        VECTOR3 Rotation();
        VECTOR3 Center();
        float Size() override;
    

        bool Status();
    };

#pragma endregion
#pragma region CYLINDER

    class CYLINDER : public COLLIDER_BASE
    {
        VECTOR3 top, bottom;
        VECTOR3 center;
        float height;
        float radius;
    public:
        CYLINDER();
        CYLINDER(VECTOR3 tp, VECTOR3 bot, float rad);
        HRESULT Initialize() override;
        void Execute(XMMATRIX mat) override;
        void Render() override;
        bool Collide(COLLIDER_BASE* other) override;
        void SetData(VECTOR3 tp, VECTOR3 bot, float rad);

        VECTOR3 Top();
        VECTOR3 Bottom();
        float Height();
        float Radius();
    };

#pragma endregion
#pragma region CAPSULE

    class CAPSULE : public COLLIDER_BASE
    {
        VECTOR3 center;
        float radius{};
        float height;
    public:
        VECTOR3 Top();
        VECTOR3 Bottom();
        CAPSULE();
        CAPSULE(float rad);
        CAPSULE(VECTOR3 cent, float rad, float ht);
        HRESULT Initialize() override;
        void Execute(XMMATRIX mat) override;
        void Render() override;
        bool Collide(COLLIDER_BASE* other) override;
        bool Collide(VECTOR3 p) override;

        VECTOR3 Center();
        float Radius();
        float Size() override;
        void OffsetCenter(XMMATRIX world);
        void SetCenter(VECTOR3 c);
        void SetRadius(float r);
        void SetHeight(float h);
        void SetData(COMPONENT_DATA* d) override;
    };

#pragma endregion
    /// <summary>
    /// <para> Calculates a point on a line that is closest to the target point </para>
    /// <para> 目標点に一番近い点を計算 </para>
    /// </summary>
    /// <param name="top"> : Starting point of vector</param>
    /// <param name="bot"> : Ending point of vector</param>
    /// <returns></returns>
    VECTOR3 PointLineClosest(VECTOR3 top, VECTOR3 bot, VECTOR3 target);
    /// <summary>
    /// <para> Calculates a point on a line that is closest to the target point </para>
    /// <para> 目標点に一番近い点を計算 </para>
    /// </summary>
    /// <param name="top"> : Starting point of vector</param>
    /// <param name="bot"> : Ending point of vector</param>
    /// <returns></returns>
    VECTOR3 PointLineClosest(VECTOR3 origin, CAPSULE* target);
    void AxisCasting(VECTOR3 oriMin, VECTOR3 oriMax, VECTOR3 tarMin, VECTOR3 tarMax, VECTOR3 rotation, int* colCount);
    bool OBBCollision(OBB* ori, OBB* tar);
    bool RayCast(VECTOR3& s, VECTOR3& e, MODEL* m, RAYCASTDATA& hr);
}