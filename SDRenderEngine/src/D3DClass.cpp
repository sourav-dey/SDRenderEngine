#include "D3DClass.h"
#include "Utilities.h"

D3DClass::D3DClass()
  : m_swapChain(nullptr)
  , m_device(nullptr)
  , m_deviceContext(nullptr)
  , m_renderTargetView(nullptr)
  , m_depthStencilBuffer(nullptr)
  , m_depthStencilState(nullptr)
  , m_depthStencilView(nullptr)
  , m_rasterState(nullptr)
{
}

D3DClass::D3DClass(const D3DClass&)
{
}

D3DClass::~D3DClass()
{
}

void D3DClass::getRefreshRate(unsigned screenWidth, unsigned screenHeight)
{
  IDXGIFactory *factory = nullptr;

  Utilities ut;
  if ( !CreateDXGIFactory( __uuidof( IDXGIFactory ), reinterpret_cast<void**>(&factory) ) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return;
  }

  IDXGIAdapter *adapter = nullptr;
  if ( !factory->EnumAdapters(0, &adapter) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return;
  }

  IDXGIOutput *adapterOutput = nullptr;
  if ( !adapter->EnumOutputs(0, &adapterOutput) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return;
  }

  UINT numModes;
  if ( FAILED(adapterOutput->GetDisplayModeList(DXGI_FORMAT_A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, NULL)) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
  }

  DXGI_MODE_DESC *displayModeList = new DXGI_MODE_DESC[numModes];
  if ( !displayModeList )
  {
    return;
  }

  unsigned int numerator = 0, denominator = 0;
  for( int i = 0; i < numModes; ++i )
  {
    if ( displayModeList[i].Width == static_cast< unsigned int >( screenWidth ) && displayModeList[i].Height == static_cast<  unsigned int > ( screenHeight ) )
    {
      numerator = displayModeList[i].RefreshRate.Numerator;
      denominator = displayModeList[i].RefreshRate.Denominator;
    }
  }

  DXGI_ADAPTER_DESC adapterDesc;
  if ( FAILED( adapter->GetDesc(&adapterDesc)) )
  {
    return;
  }

  m_videoCardMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

  unsigned long long stringLength;
  if ( !wcstombs_s(&stringLength, m_videoCardDescription, 128, adapterDesc.Description, 128))
  {
    return;
  }

  delete[] displayModeList;
  displayModeList = NULL;

  adapterOutput->Release( );
  adapterOutput = NULL;

  adapter->Release( );
  adapter = NULL;

  factory->Release( );
  factory = NULL;
}

bool D3DClass::initialize(unsigned screenWidth, unsigned screenHeight, bool vSyncEnabled, HWND hwnd, bool isFullScreen, const float screenDepth, const float screenNear)
{
  m_vSyncEnabled = vSyncEnabled;
  getRefreshRate(screenWidth, screenHeight);
  Utilities ut;

  UINT createDeviceFlags = 0;
  
#if defined(DEBUG) || defined(_DEBUG)
  createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

  D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;

  HRESULT hr = D3D11CreateDevice( NULL
    , D3D_DRIVER_TYPE_HARDWARE
    , NULL
    , createDeviceFlags
    , &feature_level
    , 1
    , D3D11_SDK_VERSION
    , &m_device
    , &feature_level
    , &m_deviceContext );

  if ( FAILED(hr) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }
}

void D3DClass::shutDown()
{
}

void D3DClass::beginScene(const float red, const float green, const float blue)
{
}

void D3DClass::endScene()
{
}

ID3D11Device* D3DClass::getDevice()
{
}

ID3D11DeviceContext* D3DClass::getDeviceContext()
{
}

void D3DClass::getProjectionmatrix(XMMATRIX&)
{
}

void D3DClass::getWorldMain(XMMATRIX&)
{
}

void D3DClass::getOrthoMatrix(XMMATRIX&)
{
}

void D3DClass::getVideoCardInfo(char*, int&)
{
}
