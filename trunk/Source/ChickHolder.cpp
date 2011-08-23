/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ChickHolder.cpp,v $
Revision 1.1.10.1  2005/06/05 00:25:46  Administrator
Hacks to get this to compile with const Orientations.

Revision 1.1  2004/09/08 15:42:34  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ChickHolder.h"
#include "Engine.h"
#include "SchAssert.h"

namespace Amju
{
ChickHolder::ChickHolder() : m_baseHeight(1.5f)
{
}

void ChickHolder::Reset()
{
  m_chicklist.clear();
}

bool ChickHolder::HasChicks() const
{
  return (!m_chicklist.empty());
}

void ChickHolder::ClearChick(Chick* pChick)
{
  m_chicklist.remove(pChick);
  RecalcChickHeights();
}

void ChickHolder::FreeChicks()
{
  // Iterate throught the list of Chicks we have captured. 
  // For each one, notify it that it is no longer captured. Then clear the list.
  for (ChickList::iterator it = m_chicklist.begin(); it != m_chicklist.end(); ++it)
  {
    Chick* pChick = *it;
    Assert(pChick);
    // Send message rather than directly setting Chick state ?
    //pChick->SetState(UNKNOWN); // i.e. back to default state
    pChick->SetState(JUST_HATCHED);

    Engine::Instance()->SendGameMessage(new Message("chick capturable", 2.0f, // TODO CONFIG
      pChick->GetId(), pChick->GetId(), UNKNOWN));


    // TODO or possibly back to JUST_HATCHED so it can't be picked up for a few secs ?

    // Pasted from Egg: 
    // Face a random direction, and move, rather than dropping straight down.
    float yDegs = (float)(rand() % 360);
    float hatchFwdVel = atof(Engine::Instance()->GetConfigValue("chick_hatch_fwd_vel").c_str());
    //pChick->GetOrientation()->SetYRot(yDegs);
    pChick->SetForwardVel(hatchFwdVel);
  }
  m_chicklist.clear();
}

void ChickHolder::SetBaseHeight(float h)
{
  m_baseHeight = h;
  RecalcChickHeights();
}

void ChickHolder::RecalcChickHeights()
{
  int i = 0;
  for (ChickList::iterator it = m_chicklist.begin(); it != m_chicklist.end(); ++it)
  {
    Chick* pChick = *it;
    Assert(pChick);

    // Set the height at which the Chick should float. This starts off at some
    // height, and then depends how many other chicks are captured.
    //float h = m_baseHeight + 0.5f * i;  
    //pChick->SetCaptureHeight(h);

    ++i;
  }
}

void ChickHolder::CaptureChick(Chick* pChick)
{
  if (pChick->GetState() == CAPTURED)
  {
    return;
  }

  pChick->SetState(CAPTURED);

  // Add the Chick to a list of Chicks captured by this NPC.
  // (this should probably be a set.)
  // Also, ensure the Chick can only be captured by one Croc at a time!
  m_chicklist.push_back(pChick);

  RecalcChickHeights();
}
}


