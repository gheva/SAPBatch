ROOT := $(strip ${ROOT})/..
DIRNAME := tapers

STATIC_MODULES += tapers

DIR_MODULE := tapers
include $(strip ${BLDFILES})/header.mk

DIR_OBJS += multitaper

include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

