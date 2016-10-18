#include "SystemClass.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow )
{
  SystemClass *systemClass;
  bool result;

  systemClass = new SystemClass;
  if ( !systemClass )
  {
    return 0;
  }

  result = systemClass->initialize( );
  if ( result )
  {
    systemClass->run( );
  }

  systemClass->shutdown( );
  delete systemClass;
  systemClass = nullptr;

  return 0;
}