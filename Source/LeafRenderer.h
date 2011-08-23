/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: LeafRenderer.h,v $
Revision 1.1  2004/09/08 15:42:52  jay
Added to repository
  
*/

#if !defined(LEAF_RENDERER_H_INCLUDED)
#define LEAF_RENDERER_H_INCLUDED

namespace Amju
{
class LeafData;

// LeafRenderer is an interface for classes which draw leaf nodes. OpenGL
// subclass will use display lists etc,; MFC subclass will draw to a DC.

// Patterns: Abstract Factory (TODO)
// The Leaf Renderer concrete class and Polygon Renderer concrete class will
// both use the same scheme (i.e. OpenGL or MFC, etc). An Abstract Factory
// would create objects of both concrete types depending on the drawing 
// context.
class LeafRenderer
{
public:
  // Initialise the given Leaf Data object for rendering.
  virtual void Init(LeafData* pLd) = 0;

  // Render the given Leaf Data object.
  virtual void Draw(LeafData* pLd) = 0;

  virtual void Clear(LeafData* pLd) = 0;

  // Lets subclasses build up a list of things to Draw. Calling this function
  // draws every item in the list, then empties the list.
  virtual void DrawList() {}
};
}
#endif

