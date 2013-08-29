/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineStatePoolBase.cpp,v $
Revision 1.1.2.9  2007/12/13 09:29:34  jay
Move hearts score to make room for thousands

Revision 1.1.2.8  2007/12/11 19:43:29  jay
Clean up; move heart score to left

Revision 1.1.2.7  2007/07/21 23:42:35  Administrator
Store Online ID in player names: only request ID from server when uploading
stats for playing an online game for the first time.

Revision 1.1.2.6  2007/03/27 09:20:18  jay
Birds eye height debug: bug due to child(0) not always being the table!

Revision 1.1.2.5  2007/03/22 22:50:00  Administrator
Birds Eye mode: centre camera (x, z) on table centre

Revision 1.1.2.4  2007/03/22 19:05:38  jay
Place ball/birds eye improvements

Revision 1.1.2.3  2007/03/21 09:56:45  jay
Online score table: use PoolOnlineReqManager to avoid thread bugs
(on Windows the old design crashed because the Thread object had gone
away by the time the thread started. I think..)

Revision 1.1.2.2  2007/03/12 23:41:49  Administrator
Remove dead code

Revision 1.1.2.1  2006/08/14 17:50:12  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:25  jay
Moved Pool code from Pool/ to Pool/

Revision 1.2.2.1.4.19  2006/04/11 17:02:57  jay
Kill explosion effect on Birds Eye view

Revision 1.2.2.1.4.18  2005/09/29 19:03:45  jay
Reset extra hearts at start of game

Revision 1.2.2.1.4.17  2005/09/26 20:56:52  jay
Don't create no of bonuses

Revision 1.2.2.1.4.16  2005/09/20 13:14:53  jay
Add Extra hearts to players' totals

Revision 1.2.2.1.4.15  2005/09/06 17:22:02  jay
Turn off debug output

Revision 1.2.2.1.4.14  2005/09/05 20:05:04  jay
Show no. of bonuses collected - make more legible

Revision 1.2.2.1.4.13  2005/08/26 21:19:53  jay
Show bonuses collected by player

Revision 1.2.2.1.4.12  2005/07/30 12:16:49  jay
Minor tweaks

Revision 1.2.2.1.4.11  2005/07/17 23:02:02  jay
Move key icons to show vertical power guage

Revision 1.2.2.1.4.10  2005/07/05 12:55:12  jay
Centre explanation text

Revision 1.2.2.1.4.9  2005/07/05 09:35:49  jay
Big text - minor tweaking

Revision 1.2.2.1.4.8  2005/06/22 21:38:03  jay
Fixes for big text; new HUD ball

Revision 1.2.2.1.4.7  2005/06/19 21:28:27  jay
Move HUD drawing code to here

Revision 1.2.2.1.4.6  2005/05/24 20:38:05  jay
Add shading to next ball HUD

Revision 1.2.2.1.4.5  2005/05/15 17:13:09  jay
Add Big Text for Pool

Revision 1.2.2.1.4.4  2005/05/08 17:27:07  jay
Add pool cue

Revision 1.2.2.1.4.3  2005/04/03 16:29:49  jay
Added entirely separate camera for birds eye mode.

Revision 1.2.2.1.4.2  2005/04/01 05:59:21  jay
Added flag for balls to set when not in view frustum
Added rough pool HUD

Revision 1.2.2.1.4.1  2005/03/17 20:56:07  jay
Added statics to handle birds eye view

Revision 1.2.2.1  2004/09/29 13:44:49  Administrator
Don't draw ball shadow if it's out of play

Revision 1.2  2004/09/15 09:23:03  Administrator
Whitespace only

Revision 1.1  2004/09/08 15:43:04  jay
Added to repository
  
*/


#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolBase.h"
#include "PoolCamera.h"
#include "Engine.h"
#include "PoolMisc.h"
#include "VisibleGameObject.h"
#include "FreeMovingSolidGameObject.h"
#include "Trajectory.h"
#include "TextFactory.h"
#include "PoolGameState.h"
#include "StringUtils.h"
#include "Level.h"
#include "PoolHole.h"
#include "TextColourizerColourList.h"
#include "PoolCourseManager.h"
#include "Bonus.h"
#include "LevelServer.h"
#include "Frustum.h"
#include "SimpleCamera.h"
#include "TextWriter.h"
#include "MovingSolid.h"
#include "FunctionSimpleRotation.h"
#include "PoolGameState.h"
#include "PoolPlayerStats.h"
#include "PoolOnlineReqManager.h"
#include "PoolOnline.h"

