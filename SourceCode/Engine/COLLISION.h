#pragma once
#include <DirectXMath.h>
#include <vector>
#include "MODEL.h"
#include "DXMath.h"
#include "CerealHeaders.h"
using namespace Math;

using namespace DirectX;
//using namespace Math;
class Mesh_Component;
class ComponentData;
class OBBCollider_Data;
class CapsuleCollider_Data;

namespace COLLIDERS
{

#pragma region BASE CLASSES

    /// <summary>
    /// For Storing RayCast data
    /// </summary>
    struct RAYCASTDATA
    {
        std::string model_name{};
        Vector3 position{ 0, 0, 0 };
        Vector3 normal{ 0, 0, 0 };
        float distance;
        int m_Index{ -1 };
    };
    struct OBB_DIRECTION
    {
        bool horizontol;
        bool vertical;
    };

    /// <summary>
    /// Collider Base class. Inherited by OBB, CAPSULE and SPHERE
    /// </summary>
    class COLLIDER_BASE
    {
    protected:
        bool isActive{ true };
        Vector3 offset;
        Vector3 rotation;
        XMMATRIX world;
        XMMATRIX bone_World{ XMMatrixIdentity() };
    public:
        COLLIDER_BASE() {};
        /// <summary>
        /// <para> Pure virtual Function. Initializes the object </para>
        /// <para> 純粋仮想関数。クラスを初期化する </para>
        /// </summary>
        /// <returns></returns>
        virtual HRESULT Initialize() { return S_OK; }
        /// <summary>
        /// <para> Pure virtual Function. Called every frame to perform functions</para>
        /// <para> 純粋仮想関数。関数を実行するように毎フレームに呼び出す</para>
        /// </summary>
        /// <returns></returns>
        virtual void Execute(XMMATRIX mat) {}
        /// <summary>
        /// <para> Pure virtual Function. Called to render any objects within</para>
        /// <para> 純粋仮想関数。レンダー関数を実行するように毎フレームに呼び出す</para>
        /// </summary>
        /// <returns></returns>
        virtual void Render() {}
        /// <summary>
        /// <para> Pure virtual Function. Called to perform collision check with the other object</para>
        /// <para> 純粋仮想関数。引数にあるコライダーと当たり判定を計算</para>
        /// </summary>
        /// <returns></returns>
        virtual bool Collide(COLLIDER_BASE* o) { return false; }
        /// <summary>
        /// <para> Pure virtual Function. Called to perform collision check with the point</para>
        /// <para> 純粋仮想関数。引数にある座標と当たり判定を計算</para>
        /// </summary>
        /// <returns></returns>
        virtual bool Collide(Vector3 p) { return false; }
        /// <summary>
        /// <para> virtual Function. Called to fit the collider to the bone of the model</para>
        /// <para> 仮想関数。モデルにしてされたボーンにコライダーをセット</para>
        /// </summary>
        /// <param name="name"> : Name of bone</param>
        /// <param name="m"> : Pointer of model</param>
        virtual void FitToBone(std::string name, MODEL* m);
        virtual void SetData(ComponentData* data) {}
        /// <summary>
        /// Offsets the collider 
        /// </summary>
        /// <param name="off"> : User declared offset</param>
        virtual void OffsetCollider(Vector3 off) { offset = off; }
        virtual void RotateCollider(Vector3 rot) { rotation = rot; }

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
        T* CastData(ComponentData* d)
        {
            T* t = dynamic_cast<T*>(d);
            return t;

        }
        /// <summary>
        /// Returns the offset of the collider in Vector3 Form
        /// </summary>
        /// <returns></returns>
        Vector3 Offset() { return offset; }
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


    /// <summary>
    /// BOUNDING_BOX base. Currently only inherited by OBB, will implement AABB if needed
    /// </summary>
    class BOUNDING_BOX : public COLLIDER_BASE
    {
    public:
        BOUNDING_BOX() {};
        virtual void Collide(BOUNDING_BOX& tar) {};
    };

#pragma endregion
#pragma region SPHERE

    /// <summary>
    /// SPHERE Collider
    /// </summary>
    class SPHERE : public COLLIDER_BASE
    {
        Vector3 center;
        float radius;
        std::shared_ptr<MODEL>cube;
    public:
        SPHERE(Vector3 pos, float rad);
        void Execute(XMMATRIX mat) override;
        void Render() override;
        bool Collide(COLLIDER_BASE* other) override;
        bool Collide(Vector3 p) override;
        Vector3 Center();
        float Radius();
        float Size() override { return radius; }

        void SetCenter(Vector3 v);
        void SetRadius(float rad);
        void SetData(ComponentData* data) override;

    };

#pragma endregion
#pragma region OBB

    class OBB : public BOUNDING_BOX
    {
        std::vector<Vector3>points;
        Vector3 oriMin, oriMax;
        Vector3 rotation;

        /// <summary>
        /// Update the coordinates of each of the points
        /// </summary>
        /// <param name="mat"></param>
        void UpdatePosition(XMMATRIX mat);
    public:

        // Main Functions
        OBB();
        OBB(Vector3 vMin, Vector3 vMax);
        /// <summary>
        /// Initializes the collider. Call after creating 
        /// </summary>
        /// <returns></returns>
        HRESULT Initialize() override;
        void Update(Vector3 pos, Vector3 rot);
        void Execute(XMMATRIX mat) override;
        void Render() override;
        bool Collide(COLLIDER_BASE* other) override;
        bool Collide(Vector3 p) override;

        // Sub Functions

        void SetMin(Vector3 min);
        void SetMax(Vector3 max);
        void SetData(ComponentData* d)override;

        std::vector<Vector3>Points();
        Vector3 Rotation();
        Vector3 Center();
        float Size() override;


