#pragma once
//#include "MODEL.h"
//#include "OBJECT.h"
#include <wrl.h>
#include <d3d11.h>
//#include "SHADERS.h"
#include "DXMath.h"

using namespace Microsoft::WRL;
using namespace Math;
class SHADERS;
class MODEL;
class OBJECT;
class DebugController;
class DEBUG_PRIMITIVE
{
protected:
    std::shared_ptr<MODEL>model;
    Vector3 scale;
    Vector3 position;
    Vector3 rotation;
public:

    virtual void Execute();
    virtual void Render(Vector4 colour = { 1.0f, 1.0f, 1.0f, 1.0f });

    Vector3 Position() { return position; }
    Vector3 Rotation() { return rotation; }

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

class DEBUG_SPHERE : public DEBUG_PRIMITIVE
{
    float radius;
public:
    DEBUG_SPHERE();
    DEBUG_SPHERE(float r);
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
    void Execute(XMMATRIX mat);
    void Render(Vector4 colour = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
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
    void Execute(XMMATRIX mat);
    void Render(Vector4 colour = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
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
    void Execute(XMMATRIX mat);
    void Render(Vector4 colour = { 1.0f, 1.0f, 1.0f, 1.0f }) override;
    void SetTarget(XMMATRIX t);
    void SetTarget(Vector3 p, Vector3 r);
};

/// <summary>
/// Base class for dynamic debug primitives
/// </summary>
class DYNAMIC_DEBUG_PRIMITIVE : public DEBUG_PRIMITIVE
{
protected:
    struct VERTEX
    {
        Vector3 position;
    };
    struct CBUFFER_M
    {
        XMFLOAT4X4 world;
        Vector4 colour;
    };
    std::vector<VERTEX>vertices;
    std::vector<int>indices;
    std::shared_ptr<SHADERS>shader;
    ComPtr<ID3D11Buffer>vertexBuffer;
    ComPtr<ID3D11Buffer>indexBuffer;
    ComPtr<ID3D11Buffer>meshConstantBuffer;
    XMFLOAT4X4 world;
protected:
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