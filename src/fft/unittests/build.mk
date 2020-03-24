ROOT := $(strip ${ROOT})/..
DIRNAME := unittests

STATIC_MODULES += fftut

DIR_MODULE := fftut
include $(strip ${BLDFILES})/header.mk

unittests_MODULES += fftut


include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

