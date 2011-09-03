ifeq ($(strip $(AMJULIB_IPHONE_SCRIPT_PATH)),)
$(error "Please set AMJULIB_IPHONE_SCRIPT_PATH in your environment.")
endif

APP		:=	AmjuSCP
NICE_NAME	:=	Amju_SCP

BUILD		:=	../../Build/iPhoneBuild
TARGET		:=	$(BUILD)/$(APP)
AMJU      	:= 	../../../amjulib/Source
SOURCES		:=	../../Source
RUNDIR 		:=	../../Build/CompiledAssets
APP_DIR		:= 	$(BUILD)/$(NICE_NAME).app


LIBS	:= \
	$(AMJULIB_IPHONE_SCRIPT_PATH)/../../Build/iPhoneBuild/amjulib.a \
	-framework UIKit \
	-framework Foundation \
	-framework OpenGLES \
	-framework QuartzCore \
	-lbassmidi \
	-lbass \
	-framework AudioToolbox \
	-framework CFNetwork \
	-framework SystemConfiguration \
	-lstdc++ -lm 

include $(AMJULIB_IPHONE_SCRIPT_PATH)/GenMakefile

sign:
	export EXECUTABLE_NAME=$(NICE_NAME); \
	export PLATFORM_NAME="iphoneos"; \
	export PROJECT_NAME=$(NICE_NAME); \
	export BUILT_PRODUCTS_DIR=$(BUILD); \
	export WRAPPER_NAME=$(NICE_NAME).app; \
	./codesignscript.sh

setup:
	mkdir -p "$(APP_DIR)"; \
	cp $(TARGET) "$(APP_DIR)/$(NICE_NAME)"; \
	cp ../../Assets/iphone/Info.plist "$(APP_DIR)"; \
	cp $(AMJULIB_IPHONE_SCRIPT_PATH)/../../Assets/iPhone/MainWindow.nib "$(APP_DIR)"; \
	cp $(AMJULIB_IPHONE_SCRIPT_PATH)/../../Assets/iPhone/PkgInfo "$(APP_DIR)"; \
	cp ../../Assets/iphone/icon.png "$(APP_DIR)"; \
	cp ../../Build/CompiledAssets/data-iphone.glue "$(APP_DIR)"; \
	cp ../../Build/CompiledAssets/music-iphone.glue "$(APP_DIR)"; \
	chmod +r "$(APP_DIR)/data-iphone.glue"; \
	chmod +r "$(APP_DIR)/music-iphone.glue"; \
	make sign

run:
	scp -r $(APP_DIR) root@192.168.0.3:/Applications/

