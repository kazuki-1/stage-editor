#include "Graphics.h"
#include "DirectX11.h"
#include "Sprite.h"
#include "Model.h"
#include "Input.h"
#include "Camera.h"
#include "BlendMode.h"
#include "MODEL.h"
#include "IMGUI.h"
#include "../GUI.h"
#include "../SCENEMANAGER.h"
#include "Audio.h"
#include <time.h>
using namespace DirectX;
std::shared_ptr<MODEL>stage;

HRESULT Graphics::Initialize(int Width, int Height, HWND hwnd)
{
#pragma region SETTING INITIALIZATION
    DirectX11* DX = DirectX11::Instance();
    if (FAILED(DirectX11::Instance()->Initialize(Width, Height, true, hwnd, false, 10000.0f, 0.001f)))
        assert(!"Failed to Initilize DirectX11 Class");

    D3D11_BUFFER_DESC cbd{};
    cbd.ByteWidth = sizeof(SCENE_CONSTANT_DATA);
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    HRESULT hr = DX->Device()->CreateBuffer(&cbd, nullptr, dxSceneConstantBuffer.ReleaseAndGetAddressOf());
    if (FAILED(hr))
        assert(!"Failed to create constant buffer");
#pragma endregion
#pragma region SHADER SETTINGS
    SHADERMANAGER::Instance()->Initialize();
    AUDIOENGINE::Instance()->Initialize();
#pragma endregion
    BLENDMODE::Instance()->Create(BLENDMODE::BLEND_MODE::ALPHA, DirectX11::Instance()->Device());
    Camera::Instance()->Initialize({ 0, 0, 1 }, { 0, 0, 0 });
    Camera::Instance()->SetRange(10);
    GUI::Instance()->Initialize();
    std::shared_ptr<LIGHTING>data = std::make_shared<LIGHTING>(LIGHTING::L_TYPE::DIRECTIONAL);
    data->SetDirection({ 0, -1, 1 });
    data->SetColour({ 1.0f, 1.0f, 1.0f, 1.0f });
    LIGHTINGMANAGER::Instance()->Insert("Default", data);
    stage = std::make_shared<MODEL>();
    stage->Initialize("./Data/Model/stage.mrs");
    //stage->Resource()->SetShader(L"Base3DShader.fx");
    SCENEMANAGER::Instance()->Initialize();
    stage->SetTake(0);
    stage->SetScale({ 20, 20, 20});
    stage->SetTranslation({ 0, -20, 0 });
    //AUDIOENGINE::Instance()->Retrieve("Test")->Play();
    return S_OK;
}

bool Graphics::Frame()
{

    //GUI::Instance()->Execute();
    SCENEMANAGER::Instance()->Execute();
    stage->UpdateTransform();

    //if (INPUTMANAGER::Instance()->Keyboard()->Triggered('L'))
    //    AUDIOENGINE::Instance()->Retrieve("Test2")->Play();


#pragma region CALLS RENDER (DO NOT PUT FUNCTIONS BELOW HERE)



    if (Render())
        return false;
    return true;

#pragma endregion
}

bool Graphics::Render()
{

#pragma region BASE SETTINGS
    ID3D11DeviceContext* dc{ DirectX11::Instance()->DeviceContext() };
    DirectX11::Instance()->Begin({ .2f, .2f, .2f, 1.0f });

#pragma endregion
#pragma region CAMERA SETTINGS
    Camera::Instance()->Execute();
#pragma endregion
#pragma region SCENE_CONSTANTS

    // Scene Constant buffer update (Camera Settings and Light Directions)
    SCENE_CONSTANT_DATA data;
    XMMATRIX v{ Camera::Instance()->ViewMatrix() }, pr{ DirectX11::Instance()->ProjectionMatrix() };
    DirectX::XMStoreFloat4x4(&data.view_proj, v * pr);


    // Camera Position
    data.camera_position.x = Camera::Instance()->EyePosition().x;
    data.camera_position.y = Camera::Instance()->EyePosition().y;
    data.camera_position.z = Camera::Instance()->EyePosition().z;
    data.camera_position.w = 1;
    data.ambientLightColour = { 0.2f, 0.2f, 0.2f, 1.0f };

    // Light Direction
    //data.light_dir = { 0.0f, -1.0f, -1.0f, 1 };

    LIGHTINGMANAGER::Instance()->Retrieve("Default")->WriteBuffer<DLIGHT_DATA>(&data.directional);
    int p_LightCount{}, s_LightCount{};
    for (auto& d : LIGHTINGMANAGER::Instance()->Dataset())
    {
        switch (d.second->Type())
        {
        case LIGHTING::L_TYPE::DIRECTIONAL:
            d.second->WriteBuffer<DLIGHT_DATA>(&data.directional);
            break;
        case LIGHTING::L_TYPE::POINT:
            d.second->WriteBuffer<PLIGHT_DATA>(&data.pointlights[p_LightCount]);
            ++data.pLightCount;
            break;
        case LIGHTING::L_TYPE::SPOT:
            d.second->WriteBuffer<SLIGHT_DATA>(&data.spotlights[p_LightCount]);
            ++data.sLightCount;
            break;
        }
    }

    dc->UpdateSubresource(dxSceneConstantBuffer.Get(), 0, 0, &data, 0, 0);
    dc->VSSetConstantBuffers(0, 1, dxSceneConstantBuffer.GetAddressOf());
    dc->PSSetConstantBuffers(0, 1, dxSceneConstantBuffer.GetAddressOf());
#pragma endregion

#pragma region PUT RENDER FUNCTIONS HERE 

    stage->Render(0.0f, { 1.0f, 1.0f, 1.0f, 0.7f });
    Camera::Instance()->Render();
    //GUI::Instance()->Render();
    SCENEMANAGER::Instance()->Render();
    LIGHTINGMANAGER::Instance()->RenderDebug();
#pragma endregion

    INPUTMANAGER::Instance()->Execute();
    DirectX11::Instance()->End();









    return true;
}

void Graphics::Finalize()
{
    GUI::Instance()->Finalize();
}