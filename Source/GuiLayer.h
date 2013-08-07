/*
Amju Games source code (c) Copyright Jason Colman 2000-2006
*/

#ifndef GUI_LAYER_H_INCLUDED
#define GUI_LAYER_H_INCLUDED

#include "RCPtr.h"
#include "TexturedQuad.h"
#include "Colour.h"
#include <vector>

namespace Amju
{
// A layer in a GuiButton ( + other GuiElements ?)
// A layer can be a TexturedQuad, or decorated with colours, etc.
class GuiLayer : public RefCounted
{
public:
  virtual void Draw(float top, float left, float bottom, float right) = 0;
};

class GuiLayerTq : public GuiLayer
{
public:
  GuiLayerTq(const TexturedQuad& tq);
  virtual void Draw(float top, float left, float bottom, float right);
private:
  TexturedQuad m_tq;
};

// Base class for decorators
class GuiLayerDecorator : public GuiLayer
{
public:
  GuiLayerDecorator(GuiLayer*);   
  virtual void Draw(float top, float left, float bottom, float right);
protected:
  RCPtr<GuiLayer> m_pChild;
};

typedef std::vector<Colour> ColourList;

class GuiLayerColourList : public GuiLayerDecorator
{
public:
  GuiLayerColourList(GuiLayer* pChild, const ColourList& colours, float colourTimeSecs, bool interpolate = false);
  virtual void Draw(float top, float left, float bottom, float right);

protected:
  ColourList m_colours;
  float m_colourTime;
  float m_time; // time spent on current colour
  bool m_interpolate;  
  int m_index; // current index into m_colours
};
}

#endif

