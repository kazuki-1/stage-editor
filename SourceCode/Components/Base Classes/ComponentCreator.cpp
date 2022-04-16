#include "ComponentCreator.h"
#include "../../GAMEOBJECT.h"
#include "Component.h"
#include "ComponentHeaders.h"


#pragma region Component Constructors

std::shared_ptr<TRANSFORM_3D> __TRANSFORM_3D(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<TRANSFORM_3D>(g, d.get());
}
std::shared_ptr<TRANSFORM_2D> __TRANSFORM_2D(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<TRANSFORM_2D>(g, d.get());
}
std::shared_ptr<SPHERE_COLLIDER>__SPHERE_COLLIDER(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<SPHERE_COLLIDER>(g, d.get());
}
std::shared_ptr<CapsuleCollider>__CAPSULE_COLLIDER(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<CapsuleCollider>(g, d.get());
}
std::shared_ptr<OBB_COLLIDER> __OBB_COLLIDER(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<OBB_COLLIDER>(g, d.get());
}
std::shared_ptr<MESH_COLLIDER>__MESH_COLLIDER(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<MESH_COLLIDER>(g, d.get());
}
std::shared_ptr<MESH> __MESH(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<MESH>(g, d.get());
}
std::shared_ptr<SPRITE_2D> __SPRITE_2D(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<SPRITE_2D>(g, d.get());
}
std::shared_ptr<POINT_LIGHT> __POINT_LIGHT(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<POINT_LIGHT>(g, d.get());
}
std::shared_ptr<SPOTLIGHT> __SPOTLIGHT(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<SPOTLIGHT>(g, d.get());
}
std::shared_ptr<BGMComponent>__BGM(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<BGMComponent>(g, d.get());
}
std::shared_ptr<ENVIRONMENTAL_AUDIO>__ENVIRONMENTAL_AUDIO(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<ENVIRONMENTAL_AUDIO>(g, d.get());
}
std::shared_ptr<TERRAIN_AUDIO>__TERRAIN_AUDIO(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<TERRAIN_AUDIO>(g, d.get());
}
std::shared_ptr<OBB_TERRAIN>__OBB_TERRAIN(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<OBB_TERRAIN>(g, d.get());
}
std::shared_ptr<EFFECT>__EFFECT(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<EFFECT>(g, d.get());
}
std::shared_ptr<EnemyController>__ENEMY_CONTROLLER(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<EnemyController>(g, d.get());
}
std::shared_ptr<PLAYER_CONTROLLER>__PLAYER_CONTROLLER(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<PLAYER_CONTROLLER>(g, d.get());
}
std::shared_ptr<NPCDialogue>__NPCDialogue(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<NPCDialogue>(g, d.get());
}
std::shared_ptr<Component>__COMPONENT(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Component>(g, d);
}


#pragma endregion

#pragma region Component Data Constructors

