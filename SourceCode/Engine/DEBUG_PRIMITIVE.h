#pragma once
#include <wrl.h>
#include <d3d11.h>
#include "OBJECT.h"
#include "Math.h"
using namespace Microsoft::WRL;
using namespace Math;



//class MODEL;
class OBJECT;
class MODEL;
class DebugController;
class DEBUG_PRIMITIVE : public OBJECT
{
protected:
    struct CBuffer_DebugMesh
    {
        XMFLOAT4X4 world;
        Math::Vector4 colour;
    };




    std::shared_ptr<MODEL>model;
    Vector3 scale;
    Vector3 position;
    Vector3 rotation;
public:
    CBuffer_DebugMesh debugMeshData{};
    virtual void Execute();
    virtual void Render(Vector4 colour = { 1.0f, 1.0f, 1.0f, 1.0f });

    Vector3 Position() { return position; }
    Vector3 Rotation() { return rotation; }
    std::shared_ptr<MODEL> GetModel() { return model; }

    void SetPosition(Vector3 p) { position = p; }
    void SetRotation(Vector3 r) { rotation = r; }

};


class DEBUG_CUBE : public DEBUG_PRIMITIVE
{
public:
    DEBUG_CUBE();

};

class DEBUG_CYLINDER : public DEBUG_PRIMITIVE
{
    float radius;
public:
    DEBUG_CYLINDER();
    DEBUG_CYLINDER(float r);
};

class Sphere_Debug : public DEBUG_PRIMITIVE
{
    float radius;
public:
    Sphere_Debug();
    Sphere_Debug(float r);
    void SetRadius(float r);
};

class DEBUG_ARROWS : public DEBUG_PRIMITIVE
{
    std::shared_ptr<MODEL>xAxis;
    std::shared_ptr<MODEL>yAxis;
    std::shared_ptr<MODEL>zAxis;
    XMMATRIX target{ };
    Vector3 position, rotation;
    friend class DebugController;
public:
    DEBUG_ARROWS() {};
    void Initialize();
    void Execute() override;
    void Finalize() override;
    void Render(Vector4 colour = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
    void DisableRendering() override;
    void SetTarget(XMMATRIX t);
    void SetTarget(Vector3 p, Vector3 r);
};

class DEBUG_SCALARS: public DEBUG_PRIMITIVE
{
    std::shared_ptr<MODEL>xAxis;
    std::shared_ptr<MODEL>yAxis;
    std::shared_ptr<MODEL>zAxis;
    XMMATRIX target{ };
    Vector3 position, rotation;
    friend class DebugController;
public:
    DEBUG_SCALARS() {};
    void Initialize();
    void Execute() override;
    void Render(Vector4 colour = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
    void DisableRendering() override;
    void Finalize() override;
    void SetTarget(XMMATRIX t);
    void SetTarget(Vector3 p, Vector3 r);
};

class DEBUG_DISCS : public DEBUG_PRIMITIVE
{
    std::shared_ptr<MODEL>xAxis;
    std::shared_ptr<MODEL>yAxis;
    std::shared_ptr<MODEL>zAxis;
    XMMATRIX target{ };
    Vector3 position, rotation;
    friend class DebugController;
public:
    DEBUG_DISCS() {};
    void Initialize();
    void Execute() override;
    void Finalize() override;
    void Render(Vector4 colour = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
    void DisableRendering() override;
    void SetTarget(XMMATRIX t);
    void SetTarget(Vector3 p, Vector3 r);
};

/// <summary>
/// Base class for dynamic debug primitives
/// </summary>
class DYNAMIC_DEBUG_PRIMITIVE : public DEBUG_PRIMITIVE
{


public:
    struct VERTEX
    {
        Vector3 position{};
    };

protected:
    struct CBUFFER_M
    {
        XMFLOAT4X4 world{};
        Vector4 colour{};
    };
    std::vector<VERTEX>vertices;
    std::vector<int>indices;
    ComPtr<ID3D11Buffer>vertexBuffer;
    ComPtr<ID3D11Buffer>indexBuffer;
    ComPtr<ID3D11Buffer>meshConstantBuffer;
    XMFLOAT4X4 world{};


    virtual HRESULT Initialize();
public:

