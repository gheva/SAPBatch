ROOT := $(strip ${ROOT})/..
DIRNAME := thread

STATIC_MODULES += thread

DIR_MODULE := thread
include $(strip ${BLDFILES})/header.mk

DIR_OBJS := synqueue

DIR_COMPILATION_FLAGS += -I$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/include)

include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

