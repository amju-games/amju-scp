/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolCourseSelect.cpp,v $
Revision 1.1.2.10  2007/07/15 21:58:40  Administrator
Revert change which unlocks all Trick Shots

Revision 1.1.2.9  2007/07/12 20:25:09  jay
Enabled all trick shots --- TODO TEMP TEST

Revision 1.1.2.8  2007/07/01 20:36:09  jay
Fix room select to check for Online flag

Revision 1.1.2.7  2007/06/25 20:45:50  jay
Fix trick shot unlock: this was unlocking all the trick shots

Revision 1.1.2.6  2007/06/10 08:54:39  jay
Fix null pointer: member variable set according to a static

Revision 1.1.2.5  2007/05/15 09:29:12  jay
Debug output

Revision 1.1.2.4  2007/03/27 08:43:09  jay
Don't play gong.wav. This is a recurring problem for users

Revision 1.1.2.3  2007/03/17 23:58:03  jay
Fix non-online game

Revision 1.1.2.2  2007/03/09 21:50:41  jay
Online game - ongoing fixes

Revision 1.1.2.1  2006/08/14 17:50:13  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:26  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.8.19  2005/09/30 12:59:51  jay
Kill off any earthquake

Revision 1.1.8.18  2005/09/29 19:04:17  jay
No Earthquake at game start

Revision 1.1.8.17  2005/09/23 19:37:15  jay
TQ for solved trick shot rooms

Revision 1.1.8.16  2005/09/20 13:15:35  jay
Show different cost types

Revision 1.1.8.15  2005/09/14 19:43:05  jay
Some rooms are now one- or two-player only; or practice mode not allowed.

Revision 1.1.8.14  2005/09/05 20:05:30  jay
Make sure we play theme song when in this state.

Revision 1.1.8.13  2005/09/02 18:32:13  jay
Check is Registered for unlockable rooms

Revision 1.1.8.12  2005/09/01 19:51:10  jay
Some rooms are unlockable in trial period

Revision 1.1.8.11  2005/08/26 21:21:14  jay
Show bonus "cost" for room, show Pool Background

Revision 1.1.8.10  2005/08/02 22:10:57  jay
Minor tweaks

Revision 1.1.8.9  2005/08/02 18:33:34  jay
Tweaks for v.1.1

Revision 1.1.8.8  2005/07/30 23:34:06  jay
Allow multiple levels

Revision 1.1.8.7  2005/07/05 13:03:31  Administrator
Always enabled for POOL 1.0

Revision 1.1.8.6  2005/06/29 20:20:01  jay
Always enable all games - at least for v.1.0

Revision 1.1.8.5  2005/06/13 22:19:46  jay
Ongoing improvements to course/game/player select screens

Revision 1.1.8.4  2005/06/04 22:29:54  jay
Minor cosmetic changes to game select screen

Revision 1.1.8.3  2005/05/24 20:38:38  jay
Remove unneeded golf code

Revision 1.1.8.2  2005/04/17 22:03:03  jay
Draw thumbnails correctly (for 4 rooms per level)

Revision 1.1.8.1  2005/04/11 21:19:28  jay
Show 4 rooms per level for pool

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolCourseSelect.h"
#include "EngineStatePoolGameSelect.h"
#include "EngineStateTitle.h"
#include "PoolMisc.h"
#include "TextFactory.h"
#include "TextMenu.h"
#include "StringUtils.h"
#include "Engine.h"
#include "PoolCourseManager.h"
#include "LevelServer.h"
#include "TextWriter.h"
#include "TextColourizerColourList.h"
#include "GuiButton.h"
#include "PoolGameState.h"
#include "PoolPlayerStats.h"
#include "PoolBg.h"
#include "PoolOnline.h"

namespace Amju
{
bool IsRegistered() { return true; }

void ReportError(const std::string&);

static const int NUM_THUMBNAILS = 4;

const char* EngineStatePoolCourseSelect::Name = "course_select";
int EngineStatePoolCourseSelect::s_currentCourse;
bool EngineStatePoolCourseSelect::s_canClick = true;

static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePoolCourseSelect::Name, new EngineStatePoolCourseSelect);

