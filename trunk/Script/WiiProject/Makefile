ifeq ($(strip $(AMJULIB_WII_SCRIPT_PATH)),)
$(error "Please set AMJULIB_WII_SCRIPT_PATH in your environment.")
endif

BUILD		:=	../../Build/WiiBuild
TARGET		:=	$(BUILD)/boot
AMJU      	:= 	../../../amjulib/Source

include $(AMJULIB_WII_SCRIPT_PATH)/AmjuLibDirs

SOURCES		:=	../../Source 

include $(AMJULIB_WII_SCRIPT_PATH)/GenMakefile


