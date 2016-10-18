#pragma once
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "InputClass.h"
#include "GraphicsClass.h"

class SystemClass
{
public:
  SystemClass( );
  SystemClass( const SystemClass& );
  ~SystemClass( );

  bool initialize( );
  void run( );
  void shutdown( );

  LRESULT CALLBACK MessageHandler( HWND, UINT, WPARAM, LPARAM );

private:
  bool frame( );
  void initializeWindows(UINT&, UINT& );
  void shutdownWindows( );

  LPCSTR m_applicationName;
  HINSTANCE m_hInstance;
  HWND m_hwnd;

  InputClass *m_inputClass;
  GraphicsClass *m_graphicsClass;

};

static LRESULT CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM );
static SystemClass *Application = nullptr;
#endif