void OnNext()
{
  int c = EngineStatePoolCourseSelect::GetCourse();

#ifdef _DEBUG
std::cout << "OnNext.. c: " << c << "\n";
#endif

  if (c < (PoolCourseManager::Instance()->GetNumCourses() - 1))
  {
    ++c;
    EngineStatePoolCourseSelect::SetCourse(c);
  }
  else
  {
    // Back to level 0
    EngineStatePoolCourseSelect::SetCourse(0);
   
//    Engine::Instance()->PlayWav(Engine::Instance()->
//     GetConfigValue("wav_bad_choice"));
  }
}

void OnBackToPlayerSelect()
{
  // POOL - back to Player Select
  // TODO if > 1 level, go back to previous level if current level > 0.

  Engine::Instance()->ChangeState(EngineStatePoolGameSelect::Name, 
    Engine::IMMEDIATE);

}

void OnBack()
{
  int c = EngineStatePoolCourseSelect::GetCourse();
  if (c > 0)
  {
    --c;
    EngineStatePoolCourseSelect::SetCourse(c);
  }
  else
  {
    // Go to max level num
    c = PoolCourseManager::Instance()->GetNumCourses() - 1;
    EngineStatePoolCourseSelect::SetCourse(c);


   // Engine::Instance()->PlayWav(Engine::Instance()->
   //   GetConfigValue("wav_bad_choice"));
  }
}

void EngineStatePoolCourseSelect::Update()
{
}

bool EngineStatePoolCourseSelect::Load()
{
  if (!EngineStateText::Load())
  {
    return false;
  }

  if (!m_bg.Load(GetEngine()->GetConfigValue("file_bg"), ""))
  {
    return false;
  }

  // Heart quad to show room "costs"
  TexturedQuad tq1;
  tq1.Load("heart4.bmp", "heart4_a.bmp");
  m_costTqs.push_back(tq1);
  TexturedQuad tq2;
  tq2.Load("star.bmp", "star_a.bmp");
  m_costTqs.push_back(tq2);

  //m_pNextText = TextFactory::Instance()->Create("next", &tc);
  //m_pBackText = TextFactory::Instance()->Create("back to player select", &tc);

  static const float BUTTON_X1 = Engine::Instance()->GetConfigFloat(
    "course_select_btn_x1");
  static const float BUTTON_X2 = Engine::Instance()->GetConfigFloat(
    "course_select_btn_x2");
  static const float BUTTON_X3 = Engine::Instance()->GetConfigFloat(
    "course_select_btn_x3");
  static const float BUTTON_Y = Engine::Instance()->GetConfigFloat(
    "course_select_btn_y");

  // "Back to player select" button
  m_pSelectButton = new GuiButton;
  std::string selectButtonFile = GetEngine()->GetConfigValue(
    "player_select_button");
  if (!m_pSelectButton->Load(selectButtonFile))
  {
    return false;
  }
  m_pSelectButton->SetSize(8.0f, 2.0f);
  m_pSelectButton->SetRelPos(BUTTON_Y, BUTTON_X3); 
  m_pSelectButton->SetCommand(&OnBackToPlayerSelect);

  // Use same GUI file for next and back buttons
  m_pNextButton = new GuiButton;
  std::string more2Button = GetEngine()->GetConfigValue("more2_button");
  if (!m_pNextButton->Load(more2Button))
  {
    ReportError("Failed to load more2 button.");
    return false; 
  }
  m_pNextButton->SetSize(4.0f, 2.0f);
  m_pNextButton->SetRelPos(BUTTON_Y, BUTTON_X2);
  m_pNextButton->SetCommand(&OnNext);
 
  m_pBackButton = new GuiButton;
  std::string more1Button = GetEngine()->GetConfigValue("more1_button");
  if (!m_pBackButton->Load(more1Button))
  {
    ReportError("Failed to load more1 button.");
    return false; 
  }
  m_pBackButton->SetSize(4.0f, 2.0f);
  m_pBackButton->SetRelPos(BUTTON_Y, BUTTON_X1);
  m_pBackButton->SetCommand(&OnBack);

  s_currentCourse = 0;
  return true;
}