std::shared_ptr<TRANSFORM_3D_DATA>__TRANSFORM_3D_DATA()
{
    return std::make_shared<TRANSFORM_3D_DATA>();
}
std::shared_ptr<TRANSFORM_2D_DATA>__TRANSFORM_2D_DATA()
{
    return std::make_shared<TRANSFORM_2D_DATA>();
}
std::shared_ptr<SPHERE_COLLIDER_DATA>__SPHERE_COLLIDER_DATA()
{
    return std::make_shared<SPHERE_COLLIDER_DATA>();
}
std::shared_ptr<CapsuleCollider_Data>__CAPSULE_COLLIDER_DATA()
{
    return std::make_shared<CapsuleCollider_Data>();
}
std::shared_ptr<OBB_COLLIDER_DATA>__OBB_COLLIDER_DATA()
{
    return std::make_shared<OBB_COLLIDER_DATA>();
}
std::shared_ptr<MESH_COLLIDER_DATA>__MESH_COLLIDER_DATA()
{
    return std::make_shared<MESH_COLLIDER_DATA>();
}
std::shared_ptr<MESH_DATA>__MESH_DATA()
{
    return std::make_shared<MESH_DATA>();
}
std::shared_ptr<SPRITE_2D_DATA>__SPRITE_2D_DATA()
{
    return std::make_shared<SPRITE_2D_DATA>();
}
std::shared_ptr<POINT_LIGHT_DATA>__POINT_LIGHT_DATA()
{
    return std::make_shared<POINT_LIGHT_DATA>();
}
std::shared_ptr<SPOTLIGHT_DATA>__SPOTLIGHT_DATA()
{
    return std::make_shared<SPOTLIGHT_DATA>();
}
std::shared_ptr<AudioData>__AUDIO_DATA()
{
    return std::make_shared<AudioData>();
}
std::shared_ptr<ENVIRONMENTAL_AUDIO_DATA>__ENVIRONMENTAL_AUDIO_DATA()
{
    return std::make_shared<ENVIRONMENTAL_AUDIO_DATA>();
}
std::shared_ptr<TERRAIN_AUDIO_DATA>__TERRAIN_AUDIO_DATA()
{
    return std::make_shared<TERRAIN_AUDIO_DATA>();
}
std::shared_ptr<OBB_TERRAIN_DATA>__OBB_TERRAIN_DATA()
{
    return std::make_shared<OBB_TERRAIN_DATA>();
}
std::shared_ptr<BGMComponent_Data>__BGM_DATA()
{
    return std::make_shared<BGMComponent_Data>();
}
std::shared_ptr<EFFECT_DATA>__EFFECT_DATA()
{
    return std::make_shared<EFFECT_DATA>();
}
std::shared_ptr<CharacterData>__CHARACTER_DATA()
{
    return std::make_shared<CharacterData>();
}
std::shared_ptr<PLAYER_CONTROLLER_DATA>__PLAYER_CONTROLLER_DATA()
{
    return std::make_shared<PLAYER_CONTROLLER_DATA>();
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

    RegisterData(COMPONENT_TYPE::TRANSFORM_3D, __TRANSFORM_3D_DATA);
    RegisterData(COMPONENT_TYPE::TRANSFORM_2D, __TRANSFORM_2D_DATA);
    RegisterData(COMPONENT_TYPE::SPHERE_COL, __SPHERE_COLLIDER_DATA);
    RegisterData(COMPONENT_TYPE::CAPSULE_COL, __CAPSULE_COLLIDER_DATA);
    RegisterData(COMPONENT_TYPE::OBB_COL, __OBB_COLLIDER_DATA);
    RegisterData(COMPONENT_TYPE::MESH_COL, __MESH_COLLIDER_DATA);
    RegisterData(COMPONENT_TYPE::MESH, __MESH_DATA);
    RegisterData(COMPONENT_TYPE::TERRAIN_AUDIO, __TERRAIN_AUDIO_DATA);
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

    RegisterComponent(COMPONENT_TYPE::TRANSFORM_3D, __TRANSFORM_3D);
    RegisterComponent(COMPONENT_TYPE::TRANSFORM_2D, __TRANSFORM_2D);
    RegisterComponent(COMPONENT_TYPE::SPHERE_COL, __SPHERE_COLLIDER);
    RegisterComponent(COMPONENT_TYPE::CAPSULE_COL, __CAPSULE_COLLIDER);
    RegisterComponent(COMPONENT_TYPE::OBB_COL, __OBB_COLLIDER);
    RegisterComponent(COMPONENT_TYPE::MESH_COL, __MESH_COLLIDER);
    RegisterComponent(COMPONENT_TYPE::MESH, __MESH);
    RegisterComponent(COMPONENT_TYPE::SPRITE_2D, __SPRITE_2D);
    RegisterComponent(COMPONENT_TYPE::TERRAIN_AUDIO, __TERRAIN_AUDIO);
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
