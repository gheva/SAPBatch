EXECUTABLES += simple_sql

DIR_EXES += simple_sql

DIR_COMPILATION_FLAGS += -I$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/include)
simple_sql_OBJS += simple_sql
simple_sql_FLAGS += -L$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/lib)
ifdef WINDOWS
simple_sql_FLAGS += -lmysql
else
simple_sql_FLAGS += -lmysqlclient
endif

# vim: set noexpandtab :

