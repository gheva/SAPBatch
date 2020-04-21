EXECUTABLES += wip

DIR_EXES += wip
DIR_COMPILATION_FLAGS += -I$(strip ${THIRD_PARTY_SOURCE})/AudioFile 
DIR_COMPILATION_FLAGS += -I$(strip ${THIRD_PARTY_BUILD})/fftw/include 
wip_OBJS += wip
wip_MODULES += wavfile
wip_MODULES += yin
wip_MODULES += tapers
wip_MODULES += fft
wip_MODULES += sql
wip_MODULES += thread
wip_FLAGS += -L$(strip ${THIRD_PARTY_BUILD})/fftw/lib 
wip_FLAGS += -lfftw3f
wip_FLAGS += -L$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/lib)
ifdef WINDOWS
wip_FLAGS += -lmysql
else
wip_FLAGS += -lmysqlclient
endif

# vim: set noexpandtab :

