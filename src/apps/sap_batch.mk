EXECUTABLES += sap_batch

DIR_EXES += sap_batch

sap_batch_OBJS += sap_batch
sap_batch_MODULES += fs
sap_batch_MODULES += wavfile
sap_batch_MODULES += fft
sap_batch_MODULES += yin
sap_batch_MODULES += tapers
sap_batch_MODULES += sql
sap_batch_MODULES += thread

sap_batch_FLAGS += -L$(strip ${THIRD_PARTY_BUILD})/fftw/lib 
sap_batch_FLAGS += -lfftw3f
sap_batch_FLAGS += -L$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/lib)
ifdef WINDOWS
sap_batch_FLAGS += -lmysql
else
sap_batch_FLAGS += -lmysqlclient
endif

# vim: set noexpandtab :

