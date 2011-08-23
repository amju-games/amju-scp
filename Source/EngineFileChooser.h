/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: EngineFileChooser.h,v $
Revision 1.1  2004/09/08 15:42:35  jay
Added to repository
 
*/

#if !defined(ENGINE_FILE_CHOOSER_H_INCLUDED)
#define ENGINE_FILE_CHOOSER_H_INCLUDED

#include "EngineMenuState.h"
#include "TextLeaf.h"

namespace Amju
{
// EngineFileChooser: Engine state where player picks saved file before starting to play.
class EngineFileChooser : public EngineMenuState
{
public:
  static const std::string Name; // ID of this state.
  virtual void SetActive(bool active);
  virtual bool Load();
  virtual void Clear();
  // Draw scene.
  virtual void Draw();
  // Draw anything to be overlaid on top of everything else. 
  virtual void DrawOverlays();

  virtual void Blue(bool);
  virtual void Red(bool);

protected:
  // Greeting
  TextLeaf m_greet;
};
}
#endif
