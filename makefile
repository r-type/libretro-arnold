DEBUG=0
GIT_VERSION := $(shell git describe --abbrev=4 --dirty --always --tags)

UNAME=$(shell uname -a)

ifeq ($(platform),)
platform = unix
ifeq ($(shell uname -a),)
   platform = win
else ifneq ($(findstring MINGW,$(UNAME)),)
   platform = win
else ifneq ($(findstring Darwin,$(UNAME)),)
   platform = osx
else ifneq ($(findstring win,$(UNAME)),)
   platform = win
endif
endif

ifeq ($(EMUTYPE),)
	EMUTYPE = arnold
endif

TARGET_NAME := $(EMUTYPE)

# system platform
system_platform = unix
ifeq ($(shell uname -a),)
EXE_EXT = .exe
   system_platform = win
else ifneq ($(findstring Darwin,$(shell uname -a)),)
   system_platform = osx
else ifneq ($(findstring MINGW,$(shell uname -a)),)
   system_platform = win
endif


ifeq ($(platform), unix)
   TARGET := $(TARGET_NAME)_libretro.so
   LDFLAGS += -shared -Wl,--version-script=libretro/link.T -lz
   CFLAGS += -DUNIX -DCPC_LSB_FIRST -DCPC_NODEBUGGER
   fpic = -fPIC
else ifeq ($(platform), crosspi)
   CC = ~/RPI/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-gcc
	CXX =~/RPI/tools-master/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin/arm-linux-gnueabihf-g++
   TARGET := $(TARGET_NAME)_libretro.so
   fpic := -fPIC
   SHARED :=   -L~/RPI/usr/lib/arm-linux-gnueabihf -L~/RPI/lib -L~/RPI/lib/arm-linux-gnueabihf -shared -Wl,--no-undefined 
#-Wl,--version-script=$(LIBRETRO_DIR)/link.T 
   CFLAGS +=  -DARM -DRPIPORT -DLSB_FIRST -DCPC_NODEBUGGER -DALIGN_DWORD -mstructure-size-boundary=32 -mthumb-interwork -falign-functions=16 -marm
   CFLAGS += -march=armv7-a -mfloat-abi=hard -mfpu=vfpv3 -O2 -pipe -fstack-protector 

   CFLAGS +=-I~/RPI/usr/include/arm-linux-gnueabihf -I/home/tech/RPI/usr/include
 LDFLAGS += -shared -Wl,--version-script=libretro/link.T -L/home/tech/RPI/usr/lib
#-I./../sources/src/include
else ifeq ($(platform), osx)
   TARGET := $(TARGET_NAME)_libretro.dylib
   LDFLAGS += -dynamiclib
   
   fpic = -fPIC
	ifeq ($(arch),ppc)
		COMMONFLAGS += -DBLARGG_BIG_ENDIAN=1 -D__ppc__
	endif
	OSXVER = `sw_vers -productVersion | cut -d. -f 2`
	OSX_LT_MAVERICKS = `(( $(OSXVER) <= 9)) && echo "YES"`
	fpic += -mmacosx-version-min=10.1
ifndef ($(UNIVERSAL))
	CFLAGS += $(ARCHFLAGS)
	CXXFLAGS += $(ARCHFLAGS)
	LDFLAGS += $(ARCHFLAGS)
endif
else ifeq ($(platform), ios)
   TARGET := $(TARGET_NAME)_libretro_ios.dylib
   COMMONFLAGS += -DHAVE_POSIX_MEMALIGN=1 -marm
   fpic = -fPIC
   LDFLAGS += -dynamiclib
 
	ifeq ($(IOSSDK),)
		IOSSDK := $(shell xcodebuild -version -sdk iphoneos Path)
	endif

   CC = clang -arch armv7 -isysroot $(IOSSDK)
   CXX = clang++ -arch armv7 -isysroot $(IOSSDK)
	COMMONFLAGS += -DIOS 
	OSXVER = `sw_vers -productVersion | cut -d. -f 2`
	OSX_LT_MAVERICKS = `(( $(OSXVER) <= 9)) && echo "YES"`
	ifeq ($(OSX_LT_MAVERICKS),"YES")
		CC += -miphoneos-version-min=5.0
		COMMONFLAGS += -miphoneos-version-min=5.0
	endif
else ifeq ($(platform), theos_ios)
DEPLOYMENT_IOSVERSION = 5.0
TARGET = iphone:latest:$(DEPLOYMENT_IOSVERSION)
ARCHS = armv7 armv7s
TARGET_IPHONEOS_DEPLOYMENT_VERSION=$(DEPLOYMENT_IOSVERSION)
THEOS_BUILD_DIR := objs
COMMONFLAGS += -DIOS
COMMONFLAGS += -DHAVE_POSIX_MEMALIGN=1 -marm
include $(THEOS)/makefiles/common.mk

LIBRARY_NAME = $(TARGET_NAME)_libretro_ios
else ifeq ($(platform), qnx)
   TARGET := $(TARGET_NAME)_libretro_qnx.so
   fpic := -fPIC
   SHARED := -lcpp -lm -shared -Wl,-version-script=link.T
	CC = qcc -Vgcc_ntoarmv7le
   CC_AS = qcc -Vgcc_ntoarmv7le
	CXX = QCC -Vgcc_ntoarmv7le_cpp
	AR = QCC -Vgcc_ntoarmv7le
   PLATFORM_DEFINES := -D__BLACKBERRY_QNX__ -fexceptions -marm -mcpu=cortex-a9 -mfpu=neon -mfloat-abi=softfp
