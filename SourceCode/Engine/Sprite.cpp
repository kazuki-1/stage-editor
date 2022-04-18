#include "RASTERIZER.h"
#include "Sprite.h"
#include "BlendMode.h"
SPRITE::SPRITE(const wchar_t* img_path)
{
    HRESULT hr{ S_OK };
    Vertices.resize(4);
    
    Vertices[0] = { { -0.5,   0.5, 0}, {1, 1, 1, 1}, {0, 0} };
    Vertices[1] = { {  0.5,   0.5, 0}, {1, 1, 1, 1}, {1, 0} };
    Vertices[2] = { {  0.5, - 0.5, 0}, {1, 1, 1, 1}, {1, 1} };
    Vertices[3] = { { -0.5,  -0.5, 0}, {1, 1, 1, 1}, {0, 1} };
    

    indices = new int[6]
    {
        0, 1, 2,
        2, 3, 0
    };

    vertexCount = (int)Vertices.size();
    indexCount = 6;
    // Vertex Buffer Desc and Index Buffer Desc
    D3D11_BUFFER_DESC vbd{}, ibd{};
    // Vertex and Index Buffer Subresource Data
    D3D11_SUBRESOURCE_DATA vd{}, id{};

    /*-------------------------------------Vertex Buffer Creation--------------------------------------------*/
    vbd.ByteWidth = (UINT)(sizeof(VERTEX) * Vertices.size());
    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vd.pSysMem = Vertices.data();

    ID3D11Device* dv = DirectX11::Instance()->Device();
    hr = dv->CreateBuffer(&vbd, &vd, dxVertexBuffer.GetAddressOf());
    if (FAILED(hr))
        assert(!"Failed to create Vertex Buffer");
    /*-------------------------------------End of Vertex Buffer Creation--------------------------------------------*/


    /*-------------------------------------Index Buffer Creation--------------------------------------------*/

    ibd.ByteWidth = sizeof(int) * indexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;

    id.pSysMem = indices;
    hr = dv->CreateBuffer(&ibd, &id, dxIndexBuffer.GetAddressOf());
    if (FAILED(hr))
        assert(!"Failed to create Index Buffer");
    /*-------------------------------------End of Index Buffer Creation--------------------------------------------*/





    // Look for existing texture in TextureManager and create new if non-existant
    // TEXTURE_MANAGERの中から既存のテキスチャを探す。なければ新規生成
    texture = TextureManager::Instance()->Retrieve(img_path);

    // Insert shader from ShaderManager and create new if non-existant
    // SHADERMANAGERの中から既存のシェーダーを探す。なければ新規生成
    InsertShader(L"Shader_2d.fx");



}

HRESULT SPRITE::Initialize(const wchar_t* img_path)
{
    HRESULT hr{ S_OK };
    Vertices.resize(4);

    Vertices[0] = { { -0.5,   0.5, 0}, {1, 1, 1, 1}, {0, 0} };
    Vertices[1] = { {  0.5,   0.5, 0}, {1, 1, 1, 1}, {1, 0} };
    Vertices[2] = { {  0.5, -0.5, 0}, {1, 1, 1, 1}, {1, 1} };
    Vertices[3] = { { -0.5,  -0.5, 0}, {1, 1, 1, 1}, {0, 1} };


    indices = new int[6]
    {
        0, 1, 2,
        2, 3, 0
    };

    vertexCount = (int)Vertices.size();
    indexCount = 6;
    // Vertex Buffer Desc and Index Buffer Desc
    D3D11_BUFFER_DESC vbd{}, ibd{};
    // Vertex and Index Buffer Subresource Data
    D3D11_SUBRESOURCE_DATA vd{}, id{};

    ID3D11Device* dv = DirectX11::Instance()->Device();

    /*-------------------------------------Vertex Buffer Creation--------------------------------------------*/
    vbd.ByteWidth = (UINT)(sizeof(VERTEX) * Vertices.size());
    vbd.Usage = D3D11_USAGE_DYNAMIC;
    vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vd.pSysMem = Vertices.data();


    hr = dv->CreateBuffer(&vbd, &vd, dxVertexBuffer.GetAddressOf());
    if (FAILED(hr))
        assert(!"Failed to create Vertex Buffer");
    /*-------------------------------------End of Vertex Buffer Creation--------------------------------------------*/


    /*-------------------------------------Index Buffer Creation--------------------------------------------*/

    ibd.ByteWidth = sizeof(int) * indexCount;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.Usage = D3D11_USAGE_DEFAULT;

    id.pSysMem = indices;
    hr = dv->CreateBuffer(&ibd, &id, dxIndexBuffer.GetAddressOf());
    if (FAILED(hr))
        assert(!"Failed to create Index Buffer");
    /*-------------------------------------End of Index Buffer Creation--------------------------------------------*/



    // Look for existing texture in TextureManager and create new if non-existant
    // TEXTURE_MANAGERの中から既存のテキスチャを探す。なければ新規生成
    texture = TextureManager::Instance()->Retrieve(img_path);

    // Insert shader from ShaderManager and create new if non-existant
    // SHADERMANAGERの中から既存のシェーダーを探す。なければ新規生成
    InsertShader(L"Shader_2d.fx");



    return S_OK;
}



