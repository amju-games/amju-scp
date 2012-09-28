# MAKE RELEASE SCRIPT FOR AMJU SUPER COOL POOL FOR MAC

export VERSION=0.1

# Make amjulib
export AMJU_BUILD_ARCH=Mac-i386
export AMJU_MACHDEP='-arch i386 -DMACOSX -DXP_MACOSX -D__i386__'
pushd ../../../amjulib/Script/MacOSX
make
popd

# Make ASCP
make

export AMJU_BUILD_ARCH=Mac-ppc
export AMJU_MACHDEP='-arch ppc -DMACOSX -DXP_MACOSX'
pushd ../../../amjulib/Script/MacOSX
make
popd
make

# Make fat binary
export BUILD=../../Build/

lipo -create $BUILD/Mac-i386/ascp $BUILD/Mac-ppc/ascp -output $BUILD/ascp.univ
strip $BUILD/ascp.univ

# Make glue files
# TODO

# Make app bundle, put universal binary and glue files in it.

export ASSETSDIR=$BUILD/CompiledAssets

export APP_NAME="ASCP.app"

export ZIP_FILE="ASCP.zip"

rm -rf $BUILD/$APP_NAME
cp -R $APP_NAME $BUILD
mkdir -p $BUILD/$APP_NAME/Contents/Resources/Data
cp $ASSETSDIR/data-Mac.glue $BUILD/$APP_NAME/Contents/Resources/Data/
cp $ASSETSDIR/music-Mac.glue $BUILD/$APP_NAME/Contents/Resources/Data/
cp $BUILD/ascp.univ $BUILD/$APP_NAME/Contents/MacOS/ascp
cd $BUILD

# TODO Update info plist etc

# Zip bundle
rm ZIP_FILE
zip -9 ZIP_FILE "My Game.app"
zip -9 ZIP_FILE "My Game.app/Contents"
zip -9 ZIP_FILE "My Game.app/Contents/Info.plist"
zip -9 ZIP_FILE "My Game.app/Contents/MacOS/ascp"
zip -9 ZIP_FILE "My Game.app/Contents/Resources"
zip -9 ZIP_FILE "My Game.app/Contents/Resources/Data"
zip -9 ZIP_FILE "My Game.app/Contents/Resources/Data/data-Mac.glue"
zip -9 ZIP_FILE "My Game.app/Contents/Resources/Data/music-Mac.glue"
zip -9 ZIP_FILE "My Game.app/Contents/Resources/MyGame.icns"
zip -9 ZIP_FILE "My Game.app/Contents/Resources/English.lproj"
zip -9 ZIP_FILE "My Game.app/Contents/Resources/English.lproj/InfoPlist.strings"
mv ZIP_FILE "ASCP-v."$VERSION".zip"


