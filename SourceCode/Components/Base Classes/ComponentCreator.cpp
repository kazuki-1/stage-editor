#include "ComponentCreator.h"
#include "../../GAMEOBJECT.h"
#include "Component.h"
#include "ComponentHeaders.h"


#pragma region Component Constructors

std::shared_ptr<Transform3D_Component> __Transform3D_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Transform3D_Component>(g, d.get());
}
std::shared_ptr<Transform2D_Component> __Transform2D_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Transform2D_Component>(g, d.get());
}
std::shared_ptr<SphereCollider_Component>__SphereCollider_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<SphereCollider_Component>(g, d.get());
}
std::shared_ptr<CapsuleCollider_Component>__CapsuleCollider_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<CapsuleCollider_Component>(g, d.get());
}
std::shared_ptr<OBBCollider_Component> __OBBCollider_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<OBBCollider_Component>(g, d.get());
}
std::shared_ptr<MeshCollider_Component>__MeshCollider_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<MeshCollider_Component>(g, d.get());
}
std::shared_ptr<Mesh_Component> __Mesh_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Mesh_Component>(g, d.get());
}
std::shared_ptr<Sprite2D_Component> __Sprite2D_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Sprite2D_Component>(g, d.get());
}
std::shared_ptr<PointLight_Component> __PointLight_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<PointLight_Component>(g, d.get());
}
std::shared_ptr<SpotLight_Component> __SpotLight_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<SpotLight_Component>(g, d.get());
}
std::shared_ptr<BGM_Component>__BGM_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<BGM_Component>(g, d.get());
}
std::shared_ptr<EnvironmentalAudio_Component>__EnvironmentalAudio_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<EnvironmentalAudio_Component>(g, d.get());
}
std::shared_ptr<TerrainAudio_Component>__TerrainAudio_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<TerrainAudio_Component>(g, d.get());
}
std::shared_ptr<OBBTerrain_Component>__OBBTerrain_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<OBBTerrain_Component>(g, d.get());
}
std::shared_ptr<Effect_Component>__Effect_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Effect_Component>(g, d.get());
}
std::shared_ptr<EnemyController_Component>__EnemyController_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<EnemyController_Component>(g, d.get());
}
std::shared_ptr<PlayerController_Component>__PlayerController_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<PlayerController_Component>(g, d.get());
}
std::shared_ptr<Dialogue_Component>__Dialogue_Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Dialogue_Component>(g, d.get());
}
std::shared_ptr<Component>__Component(GameObject* g, std::shared_ptr<ComponentData>d)
{
    return std::make_shared<Component>(g, d);
}


#pragma endregion

#pragma region Component Data Constructors

