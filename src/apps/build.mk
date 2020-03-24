ROOT := $(strip ${ROOT})/..
DIRNAME := apps

include $(strip ${BLDFILES})/header.mk

$(call ADD_EXE, wip)

include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

