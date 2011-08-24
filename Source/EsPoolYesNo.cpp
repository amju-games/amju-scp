/*
Amju Games source code (c) Copyright Jason Colman 2007
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EsPoolYesNo.h"
#include "Engine.h"
#include "PoolBg.h"
#include "PoolOnline.h"
#include "TextWriter.h"

namespace Amju
{
static float TOP = 2.3f;
static float LEFT = 8.0f;
//static float WIDTH = 8.0f;
//static float HEIGHT = 2.0f;
static float HSPACE = 2.25f; 

void ReportError(const std::string&);

void OnYes()
{
  ((EsPoolYesNo*)Engine::Instance()->GetEngineState().GetPtr())->OnYes();
}

void OnNo()
{
  ((EsPoolYesNo*)Engine::Instance()->GetEngineState().GetPtr())->OnNo();
}

EsPoolYesNo::EsPoolYesNo() : m_firstLineY(3.0f)
{
}

void EsPoolYesNo::AddQ(const std::string& s)
{
  m_question.push_back(s);
}

void EsPoolYesNo::SetActive(bool active)
{
  EngineStateText::SetActive(active);
  Engine::Instance()->SetLetterbox(false);
}

bool EsPoolYesNo::Load()
{
  if (!m_bg.Load(GetEngine()->GetConfigValue("file_bg"), ""))
  {
    ReportError("Failed to load BG for yes/no state");
    return false;
  }

  m_pYesButton = new GuiButton;
  if (!m_pYesButton->Load(m_yesFilename))
  {
    ReportError("Failed to load yes button.");
    return false;
  }
  m_pYesButton->SetSize(8.0f, 2.0f); // width, height
  m_pYesButton->SetRelPos(TOP + 2.0f * HSPACE, LEFT); // top, left
  m_pYesButton->SetCommand(&Amju::OnYes);

  m_pNoButton = new GuiButton;
  if (!m_pNoButton->Load(m_noFilename))
  {
    ReportError("Failed to load no button.");
    return false;
  }
  m_pNoButton->SetSize(8.0f, 2.0f); // width, height
  m_pNoButton->SetRelPos(TOP + 3.0f * HSPACE, LEFT); // top, left
  m_pNoButton->SetCommand(&Amju::OnNo);

  return true;
}

void EsPoolYesNo::Update()
{
  EngineStateText::Update();

  // TODO This should be called by Engine, or EngineState, so it's
  // always updated.
  // Call here just in case we are in the middle of an online operation
  ThePoolOnlineManager::Instance()->Update();

}

void EsPoolYesNo::Draw()
{
  AmjuGL::PushMatrix();
  DrawBg();
  
  m_pCam->Update();
  m_pCam->Draw(); 
  
  m_lighting.DrawLight();
  
  m_or.Draw();

  ThePoolBg::Instance()->Draw();
  AmjuGL::PopMatrix();
}

void EsPoolYesNo::DrawOverlays()
{
  m_pYesButton->Draw();
  m_pNoButton->Draw();

  static const float LINE_SPACING = 1.0f;
  for (unsigned int i = 0; i < m_question.size(); i++)
  {
    float y = m_firstLineY + LINE_SPACING * (float)i;
    Engine::Instance()->GetTextWriter()->PrintCentre(y, m_question[i].c_str());
  }
}

void EsPoolYesNo::MouseButton(bool down, bool ctrl, bool shift)
{
  m_pYesButton->MouseButton(down, ctrl, shift);
  m_pNoButton->MouseButton(down, ctrl, shift);
}
}

