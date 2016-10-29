#pragma once
#ifndef _COLORSHADERCLASS_H_
#define _COLORSHADERCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>
#include <fstream>
#include "Utilities.h"
using namespace std;

class ColorShaderClass
{
  struct MatrixBufferType
  {
    D3DXMATRIX world;
    D3DXMATRIX view;
    D3DXMATRIX projection;
  };

  bool initializeShader( ID3D11Device *device, HWND hwnd, WCHAR *vsFileName, WCHAR *psFileName );
  void shutdownShader( );
  void outputShaderErrorMessage( ID3D10Blob *errorMessage, HWND hwnd, WCHAR *shaderFileName );

  bool setShaderParams( ID3D11DeviceContext *context, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix );
  void renderShader( ID3D11DeviceContext *context, int indexCount );

  ID3D11VertexShader *m_vertexShader;
  ID3D11PixelShader *m_pixelShader;
  ID3D11InputLayout *m_layout;
  ID3D11Buffer *m_buffer;

  Utilities ut;

public:
  ColorShaderClass( );
  ColorShaderClass( const ColorShaderClass& );
  ~ColorShaderClass( );

  bool initialize( ID3D11Device* device, HWND hwnd );
  void shutdown( );
  void render( ID3D11DeviceContext *deviceContext, int indexCount, D3DXMATRIX worldMatrix, D3DXMATRIX viewMatrix, D3DXMATRIX projectionMatrix );

};
#endif
