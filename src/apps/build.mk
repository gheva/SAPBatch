ROOT := $(strip ${ROOT})/..
DIRNAME := apps

include $(strip ${BLDFILES})/header.mk

$(call ADD_EXE, sap_batch)
DIR_COMPILATION_FLAGS += -I$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/include)
DIR_COMPILATION_FLAGS += -I$(abspath $(strip ${THIRD_PARTY_BUILD})/fftw/include)
DIR_COMPILATION_FLAGS += -I$(abspath $(strip ${THIRD_PARTY_SOURCE})/AudioFile)

include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

