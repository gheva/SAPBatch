ROOT := $(strip ${ROOT})/..
DIRNAME := fs

STATIC_MODULES += fs

DIR_MODULE := fs
include $(strip ${BLDFILES})/header.mk

DIR_OBJS += directoryiterator

include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

