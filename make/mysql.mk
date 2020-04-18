MYSQL_ZIP := $(abspath  $(strip ${THIRD_PARTY_SOURCE})/mysql_windows.tgz)
MYSQL_TGZ := $(abspath  $(strip ${THIRD_PARTY_SOURCE})/mysql_mac.tgz)
MYSQL_INSTALL_DIR := $(abspath $(strip ${THIRD_PARTY_BUILD}))
MYSQL_ROOT_DIR := $(abspath $(strip ${THIRD_PARTY_BUILD})/mysql)

ifdef WINDOWS
mysql_build:
	cd ${MYSQL_INSTALL_DIR} && tar xzf ${MYSQL_ZIP}
else
mysql_build:
	cd ${MYSQL_INSTALL_DIR} && tar xzf ${MYSQL_TGZ}
endif

PREREQUISITES += mysql_build

# vim: set noexpandtab :

