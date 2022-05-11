#include "Shader.h"
#include "../SamplerStateManager.h"
#include "../DepthStencilStateManager.h"
//#include "PhongShader.h"
//#include "../MODEL_RESOURCE.h"


/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------Shader Class--------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------Shader Constructor--------------------------------------------------------------------*/

Shader::Shader(std::wstring path) : shader_path(path)
{


}

/*---------------------------------------------Shader Initialize()--------------------------------------------------------------------*/

HRESULT Shader::Initialize()
{
    return S_OK;
}

/*---------------------------------------------Shader SetShaders--------------------------------------------------------------------*/

void Shader::SetShaders(ID3D11DeviceContext* dc, OBJECT* parent)
{
    dc->VSSetShader(vertexShader.Get(), 0, 0);
    dc->PSSetShader(pixelShader.Get(), 0, 0);
    dc->IASetInputLayout(inputLayout.Get());
    DepthStencilStateManager::Instance()->Set(DepthStencilStateTypes::Default);
    SamplerStateManager::Instance()->Set(SamplerStateType::Default);
    //ID3D11SamplerState* sampler = SamplerStateManager::Instance()->Retrieve(SamplerStateType::Default)->sampler.Get();
    //dc->PSSetSamplers(0, 1, &sampler);

}




