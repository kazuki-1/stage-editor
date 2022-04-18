#include <time.h>
#include "Graphics.h"
#include "DirectX11.h"
#include "Sprite.h"
#include "Model.h"
#include "Input.h"
#include "Camera.h"
#include "BlendMode.h"
#include "MODEL.h"
#include "IMGUI.h"
#include "Audio.h"
#include "Text.h"
#include "../GUI.h"
#include "../Scenes/SCENEMANAGER.h"
using namespace DirectX;
std::shared_ptr<MODEL>stage;

HRESULT Graphics::Initialize(int Width, int Height, HWND hwnd)
{


    // DirectX Initialzation
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

    // Perform shader initialization by inserting used shaders into the map
    ShaderManager::Instance()->Initialize();

    // Create a default blend mode for use
    BlendModeManager::Instance()->Create(BlendModeManager::BLEND_MODE::ALPHA, DirectX11::Instance()->Device());

    // Initializes anything needed by XAudio2
    AudioEngine::Instance()->Initialize();

    // Perform Camera Initialization 
    Camera::Instance()->Initialize({ 0, 0, 1 }, { 0, 0, 0 });
    Camera::Instance()->SetRange(10);

    // Perform initialization for the user interface
    GUI::Instance()->Initialize();

    // Insert a directional light into the map for preview
    std::shared_ptr<LIGHTING>data = std::make_shared<LIGHTING>(LIGHTING::L_TYPE::DIRECTIONAL);
    data->SetDirection({ 0, -1, 1 });
    data->SetColour({ 1.0f, 1.0f, 1.0f, 1.0f });

    LightingManager::Instance()->Insert("Default", data);

    // Initialzes the text manager by inserting used fonts into the map
    TextManager::Instance()->Initialize();

    // Initialzes the scene manager by inserting used scenes into the map
    SceneManager::Instance()->Initialize();



    // Initializes the white board below
    stage = std::make_shared<MODEL>();
    stage->Initialize("./Data/Model/stage.mrs");
    stage->SetTake(0);
    stage->SetScale({ 20, 20, 20});
    stage->SetTranslation({ 0, -20, 0 });





    return S_OK;
}

bool Graphics::Frame()
{

    SceneManager::Instance()->Execute();
    stage->UpdateTransform();

    //if (INPUTMANAGER::Instance()->Keyboard()->Triggered('L'))
    //    AudioEngine::Instance()->Retrieve("Test2")->Play();


#pragma region CALLS RENDER (DO NOT PUT FUNCTIONS BELOW HERE)



    if (Render())
        return false;
    return true;

#pragma endregion
}

bool Graphics::Render()
{
    // Prepares the viewport
    ID3D11DeviceContext* dc{ DirectX11::Instance()->DeviceContext() };
    DirectX11::Instance()->Begin({ .2f, .2f, .2f, 1.0f });
    
    // Perform execution of camera
    Camera::Instance()->Execute();

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

    // Write lighting properties into the constant buffer
    LightingManager::Instance()->Retrieve("Default")->WriteBuffer<DLIGHT_DATA>(&data.directional);
    int p_LightCount{}, s_LightCount{};
    for (auto& d : LightingManager::Instance()->Dataset())
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

    // Updates the constant buffer and uploads it
    dc->UpdateSubresource(dxSceneConstantBuffer.Get(), 0, 0, &data, 0, 0);
    dc->VSSetConstantBuffers(0, 1, dxSceneConstantBuffer.GetAddressOf());
    dc->PSSetConstantBuffers(0, 1, dxSceneConstantBuffer.GetAddressOf());


    stage->Render(0.0f, { 1.0f, 1.0f, 1.0f, 0.7f });


    Camera::Instance()->Render();
    SceneManager::Instance()->Render();
    LightingManager::Instance()->RenderDebug();
#pragma endregion

    INPUTMANAGER::Instance()->Execute();


    DirectX11::Instance()->End();


    return true;
}

void Graphics::Finalize()
{
    GUI::Instance()->Finalize();
}