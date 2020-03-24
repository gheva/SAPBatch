ROOT := $(strip ${ROOT})/..
DIRNAME := fft

STATIC_MODULES += fft

DIR_MODULE := fft
include $(strip ${BLDFILES})/header.mk

DIR_COMPILATION_FLAGS += -I$(strip ${THIRD_PARTY_BUILD})/fftw/include 
DIR_OBJS += fft

include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