void SPRITE::Render(Vector2 position, Vector2 scale , Vector2 tPos, Vector2 tSize, Vector2 pivot, Vector4 colour, float angle)
{
    ID3D11DeviceContext* dc = DirectX11::Instance()->DeviceContext();
    dc->RSSetState(RASTERIZERMANAGER::Instance()->Retrieve("2D")->Rasterizer().Get());

    D3D11_TEXTURE2D_DESC t2d{};
    texture->QueryTextureDesc(&t2d);
    D3D11_VIEWPORT vp{};
    UINT vpn{ 1 };
    dc->RSGetViewports(&vpn, &vp);

    if (tSize.x == 0 && tSize.y == 0)
    {
        tSize.x = (float)t2d.Width;
        tSize.y = (float)t2d.Height;
    }

    Vertices[0].uv = { tPos.x, tPos.y };
    Vertices[1].uv = { tPos.x + tSize.x, tPos.y };
    Vertices[2].uv = { tPos.x + tSize.x, tPos.y + tSize.y };
    Vertices[3].uv = { tPos.x, tPos.y + tSize.y };


    XMFLOAT2 t_pos[4];

    t_pos[0] = { 0, 0 };
    t_pos[1] = { tSize.x, 0 };
    t_pos[2] = { tSize.x, tSize.y };
    t_pos[3] = { 0, tSize.y };

    int ind{};
    for (auto& a : Vertices)
    {
        // Colour and UV transposition
        a.colour = colour;
        a.uv.x /= t2d.Width;
        a.uv.y /= t2d.Height;


        // Scaling the pivot
        pivot.x *= scale.x;
        pivot.y *= scale.y;

        // Casting the image size onto the Vertex positions (and checking for pivots)
        a.position = { t_pos[ind].x * scale.x, t_pos[ind].y * scale.y, 0 };
        if (pivot.x || pivot.y)
        {
            a.position.x -= pivot.x;
            a.position.y -= pivot.y;
        }


        // Rotation
        XMFLOAT2 temp_pos = { a.position.x, a.position.y };
        a.position.x = temp_pos.x * cosf(angle) - temp_pos.y * sinf(angle);
        a.position.y = temp_pos.x * sinf(angle) + temp_pos.y * cosf(angle);

        // Translation
        a.position.x += position.x;
        a.position.y += position.y;

        a.position.x = 2.0f * a.position.x / vp.Width - 1.0f;
        a.position.y = 1.0f - 2.0f * a.position.y / vp.Height;
        ++ind;
    }


    
    D3D11_MAPPED_SUBRESOURCE data{};
    dc->Map(dxVertexBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &data);
    memcpy(data.pData, Vertices.data(), sizeof(VERTEX) * Vertices.size());
    dc->Unmap(dxVertexBuffer.Get(), 0);
    UINT stride{ sizeof(VERTEX) }, offset{ 0 };
    for (auto& s : shaders)
    {
        
        s.second->SetShaders(dc);
        dc->OMSetBlendState(BlendModeManager::Instance()->Get().Get(), 0, 0xffffffff);
        dc->IASetVertexBuffers(0, 1, dxVertexBuffer.GetAddressOf(), &stride, &offset);
        dc->IASetIndexBuffer(dxIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, offset);
        dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        texture->SetTexture(dc);
        dc->DrawIndexed(6, 0, 0);
    }

}