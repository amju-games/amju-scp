/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PowerGuage.h,v $
Revision 1.3.8.1  2005/07/17 23:00:24  jay
Allow vertical power guages

Revision 1.3  2004/09/17 13:50:44  jay
Added extra features for player stats

Revision 1.2  2004/09/15 09:13:17  Administrator
More functionality for Stats pages

Revision 1.1  2004/09/08 15:42:40  jay
Added to repository
  
*/

#if !defined(POWER_GUAGE_H_INCLUDED)
#define POWER_GUAGE_H_INCLUDED

#include "Guage.h"
#include "Colour.h"

namespace Amju
{
// Shows how much super-power is left.
// Displays a horizontal bar with a rectangle inside. The size of
// the rectangle ranges from 0 to the width of the bar.
// To use this kind of guage, set how long the power-up should last.
class PowerGuage : public Guage
{
public:
  PowerGuage();

  virtual void Draw();
  virtual void Update();

  virtual bool Load(File*);

  // Set inner rectangle colour.
  void SetColour(const Colour& c);

  // Set how long the guage should take to run from full to empty.
  void SetPowerTime(float time);
  float GetPowerTime() const;

  void IncreaseTime(float secs);

  // Set anchor position - defaults to Left hand side.
  void SetAnchor(float);

  bool IsVertical() const;
  void SetVertical(bool isVertical);

protected:
  // Foreground position
  float m_ftop, m_fleft, m_fbottom, m_fright;
  // Current time left for guage
  float m_time;
  // Time set in SetPowerTime()
  float m_maxTime;

  Colour m_colour;

  // Anchor value: the point from which the coloured bar is drawn.
  // This means that if the value is the same as the anchor value, the
  // bar has zero length.
  // Typically the value will be zero (start at left), 0.5 (start at middle)
  // or 1.0 (start at right).
  float m_anchor;

  bool m_isVertical;
};
}

#endif