        bool Status();
    };

#pragma endregion
#pragma region CYLINDER

    class CYLINDER : public COLLIDER_BASE
    {
        Vector3 top, bottom;
        Vector3 center;
        float height;
        float radius;
    public:
        CYLINDER();
        CYLINDER(Vector3 tp, Vector3 bot, float rad);
        HRESULT Initialize() override;
        void Execute(XMMATRIX mat) override;
        void Render() override;
        bool Collide(COLLIDER_BASE* other) override;
        void SetData(Vector3 tp, Vector3 bot, float rad);

        Vector3 Top();
        Vector3 Bottom();
        float Height();
        float Radius();
    };

#pragma endregion
#pragma region CAPSULE

    class CAPSULE : public COLLIDER_BASE
    {
        Vector3 center;
        float radius{};
        float height;
    public:
        Vector3 Top();
        Vector3 Bottom();
        CAPSULE();
        CAPSULE(float rad);
        CAPSULE(Vector3 cent, float rad, float ht);
        HRESULT Initialize() override;
        void Execute(XMMATRIX mat) override;
        void Render() override;
        bool Collide(COLLIDER_BASE* other) override;
        bool Collide(Vector3 p) override;

        Vector3 Center();
        float Radius();
        float Size() override;
        void OffsetCenter(XMMATRIX world);
        void SetCenter(Vector3 c);
        void SetRadius(float r);
        void SetHeight(float h);
        void SetData(ComponentData* d) override;
    };

#pragma endregion
#pragma region RAYCAST_MANAGER

    /// <summary>
    /// MeshCollider_Component models are inserted here.
    /// </summary>
    class RAYCAST_MANAGER : public Singleton<RAYCAST_MANAGER>
    {
        std::vector<Mesh_Component*>meshes;
    public:
        /// <summary>
        /// <para> Inserts the model into the map and allow it to perform collision check </para>
        /// <para> モデルをマップに登録し、MESH COLLIDERの対象内になる</para>
        /// </summary>
        /// <param name="name"> : Name of model</param>
        /// <param name="m"> : Model pointer</param>
        void Insert(Mesh_Component* m);
        /// <summary>
        /// Called at the end of the program or when switching scenes
        /// </summary>
        void Finalize();
        /// <summary>
        /// <para> Perform ray casting collision check </para>
        /// <para> レイーキャストを利用して当たり判定を計算 </para>
        /// </summary>
        /// <param name="startOfRay"> : Starting point of object</param>
        /// <param name="endOfRay"> : Direction of movement</param>
        /// <param name="cur_mesh"> : Pointer of current mesh component. Collision check will not be performed onto this mesh</param>
        /// <param name="rcd"> : Output. RayCastData is stored here. Create a new and put it here</param>
        /// <returns></returns>
        bool Collide(Vector3 startOfRay, Vector3 endOfRay, Mesh_Component* cur_mesh, RAYCASTDATA& rcd);
        bool Collide(Vector3 startofRay, Vector3 endOfRay, Mesh_Component* target_mesh, int target_mesh_index, RAYCASTDATA& rcd);
        /// <summary>
        /// <para> Performs ray cast on all models in the vector and returns a list of collided model and collision data </para>
        /// <para> モデルリストのすべてのモデルをレイーキャスト当たり判定を計算し、あたったモデルのリストを返す </para>
        /// </summary>
        /// <param name="cur_mesh"> : Pointer of current mesh component. Collision check will not be performed onto this mesh</param>
        /// <param name="startOfRay"> : Starting point of object</param>
        /// <param name="direction_vector"> : Direction of movement</param>
        /// <param name="rcd"> : Output. RayCastData is stored here. Create a new and put it here</param>
        /// <returns></returns>
        void GetListOfCollided(Mesh_Component* cur_Mesh, Vector3 startOfRay, Vector3 directionVector, std::vector<RAYCASTDATA>& rcd);
        std::vector<Mesh_Component*>Meshes();


    };

#pragma endregion
    /// <summary>
    /// <para> Calculates a point on a line that is closest to the target point </para>
    /// <para> 目標点に一番近い点を計算 </para>
    /// </summary>
    /// <param name="top"> : Starting point of vector</param>
    /// <param name="bot"> : Ending point of vector</param>
    /// <returns></returns>
    Vector3 PointLineClosest(Vector3 top, Vector3 bot, Vector3 target);
    /// <summary>
    /// <para> Calculates a point on a line that is closest to the target point </para>
    /// <para> 目標点に一番近い点を計算 </para>
    /// </summary>
    /// <param name="top"> : Starting point of vector</param>
    /// <param name="bot"> : Ending point of vector</param>
    /// <returns></returns>
    Vector3 PointLineClosest(Vector3 origin, CAPSULE* target);
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
    void AxisCasting(Vector3 oriMin, Vector3 oriMax, Vector3 tarMin, Vector3 tarMax, Vector3 rotation, int* colCount);
    /// <summary>
    /// <para> Performs collision check between 2 OBBs </para>
    /// <para> 2つのOBBに当たり判定を計算 </para>
    /// </summary>
    /// <returns></returns>
    bool OBBCollision(OBB* ori, OBB* tar);
    /// <summary>
    /// <para> Perform Raycasting </para>
    /// <para> レイーキャストを計算 </para>
    /// </summary>
    /// <param name="s"> : Starting point of ray</param>
    /// <param name="e"> : Direction of ray</param>
    /// <param name="m"> : Target model</param>
    /// <param name="hr"> : Output. RayCastData is stored here. Create a new and put it here</param>
    /// <returns></returns>
    bool RayCast(Vector3& s, Vector3& e, MODEL* m, RAYCASTDATA& hr, int mesh_index = -1);

}