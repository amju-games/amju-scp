/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: Md2Model.h,v $
Revision 1.2  2004/09/28 21:38:24  jay
Adding Save() - not finished!

Revision 1.1  2004/09/08 15:43:09  jay
Added to repository
  
*/

// The MD2 loading and drawing code is a rewrite of a demo 
// found on www.flipcode.com.

#if !defined(MD2_MODEL_H_INCLUDED)
#define MD2_MODEL_H_INCLUDED

#include "SharedPtr.h"
#include "Endian.h"
#include <map>
#include <utility>
#include <string>
#include <vector>

namespace Amju
{
// Loads and draws a Quake2 (.MD2) model.
class Md2Model : public Shareable
{
public:
  Md2Model();
  ~Md2Model();

  bool Load(const std::string& meshFilename);
  bool Save(const std::string& meshFilename); // in progress

  // The animations/actions available for a standard Q2 model. 
  typedef int Animation;

  // Get the number of animations available for this mesh.
  int GetNumAnimations() const;

  // Draw the given frame and animation. 'between'  is between 0 and 1, and 
  // determines the point of interpolation between current and next frame.
  // This may be used to interpolate two frames.
  void Draw(Animation animation, int frame, float between);
  
  // This version draws the mesh with no interpolation.
  void DrawNoInterpolation(Animation animation, int frameIndex, float between);

  int GetAnimationSize(Animation animation) const;
  
  // For this mesh, get the name for the given animation.
  const char* GetAnimationName(Animation animation);

  // Get Animation enum from name for this mesh.
  Animation GetAnimationFromName(const std::string&);

  // For this mesh, does the given animation repeat ?
  bool GetDoesActionRepeat(Animation animation) const;
  bool GetDoesActionFreeze(Animation animation) const;

  void SetDoesRepeat(Animation a, bool b);
  void SetDoesFreeze(Animation a, bool b);

protected:
  // Add an animation name for this mesh
  void AddAnimationName(const std::string&);

  int GetStartFrame(Animation animation) const;
  // Check that a string is a valid Animation name 
  bool IsValidName(const std::string& s) const;

  void StoreNewFrame(const std::string& name, int start, int end);

  // Call if a frame name is not recognised.
  void IgnoreFrameNames(const std::string& frameName);

  // Map written to by StoreNewFrame.
  // Key is animation code. Value is a pair of ints. The first int is the
  // start frame for the animation; the second int is the final frame of 
  // the animation.
  typedef std::map<Animation, std::pair<int, int> > AnimationMap;
  AnimationMap m_animationMap;
  bool m_useAnimationMap;

protected:

  struct dstvert_t 
  {
    short   s;
    short   t;
    
    void Endianize()
    {
      s = Endian(uint16(s));
      t = Endian(uint16(t));
    }
  };

  struct dtriangle_t
  {
    short   index_xyz[3];
    short   index_st[3];
    
    void Endianize()
    {
      for (int i = 0; i < 3; i++)
      {
        index_xyz[i] = Endian(uint16(index_xyz[i]));
        index_st[i] = Endian(uint16(index_st[i]));
      }
    }
  };

  struct dtrivertx_t
  {
    unsigned char v[3];                  // scaled byte to fit in frame mins/maxs
    unsigned char lightnormalindex;
  };

  struct frame2_t
  {
    float scale[3];       
    float translate[3];
    char  name[16];   
    
    void Endianize()
    {
      for (int i = 0; i < 3; i++)
      {
        scale[i] = Endian(scale[i]);
        translate[i] = Endian(translate[i]);
      }
    }
  };

  struct dmdl_t
  {
    int ident;
    int version;
    int skinwidth;
    int skinheight;
    int framesize;   // byte size of each frame
    int num_skins;
    int num_xyz;
    int num_st;      // greater than num_xyz for seams
    int num_tris;
    int num_glcmds;  // dwords in strip/fan command list
    int num_frames;
    int ofs_skins;   // each skin is a MAX_SKINNAME string
    int ofs_st;      // byte offset from start for stverts
    int ofs_tris;    // offset for dtriangles
    int ofs_frames;  // offset for first frame
    int ofs_glcmds;
    int ofs_end;     // end of file
  };

  struct glCommandVertex_t
  {
    float s,t;
    int vertexIndex;
  };

  struct Md2Vertex
  {
    Md2Vertex() {}

    Md2Vertex(float x, float y, float z, float nx, float ny, float nz) 
    {
      m_pos[0] = x;
      m_pos[1] = y;
      m_pos[2] = z; 
      m_normal[0] = nx;
      m_normal[1] = ny;
      m_normal[2] = nz;
    }

    float m_pos[3];
    float m_normal[3];
  };

  typedef std::vector<Md2Vertex> Frame;
  std::vector<Frame> m_frames;
  typedef std::vector<dtriangle_t> TriangleList;
  TriangleList m_triangles;

  // OpenGL commands: this array is allocated in Load().
  // The 'commands' are sequences of 1 int followed by a number of 
  // glCommandVertex_t structs.
  unsigned char* m_glCommands;
  // Number of commands, makes saving simpler
  uint32 m_numGlCmds;

  // Filename for error reporting
  std::string m_name; 

protected:
  // Get Interpolated vertex, between v1 and v2. d is the interpolation point,
  // 0 <= d <= 1
  void Interpolate(const Md2Vertex& v1, const Md2Vertex& v2, float d, Md2Vertex* pResult);

  // Not lighting characters, so normals are unnecessary.
  void CalcNormals();

  // Change the endianness of GL commands up front (if necessary), 
  // so we don't call Endian() functions every frame.
  void EndianizeGlCommands();

  // Drawing implementation. Used by Draw() and DrawOutline().
  void DrawImpl(Animation animation, int frameIndex, float between, bool allowInterpolation);

protected:
  // animation names for this mesh. The index of each name gives the corresponding
  // Animation code.
  std::vector<std::string> m_animationNames;

  // Store repeat flags
  std::vector<bool> m_repeatFlags;
  // Store freeze flags
  std::vector<bool> m_freezeFlags;

};
}
#endif
