/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AttribDecorator.h,v $
Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

#if !defined(ATTRIB_DECORATOR_H_INCLUDED)
#define ATTRIB_DECORATOR_H_INCLUDED

#include "SolidComposite.h"
#include "Attrib.h"

namespace Amju
{
// Decorates a Solid Component, so this class specializes SolidComposite.
// Modifies one attribute of the Component, e.g. Depth Test, colour, etc.
// Patterns: Decorator, Composite.
// We would expect to have only one child, but that's not necessary.
class AttribDecorator : public SolidComposite
{
public:
  AttribDecorator();
  AttribDecorator(const AttribDecorator&);

  virtual ~AttribDecorator();

  virtual PSolidComponent Clone();

  virtual void Draw();
  virtual int CullDraw(const Frustum& f);

  // Load a composite, keeping track of the current translation/rotation.
  virtual bool Load(const std::string& filename);

  // Non virtuals
  // ------------

  // Create an attribute object from its name; returns 0 if name is
  // invalid.
  static PAttrib CreateAttrib(const std::string& name);

  void SetAttrib(PAttrib pAttrib);
 
  PAttrib GetAttrib() { return m_pAttrib; }

protected:

  // The attribute we modify. E.g. lighting, blend, etc.
  PAttrib m_pAttrib;

  // The ID/name of the attribute, which we use to create the Attribute in 
  // CreateAttrib()
  std::string m_attribName;

#if defined(SCENE_EDITOR)
  // Save - used by SceneEditor.
  virtual bool Save(bool recursive = false);
#endif
};
}
#endif

