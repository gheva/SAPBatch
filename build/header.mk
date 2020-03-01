# Cache our parent
PARENT := ${DIR}
# We are a subdirectory of our parent
DIR := $(strip ${DIR})/$(strip ${DIRNAME})

include $(strip ${BLDFILES})/functions.mk

# add this directory to the list of directories to be created during the build
CREATE_DIRS += $(strip ${OUTDIR})$(strip ${DIR})

# reset everything befora evaluating the contents of this drectory
$(eval $(call RESET_ALL))

# vim: set noexpandtab :

