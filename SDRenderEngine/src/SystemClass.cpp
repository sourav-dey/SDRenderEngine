#include "SystemClass.h"
#include "Utilities.h"
#include <minwinbase.h>
#include <stdio.h>

SystemClass::SystemClass()
{
  m_inputClass = nullptr;
  m_graphicsClass = nullptr;
}

SystemClass::SystemClass(const SystemClass&)
{
}

SystemClass::~SystemClass()
{
}

bool SystemClass::initialize()
{
  UINT screenWidth = 0, screenHeight = 0;
  bool result;

  initializeWindows( screenWidth, screenHeight );

  m_inputClass = new InputClass;
  if ( !m_inputClass )
  {
    return false;
  }

  m_inputClass->initialize( );

  m_graphicsClass = new GraphicsClass;
  if ( !m_graphicsClass )
  {
    return false;
  }

  result = m_graphicsClass->initialize( screenWidth, screenHeight, m_hwnd );
  if ( !result )
  {
    return false;
  }
  return true;
}

void SystemClass::run()
{
  MSG msg;
  bool done = false, result = false;

  ZeroMemory( &msg, sizeof( MSG ) );

  while(!done )
  {
    if ( PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
    {
      TranslateMessage( &msg );
      DispatchMessage( &msg );
    }

    if (msg.message == WM_QUIT )
    {
      done = true;
    }
    else
    {
      result = frame( );
      if ( !result )
      {
        done = true;
      }
    }
  }
  return;
}

void SystemClass::shutdown()
{
  if ( m_graphicsClass )
  {
    m_graphicsClass->shutdown( );
    delete m_graphicsClass;
    m_graphicsClass = nullptr;
  }

  if ( m_inputClass )
  {
    delete m_inputClass;
    m_inputClass = nullptr;
  }
  shutdownWindows( );
  return;
}

LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
  switch (umsg)
  {
  case WM_KEYDOWN:
    m_inputClass->keyDown( ( unsigned int ) wparam );
    return 0;

  case WM_KEYUP:
    m_inputClass->keyUp( ( unsigned int ) wparam );
    return 0;

  default:
    return DefWindowProc( hwnd, umsg, wparam, lparam );
  }
}

bool SystemClass::frame()
{
  bool result;

  if ( m_inputClass->iskeyDown(VK_ESCAPE) )
  {
    return false;
  }

  result = m_graphicsClass->frame( );
  if ( !result )
  {
    return false;
  }
  return true;
}

void SystemClass::initializeWindows(UINT& screenWidth, UINT& screenHeight)
{
  WNDCLASSEX wc;
  DEVMODE dmScreenSettings;
  int posX, posY;

  Application = this;

  m_hInstance = GetModuleHandle( NULL );

  m_applicationName = "Engine";

  wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;
  wc.hInstance = m_hInstance;
  wc.hIcon = LoadIcon( NULL, IDI_WINLOGO );
  wc.hIconSm = wc.hIcon;
  wc.hCursor = LoadCursor( NULL, IDC_ARROW );
  wc.hbrBackground = ( HBRUSH ) GetStockObject( BLACK_BRUSH );
  wc.lpszMenuName = NULL;
  wc.lpszClassName = m_applicationName;
  wc.cbSize = sizeof( WNDCLASSEX );

  if ( !RegisterClassEx( &wc ) )
  {
    Utilities ut;
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
  }

  screenWidth = GetSystemMetrics( SM_CXSCREEN );
  screenHeight = GetSystemMetrics( SM_CYSCREEN );

  if ( FULL_SCREEN )
  {
    memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );
    dmScreenSettings.dmSize = sizeof( dmScreenSettings );
    dmScreenSettings.dmPelsWidth = screenWidth;
    dmScreenSettings.dmPelsWidth = screenHeight;
    dmScreenSettings.dmBitsPerPel = 32;
    dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

    ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN );

    posX = posY = 0;
  }
  else
  {
    screenWidth = 800;
    screenHeight = 600;

    posX = ( GetSystemMetrics( SM_CXSCREEN ) - screenWidth ) / 2;
    posY = ( GetSystemMetrics( SM_CYSCREEN ) - screenHeight ) / 2;
  }

  m_hwnd = CreateWindowEx( WS_EX_APPWINDOW, m_applicationName, m_applicationName, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP, posX, posY, screenWidth, screenHeight, NULL, NULL, m_hInstance, NULL );
  if ( !m_hwnd )
  {
    Utilities ut;
    printf_s( "Error:%s", ut.GetLastErrorAsString( ).c_str() );
  }

  ShowWindow( m_hwnd, SW_SHOW );
  SetForegroundWindow( m_hwnd );
  SetFocus( m_hwnd );

  ShowCursor( false );
  return;
}

void SystemClass::shutdownWindows()
{
  ShowCursor( true );

  if ( FULL_SCREEN )
  {
    ChangeDisplaySettings( NULL, 0 );
  }

  DestroyWindow( m_hwnd );
  m_hwnd = NULL;

  UnregisterClass( m_applicationName, m_hInstance );
  m_hInstance = NULL;

  Application = NULL;
  return;
}

LRESULT __stdcall WndProc(HWND hWnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
  switch( umsg )
  {
  case WM_DESTROY:
    PostQuitMessage( 0 );
    return 0;

  case WM_CLOSE:
    PostQuitMessage( 0 );
    return 0;

  default:
    return Application->MessageHandler( hWnd, umsg, wparam, lparam );
  }
}
