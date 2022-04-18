#include "GameObject_2D.h"
//#include "GameObject.h"
#include "Components\Transform2D.h"
#include "Components\Transform3D.h"

/*--------------------------------GameObject_2D Constructor()----------------------------------*/

GameObject_2D::GameObject_2D(std::shared_ptr<OBJECT_DATA>d) 
{
    data = d;
    //name_buffer = (char*)data->name.c_str();
    for (int a = 0; a < data->name.size(); ++a)
        name_buffer[a] = data->name[a];
}

/*--------------------------------GameObject_2D Initialize()----------------------------------*/

HRESULT GameObject_2D::Initialize()
{
    if (data->Dataset().size() <= 0)
    {
        __InternalAddComponent(COMPONENT_TYPE::Transform2D_Component);
        __InternalAddComponent(COMPONENT_TYPE::SPRITE_2D);
    }

    else
        for (auto& d : data->Dataset())
            __InternalAddComponent(d->type, d);
    return S_OK;

}

/*--------------------------------GameObject_2D Execute()----------------------------------*/

void GameObject_2D::Execute()
{
    for (auto& c : components)
    {
        if(dynamic_cast<Transform2D_Component*>(c.get()))
            continue;
        c->Execute();
            
    }
    GetComponent<Transform2D_Component>()->Execute();
}

/*--------------------------------GameObject_2D ExecuteUI()----------------------------------*/

void GameObject_2D::ExecuteUI()
{
    Execute();
}

/*--------------------------------GameObject_2D Render()----------------------------------*/

void GameObject_2D::Render()
{
    for(auto& c : components)
    {
        if (dynamic_cast<Transform2D_Component*>(c.get()))
            continue;
        c->Render();
    }
    GetComponent<Transform2D_Component>()->Execute();
}

/*--------------------------------GameObject_2D RenderUI()----------------------------------*/

void GameObject_2D::RenderUI()
{
    for (auto& c : components)
        c->UI();
}