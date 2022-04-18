#include "ComponentCreator.h"
#include "../../GAMEOBJECT.h"
#include "Component.h"
#include "ComponentHeaders.h"


#pragma region Component Constructors

std::shared_ptr<Transform3D_Component> __TRANSFORM_3D(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Transform3D_Component>(g, d.get());
}
std::shared_ptr<Transform2D_Component> __TRANSFORM_2D(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Transform2D_Component>(g, d.get());
}
std::shared_ptr<SphereCollider_Component>__SPHERE_COLLIDER(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<SphereCollider_Component>(g, d.get());
}
std::shared_ptr<CapsuleCollider_Component>__CAPSULE_COLLIDER(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<CapsuleCollider_Component>(g, d.get());
}
std::shared_ptr<OBBCollider_Component> __OBB_COLLIDER(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<OBBCollider_Component>(g, d.get());
}
std::shared_ptr<MeshCollider_Component>__MESH_COLLIDER(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<MeshCollider_Component>(g, d.get());
}
std::shared_ptr<Mesh_Component> __MESH(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Mesh_Component>(g, d.get());
}
std::shared_ptr<Sprite2D_Component> __SPRITE_2D(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Sprite2D_Component>(g, d.get());
}
std::shared_ptr<PointLight_Component> __POINT_LIGHT(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<PointLight_Component>(g, d.get());
}
std::shared_ptr<SpotLight_Component> __SPOTLIGHT(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<SpotLight_Component>(g, d.get());
}
std::shared_ptr<BGM_Component>__BGM(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<BGM_Component>(g, d.get());
}
std::shared_ptr<EnvironmentalAudio_Component>__ENVIRONMENTAL_AUDIO(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<EnvironmentalAudio_Component>(g, d.get());
}
std::shared_ptr<TerrainAudio_Component>__TERRAIN_AUDIO(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<TerrainAudio_Component>(g, d.get());
}
std::shared_ptr<OBBTerrain_Component>__OBB_TERRAIN(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<OBBTerrain_Component>(g, d.get());
}
std::shared_ptr<Effect_Component>__EFFECT(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Effect_Component>(g, d.get());
}
std::shared_ptr<EnemyController_Component>__ENEMY_CONTROLLER(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<EnemyController_Component>(g, d.get());
}
std::shared_ptr<PlayerController_Component>__PLAYER_CONTROLLER(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<PlayerController_Component>(g, d.get());
}
std::shared_ptr<Dialogue_Component>__NPCDialogue(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Dialogue_Component>(g, d.get());
}
std::shared_ptr<Component>__COMPONENT(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Component>(g, d);
}


#pragma endregion

#pragma region Component Data Constructors

std::shared_ptr<Transform3D_Data>__TRANSFORM_3D_DATA()
{
    return std::make_shared<Transform3D_Data>();
}
std::shared_ptr<Transform2D_Data>__TRANSFORM_2D_DATA()
{
    return std::make_shared<Transform2D_Data>();
}
std::shared_ptr<SphereCollider_Data>__SPHERE_COLLIDER_DATA()
{
    return std::make_shared<SphereCollider_Data>();
}
std::shared_ptr<CapsuleCollider_Data>__CAPSULE_COLLIDER_DATA()
{
    return std::make_shared<CapsuleCollider_Data>();
}
std::shared_ptr<OBBCollider_Data>__OBB_COLLIDER_DATA()
{
    return std::make_shared<OBBCollider_Data>();
}
std::shared_ptr<MeshCollider_Data>__MESH_COLLIDER_DATA()
{
    return std::make_shared<MeshCollider_Data>();
}
std::shared_ptr<Mesh_Data>__MESH_DATA()
{
    return std::make_shared<Mesh_Data>();
}
std::shared_ptr<Sprite2D_Data>__SPRITE_2D_DATA()
{
    return std::make_shared<Sprite2D_Data>();
}
std::shared_ptr<PointLight_Data>__POINT_LIGHT_DATA()
{
    return std::make_shared<PointLight_Data>();
}
std::shared_ptr<SpotLight_Data>__SPOTLIGHT_DATA()
{
    return std::make_shared<SpotLight_Data>();
}
std::shared_ptr<AudioData>__AUDIO_DATA()
{
    return std::make_shared<AudioData>();
}
std::shared_ptr<EnvironmentalAudio_Data>__ENVIRONMENTAL_AUDIO_DATA()
{
    return std::make_shared<EnvironmentalAudio_Data>();
}
std::shared_ptr<TerrainAudio_Data>__TERRAIN_AUDIO_DATA()
{
    return std::make_shared<TerrainAudio_Data>();
}
std::shared_ptr<OBBTerrain_Data>__OBB_TERRAIN_DATA()
{
    return std::make_shared<OBBTerrain_Data>();
}
std::shared_ptr<BGMComponent_Data>__BGM_DATA()
{
    return std::make_shared<BGMComponent_Data>();
}
std::shared_ptr<Effect_Data>__EFFECT_DATA()
{
    return std::make_shared<Effect_Data>();
}
std::shared_ptr<CharacterData>__CHARACTER_DATA()
{
    return std::make_shared<CharacterData>();
}
std::shared_ptr<PlayerController_Data>__PLAYER_CONTROLLER_DATA()
{
    return std::make_shared<PlayerController_Data>();
}
std::shared_ptr<EnemyController_Data>__ENEMY_CONTROLLER_DATA()
{
    return std::make_shared<EnemyController_Data>();
}
std::shared_ptr<NPCDialogue_Data>__NPCDialogue_Data()
{
    return std::make_shared<NPCDialogue_Data>();
}
//std::shared_ptr<CYLINDER_COLLIDER_DATA>CCDD()
//{
//    return std::make_shared<CYLINDER_COLLIDER_DATA>();
//}
//std::shared_ptr<CYLINDER_COLLIDER>CCD(GameObject* g, std::shared_ptr<ComponentData>d)
//{
//    return std::make_shared<CYLINDER_COLLIDER>(g, d.get());
//}