namespace Amju
{
PSolidComponent EngineStatePoolBase::s_pPlayerNameText;
PSolidComponent EngineStatePoolBase::s_pScoreText;
PSolidComponent EngineStatePoolBase::s_pBonusText;
PSolidComponent EngineStatePoolBase::s_pBallText;
TexturedQuad EngineStatePoolBase::s_tqBall;
PCamera EngineStatePoolBase::s_pCamera;
PCamera EngineStatePoolBase::s_pBirdsEyeCam;
bool EngineStatePoolBase::s_isBirdsEye = false;
bool EngineStatePoolBase::s_movingBallNotInFrustum = false;
float EngineStatePoolBase::s_left;
float EngineStatePoolBase::s_top;
float EngineStatePoolBase::s_z;
float EngineStatePoolBase::s_ballSize = 5.0f;
bool EngineStatePoolBase::s_flashBalls = true;
RCPtr<Trajectory> EngineStatePoolBase::s_pTrajectory;
PoolCue EngineStatePoolBase::s_cue;
PSolidComponent EngineStatePoolBase::s_pBigText;
float EngineStatePoolBase::s_bigTextTime;
RCPtr<ExplosionEffect> EngineStatePoolBase::s_pBigTextExpl;
int EngineStatePoolBase::s_extraHearts[2];

Trajectory* EngineStatePoolBase::GetTrajectory()
{
  return s_pTrajectory.GetPtr();
}  

void EngineStatePoolBase::SetBirdsEye(bool b)
{
  SetBigText("");
  s_isBirdsEye = b;
  if (b)
  {
    // TODO This height should depend on the size of the table.
    // We could work this out by finding the max distance between
    // two pockets (PoolHoles)

    // Good plan, but for now let's use the b.sphere of the table.
    // Assume table is child(0) of the scene.
    SolidComponent* pScene = LevelServer::Instance()->GetCurrentLevel()->GetScene().GetPtr();
    Assert(pScene);
    Assert(dynamic_cast<SolidComposite*>(pScene));
    SolidComponent* pTable = dynamic_cast<SolidComposite*>(pScene)->GetChild(0).GetPtr();
    Assert(pTable); 
    float r = pTable->GetBoundingSphere()->GetRadius();
#ifdef _DEBUG
std::cout << "Birds Eye View: Table: " << pTable->GetName() 
  << ":  b.sphere radius: " << r 
  << "\n";
#endif

    // Use r as height 
    Vec3f v = pTable->GetBoundingSphere()->GetCentre();
    v.y = r * 1.5f;
    v.z += 0.01f; // avoid div by 0 ???
    s_pBirdsEyeCam->SetOrientation(Orientation(v.x, v.y, v.z, 0, 90.0f, 0));
  }
  else
  {
    s_pCamera->Reset();
  }
}

bool EngineStatePoolBase::IsBirdsEye()
{
  return s_isBirdsEye;
}

EngineStatePoolBase::EngineStatePoolBase()
{
  s_extraHearts[0] = 0;
  s_extraHearts[1] = 0;
}

void EngineStatePoolBase::AddExtraHearts(int playerId, int numHearts)
{
  Assert(playerId == 0 || playerId == 1);
  s_extraHearts[playerId] += numHearts;
}

void EngineStatePoolBase::Init()
{
  s_left = Engine::Instance()->GetConfigFloat("name_left");
  s_top = Engine::Instance()->GetConfigFloat("name_top");
  s_z = Engine::Instance()->GetConfigFloat("name_z");

  s_pTrajectory = new Trajectory;
  Trajectory::Init();
  float trajecDt = Engine::Instance()->GetConfigFloat("trajec_dt");
  s_pTrajectory->SetDeltaTime(trajecDt);

  std::string ball = Engine::Instance()->GetConfigValue("golf_ball_tq");
  std::string ballAlpha = Engine::Instance()->GetConfigValue("golf_ball_tq_a");
  s_tqBall.Load(ball, ballAlpha);

  if (!s_cue.Load())
  {
    //ReportError("Failed to load cue.");
  }
}

void EngineStatePoolBase::OnNewLevel()
{
}

void EngineStatePoolBase::OnNewGame()
{
#ifdef RESET_EXTRA_HEARTS_DEBUG
  std::cout << "RESET EXTRA HEARTS!\n";
#endif

  s_extraHearts[0] = 0;
  s_extraHearts[1] = 0;
}

void EngineStatePoolBase::DrawHudBall()
{
  DrawBonusesLeft();

  // Shading overlay quad
  static TexturedQuad tqOver;
  static bool first = true;
  if (first)
  {
    first = false;
    std::string tqover = Engine::Instance()->GetConfigValue("pool_tqover");
    std::string tqovera = Engine::Instance()->GetConfigValue("pool_tqovera");
    tqOver.Load(tqover, tqovera);
  }

  // POOL: Draw ball which player should hit.
  TexturedQuad* pBall = GetRules(m_pLevel.GetPtr())->GetBehaviour()->GetHudBall();
  if (pBall)
  {
    float w = 2.0f; // TODO
    float t = 3.0f; // TODO
    float l = 2.0f; // TODO
    float b = t + w;
    float r = l + w;
    pBall->Draw(t, l, b, r);
    tqOver.Draw(t, l, b, r);
  }
#ifdef TARGET_DEBUG
  else
  {
    std::cout << "TARGET: No target ball.\n";
  }
#endif
}

void EngineStatePoolBase::DrawPlayerName()
{
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PushMatrix();
  AmjuGL::Translate(s_left, s_top, s_z);
  s_pPlayerNameText->Draw();
  //AmjuGL::Translate(0, -1.5f, 0);
  //AmjuGL::Scale(0.5f, 0.5f, 0.5f);
  //s_pScoreText->Draw();
  AmjuGL::PopMatrix();
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
}

void EngineStatePoolBase::DrawScore()
{
  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PushMatrix();
  AmjuGL::Translate(s_left, s_top, s_z);
  //s_pPlayerNameText->Draw();
  AmjuGL::Translate(0, -1.5f, 0);
  AmjuGL::Scale(0.5f, 0.5f, 0.5f);
  s_pScoreText->Draw();
  AmjuGL::PopMatrix();
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);
}

