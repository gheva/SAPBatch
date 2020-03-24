ROOT := $(strip ${ROOT})/..
DIRNAME := wavfile

STATIC_MODULES += wavfile

DIR_MODULE := wavfile
include $(strip ${BLDFILES})/header.mk

DIR_COMPILATION_FLAGS += -I$(strip ${THIRD_PARTY_SOURCE})/AudioFile

DIR_OBJS += wavfile

include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