#pragma endregion

void ComponentCreator::Initialize()
{

#pragma region Component Data Registering

    RegisterData(COMPONENT_TYPE::Transform3D_Component, __TRANSFORM_3D_DATA);
    RegisterData(COMPONENT_TYPE::Transform2D_Component, __TRANSFORM_2D_DATA);
    RegisterData(COMPONENT_TYPE::SPHERE_COL, __SPHERE_COLLIDER_DATA);
    RegisterData(COMPONENT_TYPE::CAPSULE_COL, __CAPSULE_COLLIDER_DATA);
    RegisterData(COMPONENT_TYPE::OBB_COL, __OBB_COLLIDER_DATA);
    RegisterData(COMPONENT_TYPE::MESH_COL, __MESH_COLLIDER_DATA);
    RegisterData(COMPONENT_TYPE::MESH, __MESH_DATA);
    RegisterData(COMPONENT_TYPE::TerrainAudio_Component, __TERRAIN_AUDIO_DATA);
    RegisterData(COMPONENT_TYPE::OBB_TERRAIN, __OBB_TERRAIN_DATA);
    RegisterData(COMPONENT_TYPE::SPRITE_2D, __SPRITE_2D_DATA);
    RegisterData(COMPONENT_TYPE::POINT_LIGHT, __POINT_LIGHT_DATA);
    RegisterData(COMPONENT_TYPE::SPOTLIGHT, __SPOTLIGHT_DATA);
    RegisterData(COMPONENT_TYPE::ENV_AUDIO, __ENVIRONMENTAL_AUDIO_DATA);
    RegisterData(COMPONENT_TYPE::BGM, __BGM_DATA);
    RegisterData(COMPONENT_TYPE::AUDIO, __AUDIO_DATA);
    RegisterData(COMPONENT_TYPE::CHARACTER, __CHARACTER_DATA);
    RegisterData(COMPONENT_TYPE::PLAYER_CONTROLLER, __PLAYER_CONTROLLER_DATA);
    RegisterData(COMPONENT_TYPE::ENEMY_CONTROLLER, __ENEMY_CONTROLLER_DATA);
    RegisterData(COMPONENT_TYPE::NPC_DIALOGUE, __NPCDialogue_Data);

#pragma endregion

#pragma region Component Registering

    RegisterComponent(COMPONENT_TYPE::Transform3D_Component, __TRANSFORM_3D);
    RegisterComponent(COMPONENT_TYPE::Transform2D_Component, __TRANSFORM_2D);
    RegisterComponent(COMPONENT_TYPE::SPHERE_COL, __SPHERE_COLLIDER);
    RegisterComponent(COMPONENT_TYPE::CAPSULE_COL, __CAPSULE_COLLIDER);
    RegisterComponent(COMPONENT_TYPE::OBB_COL, __OBB_COLLIDER);
    RegisterComponent(COMPONENT_TYPE::MESH_COL, __MESH_COLLIDER);
    RegisterComponent(COMPONENT_TYPE::MESH, __MESH);
    RegisterComponent(COMPONENT_TYPE::SPRITE_2D, __SPRITE_2D);
    RegisterComponent(COMPONENT_TYPE::TerrainAudio_Component, __TERRAIN_AUDIO);
    RegisterComponent(COMPONENT_TYPE::OBB_TERRAIN, __OBB_TERRAIN);
    RegisterComponent(COMPONENT_TYPE::POINT_LIGHT, __POINT_LIGHT);
    RegisterComponent(COMPONENT_TYPE::SPOTLIGHT, __SPOTLIGHT);
    RegisterComponent(COMPONENT_TYPE::BGM, __BGM);
    RegisterComponent(COMPONENT_TYPE::ENV_AUDIO, __ENVIRONMENTAL_AUDIO);
    RegisterComponent(COMPONENT_TYPE::PLAYER_CONTROLLER, __PLAYER_CONTROLLER);
    RegisterComponent(COMPONENT_TYPE::ENEMY_CONTROLLER, __ENEMY_CONTROLLER);
    RegisterComponent(COMPONENT_TYPE::NPC_DIALOGUE, __NPCDialogue);

#pragma endregion

}
void ComponentCreator::RegisterComponent(COMPONENT_TYPE t, std::function<std::shared_ptr<Component>(GameObject*, std::shared_ptr<ComponentData>)> constructor)
{
    componentTypes.emplace(t, constructor);

}
void ComponentCreator::RegisterData(COMPONENT_TYPE t, std::function<std::shared_ptr<ComponentData>()>constructor)
{
    dataTypes.emplace(t, constructor);
}
std::shared_ptr<Component>ComponentCreator::Instantiate(GameObject* g, std::shared_ptr<ComponentData>data)
{
    int ind{-1};
    for (auto& d : g->Data()->Dataset())
    {
        if (d->type == data->type)
            ++ind;
    }
    data->id = ind;
    return componentTypes.find(data->type)->second(g, data);

}
std::shared_ptr<ComponentData>ComponentCreator::Instantiate(COMPONENT_TYPE t)
{
    return dataTypes.find(t)->second();
}
