include $(strip ${BLDFILES})/functions.mk

# Add the exported headers to the headerslist
$(foreach h, ${DIR_EXPORT_HEADERS}, $(eval $(call ADD_HEADER, ${DIR}, ${h})))

# Add each object that needs to be built for this directories module
$(foreach o, ${DIR_OBJS}, $(eval $(call MODULE_OBJ, ${DIR}, ${DIR_MODULE}, ${o})))

# Add the executables that need to be built in this directory
$(foreach e, ${DIR_EXES}, $(eval $(call DIR_EXE_DEP, ${e}, ${DIR})))

ifdef DIR_COMPILATION_FLAGS
$(foreach o, ${DIR_OBJS}, $(eval $(call OBJ_COMPILE_FLAGS, $(call OBJ_NAME, ${DIR}, ${o}), ${DIR_COMPILATION_FLAGS})))

$(foreach e, ${DIR_EXES}, $(eval $(call EXE_COMPILE_FLAGS, ${e}, ${DIR}, ${DIR_COMPILATION_FLAGS})))
DIR_COMPILATION_FLAGS :=
endif

# Go into the sibdirectories and build them
$(foreach d, ${DIR_SUBDIRS}, $(eval $(call ADD_DIR_SUBDIR, ${DIR}, ${d})))

# Reset all temporary variables so when we exit a subdirectory we start fresh
$(eval $(call RESET_ALL))

# Move to parent directory
DIR := $(abspath $(strip ${DIR})/..)

# vim: set noexpandtab :

