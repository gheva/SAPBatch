ROOT := $(strip ${ROOT})/..
DIRNAME := sql

STATIC_MODULES += sql

DIR_MODULE := sql
include $(strip ${BLDFILES})/header.mk

DIR_OBJS := mysql
DIR_OBJS += column
DIR_OBJS += table
DIR_COMPILATION_FLAGS += -I$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/include)

include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

