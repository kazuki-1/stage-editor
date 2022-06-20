#pragma once
//using namespace Math;
#include <string>
#include "../../Engine/CerealHeaders.h"
enum class COMPONENT_TYPE
{
    BASE = -1,
    BGM, 
    CAPSULE_COL,
    ENEMY_CONTROLLER, 
    ENV_AUDIO, 
    MESH, 
    MESH_COL, 
    NPC_DIALOGUE,
    OBB_COL, 
    PLAYER_CONTROLLER,
    POINT_LIGHT, 
   
    SOUND_OBSTRUCTOR, 
    SPHERE_COL,
    SPOTLIGHT,
    SPRITE_2D,
    TerrainAudio_Component, 

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
    char name[256] = "";
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


CEREAL_REGISTER_TYPE(CharacterData)
CEREAL_REGISTER_TYPE(AudioData)