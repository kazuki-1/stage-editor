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
    TerrainAudio_Component, 
    PLAYER_CONTROLLER,
    ENEMY_CONTROLLER, 
    NPC_DIALOGUE,
    SPRITE_2D,
    EFFECT,                 // Unused
    CAMERA,                 // Unused
    AUDIO,                  // Base class
    OBB_TERRAIN,            // Unused 
    CHARACTER,              // Base Class
    Transform2D_Component,           
    Transform3D_Component, 
    DIRECTIONAL_LIGHT
};

class ComponentData
{
public:
    int id{};
    COMPONENT_TYPE type;

    ComponentData() { type = COMPONENT_TYPE::BASE; }
    virtual ~ComponentData() {}
    template <class T>
    void serialize(T& t)
    {
        t(id, type);
    }

};

class AudioData : public ComponentData
{
public:
    AudioData() { type = COMPONENT_TYPE::AUDIO; }
    char name[256] = "Audio";
    std::wstring file_path{};
    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this), name, file_path);
    }
};

class CharacterData : public ComponentData
{
public:
    CharacterData() { type = COMPONENT_TYPE::CHARACTER; }
    char name[256] = "";

    template <class T>
    void serialize(T& t)
    {
        t(cereal::base_class<ComponentData>(this), name);
    }
};


//class CYLINDER_COLLIDER_DATA : public ComponentData
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









CEREAL_REGISTER_TYPE(CharacterData)
CEREAL_REGISTER_TYPE(AudioData)