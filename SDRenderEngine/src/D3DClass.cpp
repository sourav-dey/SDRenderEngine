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
  , m_refreshRateNumerator( 0 )
  , m_refreshRateDenominator( 0 )
  , m_enableMSAA4xQuality( false )
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

  for( int i = 0; i < numModes; ++i )
  {
    if ( displayModeList[i].Width == static_cast< unsigned int >( screenWidth ) && displayModeList[i].Height == static_cast<  unsigned int > ( screenHeight ) )
    {
      m_refreshRateNumerator = displayModeList[i].RefreshRate.Numerator;
      m_refreshRateDenominator = displayModeList[i].RefreshRate.Denominator;
    }
  }

  DXGI_ADAPTER_DESC adapterDesc;
  if ( FAILED( adapter->GetDesc(&adapterDesc)) )
  {
    return;
  }

  m_videoCardMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

  size_t stringLength;
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

  D3D_FEATURE_LEVEL feature_level;

  HRESULT hr = D3D11CreateDevice( nullptr
    , D3D_DRIVER_TYPE_HARDWARE
    , nullptr
    , createDeviceFlags
    , nullptr
    , 0
    , D3D11_SDK_VERSION
    , &m_device
    , &feature_level
    , &m_deviceContext );

  if ( FAILED(hr) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }

  if ( feature_level != D3D_FEATURE_LEVEL_11_0 )
  {
    MessageBox( nullptr, "Direct3D Feature Level 11 unsupported", nullptr, 0 );
  }

  // Check 4X MSAA Quality Support
  UINT m4xMsaaQuality;
  hr = m_device->CheckMultisampleQualityLevels( DXGI_FORMAT_B8G8R8A8_UNORM, 4, &m4xMsaaQuality );
  assert( m4xMsaaQuality > 0 );

  // Creating swap chain
  DXGI_SWAP_CHAIN_DESC swapChainDesc;
  swapChainDesc.BufferDesc.Width = screenWidth;
  swapChainDesc.BufferDesc.Height = screenHeight;
  swapChainDesc.BufferDesc.RefreshRate.Numerator = m_refreshRateNumerator;
  swapChainDesc.BufferDesc.RefreshRate.Denominator = m_refreshRateDenominator;
  swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
  swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
  swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

  if ( m_enableMSAA4xQuality )
  {
    swapChainDesc.SampleDesc.Count = 4;
    swapChainDesc.SampleDesc.Quality = m4xMsaaQuality - 1;
  }
  else
  {
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
  }

  swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
  swapChainDesc.BufferCount = 1;
  swapChainDesc.OutputWindow = hwnd;
  swapChainDesc.Windowed = isFullScreen;
  swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
  swapChainDesc.Flags = 0;

  // Create swap data
  IDXGIDevice *dxgiDevice = nullptr;
  if ( FAILED( m_device->QueryInterface( _uuidof( IDXGIDevice ), ( void** ) &dxgiDevice ) ) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }

  IDXGIAdapter *dxgiAdapter = nullptr;
  if ( FAILED( dxgiDevice->GetParent( __uuidof( IDXGIAdapter ), ( void** ) &dxgiAdapter ) ) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }

  IDXGIFactory *dxgiFactory = nullptr;
  if ( FAILED( dxgiAdapter->GetParent( __uuidof( IDXGIFactory ), ( void** ) &dxgiFactory ) ) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }

  if ( FAILED( dxgiFactory->CreateSwapChain( m_device, &swapChainDesc, &m_swapChain ) ) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }

  dxgiDevice->Release( );
  dxgiDevice = nullptr;

  dxgiAdapter->Release( );
  dxgiAdapter = nullptr;

  dxgiFactory->Release( );
  dxgiFactory = nullptr;

  //Create the render target view
  ID3D11Texture2D *backBufferPtr = nullptr;

  if ( FAILED( m_swapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* ) &backBufferPtr ) ) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }

  if ( FAILED( m_device->CreateRenderTargetView( backBufferPtr, NULL, &m_renderTargetView ) ) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }

  backBufferPtr->Release( );
  backBufferPtr = nullptr;

  // Create the depth buffer
  D3D11_TEXTURE2D_DESC depthStencilDesc;
  depthStencilDesc.Width = screenWidth;
  depthStencilDesc.Height = screenHeight;
  depthStencilDesc.MipLevels = 1;
  depthStencilDesc.ArraySize = 1;
  depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

  if ( m_enableMSAA4xQuality )
  {
    depthStencilDesc.SampleDesc.Count = 4;
    depthStencilDesc.SampleDesc.Quality = m4xMsaaQuality;
  }
  else
  {
    depthStencilDesc.SampleDesc.Count = 1;
    depthStencilDesc.SampleDesc.Quality = 0;
  }

  depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
  depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
  depthStencilDesc.CPUAccessFlags = 0;
  depthStencilDesc.MiscFlags = 0;

  if ( FAILED( m_device->CreateTexture2D( &depthStencilDesc, NULL, &m_depthStencilBuffer ) ) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }

  if ( FAILED( m_device->CreateDepthStencilView( m_depthStencilBuffer, NULL, &m_depthStencilView ) ) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }

  m_deviceContext->OMSetRenderTargets( 1, &m_renderTargetView, m_depthStencilView );

  D3D11_RASTERIZER_DESC rasterDesc;
  rasterDesc.AntialiasedLineEnable = false;
  rasterDesc.CullMode = D3D11_CULL_BACK;
  rasterDesc.DepthBias = 0;
  rasterDesc.DepthBiasClamp = 0.0f;
  rasterDesc.DepthClipEnable = true;
  rasterDesc.FillMode = D3D11_FILL_SOLID;
  rasterDesc.FrontCounterClockwise = false;
  rasterDesc.MultisampleEnable = false;
  rasterDesc.ScissorEnable = false;
  rasterDesc.SlopeScaledDepthBias = 0.0f;

  if ( FAILED( m_device->CreateRasterizerState( &rasterDesc, &m_rasterState ) ) )
  {
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }
  m_deviceContext->RSSetState( m_rasterState );

  D3D11_VIEWPORT viewport;
  viewport.Width = ( float ) screenWidth;
  viewport.Height = ( float ) screenHeight;
  viewport.TopLeftX = 0.0f;
  viewport.TopLeftY = 0.0f;
  viewport.MinDepth = 0.0f;
  viewport.MaxDepth = 1.0f;

  m_deviceContext->RSSetViewports( 1, &viewport );

  float fieldOfView = ( float ) XM_PI / 4.0f;
  float screenAspect = ( float ) screenWidth / ( float ) screenHeight;

  m_projectionMatrix = XMMatrixPerspectiveFovLH( fieldOfView, screenAspect, screenNear, screenDepth );

  m_worldMatrix = XMMatrixIdentity( );

  m_orthomatrix = XMMatrixOrthographicLH( ( float ) screenWidth, ( float ) screenHeight, screenNear, screenDepth );
  return true;
}

