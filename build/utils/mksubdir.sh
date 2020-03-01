#!/bin/bash

#../../mk/utils/mksubdir.sh parser compiler .

dir=${1}
module=${2}
path=${3}
mkunittests=$(dirname ${0})/mkunittests.sh

build_mk=${path}/${dir}/build.mk

mkdir -p ${path}/${dir}

echo 'ROOT := $(strip ${ROOT})/..' > ${build_mk}
echo 'DIRNAME := '${dir} >> ${build_mk}
echo "" >> ${build_mk}
echo "STATIC_MODULES += ${module}" >> ${build_mk}
echo "" >> ${build_mk}
echo "DIR_MODULE := ${module}" >> ${build_mk}
echo 'include $(strip ${BLDFILES})/header.mk' >> ${build_mk}
echo "" >> ${build_mk}
echo "DIR_SUBDIRS += unittests" >> ${build_mk}
echo "" >> ${build_mk}
echo 'include $(strip ${BLDFILES})/footer.mk' >> ${build_mk}
echo "" >> ${build_mk}
echo '# vim: set noexpandtab :' >> ${build_mk}
echo "" >> ${build_mk}

${mkunittests} ${module}ut ${path}/${dir}

