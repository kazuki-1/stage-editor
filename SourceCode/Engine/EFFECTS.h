//#pragma once
//#include<Effekseer\Effekseer.h>
//#include <EffekseerRendererDX11\EffekseerRendererDX11.h>
//#include "Singleton.h"
//#include "DXMath.h"
//using namespace Math;
//class FX
//{
//    //Effekseer::Effect 
//    Effekseer::Effect* effect;
//    Effekseer::Handle handle;
//    VECTOR3 position;
//    float scale;
//public:
//    FX() {};
//
//    HRESULT             Initialize(std::string fx_path);
//    void                Play();
//    void                Stop();
//    void SetPosition(VECTOR3 t);
//    void SetScale(float sc);
//
//    VECTOR3 Position();
//    float Scale();
//    Effekseer::Handle Handle();
//};
//class FX_MANAGER : public SINGLETON<FX_MANAGER>
//{
//    std::map < std::string, std::shared_ptr<FX>>effects;
//    Effekseer::Manager* manager;
//    EffekseerRenderer::Renderer* renderer;
//
//
//public:
//    HRESULT Initialize();
//    void Insert(std::string name, std::shared_ptr<FX>fx);
//    void Execute(float frametime);
//    void Render();
//    void Remove(std::string name);
//    void Finalize();
//
//    Effekseer::Manager* Manager();
//    EffekseerRenderer::Renderer* Renderer();
//    std::shared_ptr<FX>Retrieve(std::string name);
//    std::map < std::string, std::shared_ptr<FX>>Effects();
//
//};