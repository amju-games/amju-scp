ifeq ($(strip $(AMJULIB_MAC_SCRIPT_PATH)),)
$(error "Please set AMJULIB_MAC_SCRIPT_PATH in your environment.")
endif

EXE_NAME	:=	ascp-mac
RUNDIR		:=	../../Build/CompiledAssets
BUILD		:=	../../Build/MacBuild
TARGET		:=	$(BUILD)/$(EXE_NAME)
AMJU      	:= 	../../../amjulib/Source
SOURCES		:=	../../Source 

LIBS	:= \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Build/Mac/amjulib.a \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Source/SoundBass/Bass2.4/Macosx/mp3-free/libbass.dylib \
	$(AMJULIB_MAC_SCRIPT_PATH)/../../Source/SoundBass/Bass2.4/Macosx/libbassmidi.dylib \
	-framework OpenGL \
	-framework GLUT \
	-framework Carbon \
	-framework IOKit \
	-lstdc++ -lm 

EXTRA_CFLAGS	:=	-g -O0 -D_DEBUG 
#-DHS_DEBUG
#-DPOOL_ONLINE

include $(AMJULIB_MAC_SCRIPT_PATH)/GenMakefile

run:
	cp $(TARGET) $(RUNDIR); \
	cp $(AMJU)/SoundBass/Bass2.4/Macosx/libbassmidi.dylib $(RUNDIR); \
	cp $(AMJU)/SoundBass/Bass2.4/Macosx/mp3-free/libbass.dylib $(RUNDIR); \
	cd $(RUNDIR); \
	./$(EXE_NAME)


runsetup:
	cp $(TARGET) $(RUNDIR); \
	cp $(AMJU)/SoundBass/Bass2.4/Macosx/libbassmidi.dylib $(RUNDIR); \
	cp $(AMJU)/SoundBass/Bass2.4/Macosx/mp3-free/libbass.dylib $(RUNDIR); 


