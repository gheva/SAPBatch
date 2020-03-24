# Configure the build system

# Configure directories
BLDFILES := build
SRCDIR := src
LIBDIR := lib
EXEDIR := exe
THIRD_PARTY_SOURCE := src/third_party
CMAKE := cmake

sysname=$(shell "uname")
ifeq "${sysname}" "Darwin"
include make/darwin.config
else
include make/linux.config
endif

# Configure file suffixes
OBJ_SFX := o
SHARED_SFX := so
STATIC_SFX := a
EXE_SFX := exe

# Configure commands
MKDIR := mkdir -p
RMDIR := rm -rf
RM_F := rm -f

# To create a release version, comment out the next line
DEBUG := 1

CXXFLAGS += -std=c++14

# If you want to debug make, insert back the next line
#DEBUG_MAKE := 1

# Use the build system
include $(strip ${BLDFILES})/project.mk

# additional targets come here, hint, don't forget to document stuff
docs: ${OUTDIR}
	doxygen Doxyfile

include make/fftw.mk
include make/soundfile.mk

.PHONY : prereq
prereq: ${PREREQUISITES}

# vim: set noexpandtab :