void EngineStatePoolCourseSelect::DrawOverlays()
{
  // Update thumbnails if we need to.
  // This is all over the place :-(
  static int course = -1; //s_currentCourse;
  if (course != s_currentCourse || !m_pText.GetPtr())
  {
    GetThumbnails();
    course = s_currentCourse;
  
    // TODO INTL8
    TextColourizerColourList tc;
    //tc.AddColour(Colour(1.0f, 0, 0, 1.0f));
    tc.AddColour(Colour(1.0f, 1.0f, 1.0f, 1.0f));
    std::string name = PoolCourseManager::Instance()->GetCourseName(course);
    m_pText = TextFactory::Instance()->Create(name, &tc);
  }

  static float s_left = Engine::Instance()->GetConfigFloat("coursename_left");
  static float s_top = Engine::Instance()->GetConfigFloat("name_top");
  static float s_z = Engine::Instance()->GetConfigFloat("name_z");
  
  m_pSelectButton->Draw();
  m_pNextButton->Draw();
  m_pBackButton->Draw();

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PushMatrix();
  AmjuGL::Translate(s_left + 10.0f, s_top, s_z);

  AmjuGL::Translate(1.0f, 0, 0);
  m_pText->Draw(); // name of level 

  AmjuGL::PopMatrix();

  // Draw thumnails -- POOL: 4 per level 
  int hole = 0;
  for (int i = 0; i < 1; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      static const float h = 4.5f;
      static const float w = 4.5f;
      
      float t = 5.5 + i * 4.6f;
      float b = t + h;
      float l = 3.0f + j * 4.6f;

      m_thumbnails[hole]->SetSize(w, h);     
      m_thumbnails[hole]->SetRelPos(t, l);
      m_thumbnails[hole]->Draw();

      // If this room has been solved, superimpose something over the
      // thumb.
      bool solved = false;
      for (int k = 0; k < 2; k++)
      {
        PlayerStats* p = Engine::Instance()->GetGameState()->
          GetPlayerInfo(k)->GetPlayerStats();

        PoolPlayerStats* pp = dynamic_cast<PoolPlayerStats*>(p);
        if (pp)
        {
          int levelId = s_currentCourse;
          int roomId = j;
          if (pp->GetRoomSolved(levelId, roomId))
          {
            solved = true;
          }
        }
      }
      if (solved)
      {
        // TODO TEMP TEST
        TexturedQuad& tq = m_costTqs[1];
        tq.Draw(t + h * 0.5f, l + w * 0.5f, t + h, l + w);
//        Engine::Instance()->GetTextWriter()->Print(l, t, "SOLVED!");
      }

      // If this room has a cost, show the cost for it.
      int type = 0, cost = 0;
      PoolCourseManager::Instance()->GetRoomCost(
        s_currentCourse, hole, &type, &cost);

      // Show the cost even if we can "afford" it
      if (cost > 0)
      {
        Assert(type > 0);
        Assert(type <= (int)m_costTqs.size());

        TexturedQuad& tq = m_costTqs[type - 1];

        tq.Draw(t - 1.5f, l + 1.5f, t, l + w - 1.5f);
        static Font* pFont = TheFontManager::Instance()->GetFont("cheri-1.0");
        std::string coststr = ToString(cost);
        pFont->PrintNoBlend(l + w * 0.5f - 0.25f * coststr.length(), 
          t - 1.0f, coststr.c_str());
      }

      static Font* pFont = 
        Engine::Instance()->GetTextWriter()->GetDefaultFont();
      float s = pFont->GetSize();
      pFont->SetSize(0.6f);
      if (!PoolCourseManager::Instance()->
          IsRoomUnlockableDuringTrialPeriod(s_currentCourse, hole) &&
          !IsRegistered())
      {
        pFont->PrintNoBlend(l + 0.5f, b, "Full version only!");
      }
      else if (PoolCourseManager::Instance()->
               IsRoomOnePlayerOnly(s_currentCourse, hole) &&
               PoolCourseManager::Instance()->
               IsRoomTwoPlayerOnly(s_currentCourse, hole))
      {
        pFont->PrintNoBlend(l + 0.5f, b, "No practice game!");
      }
      // TODO check current game type
      else if (PoolCourseManager::Instance()->
               IsRoomOnePlayerOnly(s_currentCourse, hole) &&
               (Engine::Instance()->GetGameState()->IsFriendly() ||
               NumHumanPlayers() > 1))
      {
        pFont->PrintNoBlend(l + 0.5f, b, "One player only");
      }
      else if (PoolCourseManager::Instance()->
               IsRoomTwoPlayerOnly(s_currentCourse, hole) &&
               (Engine::Instance()->GetGameState()->IsFriendly() ||
               NumHumanPlayers() < 2))
      {
        pFont->PrintNoBlend(l + 0.5f, b, "Two player only");
      }
      else if (!PoolCourseManager::Instance()->
               IsRoomOkForOnline(s_currentCourse, i) &&
               IsOnlineGame())
      {
        pFont->PrintNoBlend(l + 0.5f, b, "No Online game!");
      }

      pFont->SetSize(s);
      ++hole;
    }
  }
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
}

