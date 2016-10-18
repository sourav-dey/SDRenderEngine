#pragma once
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_
#include <windows.h>
#include "D3DClass.h"

class GraphicsClass
{
public:
  GraphicsClass( );
  GraphicsClass( const GraphicsClass& );
  ~GraphicsClass( );

  bool initialize( UINT screenWidth, UINT screenHeight, HWND hwnd );

  void shutdown( );
  bool frame( );

private:
  bool render( );
  D3DClass *m_d3d;
};

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;
#endif
