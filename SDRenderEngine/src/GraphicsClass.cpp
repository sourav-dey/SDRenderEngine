#include "GraphicsClass.h"

GraphicsClass::GraphicsClass()
{
  m_d3d = nullptr;
}

GraphicsClass::GraphicsClass(const GraphicsClass&)
{
}

GraphicsClass::~GraphicsClass()
{
}

bool GraphicsClass::initialize(UINT screenWidth, UINT screenHeight, HWND hwnd)
{
  m_d3d = new D3DClass;
  if ( !m_d3d )
  {
    return false;
  }

  bool result = m_d3d->initialize( screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR );

  if ( !result )
  {
    return false;
  }
  return true;
}

void GraphicsClass::shutdown()
{
  if ( m_d3d )
  {
    m_d3d->shutDown( );
    delete m_d3d;
    m_d3d = nullptr;
  }
}

bool GraphicsClass::frame()
{
  if ( !render( ) )
  {
    return false;
  }
  return true;
}

bool GraphicsClass::render()
{
  m_d3d->beginScene( 0.5f, 0.5f, 0.5f );
  m_d3d->endScene( );

  return true;
}
