EXECUTABLES += multi_thread_no_sql

DIR_EXES += multi_thread_no_sql

multi_thread_no_sql_OBJS += multi_thread_no_sql
multi_thread_no_sql_MODULES += fs
multi_thread_no_sql_MODULES += wavfile
multi_thread_no_sql_MODULES += fft
multi_thread_no_sql_MODULES += yin
multi_thread_no_sql_MODULES += tapers
multi_thread_no_sql_MODULES += sql
multi_thread_no_sql_MODULES += thread
multi_thread_no_sql_FLAGS += -L$(strip ${THIRD_PARTY_BUILD})/fftw/lib 
multi_thread_no_sql_FLAGS += -lfftw3f
ifndef WINDOWS
multi_thread_no_sql_FLAGS += -lfftw3f_threads
endif
multi_thread_no_sql_FLAGS += -L$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/lib)
ifdef WINDOWS
multi_thread_no_sql_FLAGS += -lmysql
else
multi_thread_no_sql_FLAGS += -lmysqlclient
endif

# vim: set noexpandtab :

