#pragma once
#ifndef _UTILITIES_
#define _UTILITIES_
#include <string>
#include <windows.h>

class Utilities
{
public:
  Utilities( );
  Utilities( const Utilities& );
  ~Utilities( );

  std::string GetLastErrorAsString( ) const;
};
#endif