    /// <summary>
    /// Virtual execute function. Called every frame, usually to update the position of the primitive
    /// </summary>
    virtual void Execute();
    /// <summary>
    /// Virtual execute function. Called every frame, usually to update the position of the primitive
    /// </summary>
    /// <param name="target"> : The primitive will be transformed onto the target matrix</param>
    virtual void Execute(XMMATRIX target);
    /// <summary>
    /// Virtual render function. Called every frame to render the primitive object
    /// </summary>
    /// <param name="colour"> : The primitive will be rendered with these colour parameters</param>
    virtual void Render(Vector4 colour = {1.0f, 1.0f, 1.0f, 1.0f});
    virtual void SetTarget(Vector3 target);
    virtual void SetTransform(Vector3 pos, Vector3 rot) { position = pos; rotation = rot; }

    XMFLOAT4X4 GetWorldTransform() { return world; }
    /// <summary>
    /// <para>Sets the world transform. </para>
    /// <para> DO NOT USE IF YOU DONT KNOW WHAT YOURE DOIN</para>
    /// </summary>
    void SetWorldTransform(XMMATRIX matrix);
    ComPtr<ID3D11Buffer>GetVertexBuffer() { return vertexBuffer; }
    ComPtr<ID3D11Buffer>GetIndexBuffer() { return indexBuffer; }
    virtual std::vector<int>GetIndices() { return indices; }
    virtual std::vector<VERTEX>GetVertices() { return vertices; }
};


/// <summary>
/// Dynamic Cube class. Creates a wireframe cube with user declared vertices
/// </summary>
class DYNAMIC_CUBE : public DYNAMIC_DEBUG_PRIMITIVE
{
public:
    DYNAMIC_CUBE();
    void UpdateVertices(std::vector<Vector3>pos);
    
};

/// <summary>
/// Dynamic Sphere class. Creates a wireframe sphere with a user declared radius
/// </summary>
class DYNAMIC_SPHERE : public DYNAMIC_DEBUG_PRIMITIVE
{
    int vertices_count;
    float radius{ 0.1f };
    //HRESULT Initialize() override;
public:
    /// <param name="v_Count"> : Vertices of the sphere</param>
    /// <param name="rad"> : Radius</param>
    DYNAMIC_SPHERE(int v_Count = 32, float rad = 0.1f);
    /// <summary>
    /// Updates the vertices according to the radius
    /// </summary>
    /// <param name="radius"> : User declared radius. Will be written to the radius within the class</param>
    void UpdateVertices(float radius, XMMATRIX* target = nullptr);
    void SetRadius(float r);
};

class DYNAMIC_CYLINDER : public DYNAMIC_DEBUG_PRIMITIVE
{
    Vector3 center{};
    float height{};
    float radius{0.1f};
    int vertex_count{ 32 };
//    HRESULT Initialize() override;
public:
    /// <param name="rad"> : User declared radius of the cylinder</param>
    /// <param name="vert_count"> : User declared vertex count of the spherical shapes</param>
    DYNAMIC_CYLINDER(float rad = 0.1f, int vert_count = 32);
    /// <summary>
    /// Updates the vertices according to the user declared radius and height
    /// </summary>
    /// <param name="rad"> : User declared radius</param>
    /// <param name="height">: User declared height</param>
    void UpdateVertices(float rad, float height);
};

class DYNAMIC_CAPSULE : public DYNAMIC_DEBUG_PRIMITIVE
{
    Vector3 center{};
    float height;
    float radius{};
    int vertex_count{ 32 };
public:
    /// <param name="rad"> : User declared radius of the cylinder</param>
    /// <param name="vert_count"> : User declared vertex count of the spherical shapes</param>
    /// <param name="vert_count"></param>
    DYNAMIC_CAPSULE(float rad = 0.1f, int vert_count = 32, float height = 0.5f);
    /// <summary>
    /// Updates the vertices according to the user declared radius and height
    /// </summary>
    /// <param name="rad"> : User declared radius</param>
    /// <param name="height">: User declared height</param>
    void UpdateVertices(float rad, float height, XMMATRIX* target = nullptr);
};

class Dynamic_Plane : public DYNAMIC_DEBUG_PRIMITIVE
{
    Vector3 size;           // only X and Y are used
public:
    Dynamic_Plane(Vector3 size);
    void UpdateVertices(Vector3 s, XMMATRIX* target = nullptr);
    Vector2 GetSize();        // Retrieves the size length
    Vector3 GetCenter();    // Retrieves the center point
};