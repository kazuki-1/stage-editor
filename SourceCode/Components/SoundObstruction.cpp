#include "SoundObstruction.h"
#include "MESH.h"
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------SoundObstruction_Component Class-------------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------SoundObstruction_Component Constructor-------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------------------*/

SoundObstruction_Component::SoundObstruction_Component(GameObject* parent, ComponentData* d)
{
	this->parent = parent;
	data = static_cast<SoundObstruction_Data*>(d);
}

/*---------------------------------------------------SoundObstruction_Component Initialize()------------------------------------------------*/

HRESULT SoundObstruction_Component::Initialize()
{
	if (data)
	{

	}
	return S_OK;
}

/*---------------------------------------------------SoundObstruction_Component Execute()---------------------------------------------------*/

void SoundObstruction_Component::Execute()
{
}

/*---------------------------------------------------SoundObstruction_Component UI()--------------------------------------------------------*/

void SoundObstruction_Component::UI()
{
	if (!GetComponent<Mesh_Component>())
		return;

	std::shared_ptr<MODEL_RESOURCES> model = GetComponent<Mesh_Component>()->Model()->Resource();
	if (ImGui::TreeNode("Sound Obstruction"))
	{

		int index{};
		if (ImGui::BeginCombo("Mesh", model->Meshes[data->mesh_index].Name.c_str()))
		{
			for (auto& mesh : model->Meshes)
			{
				if (ImGui::Selectable(mesh.Name.c_str()))
					data->mesh_index = index;
				++index;
			}
			ImGui::EndCombo();
		}

		ImGui::DragFloat("Obstruction rate", &data->obstruction_rate, 0.05f, 0.0f, 1.0f);




		ImGui::TreePop();
	}
}

/*---------------------------------------------------SoundObstruction_Component GetComponentType()--------------------------------------------------------*/

COMPONENT_TYPE SoundObstruction_Component::GetComponentType()
{
	return data->type;
}
