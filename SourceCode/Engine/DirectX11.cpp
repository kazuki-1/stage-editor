#include "DirectX11.h"
#include "IMGUI.h"
#include <string>
#include "Rasterizer.h"
#include "SamplerStateManager.h"
#include "DepthStencilStateManager.h"
#include "BlendMode.h"
#include "Shaders/ShaderManager.h"
#include "Audio.h"
#include "AudioEngine.h"

int* test;
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------DirectX11 Class-----------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------------DirectX11 Initialize()----------------------------------------------------------------*/

HRESULT DirectX11::Initialize(int Width, int Height, bool VSYNC, HWND hwnd, bool isFullScreen, float Depth, float ScreenNear)
{
    HRESULT hr{ S_OK };
    vSync = VSYNC;
    UINT numModes{}, numerator{}, denominator{};
    hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)dxFactory.GetAddressOf());
    assert(hr == S_OK);

    hr = dxFactory->EnumAdapters(0, dxAdapter.GetAddressOf());
    assert(hr == S_OK);

    hr = dxAdapter->EnumOutputs(0, dxAdapterOutput.GetAddressOf());
    assert(hr == S_OK);

    hr = dxAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, 0);
    assert(hr == S_OK);

    DXGI_MODE_DESC* modes{};
    modes = new DXGI_MODE_DESC[numModes];
    if (!modes)
        assert(!"DXGI Mode creation failed");

    hr = dxAdapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, modes);
    assert(hr == S_OK);

    for (int a = 0; a < (int)numModes; ++a)
    {
        if (modes[a].Width == Width && modes[a].Height == Height)
        {
            numerator = modes[a].RefreshRate.Numerator;
            denominator = modes[a].RefreshRate.Denominator;
        }
    }

    // Adapter Desc
    DXGI_ADAPTER_DESC dad;
    hr = dxAdapter->GetDesc(&dad);
    assert(hr == S_OK);

    GPUMemory = (int)(dad.DedicatedVideoMemory / 1024 / 1024);


    UINT deviceFlags{ 0 };
#ifdef _DEBUG
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


    // Swap Chain Desc
    DXGI_SWAP_CHAIN_DESC scd{};

    scd.BufferCount = 1;
    scd.BufferDesc.Width = Width;
    scd.BufferDesc.Height = Height;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = hwnd;
    scd.SampleDesc.Count = 1;
    scd.SampleDesc.Quality = 0;
    scd.Windowed = isFullScreen ? false : true;
    scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    scd.Flags = 0;
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;
    hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, deviceFlags, &featureLevel, 1, D3D11_SDK_VERSION, &scd, dxSwapChain.GetAddressOf(), dxDevice.GetAddressOf(), 0, dxDeviceContext.GetAddressOf());
    assert(hr == S_OK);

    test = new int[1232];


    ID3D11Texture2D* backBuffer;
    hr = dxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    assert(hr == S_OK);

    hr = dxDevice->CreateRenderTargetView(backBuffer, 0, dxRenderTargetView.GetAddressOf());
    assert(hr == S_OK);
    backBuffer->Release();


    // Depth Stencil Buffer
    D3D11_TEXTURE2D_DESC dbd{};
    dbd.Width = Width;
    dbd.Height = Height;
    dbd.MipLevels = 1;
    dbd.ArraySize = 1;
    dbd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dbd.SampleDesc.Count = 1;
    dbd.SampleDesc.Quality = 0;
    dbd.Usage = D3D11_USAGE_DEFAULT;
    dbd.CPUAccessFlags = 0;
    dbd.MiscFlags = 0;
    dbd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    hr = dxDevice->CreateTexture2D(&dbd, NULL, dxDepthStencilBuffer.GetAddressOf());
    assert(hr == S_OK);


    D3D11_DEPTH_STENCIL_VIEW_DESC dsvd{};
    dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvd.Texture2D.MipSlice = 0;
    hr = dxDevice->CreateDepthStencilView(dxDepthStencilBuffer.Get(), &dsvd, dxDepthStencilView.GetAddressOf());
    dxDeviceContext->OMSetRenderTargets(1, dxRenderTargetView.GetAddressOf(), dxDepthStencilView.Get());


    // Viewport
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.Width = (float)Width;
    viewport.Height = (float)Height;
    dxDeviceContext->RSSetViewports(1, &viewport);


    screen_dimension = { (float)Width, (float)Height };


    // Creates used rasterizers
    RasterizerManager::Instance()->Initialize();

    // Creates used samplerStates
    SamplerStateManager::Instance()->Initialize();

    // Creates used DepthStencilStates
    DepthStencilStateManager::Instance()->Initialize();

    // Create used BlendModes
    BlendStateManager::Instance()->Initialize();
    
    // Create used shaders
    ShaderManager::Instance()->Initialize();
    
    // Initializes XAudio2 
    //AudioEngine::Instance()->Initialize();




    float fov = XM_PI / 4.0f;
    float aspect = (float)Width / (float)Height;

    PR = XMMatrixPerspectiveFovLH(fov, aspect, ScreenNear, Depth);
    OR = XMMatrixOrthographicLH((float)Width, (float)Height, ScreenNear, Depth);
    WO = XMMatrixIdentity();
    XMStoreFloat4x4(&projection, PR);
    XMStoreFloat4x4(&ortho, OR);
    XMStoreFloat4x4(&world, WO);



    hr = CoInitializeEx(0, COINIT_MULTITHREADED);
    return hr;
}

/*-----------------------------------------------------------DirectX11 Begin()----------------------------------------------------------------*/

void DirectX11::Begin(XMFLOAT4 colour)
{
    float colours[4]{ colour.x, colour.y, colour.z, colour.w };
    dxDeviceContext->ClearRenderTargetView(dxRenderTargetView.Get(), colours);
    dxDeviceContext->ClearDepthStencilView(dxDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    dxDeviceContext->OMSetRenderTargets(1, dxRenderTargetView.GetAddressOf(), dxDepthStencilView.Get());

}

/*-----------------------------------------------------------DirectX11 End()----------------------------------------------------------------*/

void DirectX11::End()
{
    vSync ? dxSwapChain->Present(1, 0) : dxSwapChain->Present(0, 0);
}