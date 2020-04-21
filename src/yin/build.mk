ROOT := $(strip ${ROOT})/..
DIRNAME := yin

STATIC_MODULES += yin

DIR_MODULE := yin
include $(strip ${BLDFILES})/header.mk

DIR_COMPILATION_FLAGS += -I$(strip ${THIRD_PARTY_SOURCE})/AudioFile
DIR_COMPILATION_FLAGS += -I$(strip ${THIRD_PARTY_BUILD})/fftw/include 
DIR_COMPILATION_FLAGS += -I$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/include)

DIR_OBJS += yin

include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

