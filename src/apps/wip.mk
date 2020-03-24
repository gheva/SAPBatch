EXECUTABLES += wip

DIR_EXES += wip
DIR_COMPILATION_FLAGS += -I$(strip ${THIRD_PARTY_SOURCE})/AudioFile 
wip_OBJS += wip
wip_MODULES += wavfile
wip_MODULES += yin

# vim: set noexpandtab :

