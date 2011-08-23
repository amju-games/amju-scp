/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: MacJoystick.h,v $
Revision 1.1  2004/09/08 15:43:08  jay
Added to repository
  
*/

#if !defined(MAC_JOYSTICK_H_INCLUDED)
#define MAC_JOYSTICK_H_INCLUDED

namespace Amju
{
class Engine;

// TODO
class MacJoystick
{
public:
  bool IsConnected() const;
  void Poll(Engine*);
  bool Connect(int);
  void SetSensitivity(int);
  void Init(Engine*) {}
};
}

#endif