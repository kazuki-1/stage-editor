#include "COMPONENT_CREATOR.h"
#include "../../GAMEOBJECT.h"
#include "COMPONENT.h"
#include "COMPONENT_HEADERS.h"
std::shared_ptr<TRANSFORM_3D> __TRANSFORM_3D(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<TRANSFORM_3D>(g, d.get());
}
std::shared_ptr<TRANSFORM_2D> __TRANSFORM_2D(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<TRANSFORM_2D>(g, d.get());
}
std::shared_ptr<SPHERE_COLLIDER>__SPHERE_COLLIDER(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<SPHERE_COLLIDER>(g, d.get());
}
std::shared_ptr<CAPSULE_COLLIDER>__CAPSULE_COLLIDER(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<CAPSULE_COLLIDER>(g, d.get());
}
std::shared_ptr<OBB_COLLIDER> __OBB_COLLIDER(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<OBB_COLLIDER>(g, d.get());
}
std::shared_ptr<MESH_COLLIDER>__MESH_COLLIDER(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<MESH_COLLIDER>(g, d.get());
}
std::shared_ptr<MESH> __MESH(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<MESH>(g, d.get());
}
std::shared_ptr<SPRITE_2D> __SPRITE_2D(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<SPRITE_2D>(g, d.get());
}
std::shared_ptr<POINT_LIGHT> __POINT_LIGHT(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<POINT_LIGHT>(g, d.get());
}
std::shared_ptr<SPOTLIGHT> __SPOTLIGHT(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<SPOTLIGHT>(g, d.get());
}
std::shared_ptr<BGM>__BGM(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<BGM>(g, d.get());
}
std::shared_ptr<ENVIRONMENTAL_AUDIO>__ENVIRONMENTAL_AUDIO(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<ENVIRONMENTAL_AUDIO>(g, d.get());
}
std::shared_ptr<TERRAIN_AUDIO>__TERRAIN_AUDIO(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<TERRAIN_AUDIO>(g, d.get());
}
std::shared_ptr<OBB_TERRAIN>__OBB_TERRAIN(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<OBB_TERRAIN>(g, d.get());
}
std::shared_ptr<EFFECT>__EFFECT(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<EFFECT>(g, d.get());
}
std::shared_ptr<ENEMY_CONTROLLER>__ENEMY_CONTROLLER(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<ENEMY_CONTROLLER>(g, d.get());
}
std::shared_ptr<PLAYER_CONTROLLER>__PLAYER_CONTROLLER(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<PLAYER_CONTROLLER>(g, d.get());
}
std::shared_ptr<COMPONENT>__COMPONENT(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
{
    return std::make_shared<COMPONENT>(g, d);
}

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
std::shared_ptr<CAPSULE_COLLIDER_DATA>__CAPSULE_COLLIDER_DATA()
{
    return std::make_shared<CAPSULE_COLLIDER_DATA>();
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
std::shared_ptr<AUDIO_DATA>__AUDIO_DATA()
{
    return std::make_shared<AUDIO_DATA>();
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
std::shared_ptr<BGM_DATA>__BGM_DATA()
{
    return std::make_shared<BGM_DATA>();
}
std::shared_ptr<EFFECT_DATA>__EFFECT_DATA()
{
    return std::make_shared<EFFECT_DATA>();
}
std::shared_ptr<CHARACTER_DATA>__CHARACTER_DATA()
{
    return std::make_shared<CHARACTER_DATA>();
}
std::shared_ptr<PLAYER_CONTROLLER_DATA>__PLAYER_CONTROLLER_DATA()
{
    return std::make_shared<PLAYER_CONTROLLER_DATA>();
}
std::shared_ptr<ENEMY_CONTROLLER_DATA>__ENEMY_CONTROLLER_DATA()
{
    return std::make_shared<ENEMY_CONTROLLER_DATA>();
}
//std::shared_ptr<CYLINDER_COLLIDER_DATA>CCDD()
//{
//    return std::make_shared<CYLINDER_COLLIDER_DATA>();
//}
//std::shared_ptr<CYLINDER_COLLIDER>CCD(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>d)
//{
//    return std::make_shared<CYLINDER_COLLIDER>(g, d.get());
//}




void COMPONENT_CREATOR::Initialize()
{
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

}
void COMPONENT_CREATOR::RegisterComponent(COMPONENT_TYPE t, std::function<std::shared_ptr<COMPONENT>(GAMEOBJECT*, std::shared_ptr<COMPONENT_DATA>)> constructor)
{
    componentTypes.emplace(t, constructor);

}
void COMPONENT_CREATOR::RegisterData(COMPONENT_TYPE t, std::function<std::shared_ptr<COMPONENT_DATA>()>constructor)
{
    dataTypes.emplace(t, constructor);
}
std::shared_ptr<COMPONENT>COMPONENT_CREATOR::Instantiate(GAMEOBJECT* g, std::shared_ptr<COMPONENT_DATA>data)
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
std::shared_ptr<COMPONENT_DATA>COMPONENT_CREATOR::Instantiate(COMPONENT_TYPE t)
{
    return dataTypes.find(t)->second();
}
