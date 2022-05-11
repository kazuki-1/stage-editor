#include <time.h>
#include "Graphics.h"
#include "DirectX11.h"
#include "Input.h"
#include "Camera.h"
#include "MODEL.h"
#include "Audio.h"
#include "Text.h"
#include "DEBUG_MANAGER.h"
#include "../Scenes/SCENEMANAGER.h"
#include "../Components/Base Classes/ComponentCreator.h"
using namespace DirectX;
std::shared_ptr<MODEL>stage;
HRESULT Graphics::Initialize(int Width, int Height, HWND hwnd)
{


    // DirectX Initialzation
    DirectX11* DX = DirectX11::Instance();
    if (FAILED(DirectX11::Instance()->Initialize(Width, Height, true, hwnd, false, 1000.0f, 0.01f)))
        assert(!"Failed to Initilize DirectX11 Class");

    D3D11_BUFFER_DESC cbd{};
    cbd.ByteWidth = sizeof(SCENE_CONSTANT_DATA);
    cbd.Usage = D3D11_USAGE_DEFAULT;
    cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    HRESULT hr = DX->Device()->CreateBuffer(&cbd, nullptr, dxSceneConstantBuffer.ReleaseAndGetAddressOf());
    if (FAILED(hr))
        assert(!"Failed to create constant buffer");


    // Perform shader initialization by inserting used shaders into the map
    //ShaderManager::Instance()->Initialize();

    // Create a default blend mode for use

    // Initializes anything needed by XAudio2
    //AudioEngine::Instance()->Initialize();

    // Perform Camera Initialization 
    Camera::Instance()->Initialize({ 0, 0, 1 }, { 0, 0, 0 });
    Camera::Instance()->SetRange(10);

    // Perform initialization for the user interface
    //GUI::Instance()->Initialize();

    // Initializes the DepthStencilStateManager by inserting used DSS into the map
    //DepthStencilStateManager::Instance()->Initialize();

    // Insert a directional light into the map for preview
    std::shared_ptr<LIGHTING>data = std::make_shared<LIGHTING>(LIGHTING::L_TYPE::DIRECTIONAL);
    data->SetDirection({ 0, -1, 1 });
    data->SetColour({ 1.0f, 1.0f, 1.0f, 1.0f });

    LightingManager::Instance()->Insert("Default", data);

    // Initialzes the text manager by inserting used fonts into the map
    TextManager::Instance()->Initialize();
    TextManager::Instance()->SetCurrentFont("Font1");

    // Initializes the scene manager by inserting used scenes into the map
    SceneManager::Instance()->Initialize();

    // Initializes the component creator by inserting used component constructors into the map
    ComponentCreator::Instance()->Initialize();

    // Debug primitive initialization
    DebugController::Instance()->Initialize();

    

    // Initializes the white board below
    stage = std::make_shared<MODEL>();
    stage->Initialize("./Data/Model/stage.mrs");
    stage->SetTake(0);
    stage->SetScale({ 20, 1, 20});
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
    DirectX11::Instance()->Begin({ .0f, .0f, .0f, 1.0f });
    
    // Perform execution of camera
    Camera::Instance()->Execute();

    // Scene Constant buffer update (Camera Settings and Light Directions)
    //SCENE_CONSTANT_DATA data;
    XMMATRIX v{ Camera::Instance()->ViewMatrix() }, pr{ DirectX11::Instance()->ProjectionMatrix() };
    DirectX::XMStoreFloat4x4(&scene_data.view_proj, v * pr);


    // Camera Position
    scene_data.camera_position.x = Camera::Instance()->EyePosition().x;
    scene_data.camera_position.y = Camera::Instance()->EyePosition().y;
    scene_data.camera_position.z = Camera::Instance()->EyePosition().z;
    scene_data.camera_position.w = 1;
    scene_data.ambientLightColour = { 0.2f, 0.2f, 0.2f, 1.0f };

    // Write lighting properties into the constant buffer
    LightingManager::Instance()->Retrieve("Default")->WriteBuffer<DLIGHT_DATA>(&scene_data.directional);
    int p_LightCount{}, s_LightCount{};
    for (auto& d : LightingManager::Instance()->Dataset())
    {
        switch (d.second->Type())
        {
        case LIGHTING::L_TYPE::DIRECTIONAL:
            d.second->WriteBuffer<DLIGHT_DATA>(&scene_data.directional);
            break;
        case LIGHTING::L_TYPE::POINT:
            d.second->WriteBuffer<PLIGHT_DATA>(&scene_data.pointlights[p_LightCount]);
            ++p_LightCount;
            break;
        case LIGHTING::L_TYPE::SPOT:
            d.second->WriteBuffer<SLIGHT_DATA>(&scene_data.spotlights[p_LightCount]);
            ++s_LightCount;
            break;
        }
    }
    
    scene_data.pLightCount = p_LightCount;
    scene_data.sLightCount = s_LightCount;

    // Updates the constant buffer and uploads it
    dc->UpdateSubresource(dxSceneConstantBuffer.Get(), 0, 0, &scene_data, 0, 0);
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
    //GUI::Instance()->Finalize();
}