void EngineStatePoolBase::DrawBallsPotted()
{
  // POOL: for each ball potted by the current player,
  // draw the ball's textured quad along the score HUD.
    
  static TexturedQuad tqOver;
  static bool first = true;
  if (first)
  {
    first = false;
    std::string tqover = Engine::Instance()->GetConfigValue("pool_tqover");
    std::string tqovera = Engine::Instance()->GetConfigValue("pool_tqovera");
    tqOver.Load(tqover, tqovera);
  }

  const std::vector<TexturedQuad>& tqs = 
    GetEngine()->GetGameState()->GetCurrentPlayerInfo()->m_tqs;

  int s = tqs.size();
  static const float h = Engine::Instance()->GetConfigFloat("pool_hud_h"); 
  static const float t = Engine::Instance()->GetConfigFloat("pool_hud_t");  
  for (int i = 0; i < s; i++)
  {
    // Draw 8 balls on one line; 8 on the next line.
    int j = i % 8;
    int k = i / 8;

    // Start at right hand side, working towards the left.
    float l = 22.0f - (float)j * h; // height == width
    float r = l + h;
    float t2 = t + (float)k * h;
    float b = t2 + h;
    const TexturedQuad& tq = tqs[i];
    (const_cast<TexturedQuad&>(tq)).Draw(t2, l, b, r);

    // Overlay the ball with second TQ to give shading.
    tqOver.Draw(t2, l, b, r);
  }
}

void EngineStatePoolBase::DrawBallsLeft()
{
  static float time = 0;
  static int count = 0;
  time += Engine::Instance()->GetDeltaTime();
  if (time > 0.5f) // TODO CONFIG
  {
    time = 0;
    ++count;
  }
  if (s_flashBalls)
  {
    if (time > 0.25f) // TODO CONFIG
    {
      if (count > 4)
      {
        s_flashBalls = false;
      }
      return;
    } 
  }
  else
  {
    count = 0;
  }

  AmjuGL::Disable(AmjuGL::AMJU_DEPTH_READ);
  AmjuGL::PushMatrix();
  AmjuGL::Translate(s_left, s_top, s_z);
  AmjuGL::Translate(9.0f, -1.5f, 0);

  // Draw ball
  //AmjuGL::PushMatrix();
  //AmjuGL::Translate(0, -0.4f, 0); // so ball is centred on number
  //AmjuGL::Scale(3.0f, 3.0f, 3.0f);
  //s_tqBall.Draw(4.0f, 6.0f, 6.0f, 8.0f); // t, l, b, r
  //AmjuGL::PopMatrix();

  static const float ballx = 9.0f;
  static const float bally = 2.25f;
  s_tqBall.Draw(bally - s_ballSize, ballx - s_ballSize, bally + s_ballSize, ballx + s_ballSize); // t, l, b, r

  AmjuGL::Scale(0.5f, 0.5f, 0.5f);

  s_pBallText->Draw();
  AmjuGL::PopMatrix();
  AmjuGL::Enable(AmjuGL::AMJU_DEPTH_READ);

}