void D3DClass::shutDown()
{
  if ( m_swapChain )
  {
    m_swapChain->SetFullscreenState( false, NULL );
  }

  if ( m_rasterState )
  {
    m_rasterState->Release( );
    m_rasterState = nullptr;
  }

  if ( m_depthStencilView )
  {
    m_depthStencilView->Release( );
    m_depthStencilView = nullptr;
  }

  if ( m_depthStencilState )
  {
    m_depthStencilState->Release( );
    m_depthStencilState = nullptr;
  }

  if ( m_depthStencilBuffer )
  {
    m_depthStencilBuffer->Release( );
    m_depthStencilBuffer = nullptr;
  }

  if ( m_renderTargetView )
  {
    m_renderTargetView->Release( );
    m_renderTargetView = nullptr;
  }

  if ( m_deviceContext )
  {
    m_deviceContext->Release( );
    m_deviceContext = nullptr;
  }

  if ( m_device )
  {
    m_device->Release( );
    m_device = nullptr;
  }

  if ( m_swapChain )
  {
    m_swapChain->Release( );
    m_swapChain = nullptr;
  }
}

void D3DClass::beginScene(const float red, const float green, const float blue, const float alpha)
{
  float color[4];


  // Setup the color to clear the buffer to.
  color[0] = red;
  color[1] = green;
  color[2] = blue;
  color[3] = alpha;

  // Clear the back buffer.
  m_deviceContext->ClearRenderTargetView( m_renderTargetView, color );

  // Clear the depth buffer.
  m_deviceContext->ClearDepthStencilView( m_depthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0 );

  return;
}

void D3DClass::endScene()
{
  if ( m_vSyncEnabled )
  {
    // Lock to screen refresh rate.
    m_swapChain->Present( 1, 0 );
  }
  else
  {
    // Present as fast as possible.
    m_swapChain->Present( 0, 0 );
  }
}

ID3D11Device* D3DClass::getDevice()
{
  return m_device;
}

ID3D11DeviceContext* D3DClass::getDeviceContext()
{
  return m_deviceContext;
}

void D3DClass::getProjectionmatrix(XMMATRIX& projectionMatrix )
{
  projectionMatrix = m_projectionMatrix;
}

void D3DClass::getWorldMain(XMMATRIX& worldMatrix )
{
  worldMatrix = m_worldMatrix;
}

void D3DClass::getOrthoMatrix(XMMATRIX& orthoMatrix )
{
  orthoMatrix = m_orthomatrix;
}

void D3DClass::getVideoCardInfo(char* cardName, int& memory )
{
  strcpy_s( cardName, 128, m_videoCardDescription );
  memory = m_videoCardMemory;
}
