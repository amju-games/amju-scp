/*
Amju Games source code (c) Copyright Jason Colman 2005
$Log: EngineStatePoolPlayerMaint.cpp,v $
Revision 1.1.2.3  2006/08/30 21:12:15  Administrator
Get Pool to build in MSVC

Revision 1.1.2.2  2006/08/14 17:50:15  jay
Rename "Pool"

Revision 1.1.2.1  2006/07/24 18:20:32  jay
Moved Pool code from Pool/ to Pool/

Revision 1.1.2.3  2005/10/21 19:53:45  Administrator
Fix constants EDIT and DELETE so we can compile on windows - presumably
they are #defined somewhere in windows.h.

Revision 1.1.2.2  2005/10/21 17:09:15  jay
Edit and delete work ok now.

Revision 1.1.2.1  2005/08/26 22:03:08  jay
Added Player Maintenance state, not used yet.

*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "EngineStatePoolPlayerMaint.h"
#include "Engine.h"
#include "GuiButton.h"
#include "Font.h"
#include "EngineStatePoolGameSelect.h"
#include "TextWriter.h"
#include "EngineStatePlayerEdit.h"
#include "EngineStatePlayerDelete.h"
#include "PlayerNames.h"

namespace Amju
{
extern void ReportError(const std::string&);

const char* EngineStatePoolPlayerMaint::Name = "pool-player-maint";
// Register this State with the Engine.
static const bool registered = Engine::Instance()->
  RegisterEngineState(EngineStatePoolPlayerMaint::Name, new EngineStatePoolPlayerMaint);

static const int MAX_PLAYERS = 4;

void OnMaintEdit()
{
  ((EngineStatePoolPlayerMaint*)Engine::Instance()->GetEngineState(EngineStatePoolPlayerMaint::Name).GetPtr())->
    Edit();
}

void OnMaintDelete()
{
  ((EngineStatePoolPlayerMaint*)Engine::Instance()->GetEngineState(EngineStatePoolPlayerMaint::Name).GetPtr())->
    Delete();
}

void OnMaintCancel()
{
  // Cancel current selection
  ((EngineStatePoolPlayerMaint*)Engine::Instance()->GetEngineState(EngineStatePoolPlayerMaint::Name).GetPtr())->
    SetPlayerSelectMode(false);
}

void OnMaintBack()
{
  // Go back to main menu
  Engine::Instance()->ChangeState(EngineStatePoolGameSelect::Name,
    Engine::IMMEDIATE);
}

// Command to perform an operation on a player.
// The command is fired when user clicks on a player name button.
class PlayerOpCommand : public GuiCommand
{
public:
  PlayerOpCommand(int i) : m_num(i) {}
  virtual bool Do()
  {
    ((EngineStatePoolPlayerMaint*)Engine::Instance()->GetEngineState(EngineStatePoolPlayerMaint::Name).GetPtr())->
      DoOperationOnPlayer(m_num);    
	return false; // can't undo
  }
private:
  int m_num; // number of player button, 0-3
};

EngineStatePoolPlayerMaint::EngineStatePoolPlayerMaint()
{
  SetPlayerSelectMode(false);
}

void EngineStatePoolPlayerMaint::DoOperationOnPlayer(int player)
{
  std::string name = ThePlayerNames::Instance()->GetName(player);

  switch (m_op)
  {
  case AMJU_EDIT:
    // Go to Edit state.
#ifdef _DEBUG
std::cout << "EDIT PLAYER " << player << "\n";
#endif
    Engine::Instance()->ChangeState(EngineStatePlayerEdit::Name,
      Engine::IMMEDIATE);

    // Set name to edit
    ((EngineStatePlayerEdit*)Engine::Instance()->GetEngineState(EngineStatePlayerEdit::Name).GetPtr())->SetName(name);

    break;

  case AMJU_DELETE:
    // Go To Delete state.
#ifdef _DEBUG
std::cout << "DELETE PLAYER " << player << "\n";
#endif

    Engine::Instance()->ChangeState(EngineStatePlayerDelete::Name,
      Engine::IMMEDIATE);

    // Set name to delete 
    ((EngineStatePlayerDelete*)Engine::Instance()->GetEngineState(EngineStatePlayerDelete::Name).GetPtr())->SetName(name);

    break;
  }
}

void EngineStatePoolPlayerMaint::SetPlayerSelectMode(bool isPlayerSelectMode)
{
  m_isPlayerSelectMode = isPlayerSelectMode;
  if (m_isPlayerSelectMode)
  {
  }
  else
  {
    // Cancel any selected player ?
    m_text = "Please select edit or delete";
  } 
}

void EngineStatePoolPlayerMaint::Edit()
{
#ifdef _DEBUG
std::cout << "EDIT operation selected - now choose player\n";
#endif

  SetPlayerSelectMode(true);
  m_op = AMJU_EDIT;
  m_text = "Please choose the player to edit";
}

void EngineStatePoolPlayerMaint::Delete()
{
#ifdef _DEBUG
std::cout << "DELETE operation selected - now choose player\n";
#endif

  SetPlayerSelectMode(true);
  m_op = AMJU_DELETE;
  m_text = "Please choose the player to delete";
}

void EngineStatePoolPlayerMaint::SetActive(bool active)
{
  if (active)
  {
    m_time = 0;
    m_maxTime = 120.0f;
  }

  EngineStatePoolChoosePlayer::SetActive(active);
  SetPlayerSelectMode(false);
}

bool EngineStatePoolPlayerMaint::Load()
{
  if (!EngineStatePoolChoosePlayer::Load())
  {
    return false;
  }

  static const float x1 = Engine::Instance()->GetConfigFloat(
    "pool_choose_button_x1");
  static const float SIZE_W = Engine::Instance()->GetConfigFloat(
    "pool_choose_button_w");
  static const float H_MULT = 0.75f; // TODO CONFIG
  static const float SIZE_H = Engine::Instance()->GetConfigFloat(
    "pool_choose_button_h") * H_MULT;
  static const float SPACE_H = Engine::Instance()->GetConfigFloat(
    "pool_choose_button_h_space") * H_MULT;
  static const float HEIGHT_OFFSET = 3.0f;

  // Make player name buttons smaller
  for (int i = 0; i < MAX_PLAYERS; i++)
  {
    m_buttons[i].m_pButton->SetRelPos(i * SPACE_H + HEIGHT_OFFSET, x1);
    m_buttons[i].m_pButton->SetSize(SIZE_W, SIZE_H);
    m_buttons[i].m_pButton->SetCommand(new PlayerOpCommand(i));
    m_buttons[i].m_y = i * SPACE_H + HEIGHT_OFFSET + 0.2f;
  }

  m_pEditButton = new GuiButton;
  if (!m_pEditButton->Load(std::string("pool-player-edit-button.txt")))
  {
    ReportError("Failed to load edit button");
    return false;
  } 
  m_pEditButton->SetSize(SIZE_W, SIZE_H);
  m_pEditButton->SetRelPos(4.0f * SPACE_H + HEIGHT_OFFSET, x1);
  m_pEditButton->SetCommand(&OnMaintEdit);

  m_pDeleteButton = new GuiButton;
  if (!m_pDeleteButton->Load(std::string("pool-player-delete-button.txt")))
  {
    ReportError("Failed to load delete button");
    return false;
  } 
  m_pDeleteButton->SetSize(SIZE_W, SIZE_H);
  m_pDeleteButton->SetRelPos(5.0f * SPACE_H + HEIGHT_OFFSET, x1);
  m_pDeleteButton->SetCommand(&OnMaintDelete);

  m_pCancelButton = new GuiButton;
  if (!m_pCancelButton->Load(std::string("choose-player-cancel.txt")))
  {
    return false;
  } 
  m_pCancelButton->SetSize(SIZE_W, SIZE_H);
  m_pCancelButton->SetRelPos(6.0f * SPACE_H + 3.0f, x1);
  m_pCancelButton->SetCommand(&OnMaintCancel);

  m_pBackButton = new GuiButton;
  if (!m_pBackButton->Load(std::string("choose-player-cancel.txt"))) // TODO
  {
    return false;
  } 
  m_pBackButton->SetSize(SIZE_W, SIZE_H);
  m_pBackButton->SetRelPos(6.0f * SPACE_H + 3.0f, x1);
  m_pBackButton->SetCommand(&OnMaintBack);

  return true;
}

void EngineStatePoolPlayerMaint::Draw()
{
  EngineStatePoolChoosePlayer::Draw();
}

void EngineStatePoolPlayerMaint::DrawOverlays()
{
  PoolFont* pFont = TheFontManager::Instance()->GetFont("cheri-1.0");
  const float size = pFont->GetSize();

  AmjuGL::PushAttrib(AmjuGL::AMJU_BLEND);

  ////////glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  AmjuGL::Enable(AmjuGL::AMJU_BLEND);

  Engine::Instance()->GetTextWriter()->PrintCentre(1, m_text.c_str());

  int numPlayers = ThePlayerNames::Instance()->GetNumNames();
  for (int i = 0; i < MAX_PLAYERS; i++)
  {
    const PlayerButton& pb = m_buttons[i];

    pb.m_pButton->SetEnabled(m_isPlayerSelectMode && i < numPlayers);    
    pb.m_pButton->Draw();
    if (true) //pb.m_pButton->IsEnabled())
    {
      float scale = 1.0f;
      if (pb.m_pButton->IsMouseOver() || pb.m_pButton->IsSelected())
      {
        scale *= 1.1f;
      }
      float e = 0;
      if (pb.m_pButton->IsSelected())
      {
        e = 0.1f;
      }
      pFont->SetSize(size * scale);
      float centre = 12.0f - 2.5f * pFont->GetTextWidth(pb.m_name.c_str());
      pFont->PrintNoBlend(centre + e, pb.m_y + 0.3f + e, pb.m_name.c_str());

/*
      // Draw stats etc string
      pFont->SetSize(size * scale * 0.5f);
      float centre2 = 12.0f - 1.5f * pFont->GetTextWidth(pb.m_stats.c_str());
      pFont->PrintNoBlend(centre2 + e, pb.m_y + 0.8f + e, pb.m_stats.c_str());
*/
      pFont->SetSize(size);
    }
  }

  AmjuGL::PopAttrib(); // Blend

  if (m_isPlayerSelectMode)
  {
    m_pEditButton->SetEnabled(false);
    m_pDeleteButton->SetEnabled(false);
    m_pCancelButton->Draw();
  }
  else
  {
    m_pEditButton->SetEnabled(true);
    m_pDeleteButton->SetEnabled(true);
    m_pBackButton->Draw();
  }
  m_pEditButton->Draw();
  m_pDeleteButton->Draw();
}

void EngineStatePoolPlayerMaint::MouseButton(bool down, bool, bool)
{
  if (m_isPlayerSelectMode)
  {
    for (int i = 0; i < MAX_PLAYERS; i++)
    {
      m_buttons[i].m_pButton->MouseButton(down, false,false);
    }
    m_pCancelButton->MouseButton(down, false, false);
  }
  else
  {
    m_pEditButton->MouseButton(down, false, false);
    m_pDeleteButton->MouseButton(down, false, false);
    m_pBackButton->MouseButton(down, false, false);
  }
}

void EngineStatePoolPlayerMaint::TimerExpired()
{
  //OnMaintCancel();
}
}


