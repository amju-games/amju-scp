/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: CharacterMd2.cpp,v $
Revision 1.2.2.2.4.2  2005/09/05 20:02:48  jay
Turn off debug output

Revision 1.2.2.2.4.1  2005/05/15 17:10:46  jay
Remove golf-specific hacks

Revision 1.2.2.2  2004/09/30 09:41:08  Administrator
Fix for non-golf characters

Revision 1.2.2.1  2004/09/29 11:10:09  Administrator
Tidied up Recalculate - the golf-specific stuff here is still in the
wrong place though :-(

Revision 1.2  2004/09/24 14:39:08  Administrator
Fixes for when an animation finishes - golf-specific!

Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "Common.h"
#include "CharacterMd2.h"
#include "File.h"
#include "Engine.h"
#include "CompositeFactory.h"
#include "Texture.h"
#include "CharacterManager.h"
#include "TextureServer.h"
#if defined(WIN32) && defined(_DEBUG) && defined(MSVC)
#include <GL/glaux.h> // TEST
#endif
#include "SchAssert.h"

using namespace std;

namespace Amju
{
CharacterMd2::CharacterMd2() : 
  m_pModel(0)
{
  m_animation = 0;
  m_frame = 1;
  m_animtime = 0;
}

CharacterMd2::CharacterMd2(const CharacterMd2& rhs) :
  Character(rhs),
  m_pModel(rhs.m_pModel)
{
  m_animation = 0;
  m_frame = 1;
  m_animtime = 0;
}

CharacterMd2::~CharacterMd2()
{
}

bool CharacterMd2::Load(File* pf)
{
  // Load mesh name.
  string meshName;
  if (!pf->GetDataLine(&meshName))
  {
    pf->ReportError("Expected MD2 mesh name.");
    return false;
  }

  // Get the model mesh from the CharacterManager.
  SharedPtr<Md2Model> pModel = CharacterManager::Instance()->GetMd2Mesh(meshName);
  m_pModel = pModel.GetPtr();
  if (!m_pModel)
  {
    pf->ReportError("Bad mesh name for MD2 character.");
    return false;
  }
  // Skins are loaded in subclasses.
  return true;
}

void CharacterMd2::Draw()
{
  BindSkin();

  // Work out the point between the current and next frame, for interpolation.
  // But don't if we are frozen.
  float between = 0;
  if (!m_isFrozen)
  {
    Assert(m_animtime >= 0);
    Assert(m_animtime <= 0.1f);
    between = m_animtime * 10.0f;
    if (between > 1.0f)
    {
      between = 1.0f;
    }
  }

  m_pModel->DrawAnim((Md2Model::Animation)m_animation, m_frame, between);
}

bool CharacterMd2::CurrentActionRepeats() const
{
  return m_pModel->GetDoesActionRepeat((Md2Model::Animation)m_animation);
}

bool CharacterMd2::CurrentActionFreezes() const
{
  return m_pModel->GetDoesActionFreeze((Md2Model::Animation)m_animation);
}

void CharacterMd2::SetActionPain()
{
  SetAction(m_pModel->GetAnimationFromName("paina")); 
}

void CharacterMd2::SetActionDead()
{
  // Pick one of 3 animation sequences.
  int deathAnimation = m_pModel->GetAnimationFromName("deatha"); 
  SetAction((Md2Model::Animation)deathAnimation);
}

void CharacterMd2::SetActionIdle()
{
  SetAction(m_pModel->GetAnimationFromName("stand")); 
}

void CharacterMd2::SetActionWalk()
{
  // Do this test first so we don't reset the frame no if unnecessary
  if (m_animation == m_pModel->GetAnimationFromName("walk"))
  {
    return;
  }

  // If we are currently running, set the frame count to the corresponding 
  // position in the WALK animation. This is to get a smooth transition
  // from running to walking.
  // In fact we simply don't reset the frame count to 1.
  if (m_animation != m_pModel->GetAnimationFromName("run"))
  {
    m_frame = 1;
  }

  SetAction(m_pModel->GetAnimationFromName("walk"));
}

void CharacterMd2::SetActionRun()
{
  if (m_animation == m_pModel->GetAnimationFromName("run"))
  {
    return;
  }

  // If we are currently walking, set the frame count to the corresponding
  // position in the RUN animation.
  // In fact we simply don't reset the frame count to 1.
  if (m_animation != m_pModel->GetAnimationFromName("walk"))
  {
    m_frame = 1;
  }

  SetAction(m_pModel->GetAnimationFromName("run"));
}

void CharacterMd2::SetActionJump()
{
  SetAction(m_pModel->GetAnimationFromName("jump"));
}

void CharacterMd2::SetActionFly()
{
}

void CharacterMd2::SetActionFall()
{
}

void CharacterMd2::SetActionShuffle()
{
  SetAction(m_pModel->GetAnimationFromName("walk"));
}

void CharacterMd2::Recalculate()
{
  Character::Recalculate();

  if (m_isFrozen)
  {
    // Don't change frame if we are frozen.
    return;
  }

  m_animtime += Engine::Instance()->GetDeltaTime();
  if (m_animtime <= 0.1f)
  {
    return;
  }

  m_animtime = 0;
  m_frame++;
  int size = m_pModel->GetAnimationSize((Md2Model::Animation)m_animation);
  if (m_frame <= size)
  {
    return;
  }

  // Get next action on queue. If empty, do default action.
  m_frame = 1;

  // Assume this action doesn't repeat.
  // If this is a Death animation, freeze on the last frame.
  if (CurrentActionFreezes())
  {
    // Dead, so now the frame number never changes.
    m_frame = m_pModel->GetAnimationSize((Md2Model::Animation)m_animation); 
    return;
  }

/*
  // TODO POOL

  // Fix for #53
  // Animations would annoyingly flicker to golf_ready for one frame.
  // That's because we have to decide which animation comes after 
  // this one - not just blindly go to golf_ready.
  // TODO This stuff doesn't belong here :-(

  // The ints from animation names can't be static, 
  // they are different for each model :-(
  const int stand = m_pModel->GetAnimationFromName("stand");
  if (m_animation == stand)
  {
    return;
  }

  const int walk =  m_pModel->GetAnimationFromName("walk");

  if (m_animation == walk)
  {
    SetAction(stand);
  } 
  else
  {
    const int swing_shuffle =  m_pModel->GetAnimationFromName("swing_shuffle");
    const int swing_ready =  m_pModel->GetAnimationFromName("swing_ready");
    if (m_animation == swing_shuffle)
    {
      SetAction(swing_ready);
    }
    else
    {
      const int putt_shuffle =  m_pModel->GetAnimationFromName("putt_shuffle");
      const int golf_ready = m_pModel->GetAnimationFromName("golf_ready");
      if (m_animation == putt_shuffle)
      {
        SetAction(golf_ready);
      }
    }
  }
*/
}

void CharacterMd2::SetAction(int actionId)
{
  if (actionId != m_animation)
  {
    m_animation = actionId;
    m_frame = 1;

    m_currentAction = m_pModel->GetAnimationName((Md2Model::Animation)actionId);

#ifdef CHARACTER_DEBUG
    std::cout << "New action: " << m_currentAction.c_str();
    if (CurrentActionFreezes())
    {
      std::cout << " FREEZE\n";
    }
    else
    {
      std::cout << " No freeze\n";
    }
#endif
  }
}

int CharacterMd2::GetNumberOfPolygons() const
{
  return 0; // TODO m_pModel->GetNTris();
}

std::string CharacterMd2::GetActionName(int action)
{
  return m_pModel->GetAnimationName((Md2Model::Animation)action);
}

int CharacterMd2::GetAction(const std::string& actionName)
{
  return m_pModel->GetAnimationFromName(actionName);
}
}