else ifeq ($(platform), android)
   TARGET := $(TARGET_NAME)_libretro.so
   COMMONFLAGS += -fpermissive 
   LDFLAGS += -lstdc++ -llog -lz -shared -Wl,--version-script=libretro/link.T -Wl,--no-undefined
  CFLAGS += -DHAVE_GETCWD=1 -DHAVE_MEMMOVE=1 -DHAVE_ATEXIT=1 -DARM -DLSB_FIRST -DALIGN_DWORD -mstructure-size-boundary=32 -mthumb-interwork -falign-functions=16 -marm
   CFLAGS += -O2 -pipe -fstack-protector -DUNIX -DCPC_LSB_FIRST -DCPC_NODEBUGGER

   CC = arm-linux-androideabi-gcc
   CXX = arm-linux-androideabi-g++
   LD = arm-linux-androideabi-ld
   fpic = -fPIC

# PSP
else ifeq ($(platform), psp1)
   TARGET := $(TARGET_NAME)_libretro_psp1.a
   CC = psp-gcc$(EXE_EXT)
   CXX = psp-g++$(EXE_EXT)
   AR = psp-ar$(EXE_EXT)
   COMMONFLAGS += -DPSP -G0
	STATIC_LINKING = 1

# Vita
else ifeq ($(platform), vita)
   TARGET := $(TARGET_NAME)_libretro_vita.a
	CC = arm-vita-eabi-gcc$(EXE_EXT)
	CXX = arm-vita-eabi-g++$(EXE_EXT)
	AR = arm-vita-eabi-ar$(EXE_EXT)
   COMMONFLAGS += -DVITA
	STATIC_LINKING = 1

else ifeq ($(platform), wii)
   TARGET := $(TARGET_NAME)_libretro_wii.a
   CC = $(DEVKITPPC)/bin/powerpc-eabi-gcc$(EXE_EXT)
   CXX = $(DEVKITPPC)/bin/powerpc-eabi-g++$(EXE_EXT)
   AR = $(DEVKITPPC)/bin/powerpc-eabi-ar$(EXE_EXT)
   COMMONFLAGS += -DGEKKO -mrvl -mcpu=750 -meabi -mhard-float -D__POWERPC__ -D__ppc__ -DWORDS_BIGENDIAN=1
	STATIC_LINKING = 1
else
   CXXFLAGS += -mno-ms-bitfields
   TARGET := $(TARGET_NAME)_libretro.dll
   LDFLAGS += -shared -static-libgcc -static-libstdc++ -Wl,--version-script=libretro/link.T -lwinmm
endif

ifeq ($(DEBUG), 1)
COMMONFLAGS += -O0 -g
else
COMMONFLAGS += -O3 -DNDEBUG -DGIT_VERSION=\"$(GIT_VERSION)\" 
endif

CORE_DIR    := .
INCFLAGS    :=
SOURCES_C   :=
SOURCES_CC  :=
SOURCES_CXX :=
OBJECTS  	:=
 
include Makefile.common

OBJECTS  += $(SOURCES_CC:.cc=.o) $(SOURCES_C:.c=.o) $(ROMS_BIN:.rom=.o) $(CPR_BIN:.cpr=.o)
CXXFLAGS += -D__LIBRETRO__ $(fpic) $(INCFLAGS) $(COMMONFLAGS) -fpermissive
CFLAGS   += -D__LIBRETRO__ $(fpic) $(INCFLAGS) $(COMMONFLAGS) -fpermissive
LDFLAGS  += -lm $(fpic)

ifeq ($(platform), theos_ios)
COMMON_FLAGS := -DIOS -DARM $(COMMON_DEFINES) $(INCFLAGS) -I$(THEOS_INCLUDE_PATH) -Wno-error
$(LIBRARY_NAME)_CFLAGS += $(CFLAGS) $(COMMON_FLAGS)
$(LIBRARY_NAME)_CXXFLAGS += $(CXXFLAGS) $(COMMON_FLAGS)
${LIBRARY_NAME}_FILES = $(SOURCES_CXX) $(SOURCES_C)
include $(THEOS_MAKE_PATH)/library.mk
else
	
all: $(TARGET)
$(TARGET): $(OBJECTS)
ifeq ($(STATIC_LINKING), 1)
	$(AR) rcs $@ $(OBJECTS)
else
	$(CXX)  -o $@ $(OBJECTS) $(LDFLAGS)
endif

%.o: %.c
	$(CC) $(CFLAGS) -c $^ -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^ -o $@

%.o: %.cc
	$(CC) $(CFLAGS) -c $^ -o $@

%.o: %.rom
#	objcopy -I binary -O elf64-x86-64 -B i386:x86-64  $^ $@
	$(LD) -r -b binary -o $@ $^ 
%.o: %.cpr
#	objcopy -I binary -O binary -B i386:x86-64  $^ $@
	$(LD) -r -b binary -o $@ $^ 

clean:
	rm -f $(OBJECTS) $(TARGET)

.PHONY: clean
endif