void EngineStatePoolBase::DrawBonusesLeft()
{
  static TexturedQuad heart;
  static bool first = true;
  if (first)
  {
    first = false;
    heart.Load("heart4.png", "heart4_a.png"); // TODO CONFIG
  }

  static float extraHeartTime = 0;
  extraHeartTime += Engine::Instance()->GetDeltaTime();
  if (extraHeartTime > 0.1f) // TODO CONFIG
  {
    extraHeartTime = 0;
    bool playwav = false;

    // Increase hearts for each player if there are extra hearts to be added.
    for (int i = 0; i < 2; i++)
    {
      if (s_extraHearts[i] > 0)
      {
        playwav = true;

        int amount = 1; // amount to add to total this time
        if (s_extraHearts[i] > 20)
        {
          amount = 10;
        }
        else if (s_extraHearts[i] > 10)
        {
          amount = 5;
        }
        s_extraHearts[i] -= amount;
      
        PlayerStats* p = Engine::Instance()->GetGameState()->
          GetPlayerInfo(i)->GetPlayerStats();
        PoolPlayerStats* pp = dynamic_cast<PoolPlayerStats*>(p);
        if (pp)
        {
          pp->AddBonus(1 /* type */, amount);
        }
      }
    }
    if (playwav)
    {
      // Play bonus heart wav
      static std::string wav = Engine::Instance()->GetConfigValue(
        "golf_wav_14");
      Engine::Instance()->PlayWav(wav);
    }
  }

  int numBonuses = 0;

  PlayerStats* p = Engine::Instance()->GetGameState()->
    GetCurrentPlayerInfo()->GetPlayerStats();
  
  PoolPlayerStats* pp = dynamic_cast<PoolPlayerStats*>(p);
  if (pp)
  {
    numBonuses = pp->GetNumBonuses(1); // TODO other types
  }

  if (numBonuses == 0)
  {
    return;
  }

  // Try moving the heart score to the left, as some players have thousands!
  static const float HEART_LEFT = 20.9f; 
  static const float HEART_WIDTH = 2.0f;
  static const float HEART_TOP = 0.1f;
  static const float HEART_HEIGHT = 1.4f;

  heart.Draw(HEART_TOP, HEART_LEFT, HEART_TOP + HEART_HEIGHT, HEART_LEFT + HEART_WIDTH);

  PoolFont* pFont = TheFontManager::Instance()->GetFont("cheri-1.0");
  float size = pFont->GetSize();
  std::string s = ToString(numBonuses);
  float left = HEART_LEFT + 0.7f - 0.125f * s.length();
  float top = HEART_TOP + 0.3f;
  pFont->SetSize(size * 0.75f);
  pFont->PrintNoBlend(left, top, s.c_str());
  pFont->SetSize(size);
}

void EngineStatePoolBase::CreateBonusesLeft()
{
  TextColourizerColourList colourizer;
  colourizer.AddColour(Colour(1.0f, 1.0f, 1.0f, 1.0f));

  int numBonuses = 0;

  PlayerStats* p = Engine::Instance()->GetGameState()->
    GetCurrentPlayerInfo()->GetPlayerStats();
  
  PoolPlayerStats* pp = dynamic_cast<PoolPlayerStats*>(p);
  if (pp)
  {
    numBonuses = pp->GetNumBonuses(1); // TODO other types
  }

  if (numBonuses == 0)
  {
    s_pBonusText = 0;
  }
  else
  {
    // Get number of uncollected bonuses in the current room.
    std::string bonus = ToString(numBonuses);

    s_pBonusText = TextFactory::Instance()->Create(
      bonus, &colourizer, false);
  }
}