class CourseSelectCommand : public GuiCommand
{
public:
  CourseSelectCommand(int i) : m_hole(i) {}
  virtual bool Do()
  {
    // Pasted from OnSelect()
    int c = EngineStatePoolCourseSelect::GetCourse();
    // TODO check unlocked ?

    EngineStatePoolCourseSelect::SetCanClick(false);

    // This wav gets repeatedly played for some users.
    // Maybe the wav has a Loop flag set ? But why some users and not others ?
    // So disable it.
//    Engine::Instance()->PlayWav("gong.wav"); // TODO CONFIG

    StartCourse(c);
     
    // Set hole number
    PLevel pLevel = LevelServer::Instance()->GetCurrentLevel();
    pLevel->SetRoomId(m_hole);

#ifdef POOL_ONLINE
    if (IsOnlineGame())
    {
      // Now we have chosen level and room, notify the opponent who is waiting.
      // Update the Game row in database on server
      int levelId = pLevel->GetId();
      ThePoolOnlineManager::Instance()->SendGameSelected(levelId, m_hole);
    }
#endif

#ifdef _DEBUG
std::cout << "** GAME SELECT! SETTING LEVEL ID: "
  << pLevel->GetId() << " ROOM ID: " << m_hole << "\n";
#endif

    // This call has been removed fcrom StartCourse(). This fixes the bug
    // where any room > 0 has no cue ball or player characters.
    // The reason was that the EngineState was changed before the
    // new room ID was set.
    StartNewHole(); 

    return false; // no undo
  }

private:
  int m_hole;
};

