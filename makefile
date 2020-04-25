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
ifdef WINDOWS
include make/win.config
else
include make/darwin.config
endif
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

package_windows: all
	mkdir sap_batch
	cp out.windows/exe/sap_batch.exe sap_batch/
	cp .third_party.windows/fftw/lib/libfftw3f.dll sap_batch/libfftw3f-3.dll
	cp .third_party.windows/fftw/lib/libfftw3.dll sap_batch/libfftw3-3.dll
	cp .third_party.windows/mysql/lib/libmysql.dll sap_batch/libmysql.dll
	cp /usr/local//Cellar/mingw-w64/7.0.0_2/toolchain-x86_64/x86_64-w64-mingw32/bin/libwinpthread-1.dll sap_batch/
	unzip src/third_party/openssl-1.1.1g-win64-mingw.zip openssl-1.1.1g-win64-mingw/libssl-1_1-x64.dll
	unzip src/third_party/openssl-1.1.1g-win64-mingw.zip openssl-1.1.1g-win64-mingw/libcrypto-1_1-x64.dll
	mv openssl-1.1.1g-win64-mingw/* sap_batch/
	rmdir openssl-1.1.1g-win64-mingw
	zip -9 sap_batch.zip -r sap_batch
	rm -rf sap_batch

include make/fftw.mk
include make/mysql.mk

.PHONY : prereq
prereq: ${PREREQUISITES}

# vim: set noexpandtab :

