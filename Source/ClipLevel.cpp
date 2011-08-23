/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: ClipLevel.cpp,v $
Revision 1.1  2004/09/08 15:42:46  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "ClipLevel.h"
#include "File.h"
#include "Level.h"
#include "CameraPath.h"
#include "LevelServer.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
ClipLevel::ClipLevel() : 
  m_pLevel(0), m_roomId(0)
{
}

ClipLevel::~ClipLevel()
{
  Clear();
}

void ClipLevel::Clear()
{
  m_pLevel = 0;
  m_pCameraPath = 0;
}

void ClipLevel::Start()
{
  Clip::Start();
  m_pCameraPath->Reset();
}

void ClipLevel::Draw()
{
  Assert(m_pLevel);
  Assert(m_pCameraPath.GetPtr());

  m_pLevel->SetRoomId(m_roomId);
}

bool ClipLevel::Load(File* pf)
{ 
  Clear();

  // Get the level ID
  int levelId;
  if (!pf->GetInteger(&levelId))
  {
    pf->ReportError("Expected level ID.");
    return false;
  }

  // Use level server to load the level - we don't want to store it 
  // more than once! 
  m_pLevel = LevelServer::Instance()->GetCurrentLevel().GetPtr();
  if (!m_pLevel)
  {
    pf->ReportError("Failed to load clip level.");
    return false;
  }

  // Get the room ID for the level.
  if (!pf->GetInteger(&m_roomId))
  {
    pf->ReportError("Expected room ID.");
    return false;
  }

  // Get the clip duration.
  float time;
  if (!pf->GetFloat(&time))
  {
    pf->ReportError("Expected clip duration.");
    return false;
  }
  SetStartTime(0);
  SetEndTime(time);

  // Get camera path.
  m_pCameraPath = new CameraPath; // TODO need factory ?
  if (!m_pCameraPath->Load(pf))
  {
    return false;
  }

  return true;
}

void ClipLevel::Blue(bool down)
{
  if (down)
  {
    Stop();
  }
}
}
