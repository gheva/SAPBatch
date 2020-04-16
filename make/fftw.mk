FFTW_SOURCE_DIR := $(realpath  $(strip ${THIRD_PARTY_SOURCE})/fftw-3.3.8)
FFTW_ZIP := $(realpath  $(strip ${THIRD_PARTY_SOURCE})/fftw-3.3.5-dll64.zip)
FFTW_INSTALL_DIR := $(abspath $(strip ${THIRD_PARTY_BUILD})/fftw/)
FFTW_BUILD_DIR := $(strip ${FFTW_SOURCE_DIR})/.build

ifdef WINDOWS
fftw-build:
	${MKDIR} ${FFTW_INSTALL_DIR}
	${MKDIR} ${FFTW_INSTALL_DIR}/lib
	${MKDIR} ${FFTW_INSTALL_DIR}/include
	cd ${FFTW_INSTALL_DIR} && unzip ${FFTW_ZIP} libfftw3-3.dll libfftw3f-3.dll fftw3.h
	cd ${FFTW_INSTALL_DIR} && mv libfftw3-3.dll lib/libfftw3.dll
	cd ${FFTW_INSTALL_DIR} && mv libfftw3f-3.dll lib/libfftw3f.dll
	cd ${FFTW_INSTALL_DIR} && mv fftw3.h include

else
fftw-build:
	${MKDIR} ${FFTW_BUILD_DIR}
	cd ${FFTW_BUILD_DIR} && ../configure --enable-float --enable-threads prefix=${FFTW_INSTALL_DIR}
	cd ${FFTW_BUILD_DIR} && make && make install
	rm -rf ${FFTW_BUILD_DIR}
endif


PREREQUISITES += fftw-build


# vim: set noexpandtab :

