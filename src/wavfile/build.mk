ROOT := $(strip ${ROOT})/..
DIRNAME := wavfile

STATIC_MODULES += wavfile

DIR_MODULE := wavfile
include $(strip ${BLDFILES})/header.mk

DIR_OBJS += wavfile
$(call ADD_EXE, wav_file_printer)

include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

