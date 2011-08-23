# Copy files from old project

cp ../../amju-pool-old/amju-src/Code/Amju/*.{h,cpp} .
cp ../../amju-pool-old/amju-src/Code/Bass/*.{h,cpp} .
cp ../../amju-pool-old/amju-src/Code/Config/*.{h,cpp} .
cp ../../amju-pool-old/amju-src/Code/Config/MakeConfig.pl .
cp ../../amju-pool-old/amju-src/Code/Core/*.{h,cpp} .
cp ../../amju-pool-old/amju-src/Code/Finder/*.{h,cpp} .
cp ../../amju-pool-old/amju-src/Code/Gui/*.{h,cpp} .
cp ../../amju-pool-old/amju-src/Code/Md2/*.{h,cpp} .
cp ../../amju-pool-old/amju-src/Code/Pool/*.{h,cpp} .
cp ../../amju-pool-old/amju-src/Code/Reg/*.{h,cpp} .
cp ../../amju-pool-old/amju-src/Code/Utils/*.{h,cpp} .

# Remove paths from includes, as all source files are in same dir

perl -pi -e 's/..\/Pool\///g' *.h
perl -pi -e 's/..\/Pool\///g' *.cpp
perl -pi -e 's/\.\.\/Amju\///g' *.cpp
perl -pi -e 's/\.\.\/Amju\///g' *.h
perl -pi -e 's/\.\.\/Md2\///g' *.cpp
perl -pi -e 's/\.\.\/Md2\///g' *.h
perl -pi -e 's/\.\.\/Gui\///g' *.cpp
perl -pi -e 's/\.\.\/Gui\///g' *.h
perl -pi -e 's/Md2\/Md2Model/Md2Model/g' *.cpp 
perl -pi -e 's/Md2\/Md2Model/Md2Model/g' *.h

# Code fixes
 
perl -pi -e 's/KeymapPrinter:://g' KeymapPrinter.h
perl -pi -e 's/LevelWorker:://g' LevelWorker.h
perl -pi -e 's/StupidBehaviour:://g' StupidBehaviour.h

mv ControlStyleMousePickBase.cpp grave/
mv ControlStyleMovePlayerBase.cpp grave/
mv *Golf*.cpp grave/
mv EngineStateObjectMenu.cpp grave/
mv EngineStatePoolLoser.cpp grave/

# TODO Make changes to improve code

#perl -pi -e 's/SchAssert/AmjuAssert/g' *.cpp
#perl -pi -e 's/SchAssert/AmjuAssert/g' *.h
#perl -pi -e 's/SCHMICKEN/AMJU/g' *.cpp
#perl -pi -e 's/SCHMICKEN/AMJU/g' *.h
#perl -pi -e 's/VertexBase/Vec3f/g' *.cpp
#perl -pi -e 's/VertexBase/Vec3f/g' *.h 
#perl -pi -e 's/identity()/SetIdentity()/g' *.cpp
