#include "SoundObstructor.h"
#include "MESH.h"
#include "Transform3D.h"
#include "../Engine/AudioEngine.h"

/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------SoundObstructor_Component Class-------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------SoundObstructor_Component Constructor-------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

SoundObstructor_Component::SoundObstructor_Component(GameObject* parent, ComponentData* d)
{
	this->parent = parent;
	data = static_cast<SoundObstructor_Data*>(d);
}

/*---------------------------------------------------SoundObstructor_Component Initialize()------------------------------------------------*/

HRESULT SoundObstructor_Component::Initialize()
{
	
	if (data)
	{
		plane = std::make_shared<Dynamic_Plane>(data->size);
		AudioEngine::Instance()->InsertObstructor(plane.get(), data->obstruction_rate);
	}
	return S_OK;
}

/*---------------------------------------------------SoundObstructor_Component Execute()---------------------------------------------------*/

void SoundObstructor_Component::Execute()
{
	Transform3D_Component* transform = GetComponent< Transform3D_Component>();

	XMMATRIX matrix{ transform->TransformMatrix() };

	plane->UpdateVertices(data->size, &matrix);
}

/*---------------------------------------------------SoundObstructor_Component Render()--------------------------------------------------------*/

void SoundObstructor_Component::Render()
{
	plane->Render();
}


/*---------------------------------------------------SoundObstructor_Component UI()--------------------------------------------------------*/

void SoundObstructor_Component::UI()
{
	if (ImGui::TreeNode("Sound Obstruction"))
	{
		// Parameters
		ImGui::DragFloat3("Size", &data->size.x, 0.1f, 0.0f);
		ImGui::DragFloat("Obstruction rate", &data->obstruction_rate, 0.1f, 0.0f);
		ImGui::TreePop();
	}
}

/*---------------------------------------------------SoundObstructor_Component Finalize()--------------------------------------------------------*/

void SoundObstructor_Component::Finalize()
{
	plane->Finalize();
}

/*---------------------------------------------------SoundObstructor_Component GetComponentType()--------------------------------------------------------*/

COMPONENT_TYPE SoundObstructor_Component::GetComponentType()
{
	return data->type;
}
