/*
Amju Games source code (c) Copyright Jason Colman 2004
$Log: GameEngine.h,v $
Revision 1.1  2004/09/08 15:42:38  jay
Added to repository
  
*/

// A convenience: client (platform-specific) code only needs
// to #include this file.

#if !defined(GAME_ENGINE_H_INCLUDED)
#define GAME_ENGINE_H_INCLUDED

#if defined(WIN32)
#pragma warning(disable: 4786)
#endif

#include "Engine.h"

#include "HeightServer.h"
#include "TextWriter.h"
#include "Engine.h"
#include "PolyLoader.h"
#include "PolyDrawOpenGL.h"
#include "Polygon.h"
#include "LeafRenderOpenGL.h"
#include "LeafRenderNoList.h"
#include "LeafRenderOglSorted.h"
#include "LeafData.h" 
#include "BoundingSphere.h"
#include "TextureServer.h"
#include "FileCheckState.h"

#endif

