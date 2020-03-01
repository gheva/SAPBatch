# Returns the path to $2 insinde $1 under src
define IN_SRC
$(strip ${SRCDIR})/$(strip ${1})/$(strip ${2})
endef

# Returns the path to $2 insinde $1 under out
define IN_OUT
$(strip ${OUTDIR})/$(strip ${1})/$(strip ${2})
endef

# Includes the build.mk from a subdir
define ADD_SUBDIR
include $(call IN_SRC, ${1}, build.mk)
DIR :=
endef

define ADD_DIR_SUBDIR
include $(strip ${SRCDIR})/$(strip ${1})/$(strip ${2})/build.mk
endef


# Adds the header to the list of exported headers
define ADD_HEADER
HEADERS += $(strip ${1})/$(strip ${2}).h
CREATE_DIRS += $(dir $(strip ${INCLUDIR})/$(strip ${1})/$(strip ${2}).h)
endef

# Adds the name of the header destination to the list of depndent files
define HEADER_DEP
HEADER_DEPS += $(strip ${INCLUDIR})/$(strip ${1})
endef

# Target to create a directory
define DIRDEP
${1}:
	${QQ}${MKDIR} ${1}
endef

# Return the library name insinde the build directory
define LIBRARY_NAME
$(strip ${OUTDIR})/lib/lib$(strip ${1}).$(strip ${2})
endef

# Static lib handling

# The name of the static lib
define STATIC_LIB_NAME
$(call LIBRARY_NAME, ${1}, ${STATIC_SFX})
endef
#$(strip ${OUTDIR})/lib$(strip ${1}).a

# Target to build a static lib
define STATIC_LIB_TARGET
$(call STATIC_LIB_NAME, ${1}): ${$(strip ${1})_OBJS}
endef

# Adding the static library name to the list of dependencies
define STATIC_LIB_DEP
STATIC_LIBS += $(call STATIC_LIB_NAME, ${1})
endef

# Shared library handling

# The name of the shared lib
define SHARED_LIB_NAME
$(call LIBRARY_NAME, ${1}, ${SHARED_SFX})
endef

# Target to build a shared lib
define SHARED_LIB_TARGET
$(call SHARED_LIB_NAME, ${1}): ${$(strip ${1})_DEPS}
endef

# Adding the shared library name to the list of dependencies
define SHARED_LIB_DEP
SHARED_LIBS += $(call SHARED_LIB_NAME, ${1})
$(strip ${1})_DEPS := $(foreach m, ${$(strip ${1})_STATIC_MODULES}, $(call STATIC_LIB_NAME, ${m}))
endef

# Returns the name of the object under the proper parallel build tree
define OBJ_NAME
$(strip ${OUTDIR})/$(strip ${1})/$(strip ${2}).$(strip ${OBJ_SFX})
endef

define OBJ_COMPILE_FLAGS
$(strip ${1})_FLAGS := $2
endef

define EXE_COMPILE_FLAGS
$(foreach o, $(call EXE_OBJ_LIST, ${1}, ${2}), $(eval $(call OBJ_COMPILE_FLAGS, ${o}, ${3})))
endef

# Name of objects belonging to a module
define MODULE_OBJ
OBJS += $(call OBJ_NAME, ${1}, ${3})
$(strip ${2})_OBJS += $(call OBJ_NAME, ${1}, ${3})
endef

define EXE_NAME
$(strip ${OUTDIR})/exe/$(strip ${1}).$(strip ${EXE_SFX})
endef

define EXE_DEP
EXES += $(call EXE_NAME, ${1})
endef

define EXE_OBJ_LIST
$(foreach o, ${$(strip ${1})_OBJS}, $(call OBJ_NAME, ${2}, ${o}))
endef

define DIR_EXE_DEP
$(strip ${1})_DEPS += $(call EXE_OBJ_LIST, ${1}, ${2})
$(strip ${1})_LINK_OBJS += $(call EXE_OBJ_LIST, ${1}, ${2})
OBJS += $(call EXE_OBJ_LIST, ${1}, ${2})
endef

define STATC_LIB_EXE_DEPS
$(foreach m, ${$(strip ${1})_MODULES}, $(call STATIC_LIB_NAME, ${m}))
endef

define LINK_LIBS
$(foreach m, ${1}, $(addprefix -l,${m}))
endef

define MESSAGE
@echo ${1}
endef

define EXE_TARGET
$(call EXE_NAME, ${1}): ${$(strip ${1})_DEPS} $(call STATC_LIB_EXE_DEPS, ${1}) ${$(strip ${1})_EXTRAS}
	$(call MESSAGE, Linking executable $$@)
	${QQ}${CXX} ${CXXFLAGS} ${CFLAGS} ${LDFLAGS} -o $$@ ${$(strip ${1})_LINK_OBJS} ${$(strip ${1})_EXTRAS} $(call LINK_LIBS, ${$(strip ${1})_MODULES}) $(call LINK_LIBS, ${$(strip ${1})_SHARED_MODULES}) ${$(strip ${1})_FLAGS} ${LDFLAGS}
endef

define ADD_EXE
$(eval include $(call IN_SRC, ${DIR}, $(strip ${1}).mk))
endef

define RESET_ALL
DIR_SUBDIRS :=
DIR_EXPORT_HEADERS :=
DIR_OBJS :=
DIR_EXES :=
endef

define ADD_UNITESTS
$(strip ${1})_EXTRAS += $(call OBJ_NAME, $(strip ${2})/unittests, ${3})
endef

# vim: set noexpandtab :

