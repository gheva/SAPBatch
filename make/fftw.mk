FFTW_SOURCE_DIR := $(realpath  $(strip ${THIRD_PARTY_SOURCE})/fftw-3.3.8)
FFTW_INSTALL_DIR := $(abspath $(strip ${THIRD_PARTY_BUILD})/fftw/)
FFTW_BUILD_DIR := $(strip ${FFTW_SOURCE_DIR})/.build

fftw-build:
	${MKDIR} ${FFTW_BUILD_DIR}
	cd ${FFTW_BUILD_DIR} && ../configure --enable-float --enable-threads prefix=${FFTW_INSTALL_DIR}
	cd ${FFTW_BUILD_DIR} && make && make install
	rm -rf ${FFTW_BUILD_DIR}


PREREQUISITES += fftw-build


# vim: set noexpandtab :

