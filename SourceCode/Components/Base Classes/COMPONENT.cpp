#include "COMPONENT.h"
#include "../../GAMEOBJECT.h"
#include "../../Engine/Input.h"
namespace FS = std::filesystem;



//#pragma region CYLINDER_COLLIDER
//CYLINDER_COLLIDER::CYLINDER_COLLIDER(GAMEOBJECT* t, COMPONENT_DATA* data)
//{
//    parent = t;
//    this->data = static_cast<CYLINDER_COLLIDER_DATA*>(data);
//}
//HRESULT CYLINDER_COLLIDER::Initialize()
//{
//    collider = std::make_shared<COLLIDERS::CYLINDER>(data->top, data->bottom, data->radius);
//    cylinder = std::make_shared<DYNAMIC_CYLINDER>();
//    return S_OK;
//}
//void CYLINDER_COLLIDER::Execute()
//{
//    TRANSFORM_3D* transform{ GetComponent<TRANSFORM_3D>() };
//    cylinder->Execute();
//    if (data->bone_name == "")
//        collider->Execute(transform->TransformMatrix());
//    else
//        collider->FitToBone(data->bone_name, GetComponent<MESH>()->Model().get());
//}
//void CYLINDER_COLLIDER::Render()
//{
//    cylinder->Render();
//}
//void CYLINDER_COLLIDER::UI()
//{
//    if (!collider)
//        collider = std::make_shared<COLLIDERS::CYLINDER>(data->top, data->bottom, data->radius);
//    if (ImGui::TreeNode("Cylindrical Collider"))
//    {
//        ImGui::InputText("Collider name", data->name, 256);
//        //ImGui::DragFloat3("Center : ", data->)
//        ImGui::DragFloat("Radius", &data->radius, 0.05f);
//        if (ImGui::Button("Set data to collider"))
//        {
//            collider->SetData(data->top, data->bottom, data->radius);
//            cylinder->UpdateVertices(data->radius, (data->top - data->bottom).Length() / 2.0f);
//        }
//        if (GetComponent<MESH>() != nullptr && GetComponent<MESH>()->Model() != nullptr)
//        {
//            MODEL_RESOURCES& mr{ *GetComponent<MESH>()->Model().get()->Resource().get() };
//            if (ImGui::BeginCombo("Meshes", mr.Meshes[selected_mesh].Name.c_str()))
//            {
//                int ind{};
//                for (auto& m : mr.Meshes)
//                {
//                    bool s{};
//                    if (ImGui::Selectable(m.Name.c_str(), &s))
//                        selected_mesh = ind;
//                    ++ind;
//                }
//                ImGui::EndCombo();
//            }
//            MODEL_RESOURCES::SKELETON& bs{ mr.Meshes[selected_mesh].Bind_Pose };
//            if (ImGui::BeginCombo("Bones", bs.Bones[selected_bone].Name.c_str()))
//            {
//                int ind{};
//                for (auto& b : bs.Bones)
//                {
//                    bool s{};
//                    if (ImGui::Selectable(b.Name.c_str(), &s))
//                        selected_bone = ind;
//                    ++ind;
//                }
//                ImGui::EndCombo();
//            }
//
//            if (ImGui::Button("Set To Bone"))
//                data->bone_name = bs.Bones[selected_bone].Name;
//
//
//        }
//
//        ImGui::TreePop();
//    }
//}
//
//#pragma endregion


