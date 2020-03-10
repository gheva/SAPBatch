ROOT := $(strip ${ROOT})/..
DIRNAME := unittests

STATIC_MODULES += tapersut

DIR_MODULE := tapersut
include $(strip ${BLDFILES})/header.mk

unittests_MODULES += tapersut


include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

