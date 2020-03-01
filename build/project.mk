include $(strip ${BLDFILES})/functions.mk

ifndef DEBUG_MAKE
QQ := @
endif

ifdef DEBUG
CXXFLAGS += -g
endif

SUBDIRS :=
STATIC_MODULES :=
SHARED_MODULES :=
SHARED_LIBS :=
EXES :=
OBJS :=
LIBDIR := $(strip ${OUTDIR})/$(strip ${LIBDIR})
EXEDIR := $(strip ${OUTDIR})/$(strip ${EXEDIR})
INCLUDIR := $(strip ${OUTDIR})/$(strip ${INCLUDIR})
CREATE_DIRS := ${OUTDIR} ${LIBDIR} ${EXEDIR} ${INCLUDIR}
HEADERS :=

CXXFLAGS += -I${SRCDIR}
LDFLAGS += -L${LIBDIR}

.PHONY: all
all: buil_all

# Start from the source directory and traverse down
include $(strip ${SRCDIR})/build.mk

# Add all the subdirectories
$(foreach sd, ${SUBDIRS}, $(eval $(call ADD_SUBDIR, ${sd})))

# Collect all objects into dependencies
$(foreach h, ${HEADERS}, $(eval $(call HEADER_DEP, ${h})))

$(foreach m, ${STATIC_MODULES}, $(eval $(call STATIC_LIB_DEP, ${m})))

$(foreach m, ${SHARED_MODULES}, $(eval $(call SHARED_LIB_DEP, ${m})))

$(foreach e, ${EXECUTABLES}, $(eval $(call EXE_DEP, ${e})))

.PHONY: buil_all
buil_all: ${CREATE_DIRS} ${HEADER_DEPS} ${STATIC_LIBS} ${SHARED_LIBS} ${EXES} ${CREATE_DIRS}

clean:
	${QQ}${RMDIR} ${OUTDIR}
	${QQ}if [ -n "${UNITTESTS_MAIN}" ] ; then ${RM_F} ${UNITTESTS_MAIN}; fi

$(foreach d, $(sort ${CREATE_DIRS}), $(eval $(call DIRDEP, ${d})))

$(foreach m, ${STATIC_MODULES}, $(eval $(call STATIC_LIB_TARGET, ${m})))

$(foreach m, ${SHARED_MODULES}, $(eval $(call SHARED_LIB_TARGET, ${m})))
$(foreach m, ${EXECUTABLES}, $(eval $(call EXE_TARGET, ${m})))

# Handle dependencies
-include $(OBJS:.o=.d)

include $(strip ${BLDFILES})/targets.mk

${UNITTESTS_MAIN}: ${UNITTESTS_SUITES}
	${QQ}$(strip ${BLDFILES})/utils/unittests_main.awk $^ > $@

# vim: set noexpandtab :

