//#include "EFFECTS.h"
//#include "DirectX11.h"
//#include "Camera.h"
//HRESULT FX::Initialize(std::string fx_path)
//{
//    char16_t utf[256];
//    Effekseer::ConvertUtf8ToUtf16(utf, 256, fx_path.c_str());
//    effect = Effekseer::Effect::Create(FX_MANAGER::Instance()->Manager(), (EFK_CHAR*)utf);
//    return S_OK;
//}
//
//void FX::Play()
//{
//    handle = FX_MANAGER::Instance()->Manager()->Play(effect, { position.x, position.y, position.z });
//    FX_MANAGER::Instance()->Manager()->SetScale(handle, scale, scale, scale);
//}
//void FX::Stop()
//{
//    FX_MANAGER::Instance()->Manager()->StopEffect(handle);
//}
//void FX::SetPosition(VECTOR3 t)
//{
//    position = t;
//}
//void FX::SetScale(float s)
//{
//    scale = s;
//}
//VECTOR3 FX::Position()
//{
//    return position;
//}
//float FX::Scale()
//{
//    return scale;
//}
//Effekseer::Handle FX::Handle()
//{
//    return handle;
//}
//
//HRESULT FX_MANAGER::Initialize()
//{
//    DirectX11* dx{ DirectX11::Instance() };
//    renderer = EffekseerRendererDX11::Renderer::Create(dx->Device(), dx->DeviceContext(), 2048);
//    manager = Effekseer::Manager::Create(2048);
//
//    manager->SetSpriteRenderer(renderer->CreateSpriteRenderer());
//    manager->SetRibbonRenderer(renderer->CreateRibbonRenderer());
//    manager->SetRingRenderer(renderer->CreateRingRenderer());
//    manager->SetTrackRenderer(renderer->CreateTrackRenderer());
//    manager->SetModelRenderer(renderer->CreateModelRenderer());
//    manager->SetTextureLoader(renderer->CreateTextureLoader());
//    manager->SetModelLoader(renderer->CreateModelLoader());
//    manager->SetMaterialLoader(renderer->CreateMaterialLoader());
//    manager->SetCoordinateSystem(Effekseer::CoordinateSystem::LH);
//    return S_OK;
//}
//void FX_MANAGER::Execute(float frametime)
//{
//    manager->Update(frametime * 60);
//}
//void FX_MANAGER::Insert(std::string name, std::shared_ptr<FX> fx)
//{
//    effects.emplace(name, fx);
//}
//void FX_MANAGER::Render()
//{
//    XMMATRIX v, p;
//    v = Camera::Instance()->ViewMatrix();
//    p = DirectX11::Instance()->ProjectionMatrix();
//    renderer->SetCameraMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&v));
//    renderer->SetProjectionMatrix(*reinterpret_cast<const Effekseer::Matrix44*>(&p));
//    renderer->BeginRendering();
//    manager->Draw();
//    renderer->EndRendering();
//}
//void FX_MANAGER::Remove(std::string name)
//{
//    effects.erase(name);
//}
//void FX_MANAGER::Finalize()
//{
//    manager->Destroy();
//    renderer->Destroy();
//}
//Effekseer::Manager* FX_MANAGER::Manager()
//{
//    return manager;
//}
//EffekseerRenderer::Renderer* FX_MANAGER::Renderer()
//{
//    return renderer;
//}
//std::shared_ptr<FX>FX_MANAGER::Retrieve(std::string name)
//{
//    return effects.find(name)->second;
//}
//std::map < std::string, std::shared_ptr<FX>>FX_MANAGER::Effects()
//{
//    return effects;
//}