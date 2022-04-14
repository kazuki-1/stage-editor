#pragma once
#include "../../Engine/DXMath.h"
using namespace Math;

enum class COMPONENT_TYPE
{
    BASE = -1,
    MESH, 
    SPHERE_COL,
    CAPSULE_COL,
    OBB_COL, 
    MESH_COL, 
    POINT_LIGHT, 
    SPOTLIGHT,
    BGM, 
    ENV_AUDIO, 
    TERRAIN_AUDIO, 
    PLAYER_CONTROLLER,
    ENEMY_CONTROLLER, 
    NPC_DIALOGUE,
    SPRITE_2D,
    EFFECT,                 // Unused
    CAMERA,                 // Unused
    AUDIO,                  // Base class
    OBB_TERRAIN,            // Unused 
    CHARACTER,              // Base Class
    TRANSFORM_2D,           
    TRANSFORM_3D, 
    DIRECTIONAL_LIGHT
};

class COMPONENT_DATA
{
public:
    int id{};
    COMPONENT_TYPE type;

    COMPONENT_DATA() { type = COMPONENT_TYPE::BASE; }
    virtual ~COMPONENT_DATA() {}
    template <class T>
    void serialize(T& t)
    {
        t(id, type);
    }

};

class AUDIO_DATA : public COMPONENT_DATA
{
public:
    AUDIO_DATA() { type = COMPONENT_TYPE::AUDIO; }
    char name[256] = "Audio";
    std::wstring file_path{};
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<COMPONENT_DATA>(this), name, file_path);
    }
};

class CHARACTER_DATA : public COMPONENT_DATA
{
public:
    CHARACTER_DATA() { type = COMPONENT_TYPE::CHARACTER; }
    char name[256] = "";

    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<COMPONENT_DATA>(this), name);
    }
};


//class CYLINDER_COLLIDER_DATA : public COMPONENT_DATA
//{
//public:
//    char name[256] = "";
//    std::string bone_name;
//    VECTOR3 top;
//    VECTOR3 bottom;
//    VECTOR3 offset{};
//    float radius;
//    float height;
//    CYLINDER_COLLIDER_DATA() { type = COMPONENT_TYPE::CYLIN_COL; }
//    template <class T>
//    void serialize(T& t)
//    {
//        t(name, bone_name, top, bottom, radius, height);
//    }
//};









CEREAL_REGISTER_TYPE(CHARACTER_DATA)
CEREAL_REGISTER_TYPE(AUDIO_DATA)