void EngineStatePoolBase::CreatePlayerName(const std::string& text)
{
  // Whenever the player name changes, update the bonus count.
  ////CreateBonusesLeft();

  typedef std::map<std::string, PSolidComponent> Cache;
  static Cache c;
  if (c.find(text) != c.end())
  {
    s_pPlayerNameText =  c[text];
  }  
  else
  {
    s_pPlayerNameText = TextFactory::Instance()->Create(
      text,
      TextFactory::Instance()->GetCurrentColourizer(), 
      false); // false => don't centre

    c[text] = s_pPlayerNameText;
  }
}

void EngineStatePoolBase::CreateScore()
{
  std::string score;
  score += "score ";
  score += ToString(GetEngine()->GetGameState()->GetCurrentPlayerInfo()->m_score);
  
  TextColourizerColourList colourizer;
  colourizer.AddColour(Colour(1.0f, 0, 0.25f, 1.0f));

  s_pScoreText = TextFactory::Instance()->Create(
    score,
    &colourizer,
    false); // false => don't centre

}

void EngineStatePoolBase::CreateBallsLeft()
{
  std::string balls = ToString(GetEngine()->GetGameState()->GetCurrentPlayerInfo()->m_numBalls);

  TextColourizerColourList colourizer;
  colourizer.AddColour(Colour(0, 0, 0, 1.0f));
  s_pBallText = TextFactory::Instance()->Create(
    balls, &colourizer, true);
}

void EngineStatePoolBase::Draw()
{
  Assert(s_pTrajectory.GetPtr());
  EngineRunningBase::DrawBackground();

  // Code below copied from  EngineRunningBase::DrawForeground()
  // with calls to draw player removed.

  // Create the view frustum. This is used in the drawing calls to cull objects
  // out of view.
  Frustum::Instance()->Create();

  // TODO make configurable.. this is just a test
  if (IsUnderwater())
  {
    GetEngine()->PushColour(0.5f, 0.5f, 1.0f, 1.0f);
  }

  // Draw static elements of scene. Get number of polys drawn.
  m_scenePolys = m_pLevel->Draw();

  // Scene display lists may be stored in a sorted list. 
  // This call executes the sorted display lists
  // then empties the container. 
//  LeafData::GetRenderer()->DrawList();

  // Draw Game Objects. These are stateful, interactive things, including
  // moving Platforms, bonuses, etc.
  DrawGameObjects(false); // false => non-translucent

  // POOL: Draw 'cool' 'foul' etc 3D letters
  DrawBigText();

  AmjuGL::Enable(AmjuGL::AMJU_BLEND);
  DrawGameObjectShadows();
  DrawGameObjects(true); // true => translucent, or with transparent areas.
  AmjuGL::Disable(AmjuGL::AMJU_BLEND);

  // Execute GameObject display lists.
//  LeafData::GetRenderer()->DrawList();

  // If underwater, we pushed a colour onto the colour stack.
  if (IsUnderwater())
  {
    GetEngine()->PopColour();   
  }
}

void EngineStatePoolBase::DrawOverlays()
{
/*
  // POOL we don;t like this

  static TexturedQuad tq;
  static bool first = true;
  if (first)
  {
    tq.Load("wh128.png", "golf-base-overlay.png");
  }

  Engine::Instance()->PushColour(1.0f, 1.0f, 1.0f, 0.65f);
  tq.Draw(0.5f, 1.5f, 7.5f, 10.5f);
  Engine::Instance()->PopColour();
*/

  DrawPlayerName();
  DrawBallsPotted(); // POOL - not balls left.

  GetEngine()->GetTextWriter()->PrintCentre(0, GetRules(m_pLevel.GetPtr())->GetExplanation().c_str());
}

void EngineStatePoolBase::DrawPlayer()
{
}

void EngineStatePoolBase::PlayerCollisionTest()
{
}

bool EngineStatePoolBase::Load()
{
  s_pCamera = new PoolCamera;
  s_pCamera->GetConfigValues();

  SimpleCamera* pSc = new SimpleCamera; 
  // Set up birds eye camera
  pSc->SetDirection(SimpleCamera::DOWN);
  s_pBirdsEyeCam = pSc;
  s_pBirdsEyeCam->GetConfigValues();
  float h = Engine::Instance()->GetConfigFloat("pool_becamheight");
  // TODO 90 degs not working
  s_pBirdsEyeCam->SetOrientation(Orientation(0, h, 0.001f, 0, 90.0f, 0));
  return true;
}

void EngineStatePoolBase::DoCameraWork()
{
  EngineRunningBase::DoCameraWork();
}

