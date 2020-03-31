EXECUTABLES += multi_thread_no_sql

DIR_EXES += multi_thread_no_sql

multi_thread_no_sql_OBJS += multi_thread_no_sql
multi_thread_no_sql_MODULES += fs
multi_thread_no_sql_MODULES += wavfile
multi_thread_no_sql_MODULES += fft
multi_thread_no_sql_MODULES += yin
multi_thread_no_sql_MODULES += tapers
multi_thread_no_sql_FLAGS += -L$(strip ${THIRD_PARTY_BUILD})/fftw/lib 
multi_thread_no_sql_FLAGS += -lfftw3f
multi_thread_no_sql_FLAGS += -lfftw3f_threads

# vim: set noexpandtab :

