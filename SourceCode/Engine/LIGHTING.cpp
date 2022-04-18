#include "LIGHTING.h"
#include "IMGUI.h"
#include "../Components/Base Classes/Component.h"
#include "../Components/DirectionalLight.h"
#include "../Components/PointLight.h"
#include "../Components/SpotLight.h"
#include <filesystem>
#pragma region LIGHTING

LIGHTING::LIGHTING(L_TYPE t) : type(t)
{
	if (type == L_TYPE::DIRECTIONAL)
		return;
	d_Primitive = std::make_shared<DEBUG_SPHERE>();

}

LIGHTING::L_TYPE LIGHTING::Type()
{
	return type;
}

void LIGHTING::RenderDebug()
{
	if (!d_Primitive)
		return;
	d_Primitive->SetPosition(position);
	d_Primitive->Execute();
	d_Primitive->Render({ 1.0f, 0.0f, 0.0f, 1.0f });
}


#pragma region Setters

void LIGHTING::SetDirection(Vector3 dir)
{
	direction = dir;
}
void LIGHTING::SetPosition(Vector3 pos)
{
	position = pos;
}
void LIGHTING::SetColour(Vector4 col)
{
	colour = col;
}
void LIGHTING::SetInnerCorner(float in)
{
	inner = inner;
}
void LIGHTING::SetOuterCorner(float out)
{
	outer = outer;
}
void LIGHTING::SetRange(float r)
{
	range = r;
}
void LIGHTING::SetType(L_TYPE t)
{
	type = t;
}
void LIGHTING::WriteToData(DirectionalLight_Component* d)
{
	direction = { d->Direction().x, d->Direction().y, d->Direction().z };
	colour = d->Colour();
}
void LIGHTING::WriteToData(PointLight_Component* p)
{
	position = { p->Position().x, p->Position().y, p->Position().z };
	colour = p->Colour();
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

Vector3 LIGHTING::Direction()
{
	return direction;
}
Vector3 LIGHTING::Position()
{
	return position;
}
Vector4 LIGHTING::Colour()
{
	return colour;
}
float LIGHTING::Range()
{
	return range;
}
float LIGHTING::Inner()
{
	return inner;
}
float LIGHTING::Outer()
{
	return outer;
}

#pragma endregion

#pragma endregion
#pragma region LIGHTINGMANAGER

void LightingManager::Insert(std::string n, std::shared_ptr<LIGHTING> l)
{
	dataset.insert(std::make_pair(n, l));
}
void LightingManager::Remove(std::string n)
{
	for (auto d = dataset.begin(); d != dataset.end(); ++d)
	{
		if (n == d->first)
			dataset.erase(d);
	}
}
void LightingManager::Create(std::string n, LIGHTING::L_TYPE t)
{
	std::shared_ptr<LIGHTING>l = std::make_shared<LIGHTING>(t);
	Insert(n, l);
}
void LightingManager::RenderUI()
{
	ImGui::Begin("Stage Settings");
	//ImGui::SetWindowSize({ 300, 900 });
	//ImGui::InputText("Read From File", (char*)input_file.c_str(), 256);
	//if (ImGui::Button("Load"))
	//{
	//	std::filesystem::path path(input_file);
	//	path.replace_extension(".ld");

	//	ReadFromFile(input_file);
	//}


	//if (ImGui::TreeNode("Light"))
	//  {
	//	std::string types[] = { "Directional", "Point", "Spot" };
	//	static LIGHTING::L_TYPE selected_type{ LIGHTING::L_TYPE::DIRECTIONAL};

	//	if (ImGui::BeginCombo("Type : ", types[selected_type].c_str())) 
	//	{
	//			for (int a = 0; a < 3; ++a) {
	//				bool selected{ types[a] == types[selected_type] };
	//				if (ImGui::Selectable(types[a].c_str(), &selected))
	//					selected_type = (LIGHTING::L_TYPE)a;
	//			}
	//			ImGui::EndCombo();
	//		}

	//	ImGui::InputText("Name", (char*)input.c_str(), 256);
	//	if (ImGui::Button("Create ##01"))
	//		{
	//			Create(input, selected_type);
	//		}


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
			//case LIGHTING::L_TYPE::POINT: {
			//	ImGui::InputFloat3("Position : ", &d->position.x);
			//	ImGui::InputFloat("Range : ", &d->range);
			//	ImGui::ColorEdit3("Colour : ", &d->colour.x);
			//	break;
			//}
			//case LIGHTING::L_TYPE::SPOT: {
			//	ImGui::InputFloat3("Position : ", &d->position.x);
			//	ImGui::InputFloat3("Direction : ", &d->direction.x);
			//	ImGui::InputFloat("Range : ", &d->range);
			//	ImGui::InputFloat("Inner Corner : ", &d->inner);
			//	ImGui::InputFloat("Outer Corner: ", &d->outer);
			//	ImGui::ColorEdit4("Colour : ", &d->colour.x);
			//}
			}

			ImGui::TreePop();

		}
		//ImGui::InputText("Create File", (char*)output_file.c_str(), 256);
		//if (ImGui::Button("Output ##01"))
		//{
		//	std::filesystem::path dir("./Generated/Lighting/");
		//	std::string format{ ".ld" };
		//	if (!std::filesystem::exists(dir))
		//		std::filesystem::create_directories(dir);
		//	std::filesystem::path path(dir.string() + output_file);
		//	path.replace_extension(format);
		//	OutputFile(path.string());
		//}

		ImGui::End();
}
void LightingManager::OutputFile(std::string f)
{
	std::filesystem::path path(f);
	if (!std::filesystem::exists(path))
		assert("File does not exist");
	std::ofstream ofs(f);
	cereal::BinaryOutputArchive output(ofs);
	output(dataset);
}
void LightingManager::ReadFromFile(std::string f)
{
	std::filesystem::path path(f);
	if (!std::filesystem::exists(path))
		assert("File does not exist");
	std::ifstream ifs(f);
	cereal::BinaryInputArchive in(ifs);
	in(dataset);

}
void LightingManager::RenderDebug()
{
	//for (auto& d : dataset)
	//	d.second->RenderDebug();
}
std::string LightingManager::Name(std::shared_ptr<LIGHTING>d)
{
	for (auto& dt : dataset)
		if (dt.second == d)
			return dt.first;
	assert(!"No such light");
	return "";
}
std::shared_ptr<LIGHTING> LightingManager::Retrieve(std::string n)
{
	return dataset.find(n)->second;
}


#pragma endregion