void EngineStatePoolCourseSelect::GetThumbnails()
{
#ifdef _DEBUG
std::cout << "In GetThumbnails...\n";
#endif

  // Call when current course changes.
  // Get thumbnail images for each hole in the given course.
  for (int i = 0; i < NUM_THUMBNAILS; i++)
  {
    std::string imageName = "thumb-";
    imageName += ToString(s_currentCourse);
    imageName += "-";
    imageName += ToString(i);
    imageName += ".bmp";
    TexturedQuad tq;
    tq.Load(imageName, "thumb-a.bmp");

    GuiButton* pGb = new GuiButton;
    pGb->SetFromQuad(tq);
    pGb->SetCommand(new CourseSelectCommand(i));
    
    // POOL 
    // Enable this button if one of the players can "afford the cost"
    //   for this room/level 
    bool e = false;
    int type, cost;
    PoolCourseManager::Instance()->GetRoomCost(s_currentCourse, i, &type, &cost);
    // TODO Two-player ?
    int numPlayers = Engine::Instance()->GetGameState()->GetNumberOfPlayers();
    Assert(numPlayers == 1 || numPlayers == 2);

    for (int j = 0; j < numPlayers; j++)
    {
      PlayerStats* p = Engine::Instance()->GetGameState()->
        GetPlayerInfo(j)->GetPlayerStats();
      PoolPlayerStats* pp = dynamic_cast<PoolPlayerStats*>(p);
      if (pp)
      {
        if (pp->GetNumBonuses(type) >= cost)
        {
#ifdef _DEBUG
std::cout << "COST: Player " << j << " can afford to unlock room " << i << "\n";
#endif
          e = true;
          break;
        } 
        else
        {
#ifdef _DEBUG
std::cout << "COST: Player " << j << " can NOT afford to unlock room " << i << "\n";
#endif
        }
      }
      else
      {
#ifdef _DEBUG
std::cout << "No player stats for player " << j << ".. so DON'T set room enabled, right ??\n";
#endif
        //e = true;
      }
    }

#ifdef _DEBUG
std::cout << "This game " << (IsOnlineGame() ? "is" : "IS NOT") << " online\n";
std::cout << "OK for online ? " << (PoolCourseManager::Instance()->IsRoomOkForOnline(s_currentCourse, i) ? "YES" : "NO") << "\n"; 
#endif

    if (!PoolCourseManager::Instance()->
        IsRoomUnlockableDuringTrialPeriod(s_currentCourse, i) &&
        !IsRegistered())
    {
#ifdef _DEBUG
std::cout << "Room " << i << " locked, registered game only\n";
#endif

      e = false;
    }
    else if (PoolCourseManager::Instance()->
        IsRoomOnePlayerOnly(s_currentCourse, i) &&
        PoolCourseManager::Instance()->
        IsRoomTwoPlayerOnly(s_currentCourse, i) &&
        Engine::Instance()->GetGameState()->IsFriendly())
    {
#ifdef _DEBUG
std::cout << "Room " << i << " locked, no friendly game allowed\n";
#endif

      // No friendly game allowed
      e = false;
    }
    else if (PoolCourseManager::Instance()->
        IsRoomOnePlayerOnly(s_currentCourse, i) &&
        (Engine::Instance()->GetGameState()->IsFriendly() ||
         NumHumanPlayers() > 1))
    {
#ifdef _DEBUG
std::cout << "Room " << i << " locked, one player only\n";
#endif

      e = false;
    }
    else if (PoolCourseManager::Instance()->
        IsRoomTwoPlayerOnly(s_currentCourse, i) &&
        (Engine::Instance()->GetGameState()->IsFriendly() ||
         NumHumanPlayers() < 2))
    {
#ifdef _DEBUG
std::cout << "Room " << i << " locked, two player only\n";
#endif

      e = false;
    }
    else if (!PoolCourseManager::Instance()->
        IsRoomOkForOnline(s_currentCourse, i) &&
        IsOnlineGame())
    {
#ifdef _DEBUG
std::cout << "Room " << i << " is NOT for online games\n";
#endif
      e = false;
    }

    pGb->SetEnabled(e);
    m_thumbnails[i] = pGb;
  }
}

void EngineStatePoolCourseSelect::Draw()
{
  AmjuGL::PushMatrix();
  // Paint background
  DrawBg();

  // Do camera work
  m_pCam->Update();
  m_pCam->Draw();

  // Lighting
  m_lighting.DrawLight();

  m_or.Draw();

  ThePoolBg::Instance()->Draw();

  AmjuGL::PopMatrix();
}

void EngineStatePoolCourseSelect::SetActive(bool active)
{
  SetCanClick(true); 
  // Set the locked courses to greyed-out.
  if (active)
  {
    // Kill any Earthquake still active
    Engine::Instance()->GetEarthquake().Reset();

#ifdef _DEBUG
    int numCourses = PoolCourseManager::Instance()->GetNumCourses();
    Assert(numCourses > 0);
    std::cout << "There are " << numCourses << " courses, apparently.\n";
#endif

    GetThumbnails();

    // Play theme music - we may come here by player quitting a frame, 
    // so we need to stop the current song.
    std::string song = GetEngine()->GetConfigValue("golf_song_ball_in_hole");
    GetEngine()->PlaySong(song);
  }
}

void EngineStatePoolCourseSelect::Red(bool down)
{
  // Not used, as GUI buttons are clicked to choose course.
}

void EngineStatePoolCourseSelect::MouseButton(bool down, bool, bool)
{ 
  if (s_canClick)
  {
    m_pSelectButton->MouseButton(down, false, false);
    m_pNextButton->MouseButton(down, false, false);
    m_pBackButton->MouseButton(down, false, false);
    
    for (int i = 0; i < NUM_THUMBNAILS; i++)
    {
      m_thumbnails[i]->MouseButton(down, false, false);
    }
  }
}

}


