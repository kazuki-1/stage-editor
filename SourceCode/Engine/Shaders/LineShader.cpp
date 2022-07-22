#include "LineShader.h"
#include "../RASTERIZER.h"
#include "../BlendMode.h"
#include "../DEBUG_PRIMITIVE.h"
#include "../Math.h"
#include "../OBJECT.h"
#include "ShaderManager.h"
/*------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------LineShader Class------------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------LineShader Render()---------------------------------------------*/
/*------------------------------------------------------------------------------------------------------------------------------*/

void LineShader::Render()
{
    ID3D11DeviceContext* dc{ DirectX11::Instance()->DeviceContext() };
    SetShaders();
    SetConstantBuffers();
    UINT stride{ sizeof(DYNAMIC_DEBUG_PRIMITIVE::VERTEX)}, offset{};

    dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
    RasterizerManager::Instance()->Set(RasterizerTypes::Wireframe);
    BlendStateManager::Instance()->Set(BlendModes::Alpha);
    int index{};
    for (auto& object : objects)
    {
        if (!object)
        {
            objects.erase(objects.begin() + index);
            continue;
        }
        if (!object->GetRenderState())
            continue;
        DYNAMIC_DEBUG_PRIMITIVE* debugModel = (DYNAMIC_DEBUG_PRIMITIVE*)object;
        debugModel->Render();
        Vector4 rotation;

        //XMStoreFloat4x4(&debugModel->debugMeshData.world, debugModel->GetWorldTransform());
        debugModel->debugMeshData.world = debugModel->GetWorldTransform();


        UpdateConstantBuffers(object);
        dc->IASetVertexBuffers(0, 1, debugModel->GetVertexBuffer().GetAddressOf(), &stride, &offset);
        dc->IASetIndexBuffer(debugModel->GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
        dc->DrawIndexed((UINT)debugModel->GetIndices().size(), 0, 0);
        ++index;

    }

}

/*--------------------------------------------------------------LineShader UpdateConstantBuffers()---------------------------------------------*/

void LineShader::UpdateConstantBuffers(OBJECT* parent)
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();

    DYNAMIC_DEBUG_PRIMITIVE* debugModel = ((DYNAMIC_DEBUG_PRIMITIVE*)parent);


    CBuffer_Mesh data = { debugModel->debugMeshData.world, {1.0f, 1.0f, 1.0f, 1.0f}};
    dc->UpdateSubresource(meshConstantBuffer.Get(), 0, 0, &data, 0, 0);

}
