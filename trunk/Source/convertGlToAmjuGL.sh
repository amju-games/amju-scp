# Convert gl calls to AmjuGL 

perl -pi -e 's/glEnable/AmjuGL::Enable/g' *.cpp
perl -pi -e 's/glDisable/AmjuGL::Disable/g' *.cpp
perl -pi -e 's/glPushAttrib/AmjuGL::PushAttrib/g' *.cpp
perl -pi -e 's/glPopAttrib/AmjuGL::PopAttrib/g' *.cpp
perl -pi -e 's/glPushMatrix/AmjuGL::PushMatrix/g' *.cpp
perl -pi -e 's/glPopMatrix/AmjuGL::PopMatrix/g' *.cpp
perl -pi -e 's/glTranslatef/AmjuGL::Translate/g' *.cpp
perl -pi -e 's/glScalef/AmjuGL::Scale/g' *.cpp
perl -pi -e 's/glRotatef/AmjuGL::Rotate/g' *.cpp
perl -pi -e 's/glRotated/AmjuGL::Rotate/g' *.cpp

perl -pi -e 's/GL_TEXTURE_2D/AmjuGL::AMJU_TEXTURE_2D/g' *.cpp

perl -pi -e 's/glMatrixMode/AmjuGL::SetMatrixMode/g' *.cpp
perl -pi -e 's/GL_MODELVIEW/AmjuGL::AMJU_MODELVIEW_MATRIX/g' *.cpp
perl -pi -e 's/GL_PROJECTION/AmjuGL::AMJU_PROJECTION_MATRIX/g' *.cpp
perl -pi -e 's/GL_TEXTURE/AmjuGL::AMJU_TEXTURE_MATRIX/g' *.cpp
perl -pi -e 's/glLoadIdentity/AmjuGL::SetIdentity/g' *.cpp

perl -pi -e 's/GL_LIGHTING_BIT/AmjuGL::AMJU_LIGHTING/g' *.cpp
perl -pi -e 's/GL_LIGHTING/AmjuGL::AMJU_LIGHTING/g' *.cpp

perl -pi -e 's/GL_DEPTH_BUFFER_BIT/AmjuGL::AMJU_DEPTH_READ/g' *.cpp
perl -pi -e 's/GL_DEPTH_BUFFER/AmjuGL::AMJU_DEPTH_READ/g' *.cpp
perl -pi -e 's/GL_DEPTH_TEST/AmjuGL::AMJU_DEPTH_READ/g' *.cpp

perl -pi -e 's/glDepthMask\(GL_FALSE\)/AmjuGL::Disable(AmjuGL::AMJU_DEPTH_WRITE)/g' *.cpp
perl -pi -e 's/glDepthMask\(GL_TRUE\)/AmjuGL::Enable(AmjuGL::AMJU_DEPTH_WRITE)/g' *.cpp

perl -pi -e 's/GL_BLEND/AmjuGL::AMJU_BLEND/g' *.cpp

# comment out glBlendFunc calls 
perl -pi -e 's/glBlendFunc$//g' *.cpp

