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
  D3D11_SUBRESOURCE_DATA vertexDate, indexData;

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
  }
}

void ModelClass::shutdownBuffers()
{
}

void ModelClass::renderBuffers(ID3D11DeviceContext* deviceContext)
{
}
