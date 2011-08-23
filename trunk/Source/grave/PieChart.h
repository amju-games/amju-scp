/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: PieChart.h,v $
Revision 1.1  2004/09/08 15:42:54  jay
Added to repository
  
*/

#if !defined(PIE_SHART_H_INCLUDED)
#define PIE_SHART_H_INCLUDED

#include <map>
#include <string>
#include "Colour.h"
#include "TexturedQuad.h"
#include "SolidComponent.h"

namespace Amju
{
class Texture;

class PieChart
{
public:
  typedef std::string PieChartKey;

  virtual ~PieChart() {}
  virtual void Draw() = 0;

  void Set(PieChartKey key, float value);
  void SetColour(PieChartKey key, Colour c);
  void SetSliceAngle(float degrees);
  void Clear();

protected:
  typedef std::map<PieChartKey, float> ValueMap;
  ValueMap m_values;
  typedef std::map<PieChartKey, Colour> ColourMap;
  ColourMap m_colours;

  // One pie chart slice.
//  static TexturedQuad* s_pSlice;
  // Angle covered by one slice (degrees).
  // To draw the chart we draw 360 / s_sliceAngle slices.
  float m_sliceAngleDegs;

  // Sum of all values
  float m_total;
};

class PieChart2d : public PieChart
{
public:
  virtual void Draw();

  void SetSlice(Texture* pTexture);

protected:
  TexturedQuad m_slice;
};

class PieChart3d : public PieChart
{
public:
  virtual void Draw();

  void SetSlice(PSolidComponent pSlice);

protected:
  PSolidComponent m_pSlice;
};
}

#endif

