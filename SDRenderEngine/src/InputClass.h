#pragma once
#ifndef _INPUTCLASS_H_
#define _INPUTCLASS_H_

class InputClass
{
public:
  InputClass( );
  InputClass( const InputClass& );
  ~InputClass( );

  void initialize( );
  bool iskeyDown( unsigned int input );
  
  void keyDown( unsigned int input );
  void keyUp( unsigned int input );


private:
  bool m_keys[256];
};
#endif
