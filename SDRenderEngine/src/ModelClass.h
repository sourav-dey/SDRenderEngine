#pragma once
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <d3d11.h>
#include <D3DX10math.h>
#include "Utilities.h"

class ModelClass
{
private:
  struct VertexType
  {
    D3DXVECTOR3 position;
    D3DXVECTOR4 color;
  };

  ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
  int m_vertexCount, m_indexCount;

public:
  ModelClass( );
  ModelClass( const ModelClass& );
  ~ModelClass( );

  bool initialize( ID3D11Device* device );
  void shutdown( );
  void render( ID3D11DeviceContext *deviceContext );
  int getIndexCount( );

private:
  bool initializeBuffers( ID3D11Device *device );
  void shutdownBuffers( );
  void renderBuffers( ID3D11DeviceContext *deviceContext );

  Utilities ut;

};
#endif
