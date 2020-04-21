EXECUTABLES += single_thread_no_sql

DIR_EXES += single_thread_no_sql

single_thread_no_sql_OBJS += single_thread_no_sql
single_thread_no_sql_MODULES += fs
single_thread_no_sql_MODULES += wavfile
single_thread_no_sql_MODULES += fft
single_thread_no_sql_MODULES += yin
single_thread_no_sql_MODULES += tapers
single_thread_no_sql_MODULES += sql
single_thread_no_sql_MODULES += thread
single_thread_no_sql_FLAGS += -L$(strip ${THIRD_PARTY_BUILD})/fftw/lib 
single_thread_no_sql_FLAGS += -lfftw3f
single_thread_no_sql_FLAGS += -lfftw3
single_thread_no_sql_FLAGS += -L$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/lib)
ifdef WINDOWS
single_thread_no_sql_FLAGS += -lmysql
else
single_thread_no_sql_FLAGS += -lmysqlclient
endif

# vim: set noexpandtab :

