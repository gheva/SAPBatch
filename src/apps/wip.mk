EXECUTABLES += wip

DIR_EXES += wip
DIR_COMPILATION_FLAGS += -I$(strip ${THIRD_PARTY_SOURCE})/AudioFile 
DIR_COMPILATION_FLAGS += -I$(strip ${THIRD_PARTY_BUILD})/fftw/include 
wip_OBJS += wip
wip_MODULES += wavfile
wip_MODULES += yin
wip_MODULES += tapers

# vim: set noexpandtab :

