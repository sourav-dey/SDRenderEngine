#include "ModelClass.h"
#include "Utilities.h"

ModelClass::ModelClass()
{
  m_vertexBuffer = 0;
  m_indexBuffer = 0;
}

ModelClass::ModelClass(const ModelClass&)
{
}

ModelClass::~ModelClass()
{
}

bool ModelClass::initialize(ID3D11Device* device)
{
  if ( !initializeBuffers( device ) )
  {
    Utilities ut;
    printf( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }
  return true;
}

void ModelClass::shutdown()
{
  shutdownBuffers( );
}

void ModelClass::render(ID3D11DeviceContext* deviceContext)
{
  renderBuffers( deviceContext );
}

int ModelClass::getIndexCount()
{
  return m_indexCount;
}

bool ModelClass::initializeBuffers(ID3D11Device* device)
{
  VertexType *vertices;
  unsigned long *indices;
  D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
  D3D11_SUBRESOURCE_DATA vertexData, indexData;

  m_vertexCount = 3;
  m_indexCount = 3;

  vertices = new VertexType[m_vertexCount];
  if ( !vertices )
  {
    return false;
  }

  indices = new unsigned long[m_indexCount];
  if ( !indices )
  {
    return false;
  }

  vertices[0].position = D3DXVECTOR3( -1.0f, -1.0f, 0.0f );
  vertices[1].position = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
  vertices[2].position = D3DXVECTOR3( 1.0f, -1.0f, 0.0f );

  for ( int i = 0; i < m_indexCount; ++i )
  {
    vertices[i].color = D3DXVECTOR4( 0.0f, 1.0f, 0.0f, 1.0f );
    indices[i] = i;
  }

 // Setup static vertex buffer
  vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  vertexBufferDesc.ByteWidth = sizeof( VertexType )*m_vertexCount;
  vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  vertexBufferDesc.CPUAccessFlags = 0;
  vertexBufferDesc.MiscFlags = 0;
  vertexBufferDesc.StructureByteStride = 0;

  // Give the subresource a structure to the pointer to the vertex data
  vertexData.SysMemPitch = 0;
  vertexData.SysMemSlicePitch = 0;
  vertexData.pSysMem = vertices;

  // Now create the vertex buffer
  if ( FAILED( device->CreateBuffer( &vertexBufferDesc, &vertexData, &m_vertexBuffer ) ) )
  {
    printf( "Error:%s", ut.GetLastErrorAsString( ).c_str() );
    return false;
  }

  // Setup static index buffer
  indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
  indexBufferDesc.ByteWidth = sizeof( unsigned long )*m_indexCount;
  indexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
  indexBufferDesc.CPUAccessFlags = 0;
  indexBufferDesc.MiscFlags = 0;
  indexBufferDesc.StructureByteStride = 0;

  // Give the subresource a structure to the pointer to the index data
  indexData.SysMemPitch = 0;
  indexData.SysMemSlicePitch = 0;
  indexData.pSysMem = vertices;

  // Now create the vertex buffer
  if ( FAILED( device->CreateBuffer( &indexBufferDesc, &indexData, &m_indexBuffer ) ) )
  {
    printf( "Error:%s", ut.GetLastErrorAsString( ).c_str( ) );
    return false;
  }

  delete[] vertices;
  vertices = nullptr;

  delete[] indices;
  indices = nullptr;

  return true;
}

void ModelClass::shutdownBuffers()
{
  if ( m_indexBuffer )
  {
    m_indexBuffer->Release( );
    m_indexBuffer = nullptr;
  }

  if ( m_vertexBuffer )
  {
    m_vertexBuffer->Release( );
    m_vertexBuffer = nullptr;
  }
}

void ModelClass::renderBuffers(ID3D11DeviceContext* deviceContext)
{
  unsigned int stride;
  unsigned int offset;

  stride = sizeof( VertexType );
  offset = 0;

  deviceContext->IASetVertexBuffers( 0, 1, &m_vertexBuffer, &stride, &offset );

  deviceContext->IASetIndexBuffer( m_indexBuffer, DXGI_FORMAT_R32_UINT, 0 );

  deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
}
