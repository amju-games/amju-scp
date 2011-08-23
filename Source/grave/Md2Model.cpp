/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Md2Model.cpp,v $
Revision 1.2.6.4  2005/07/30 12:30:53  jay
Interpolation is now enabled!

Revision 1.2.6.3  2005/07/05 09:44:35  jay
Remove error messages from release build

Revision 1.2.6.2  2005/06/29 20:26:22  jay
Just uncommented an assert

Revision 1.2.6.1  2005/04/01 06:08:10  jay
Made characters scaleable. Pool needs characters much bigger than before.

Revision 1.2  2004/09/28 21:38:24  jay
Adding Save() - not finished!

Revision 1.1  2004/09/08 15:43:09  jay
Added to repository
  
*/

#if defined(WIN32)
#pragma warning(disable: 4786)
#pragma warning(disable: 4305) // double to float truncation
#endif

#include <algorithm>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Engine.h"
#include "Md2Model.h"
#include "File.h"
#include "CompositeFactory.h"
#include "SolidComponent.h"
#include "Geometry.h"
#include "Common.h"
#include "Md2Norms.h"
#include "Endian.h"
#include "StringUtils.h"
#include "SchAssert.h"

using namespace std;

namespace Amju
{
extern void ReportError(const string& msg);

static const int MD2_MAGIC_NUMBER = 844121161;


bool myisdigit(char c) { return (isdigit((int)c) != 0); }

Md2Model::Md2Model() 
{
  // Map of animation lengths: This is used if the MD2 data
  // contains valid frame names. This allows us to have non-standard animation 
  // lengths.
  m_useAnimationMap = true;

  m_repeatFlags.reserve(20);
  m_repeatFlags[0] = true;  // stand
  m_repeatFlags[1] = true;  // run
  m_repeatFlags[2] = true;  // attack
  m_repeatFlags[3] = false; // pain 1
  m_repeatFlags[4] = false; // pain 2
  m_repeatFlags[5] = false; // pain 3
  m_repeatFlags[6] = false; // jump,
  m_repeatFlags[7] = false; // flipoff,
  m_repeatFlags[8] = false; // salute,
  m_repeatFlags[9] = false; // taunt,
  m_repeatFlags[10] = false; // wave,
  m_repeatFlags[11] = false; // point,
  m_repeatFlags[12] = true; // crouch_stand,
  m_repeatFlags[13] = true; // crouch_walk,
  m_repeatFlags[14] = true; // crouch_attack,
  m_repeatFlags[15] = false; // crouch_pain,
  m_repeatFlags[16] = false; // crouch_death,
  m_repeatFlags[17] = false; // death1,
  m_repeatFlags[18] = false; // death2,
  m_repeatFlags[19] = false;  // death3

  m_freezeFlags.reserve(20);
  m_freezeFlags[0] = false; // stand
  m_freezeFlags[1] = false; // run
  m_freezeFlags[2] = false; // attack
  m_freezeFlags[3] = false; // pain 1
  m_freezeFlags[4] = false; // pain 2
  m_freezeFlags[5] = false; // pain 3
  m_freezeFlags[6] = false; // jump,
  m_freezeFlags[7] = false; // flipoff,
  m_freezeFlags[8] = false; // salute,
  m_freezeFlags[9] = false; // taunt,
  m_freezeFlags[10] = false; // wave,
  m_freezeFlags[11] = false; // point,
  m_freezeFlags[12] = false; // crouch_stand,
  m_freezeFlags[13] = false; // crouch_walk,
  m_freezeFlags[14] = false; // crouch_attack,
  m_freezeFlags[15] = false; // crouch_pain,
  m_freezeFlags[16] = false; // crouch_death,
  m_freezeFlags[17] = true;  // death1,
  m_freezeFlags[18] = true;  // death2,
  m_freezeFlags[19] = true;   // death3

}

Md2Model::~Md2Model()
{
}

void Md2Model::SetDoesRepeat(Animation a, bool b)
{
  m_repeatFlags[(int)a] = b;
}

void Md2Model::SetDoesFreeze(Animation a, bool b)
{
  m_freezeFlags[(int)a] = b;
}

void Md2Model::IgnoreFrameNames(const std::string& frameName)
{
  if (!m_useAnimationMap)
  {
    return;
  }

  m_useAnimationMap = false;
#if defined(_DEBUG)
  string err = "Unrecognised frame name: '";
  err += frameName;
  err += "'";
  err += " for MD2 model ";
  err += m_name;
  ReportError(err);
#endif
}

void Md2Model::StoreNewFrame(const std::string& name, int start, int end)
{
#if defined(_DEBUG)
  AnimationMap::const_iterator it = m_animationMap.find(GetAnimationFromName(name));
  if (it != m_animationMap.end())
  {
    // Same name used more than once in data.. boo!
    string err = m_name;
    err += string(": MD2 data has repeated frame name: ") + name;
    ReportError(err);
    return;
  }
#endif
  m_animationMap[GetAnimationFromName(name)] = std::make_pair(start, end);
}

int Md2Model::GetNumAnimations() const
{
  return m_animationMap.size();
}

int Md2Model::GetAnimationSize(Md2Model::Animation a) const
{
  if (m_useAnimationMap)
  {
    AnimationMap::const_iterator it = m_animationMap.find(a);
    if (it != m_animationMap.end())
    {
      int s = it->second.second;
      int f = it->second.first;
      int r = s - f;
      return r;
    }
#ifdef _DEBUG
    string err = "Bad animation ";
    err += ToString((int)a);
    ReportError(err);
#endif
    return 0;
  }

  //static const int animSizes[] = 
  //{
  //  // Standard lengths of MD2 animations
  //  /*0, */40, 6, 8, 4, 4, 4, 6, 12, 11, 17, 11, 12, 19, 6, 9, 4, 5, 6, 6 /*,8 ??? */
  //};
  //Assert(a >= 0 && a < 20);
  //return animSizes[(int)a];

  return -1;
}

int Md2Model::GetStartFrame(Animation a) const
{
  if (m_useAnimationMap)
  {
    AnimationMap::const_iterator it = m_animationMap.find(a);
    if (it != m_animationMap.end())
    {
      return it->second.first;
    }
  }
  return -1;

/*
  static const int cumulativeSizes[] = 
  {
    0, 40, 46, 54, 58, 62, 66, 72, 84, 95, 112, 123, 135, 154, 160, 169, 173, 178, 184, 190 
  };
  Assert(a >= 0 && a < 20);
  return cumulativeSizes[(int)a];
*/
}

Md2Model::Animation Md2Model::GetAnimationFromName(const std::string& s)
{
/*
  // Look up animation name in standard animation names.
  for (int i = (int)STAND; i <= (int)DEATH3; i++)
  {
    if (ToLower(s) == GetAnimationName((Animation)i))
    {
      return (Animation)i;
    }
  }  
*/
  int si = (int)m_animationNames.size();
  for (int j = 0; j < si; j++)
  {
    if (m_animationNames[j] == s)
    {
      return (Animation)(j);
    }
  }
#ifdef _DEBUG
  string err = "Unrecognised animation name ";
  err += s;
  ReportError(err);
  Assert(0);
#endif
  return (Animation)0;
}

bool Md2Model::IsValidName(const std::string& s) const
{
/*
  for (int i = (int)STAND; i <= (int)DEATH3; i++)
  {
    if (ToLower(s) == GetAnimationName((Animation)i))
    {
      return true;
    }
  }
*/ 

  // Check for extra game-specific animation names.
  if (find(m_animationNames.begin(), m_animationNames.end(), s) != 
        m_animationNames.end())
  {
    return true;
  }

  return false;
}

void Md2Model::AddAnimationName(const std::string& aName)
{
  m_animationNames.push_back(aName);
}

const char* Md2Model::GetAnimationName(Animation a)
{
/*
  static const char* actionNames[] = 
  {
    "stand", // 0
    "run",   // 1
    "attack", //  2
    "paina",  // 3
    "painb",  // 4
    "painc",  // 5
    "jump",   // 6
    "flipoff",  // 7
    "salute",   // 8
    "taunt",    // 9
    "wave",     // 10
    "point",     // 11
    "crouch stand", // 12
    "walk",         // 13
    "crouch attack",  // 14
    "crouch pain",   // 15
    "crouch death",  // 16
    "deatha",  // 17
    "deathb",  // 18
    "deathc"   // 19
  };

  // Allow for extra, custom animations.
  if (a < 20)
  {
    return actionNames[(int)a];
  }
*/
  return (m_animationNames[a].c_str());
}

/*
const char* Md2Model::GetSound(Animation a) const
{
  // TODO this should come from the config file.
  static const char* soundNames[] = 
  {
    "",  // stand
    "",  // run
    "",  // attack
    "pain25_1.wav", // pain 1
    "pain50_1.wav", // pain 2
    "pain75_1.wav", // pain 3
    "jump1.wav", // jump,
    "", // flipoff,
    "", // salute,
    "", // taunt,
    "", // wave,
    "", // point,
    "", // crouch_stand,
    "", // crouch_walk,
    "", // crouch_attack,
    "pain100_1.wav", // crouch_pain,
    "death1.wav", // crouch_death,
    "death2.wav", // death1,
    "death3.wav", // death2,
    "death4.wav"  // death3
  };
  Assert(a >= 0 && a < 20);
  return soundNames[(int)a];
} 
*/

bool Md2Model::GetDoesActionRepeat(Animation a) const
{
  bool b = m_repeatFlags[(int)a];
  return b;
/*
  static const bool actionRepeats[] = 
  {
      true,  // stand
      true,  // run
      true,  // attack
      false, // pain 1
      false, // pain 2
      false, // pain 3
      false, // jump,
      false, // flipoff,
      false, // salute,
      false, // taunt,
      false, // wave,
      false, // point,
      true, // crouch_stand,
      true, // crouch_walk,
      true, // crouch_attack,
      false, // crouch_pain,
      false, // crouch_death,
      false, // death1,
      false, // death2,
      false  // death3
  };
  
  if (a < 20)
  {
    return actionRepeats[(int)a];
  }
  return false; // TODO flag for custom animations ?
*/
}

bool Md2Model::GetDoesActionFreeze(Animation a) const
{
  bool b = m_freezeFlags[(int)a];
  return b;
/*
  static const bool death[] = 
  {
      false, // stand
      false, // run
      false, // attack
      false, // pain 1
      false, // pain 2
      false, // pain 3
      false, // jump,
      false, // flipoff,
      false, // salute,
      false, // taunt,
      false, // wave,
      false, // point,
      false, // crouch_stand,
      false, // crouch_walk,
      false, // crouch_attack,
      false, // crouch_pain,
      false, // crouch_death,
      true,  // death1,
      true,  // death2,
      true   // death3
  };
  if (a < 20)
  {
    return death[(int)a];
  }
  return false;
*/
}

bool Md2Model::Save(const std::string& filename)
{
  File f(false); // false => no version info
  if (!f.OpenWrite(filename, true)) // true => binary
  {
    string error = filename + ": Can't open mesh file.";
    ReportError(error.c_str());
    return false;
  }

  m_name = filename;

  uint32 numTris = m_triangles.size();
  // position of tris is after header.
  uint32 triPos = sizeof(dmdl_t);

  // Write file header 
  dmdl_t filehead;
  // populate the header structure with sizes etc.
  filehead.ident = Endian(uint32(MD2_MAGIC_NUMBER));
  filehead.version = 0;
  filehead.skinwidth = 0;
  filehead.skinheight = 0;
  filehead.framesize = 0;
  filehead.num_skins = 0;
  filehead.num_xyz = 0;
  filehead.num_st = 0;
  filehead.num_tris = Endian(numTris);
  filehead.num_glcmds = Endian(m_numGlCmds);
  filehead.num_frames = 0;
  filehead.ofs_skins = 0;
  filehead.ofs_st = 0;
  filehead.ofs_tris = Endian(triPos);
  filehead.ofs_frames = 0;
  filehead.ofs_glcmds = 0;
  filehead.ofs_end = 0;

  f.WriteBinary((const char*)&filehead, sizeof(dmdl_t));

  int i = 0;
  // Write triangles
  for (i = 0; i < numTris; i++)
  {
    dtriangle_t tri = m_triangles[i];
    tri.Endianize();
    f.WriteBinary((const char*)&tri, sizeof(dtriangle_t));
  }
  

  // Write OpenGL commands
  // This section of the file contains 4 * filehead.num_glcmds bytes.
  // Endianize the GL commands before writing...
  EndianizeGlCommands();
  f.WriteBinary((const char*)m_glCommands, 4 * m_numGlCmds);
  // ...now convert back to local format
  EndianizeGlCommands();


  return true;
}

bool Md2Model::Load(const std::string& filename)
{
  File f(false); // false => no version info
  if (!f.OpenRead(filename, true)) // true => binary
  {
    string error = filename + ": Can't open mesh file.";
    ReportError(error.c_str());
    return false;
  }

  m_name = filename;

  // Read file header 
  dmdl_t filehead;
  int size;
  size = f.GetBinary(sizeof(dmdl_t), (unsigned char*)&filehead);

  if (Endian(uint32(filehead.ident)) != MD2_MAGIC_NUMBER)
  {
    string err = m_name;
    err += string(": File is allegedly not an .md2 file.");
    ReportError(err);
    return false;
  }

  int i = 0;

  // Read triangles.
  // (We only need them if we create our own normals.)
  // We need these to save this object back to a file, too.

  // Seek to start of triangles
  uint32 trisPos = Endian(uint32(filehead.ofs_tris));
  f.BinarySeek(trisPos);

  // Allocate space for all the triangles up front.
  uint32 numTris = Endian(uint32(filehead.num_tris));
  m_triangles.reserve(numTris);
  for (i = 1; i <= numTris; i++)
  {
    dtriangle_t tri;
    size = f.GetBinary(sizeof(dtriangle_t), (unsigned char*)&tri);

    if (size != sizeof(dtriangle_t))
    {
      ReportError("Error while reading triangles in mesh file.");
      return false;
    }
    tri.Endianize();
    m_triangles.push_back(tri);
  }

  // Read OpenGL commands
  // This section of the file contains 4 * filehead.num_glcmds bytes.
  // Seek to start of openGL commands
  uint32 glCmdsPos = Endian(uint32(filehead.ofs_glcmds));
  f.BinarySeek(glCmdsPos);
  uint32 numGlCmds = Endian(uint32(filehead.num_glcmds));
  m_numGlCmds = numGlCmds;
  m_glCommands = new unsigned char[4 * numGlCmds];

  size = f.GetBinary(4 * numGlCmds, (unsigned char*)m_glCommands);

  if (size != numGlCmds * 4)
  {
    ReportError("Error while reading GL Commands.");
    return false;
  }

  // Read frames
  // Seek to start of frames
  uint32 framesPos = Endian(uint32(filehead.ofs_frames)); 
  f.BinarySeek(framesPos);

  int numframes = 198;
  uint32 realNumFrames = Endian(uint32(filehead.num_frames)); 
  if (realNumFrames < numframes)
  {
    numframes = realNumFrames;
  }
  m_frames.reserve(numframes);

  // Load each frame. Use the frame name to allow non-standard animation 
  // lengths.
  std::string previousFrameName;
  int startFrame = 0;
  std::string name; // current frame name
  for (i = 0; i < numframes; i++)
  {
    frame2_t fileframe;

    f.GetBinary(sizeof(frame2_t), (unsigned char*)&fileframe);
    name = fileframe.name;
    // Strip off any numbers. These can change with every frame, e.g. "stand01",
    // "stand02". We just want to know when the name changes (e.g. "stand").
    name.erase(std::remove_if(name.begin(), name.end(), myisdigit), name.end());
    // If a frame name is empty, don't use the names, just use the standard
    // animation lengths.
    if (name.empty())
    {
#if defined(_DEBUG)
      string err = "Empty frame name: ";
      err += ToString(i);
      err += " for MD2 model ";
      err += m_name;
      ReportError(err);
#endif
      IgnoreFrameNames(name);
    }
    if (!IsValidName(name))
    {
#ifdef _DEBUG
      // If an unknown name, add it to special names list.
      string err = "Adding animation name ";
      err += name;
      ReportError(err);
#endif
      AddAnimationName(name);
      // Or don't rely on names at all:
      //IgnoreFrameNames(name);
    }
    if (name != previousFrameName)
    {
      // New name. Store the frame name, the first and last frames.
      // If the frame name is unrecognised, just use the standard lengths.
      if (IsValidName(previousFrameName))
      {
        StoreNewFrame(previousFrameName, startFrame, i);
      }
      startFrame = i;
      previousFrameName = name;
    }

    fileframe.Endianize();
    
    uint32 numVertices = Endian(uint32(filehead.num_xyz));

    Frame frame;
    frame.reserve(numVertices);

    // For each vertex..
    for (int j=0; j < numVertices; j++)
    {
      dtrivertx_t filevert;
      f.GetBinary(sizeof(dtrivertx_t), (unsigned char*)&filevert);

      float v0 = (float)filevert.v[0];
      float v1 = (float)filevert.v[1];
      float v2 = (float)filevert.v[2];
      float x = v0 * fileframe.scale[0] + fileframe.translate[0];
      float y = v1 * fileframe.scale[1] + fileframe.translate[1];
      float z = v2 * fileframe.scale[2] + fileframe.translate[2];

      float temp = 0;
      // My world coord system is different to md2 models, so rotate.. 
      // Rotate 90 degs about x
      temp = y;
      y = z;
      z = -temp;
       // Rotate 90 degs about y. 
      temp = x;
      x = z;
      z = -temp;
     
      // Scale coordinates to fit.
      // POOL: It seems that the simplest way to fix the scale mismatch between
      // the characters and the balls/table, is to make the characters
      // A LOT BIGGER.
      static const float SCALE = Engine::Instance()->GetConfigFloat("pool_char_scale");
      static const float FLOOR = Engine::Instance()->GetConfigFloat("pool_char_floor"); 
      x = x * SCALE;
      y = y * SCALE;
      z = z * -SCALE;
      // Add offset to floor level.
      // This number isn't in config file as it won't change (honest)
      y += FLOOR; 

      // Get normals. Use normals array rather than calculating the normal.
      int n = filevert.lightnormalindex; // index into array
      if (n >= 0 && n < 162)
      {
        float nx = md2VertexNormals[n][0];
        float ny = md2VertexNormals[n][1];
        float nz = md2VertexNormals[n][2];

        // Do the same rotations as we did for the vertices. 
        float temp = 0;
        temp = ny;
        ny = nz;
        nz = -temp;
        temp = nx;
        nx = nz;
        nz = temp;

        Md2Vertex vm(x, y, z, nx, ny, nz);
        frame.push_back(vm);
      }
    }
    m_frames.push_back(frame);
  }
  if (IsValidName(previousFrameName))
  {
    StoreNewFrame(previousFrameName, startFrame, i);
  }

  EndianizeGlCommands();
  CalcNormals();
  return true;
}

void Md2Model::CalcNormals()
{
  for (int f = 0; f < m_frames.size(); f++)
  {
    Frame& frame = m_frames[f];
    // For this frame, get the perpendicular normal for each vertex.
    // Then average the normals for vertices which are the same.
    // Step 1: for each poly, get the perp. normal, assign to each vertex.
    int numTris = m_triangles.size();
    for (TriangleList::iterator it = m_triangles.begin(); it != m_triangles.end(); ++it)
    {
      // Get three vertices.
      const dtriangle_t& tri = *it;
      VertexBase a(frame[tri.index_xyz[0]].m_pos[0],
                   frame[tri.index_xyz[0]].m_pos[1],
                   frame[tri.index_xyz[0]].m_pos[2] );
      VertexBase b(frame[tri.index_xyz[1]].m_pos[0],
                   frame[tri.index_xyz[1]].m_pos[1],
                   frame[tri.index_xyz[1]].m_pos[2] );
      VertexBase c(frame[tri.index_xyz[2]].m_pos[0],
                   frame[tri.index_xyz[2]].m_pos[1],
                   frame[tri.index_xyz[2]].m_pos[2] );
      // Make normal from the 3 vertices.
      VertexBase n;
      Geometry::MakeNormal(a, b, c, &n);
      // Assign normal to each vertex.
      for (int g = 0; g < 3; g++)
      {
        frame[tri.index_xyz[g]].m_normal[0] = n.x;
        frame[tri.index_xyz[g]].m_normal[1] = n.y;
        frame[tri.index_xyz[g]].m_normal[2] = n.z;
      }
    }
    // (TEST: model should have correct normals at this point, but not smoothed yet.)
    
/*
    // Step 2: add normals for vertices which are the same, getting the average.
    // We can do this using a map.
    // This code is suspiciously similar to LeafData::CreateSmoothNormals().
    typedef map<VertexBase, VertexBase> VertexToNormal;
    VertexToNormal vmap;
    int i;
    for (i = 0; i< m_triangles.size(); i++)
    {
      const dtriangle_t& tri = m_triangles[i];
      for (int j = 0; j < 3; j++) // for each vertex in the triangle..
      {
        VertexBase v(frame[tri.index_xyz[j]].m_pos[0],
                     frame[tri.index_xyz[j]].m_pos[1],
                     frame[tri.index_xyz[j]].m_pos[2] );
 
        const VertexBase n(frame[tri.index_xyz[j]].m_normal[0],
                           frame[tri.index_xyz[j]].m_normal[1],
                           frame[tri.index_xyz[j]].m_normal[2] );
        vmap[v] += n;
      }
    }

    // Step 3: go through the map, assigning the new averaged normals to vertices.
    for (i = 0; i< m_triangles.size(); i++)
    {
      const dtriangle_t& tri = m_triangles[i];
      for (int j = 0; j < 3; j++) // for each vertex
      {
        VertexBase v(frame[tri.index_xyz[j]].m_pos[0],
                     frame[tri.index_xyz[j]].m_pos[1],
                     frame[tri.index_xyz[j]].m_pos[2] );

        const VertexBase n = vmap[v];

        frame[tri.index_xyz[j]].m_normal[0] = n.x;
        frame[tri.index_xyz[j]].m_normal[1] = n.y;
        frame[tri.index_xyz[j]].m_normal[2] = n.z;
      }
    }
*/
  }
}

void Md2Model::Draw(Animation animation, int frameIndex, float between)
{
  DrawImpl(animation, frameIndex, between, true);
}

void Md2Model::DrawNoInterpolation(Animation animation, int frameIndex, float between)
{
  DrawImpl(animation, frameIndex, between, false);
}

void Md2Model::DrawImpl(Animation animation, int frameIndex, float between, bool allowInterpolation)
{
  // 0 <= between <= 1: sets the interpolation.
  Assert(between >= 0);
  Assert(between <= 1.0f);

  // Frame index is the index into animation. We can use the two to get an 
  // absolute frame number, 0 < f  < 198
  int startframe = GetStartFrame(animation);
  int f = startframe + frameIndex - 1; // one-based frame index
  const Frame& frame = m_frames[f];

  // Get the next frame, allowing us to interpolate.
  // TODO if animation doesn't repeat, nextf = f

  int nextf = startframe;
  // If the current frame is not the last frame of the animation, get the next one.
  // Otherwise get the first one.
  const float animationSize = GetAnimationSize(animation);
  if (frameIndex < animationSize) 
  {
    nextf += frameIndex;
  }

  const Frame& nextFrame = m_frames[nextf]; 

  uint32* glcs = (uint32*)m_glCommands;
  bool doInterpolate = false;
  if (allowInterpolation)
  {
    // If this is not a repeating animation, and we are drawing the final
    // frame, then don't interpolate.
    doInterpolate = true; 
    if (!GetDoesActionRepeat(animation) &&
        frameIndex >= animationSize)
    {
      doInterpolate = false;
    }
  }

  if (doInterpolate)
  {
    while (1)
    {
      // Get the command. The sign is used to flag FAN or STRIP. The abs value
      // is the number of vertices in the fan or strip.
      int com = (int)(uint32(*glcs)); // already Endianzed up front
      if (com == 0)
      {
        return;
      }

      if (com < 0)
      {
        com = -com;
        glBegin(GL_TRIANGLE_FAN);
      }
      else
      {
        glBegin(GL_TRIANGLE_STRIP);
      }
      glcs++;
      for (int i=0; i < com; i++)
      {
        glCommandVertex_t *vert = (glCommandVertex_t*)glcs;
        glcs += 3;
        const uint32 vertexIndex = vert->vertexIndex; // Endianized up front
      
        const Md2Vertex& vm1 = frame[vertexIndex];
        const Md2Vertex& vm2 = nextFrame[vertexIndex];
 
        // Interpolate between current and next frame, using 'between'.
        Md2Vertex vm3;
        Interpolate(vm1, vm2, between, &vm3);
        glTexCoord2f(vert->s, vert->t); // Endianized up front 
        glVertex3fv((float*)vm3.m_pos);
      }
      glEnd();
    }
  }
  else
  {
    // NON-INTERPOLATED VERSION
    while (1)
    {
      // Get the command. The sign is used to flag FAN or STRIP. The abs value
      // is the number of vertices in the fan or strip.
      int com = (int)(uint32(*glcs)); // already Endianzed up front
      if (com == 0)
      {
        return;
      }

      if (com < 0)
      {
        com = -com;
        glBegin(GL_TRIANGLE_FAN);
      }
      else
      {
        glBegin(GL_TRIANGLE_STRIP);
      }
      glcs++;
      for (int i=0; i < com; i++)
      {
        glCommandVertex_t *vert = (glCommandVertex_t*)glcs;
        glcs += 3;
        const uint32 vertexIndex = vert->vertexIndex; // Endianized up front
      
        const Md2Vertex& vm1 = frame[vertexIndex];
        const Md2Vertex& vm2 = nextFrame[vertexIndex];
 
        glTexCoord2f(vert->s, vert->t); // Endianized up front 
        glVertex3fv((float*)vm1.m_pos);
      }
      glEnd();
    }
  }
}

void Md2Model::EndianizeGlCommands()
{
  // This is a stripped-down version of Draw(), but instead of drawing we
  // fix the endianness of the GL commands.
  uint32* glcs = (uint32*)m_glCommands;

  while (1)
  {
    // Get the command. The sign is used to flag FAN or STRIP. The abs value
    // is the number of vertices in the fan or strip.
    int com = (int)(Endian(uint32(*glcs)));
    *glcs = com; // ENDIAN FIX

    if (com == 0)
    {
      return;
    }

    if (com < 0)
    {
      com = -com;
    }

    glcs++;
    for (int i = 0; i < com; i++)
    {
      glCommandVertex_t *vert = (glCommandVertex_t*)glcs;
      glcs += 3;
      const uint32 vertexIndex = Endian(uint32(vert->vertexIndex));
      vert->vertexIndex = vertexIndex; // ENDIAN FIX

      // NB We must invert the t- texture coord. Are .BMP files upside down 
      // compared to .RAW files ?? Yark.
      vert->s = Endian(vert->s); // ENDIAN FIX
      vert->t = -Endian(vert->t); // ENDIAN FIX
    }
  }

}

void Md2Model::Interpolate(
  const Md2Model::Md2Vertex& v1, 
  const Md2Model::Md2Vertex& v2, 
  float d, 
  Md2Model::Md2Vertex* pResult)
{
  Assert(d >= 0);
  Assert(d <= 1.0f);

  pResult->m_pos[0] = v1.m_pos[0] + d * (v2.m_pos[0] - v1.m_pos[0]);
  pResult->m_pos[1] = v1.m_pos[1] + d * (v2.m_pos[1] - v1.m_pos[1]);
  pResult->m_pos[2] = v1.m_pos[2] + d * (v2.m_pos[2] - v1.m_pos[2]);

#if defined(ALTERNATIVE)
  // Alternative code for above
  const float d1 = 1.0f - d;
  pResult->m_pos[0] = v1.m_pos[0] * d1 + v2.m_pos[0] * d; // extra *.. no good
  pResult->m_pos[1] = v1.m_pos[1] * d1 + v2.m_pos[1] * d;
  pResult->m_pos[2] = v1.m_pos[2] * d1 + v2.m_pos[2] * d;
#endif

/*
  // Lighting is disabled for MD2 characters, because it looks terrible, at least
  // for areas of flat colour. So we don't need to bother interpolating normals.
  pResult->m_normal[0] = v1.m_normal[0]; // + d * (v2.m_normal[0] - v1.m_normal[0]);
  pResult->m_normal[1] = v1.m_normal[1]; // + d * (v2.m_normal[1] - v1.m_normal[1]);
  pResult->m_normal[2] = v1.m_normal[2]; // + d * (v2.m_normal[2] - v1.m_normal[2]);
*/
}
}