std::shared_ptr<Transform3D_Data>__Transform3D_Data()
{
    return std::make_shared<Transform3D_Data>();
}
std::shared_ptr<Transform2D_Data>__Transform2D_Data()
{
    return std::make_shared<Transform2D_Data>();
}
std::shared_ptr<SphereCollider_Data>__SphereCollider_Data()
{
    return std::make_shared<SphereCollider_Data>();
}
std::shared_ptr<CapsuleCollider_Data>__CapsuleCollider_Data()
{
    return std::make_shared<CapsuleCollider_Data>();
}
std::shared_ptr<OBBCollider_Data>__OBBCollider_Data()
{
    return std::make_shared<OBBCollider_Data>();
}
std::shared_ptr<MeshCollider_Data>__MeshCollider_Data()
{
    return std::make_shared<MeshCollider_Data>();
}
std::shared_ptr<Mesh_Data>__Mesh_Data()
{
    return std::make_shared<Mesh_Data>();
}
std::shared_ptr<Sprite2D_Data>__Sprite2D_Data()
{
    return std::make_shared<Sprite2D_Data>();
}
std::shared_ptr<PointLight_Data>__PointLight_Data()
{
    return std::make_shared<PointLight_Data>();
}
std::shared_ptr<SpotLight_Data>__SpotLight_Data()
{
    return std::make_shared<SpotLight_Data>();
}
std::shared_ptr<AudioData>__Audio_Data()
{
    return std::make_shared<AudioData>();
}
std::shared_ptr<EnvironmentalAudio_Data>__EnvironmentalAudio_Data()
{
    return std::make_shared<EnvironmentalAudio_Data>();
}
std::shared_ptr<TerrainAudio_Data>__TerrainAudio_Data()
{
    return std::make_shared<TerrainAudio_Data>();
}
std::shared_ptr<OBBTerrain_Data>__OBBTerrain_Data()
{
    return std::make_shared<OBBTerrain_Data>();
}
std::shared_ptr<BGMComponent_Data>__BGM_Data()
{
    return std::make_shared<BGMComponent_Data>();
}
std::shared_ptr<Effect_Data>__Effect_Data()
{
    return std::make_shared<Effect_Data>();
}
std::shared_ptr<CharacterData>__Character_Data()
{
    return std::make_shared<CharacterData>();
}
std::shared_ptr<PlayerController_Data>__PlayerController_Data()
{
    return std::make_shared<PlayerController_Data>();
}
std::shared_ptr<EnemyController_Data>__EnemyController_Data()
{
    return std::make_shared<EnemyController_Data>();
}
std::shared_ptr<Dialogue_Data>__Dialogue_Data()
{
    return std::make_shared<Dialogue_Data>();
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

    RegisterData(COMPONENT_TYPE::Transform3D_Component, __Transform3D_Data);
    RegisterData(COMPONENT_TYPE::Transform2D_Component, __Transform2D_Data);
    RegisterData(COMPONENT_TYPE::SPHERE_COL, __SphereCollider_Data);
    RegisterData(COMPONENT_TYPE::CAPSULE_COL, __CapsuleCollider_Data);
    RegisterData(COMPONENT_TYPE::OBB_COL, __OBBCollider_Data);
    RegisterData(COMPONENT_TYPE::MESH_COL, __MeshCollider_Data);
    RegisterData(COMPONENT_TYPE::MESH, __Mesh_Data);
    RegisterData(COMPONENT_TYPE::TerrainAudio_Component, __TerrainAudio_Data);
    RegisterData(COMPONENT_TYPE::OBB_TERRAIN, __OBBTerrain_Data);
    RegisterData(COMPONENT_TYPE::SPRITE_2D, __Sprite2D_Data);
    RegisterData(COMPONENT_TYPE::POINT_LIGHT, __PointLight_Data);
    RegisterData(COMPONENT_TYPE::SPOTLIGHT, __SpotLight_Data);
    RegisterData(COMPONENT_TYPE::ENV_AUDIO, __EnvironmentalAudio_Data);
    RegisterData(COMPONENT_TYPE::BGM, __BGM_Data);
    RegisterData(COMPONENT_TYPE::AUDIO, __Audio_Data);
    RegisterData(COMPONENT_TYPE::CHARACTER, __Character_Data);
    RegisterData(COMPONENT_TYPE::PLAYER_CONTROLLER, __PlayerController_Data);
    RegisterData(COMPONENT_TYPE::ENEMY_CONTROLLER, __EnemyController_Data);
    RegisterData(COMPONENT_TYPE::NPC_DIALOGUE, __Dialogue_Data);

#pragma endregion

#pragma region Component Registering

    RegisterComponent(COMPONENT_TYPE::Transform3D_Component, __Transform3D_Component);
    RegisterComponent(COMPONENT_TYPE::Transform2D_Component, __Transform2D_Component);
    RegisterComponent(COMPONENT_TYPE::SPHERE_COL, __SphereCollider_Component);
    RegisterComponent(COMPONENT_TYPE::CAPSULE_COL, __CapsuleCollider_Component);
    RegisterComponent(COMPONENT_TYPE::OBB_COL, __OBBCollider_Component);
    RegisterComponent(COMPONENT_TYPE::MESH_COL, __MeshCollider_Component);
    RegisterComponent(COMPONENT_TYPE::MESH, __Mesh_Component);
    RegisterComponent(COMPONENT_TYPE::SPRITE_2D, __Sprite2D_Component);
    RegisterComponent(COMPONENT_TYPE::TerrainAudio_Component, __TerrainAudio_Component);
    RegisterComponent(COMPONENT_TYPE::OBB_TERRAIN, __OBBTerrain_Component);
    RegisterComponent(COMPONENT_TYPE::POINT_LIGHT, __PointLight_Component);
    RegisterComponent(COMPONENT_TYPE::SPOTLIGHT, __SpotLight_Component);
    RegisterComponent(COMPONENT_TYPE::BGM, __BGM_Component);
    RegisterComponent(COMPONENT_TYPE::ENV_AUDIO, __EnvironmentalAudio_Component);
    RegisterComponent(COMPONENT_TYPE::PLAYER_CONTROLLER, __PlayerController_Component);
    RegisterComponent(COMPONENT_TYPE::ENEMY_CONTROLLER, __EnemyController_Component);
    RegisterComponent(COMPONENT_TYPE::NPC_DIALOGUE, __Dialogue_Component);

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
