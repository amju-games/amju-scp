/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: AttribDecorator.cpp,v $
Revision 1.1.10.1  2005/05/24 20:34:50  jay
Draw orientation - should fix bugs with drawing decorators.

Revision 1.1  2004/09/08 15:42:44  jay
Added to repository
  
*/

// This source code originally written by JASON COLMAN 2000-2003. 
// You may use it as you wish, at your own risk!  jason@amju.com.

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include <iostream>
#include "AttribDecorator.h"
#include "File.h"
#include "CompositeFactory.h"
#include "Attrib.h"
#include "StringUtils.h"
#include "BlinkAttrib.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
AttribDecorator::AttribDecorator() 
{
}

AttribDecorator::AttribDecorator(const AttribDecorator& rhs) :
  SolidComposite(rhs)
{
  m_pAttrib = rhs.m_pAttrib->Clone();
}

AttribDecorator::~AttribDecorator()
{
}

PSolidComponent AttribDecorator::Clone()
{
  return new AttribDecorator(*this);
}

void AttribDecorator::Draw()
{
  Assert(m_pAttrib.GetPtr());

#ifdef ATTRIB_DEC_DEBUG
std::cout << "ATTRIB DECORATOR: Or: " << ToString(m_or).c_str() << "\n";
#endif

  Orientate();

  m_pAttrib->Draw();
  SolidComposite::Draw();
  m_pAttrib->Restore();
}

#if defined(SCENE_EDITOR)
bool AttribDecorator::Save(bool recursive)
{
  if (IsDirty())
  {
    File jf;
    // Write name, but strip off the path if any.
    if (!jf.OpenWrite(GetName()))
    {
      return false;
    }
    // Write attrib name and params
    jf.WriteComment("// Attrib name");
    jf.Write(m_attribName);
    m_pAttrib->Save(&jf);
    // Write decorated composite name
    jf.WriteComment("// Decorated composite");
    // TODO currently we only save the 0th element
    jf.Write(StripPath(m_children[0]->GetName()));
  }

  // If recursive, save child(ren) -- there should only be one
  if (recursive)
  {
    int numComponents = m_children.size();
    for (int i = 0; i < numComponents; i++)
    {
      SolidComponent* pComp = m_children[i].GetPtr();
      pComp->Save(true);
    }
  }

  return true;
}
#endif

bool AttribDecorator::Load(const string& filename)
{
  File f;
  if (!f.OpenRead(filename))
  {
    f.ReportError("Couldn't open file.");
    return false;
  }

#if defined(SCENE_EDITOR)
  m_name = filename; // remember the filename for saving
#endif

  // Get attribute name which we modify.
  if (!f.GetDataLine(&m_attribName))
  {
    f.ReportError("Expected attribute name.");
  }
  // Look up name to make sure it's valid.
  // Create attribute object from name.
  m_pAttrib = CreateAttrib(m_attribName);

  if (!m_pAttrib.GetPtr())
  {
    f.ReportError("Bad attribute name.");
    return false;
  }

  // Get modified value for the attribute.
  if (!m_pAttrib->Load(&f))
  {
    f.ReportError("Failed to load attribute.");
    return false;
  }

  // Now load the decorated composite.
  string compName;
  if (!f.GetDataLine(&compName))
  {
    f.ReportError("Expected composite name.");
    return false;
  }
  // Create the composite and load it.
  PSolidComponent pComp = SolidComponent::LoadSolid(compName);
  if (!pComp.GetPtr())
  {
    f.ReportError("Failed to load composite.");
    return false;
  }
  m_children.push_back(pComp);

  return true;
} 

PAttrib AttribDecorator::CreateAttrib(const string& name)
{
  // TODO Bad factory code here
  if (name == DepthAttrib::TypeName)
  {
    return new DepthAttrib;
  }
  if (name == LightAttrib::TypeName)
  {
    return new LightAttrib;
  }
  if (name == ColourAttrib::TypeName)
  {
    return new ColourAttrib;
  }
  if (name == BlendAttrib::TypeName)
  {
    return new BlendAttrib;
  }
  if (name == TextureRotateAttrib::TypeName)
  {
    return new TextureRotateAttrib;
  }
  if (name == TextureScrollAttrib::TypeName)
  {
    return new TextureScrollAttrib;
  }
  if (name == TextureMethodAttrib::TypeName)
  {
    return new TextureMethodAttrib;
  }
  if (name == TextureScaleAttrib::TypeName)
  {
    return new TextureScaleAttrib;
  }

  return 0;
}

int AttribDecorator::CullDraw(const Frustum& f)
{
  Assert(m_pAttrib.GetPtr());

  m_pAttrib->Draw();
  int retval = SolidComposite::CullDraw(f);
  m_pAttrib->Restore();

  return retval;
}

void AttribDecorator::SetAttrib(PAttrib pAttrib)
{
  m_pAttrib = pAttrib;
  Assert(m_pAttrib.GetPtr());
  m_attribName = m_pAttrib->GetTypeName();
}

}
