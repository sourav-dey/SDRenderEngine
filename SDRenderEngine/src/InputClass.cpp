#include "InputClass.h"

InputClass::InputClass()
{
}

InputClass::InputClass(const InputClass&)
{
}

InputClass::~InputClass()
{
}

void InputClass::initialize()
{
  for( int input = 0; input < 256; ++input )
  {
    m_keys[input] = false;
  }
}

bool InputClass::iskeyDown(unsigned input)
{
  return m_keys[input];
}

void InputClass::keyDown(unsigned input)
{
  m_keys[input] = true;
}

void InputClass::keyUp(unsigned input)
{
  m_keys[input] = true;
}
