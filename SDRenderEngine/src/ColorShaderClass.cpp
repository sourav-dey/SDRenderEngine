#include "ColorShaderClass.h"

bool ColorShaderClass::initializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFileName, WCHAR* psFileName)
{
}

void ColorShaderClass::shutdownShader()
{
}

void ColorShaderClass::outputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFileName)
{
}

void ColorShaderClass::setShaderParams(ID3D11DeviceContext* context, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
}

bool ColorShaderClass::renderShader(ID3D11DeviceContext* context, int indexCount)
{
  return true;
}

ColorShaderClass::ColorShaderClass()
  : m_vertexShader( nullptr )
  , m_pixelShader( nullptr )
  , m_layout( nullptr )
  , m_buffer( nullptr )
{
}

ColorShaderClass::ColorShaderClass(const ColorShaderClass&)
{
}

ColorShaderClass::~ColorShaderClass()
{
}

bool ColorShaderClass::initialize(ID3D11Device* device, HWND hwnd)
{
  if ( !initializeShader( device, hwnd, L"/src/Shaders/Color.vs", L"/src/Shaders/color.ps" ) )
  {
    printf( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }
  return true;
}

void ColorShaderClass::shutdown()
{
  shutdownShader( );
}

void ColorShaderClass::render(ID3D11DeviceContext* deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix)
{
  bool result = setShaderParams( deviceContext, worldMatrix, viewMatrix, projectionMatrix );
}
