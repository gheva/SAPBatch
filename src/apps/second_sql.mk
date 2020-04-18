EXECUTABLES += second_sql

DIR_EXES += second_sql

second_sql_OBJS += second_sql
second_sql_MODULES += sql
second_sql_FLAGS += -L$(abspath $(strip ${THIRD_PARTY_BUILD})/mysql/lib)
ifdef WINDOWS
second_sql_FLAGS += -lmysql
else
second_sql_FLAGS += -lmysqlclient
endif


# vim: set noexpandtab :

