/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: FileCheckState.h,v $
Revision 1.1.8.1  2006/03/08 19:21:31  jay
Full screen splash

Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

#if !defined(FILE_CHECK_STATE_H_INCLUDED)
#define FILE_CHECK_STATE_H_INCLUDED

#include "EngineStateProgressBar.h"

namespace Amju
{
// Initial Engine state. Reads in all files to make sure they are ok. 
class FileCheckState : public EngineStateProgressBar
{
public:
  static const std::string Name; // ID of this state.

  FileCheckState();

  virtual void Draw();
  virtual void DrawOverlays();
  virtual void SetActive(bool); 
  // Clear any warnings, greeting etc. once we have loaded everything.
  virtual void ClearScrollText();

private:
  void DeathBlow(); // Print error message.

  std::string m_currentFile;

  // Number of frames drawn. The idea is that we draw a frame, then do our 
  // loading. That way we display a welcome message first, then any error
  // messages that crop up during loading.
  int m_frames;
  // Set to true when all files have loaded successfully.
  bool m_success;
};
}
#endif
