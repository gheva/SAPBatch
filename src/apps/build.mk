ROOT := $(strip ${ROOT})/..
DIRNAME := apps

include $(strip ${BLDFILES})/header.mk

$(call ADD_EXE, wip)
$(call ADD_EXE, single_thread_no_sql)

include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

