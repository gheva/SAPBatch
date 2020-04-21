ROOT := $(strip ${ROOT})/..
DIRNAME := apps

include $(strip ${BLDFILES})/header.mk

$(call ADD_EXE, wip)
$(call ADD_EXE, single_thread_no_sql)
$(call ADD_EXE, multi_thread_no_sql)
$(call ADD_EXE, timing_no_sql)
$(call ADD_EXE, simple_sql)
$(call ADD_EXE, second_sql)
$(call ADD_EXE, sap_batch)
DIR_COMPILATION_FLAGS += -I$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/include)

include $(strip ${BLDFILES})/footer.mk

# vim: set noexpandtab :

