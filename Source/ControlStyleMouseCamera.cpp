/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ControlStyleMouseCamera.cpp,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ControlStyleMouseCamera.h"
#include "Common.h"
#include "Engine.h"
#include "Player.h"
#include "EngineRunning.h"

namespace Amju
{
ControlStyleMouseCamera::ControlStyleMouseCamera()
{
  Reset();
}

void ControlStyleMouseCamera::Update()
{
}

void ControlStyleMouseCamera::Reset()
{
  m_pickObjectMode = false;
  m_drag = false;
  m_newMousePos = false;

  m_panMode = false;

}

void ControlStyleMouseCamera::MousePos(int x, int y)
{
}

void ControlStyleMouseCamera::MouseButton(bool down, bool ctrl, bool shift)
{
}

void ControlStyleMouseCamera::Red(bool) {}
void ControlStyleMouseCamera::Blue(bool) {}
void ControlStyleMouseCamera::Green(bool) {}
void ControlStyleMouseCamera::YellowUp(bool) {}
void ControlStyleMouseCamera::YellowDown(bool) {}
void ControlStyleMouseCamera::YellowLeft(bool) {}
void ControlStyleMouseCamera::YellowRight(bool) {}
void ControlStyleMouseCamera::Z(bool) {} 
void ControlStyleMouseCamera::PlusUp(bool) {}
void ControlStyleMouseCamera::PlusDown(bool) {}
void ControlStyleMouseCamera::PlusLeft(bool) {}
void ControlStyleMouseCamera::PlusRight(bool) {}
void ControlStyleMouseCamera::JoyX(float f) {}
void ControlStyleMouseCamera::JoyY(float f) {}

}