PCamera EngineStatePoolBase::GetCamera() 
{
  return SGetCamera();
}

PCamera EngineStatePoolBase::SGetCamera() 
{
  if (IsBirdsEye())
  {
    return s_pBirdsEyeCam;
  }
  else
  { 
    return s_pCamera; 
  }
}

void EngineStatePoolBase::SetActive(bool active)
{
  if (active)
  {
    s_movingBallNotInFrustum = false;
  }
}

void EngineStatePoolBase::Update()
{
#ifdef ONLINE_PLAYER_PROFILE
  // Update Request Manager (handles http request threads)
  ThePlayerProfileManager::Instance()->Update();
#endif

  // If a moving ball is outside of the view frustum, pull the camera back.
  ((PoolCamera*)s_pCamera.GetPtr())->SetPoolPullBackRequired(s_movingBallNotInFrustum);
  s_movingBallNotInFrustum = false;

  GetCamera()->Update();

  if (!IsBirdsEye())
  {
    // Update the camera so it is always above the ball
    float camY = GetCamera()->GetOrientation()->GetY();
    float ballY = GetBall()->GetOrientation()->GetY();
    ballY += 1.0f; // TODO CONFIG
    if (camY < ballY)
    {
      Orientation o = *(GetCamera()->GetOrientation());
      o.SetY(ballY);
      GetCamera()->SetOrientation(o);
    }
  }

  EngineRunningBase::Update();
}

void EngineStatePoolBase::SetMovingBallNotInFrustum()
{
  s_movingBallNotInFrustum = true;
}

void EngineStatePoolBase::DrawBigText()
{
  if (!s_pBigText.GetPtr())
  {
    return;
  }

  if (IsBirdsEye())
  {
    return;
  }

  static const float BIG_TEXT_MAX_TIME = Engine::Instance()->GetConfigFloat(
    "pool_bigtextmaxtime");
  if (s_bigTextTime > BIG_TEXT_MAX_TIME) 
  {
    return;
  }
 
  s_pBigText->Update();
  AmjuGL::PushMatrix();
  if (s_bigTextTime < 1.0f)
  {
    float s = s_bigTextTime * 4.0f;
    AmjuGL::Scale(s, s, s);
  }
  else
  {
    AmjuGL::Scale(4.0f, 4.0f, 4.0f);
  }
  AmjuGL::Translate(0, 1.0f, 0);
  s_pBigText->Draw();
  AmjuGL::PopMatrix();

  s_pBigTextExpl->Draw();

  s_bigTextTime += Engine::Instance()->GetDeltaTime();
}

void EngineStatePoolBase::SetBigText(const char* text)
{
  static std::string prevText;
  if (text == prevText)
  {
    return;
  }
  prevText = text;
 
  if (std::string(text).empty())
  {
    s_pBigText = 0;
    if (s_pBigTextExpl.GetPtr())
    {
      s_pBigTextExpl->Kill();
    }
    return;
  }

#ifdef BIG_TEXT_DEBUG
std::cout << "**** NEW BIG TEXT ***** " << text << "\n";
#endif
 
  typedef std::map<std::string, PSolidComponent> Cache;
  static Cache c;
  if (c.find(text) != c.end())
  {
    s_pBigText =  c[text];
  }  
  else
  {
    // Create 3D text
    MovingSolid* pComp = new MovingSolid;
    TextColourizerColourList tc;
    tc.AddColour(Colour(1.0f, 0, 0, 0));
    tc.AddColour(Colour(1.0f, 1.0f, 0, 0));

    pComp->SetChildSolid(TextFactory::Instance()->Create(text, 0, true));
    // TODO Function
    FunctionSimpleRotation* pFunc = new FunctionSimpleRotation;  
    pFunc->SetMaxY(1000.0f);
    pFunc->SetPeriodY(3.0f);
    pComp->SetFunction(pFunc);
    s_pBigText = pComp; 

    c[text] = s_pBigText;
  }

  s_bigTextTime = 0;

  if (!s_pBigTextExpl.GetPtr())
  {
    s_pBigTextExpl = new ExplosionEffect;
    std::string effect = Engine::Instance()->GetConfigValue("textexplode");
    if (!s_pBigTextExpl->Load(effect))
    {
      Assert(0 &&  "Failed to load big text explosion");
    }
  }
  Vec3f v(0, 5.0f, 0); // TODO
  s_pBigTextExpl->Reset(v);
}
}

