EXECUTABLES += timing_no_sql

DIR_EXES += timing_no_sql

timing_no_sql_OBJS += timing_no_sql
timing_no_sql_MODULES += fs
timing_no_sql_MODULES += wavfile
timing_no_sql_MODULES += fft
timing_no_sql_MODULES += yin
timing_no_sql_MODULES += tapers
timing_no_sql_MODULES += sql
timing_no_sql_MODULES += thread

timing_no_sql_FLAGS += -L$(strip ${THIRD_PARTY_BUILD})/fftw/lib 
timing_no_sql_FLAGS += -lfftw3f
timing_no_sql_FLAGS += -L$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/lib)
ifdef WINDOWS
timing_no_sql_FLAGS += -lmysql
else
timing_no_sql_FLAGS += -lmysqlclient
endif

# vim: set noexpandtab :

