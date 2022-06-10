#include "LIGHTING.h"
#include "IMGUI.h"
#include "../Components/Base Classes/Component.h"
#include "../Components/DirectionalLight.h"
#include "../Components/PointLight.h"
#include "../Components/SpotLight.h"
#include <filesystem>
#pragma region LIGHTING

/*---------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------LIGHTING Class------------------------------------------------------*/
/*---------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------LIGHTING Constructor------------------------------------------------*/

LIGHTING::LIGHTING(L_TYPE t) : type(t)
{
	if (type == L_TYPE::DIRECTIONAL)
		return;
	d_Primitive = std::make_shared<Sphere_Debug>();

}

/*-------------------------------------------------LIGHTING Type()------------------------------------------------*/

LIGHTING::L_TYPE LIGHTING::Type()
{
	return type;
}

/*-------------------------------------------------LIGHTING RenderDebug()------------------------------------------------*/

void LIGHTING::RenderDebug()
{
	if (!d_Primitive)
		return;
	d_Primitive->SetPosition(position);
	d_Primitive->Execute();
	d_Primitive->Render({ 1.0f, 0.0f, 0.0f, 1.0f });
}


#pragma region Setters
/*-------------------------------------------------LIGHTING SetDirection()------------------------------------------------*/

void LIGHTING::SetDirection(Vector3 dir)
{
	direction = dir;
}

/*-------------------------------------------------LIGHTING SetPosition()------------------------------------------------*/

void LIGHTING::SetPosition(Vector3 pos)
{
	position = pos;
}

/*-------------------------------------------------LIGHTING SetColour()------------------------------------------------*/

void LIGHTING::SetColour(Vector4 col)
{
	colour = col;
}

/*-------------------------------------------------LIGHTING SetInnerCorner()------------------------------------------------*/

void LIGHTING::SetInnerCorner(float in)
{
	inner = inner;
}

/*-------------------------------------------------LIGHTING SetOuterCorner()------------------------------------------------*/

void LIGHTING::SetOuterCorner(float out)
{
	outer = outer;
}

/*-------------------------------------------------LIGHTING SetRange()------------------------------------------------*/

void LIGHTING::SetRange(float r)
{
	range = r;
}

/*-------------------------------------------------LIGHTING SetType()------------------------------------------------*/

void LIGHTING::SetType(L_TYPE t)
{
	type = t;
}

/*-------------------------------------------------LIGHTING WriteToData()------------------------------------------------*/

void LIGHTING::WriteToData(DirectionalLight_Component* d)
{
	direction = { d->Direction().x, d->Direction().y, d->Direction().z };
	colour = d->Colour();
}

void LIGHTING::WriteToData(PointLight_Component* p)
{
	position = { p->Position().x, p->Position().y, p->Position().z };
	colour = p->Colour();
	intensity = p->GetIntensity();
	range = p->Range();
}

void LIGHTING::WriteToData(SpotLight_Component* s)
{
	position = { s->Position().x, s->Position().y, s->Position().z };
	direction = { s->Direction().x, s->Direction().y, s->Direction().z };
	colour = s->Colour();
	range = s->Range();
	inner = s->InnerCorner();
	outer = s->OuterCorner();

}


#pragma endregion
#pragma region Getters

/*-------------------------------------------------LIGHTING Direction()------------------------------------------------*/


Vector3 LIGHTING::Direction()
{
	return direction;
}

/*-------------------------------------------------LIGHTING Position()------------------------------------------------*/

Vector3 LIGHTING::Position()
{
	return position;
}

/*-------------------------------------------------LIGHTING Colour()------------------------------------------------*/

Vector4 LIGHTING::Colour()
{
	return colour;
}

/*-------------------------------------------------LIGHTING Range()------------------------------------------------*/

float LIGHTING::Range()
{
	return range;
}

/*-------------------------------------------------LIGHTING Inner()------------------------------------------------*/

float LIGHTING::Inner()
{
	return inner;
}

/*-------------------------------------------------LIGHTING Outer()------------------------------------------------*/

float LIGHTING::Outer()
{
	return outer;
}

#pragma endregion

#pragma endregion
#pragma region LIGHTINGMANAGER

/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------LightingManager Class----------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------LightingManager Insert()-------------------------------------------------------------*/

void LightingManager::Insert(std::string n, std::shared_ptr<LIGHTING> l)
{
	dataset.insert(std::make_pair(n, l));
}

/*------------------------------------------------LightingManager Remove()-------------------------------------------------------------*/

void LightingManager::Remove(std::string n)
{
	for (auto d = dataset.begin(); d != dataset.end(); ++d)
	{
		if (n == d->first)
			dataset.erase(d);
	}
}

/*------------------------------------------------LightingManager Create()-------------------------------------------------------------*/

void LightingManager::Create(std::string n, LIGHTING::L_TYPE t)
{
	std::shared_ptr<LIGHTING>l = std::make_shared<LIGHTING>(t);
	Insert(n, l);
}

/*------------------------------------------------LightingManager RenderUI()-------------------------------------------------------------*/
// Deprecated. To set lighting other than directional light, create a gameObject and insert the component PointLight_Component or SpotLight_Component
void LightingManager::RenderUI()
{
	ImGui::Begin("Stage Settings");

		static std::shared_ptr<LIGHTING>d{ dataset.begin()->second };
		if (ImGui::BeginCombo("Lights : ", Name(d).c_str()))
		{
			for (auto& dt : dataset)
			{
				bool selected{ Name(d) == dt.first };
				if (ImGui::Selectable(dt.first.c_str(), &selected))
					d = dt.second;
			}
			ImGui::EndCombo();
		}

		if (ImGui::TreeNode(Name(d).c_str())) 
		{

			switch (d->Type()) {
			case LIGHTING::L_TYPE::DIRECTIONAL: {
				ImGui::DragFloat3("Direction : ", &d->direction.x, 0.05f, -10.0f, 10.0f);
				ImGui::ColorEdit4("Colour : ", &d->colour.x);
				break;
			}
			}

			ImGui::TreePop();

		}

		ImGui::End();
}

/*------------------------------------------------LightingManager OutputFile()-------------------------------------------------------------*/

void LightingManager::OutputFile(std::string f)
{
	std::filesystem::path path(f);
	if (!std::filesystem::exists(path))
		assert("File does not exist");
	std::ofstream ofs(f);
	cereal::BinaryOutputArchive output(ofs);
	output(dataset);
}

/*------------------------------------------------LightingManager ReadFromFile()-------------------------------------------------------------*/
// Deprecated, now loads the entire stage data along with the lighting data
void LightingManager::ReadFromFile(std::string f)
{
	std::filesystem::path path(f);
	if (!std::filesystem::exists(path))
		assert("File does not exist");
	std::ifstream ifs(f);
	cereal::BinaryInputArchive in(ifs);
	in(dataset);

}

/*------------------------------------------------LightingManager RenderDebug()-------------------------------------------------------------*/

void LightingManager::RenderDebug()
{
}

/*------------------------------------------------LightingManager Name()-------------------------------------------------------------*/

std::string LightingManager::Name(std::shared_ptr<LIGHTING>d)
{
	for (auto& dt : dataset)
		if (dt.second == d)
			return dt.first;
	assert(!"No such light");
	return "";
}

/*------------------------------------------------LightingManager Retrieve()-------------------------------------------------------------*/

std::shared_ptr<LIGHTING> LightingManager::Retrieve(std::string n)
{
	return dataset.find(n)->second;
}


#pragma endregion

