#!/bin/bash

# Creates the unittests directory with the build.mk in it
# Usage:
# mkunittests.sh root path/to/directory/
# The script will create the unittsts directory

module="${1}"
dir=${2}

build_mk="${dir}/unittests/build.mk"

mkdir -p "${dir}/unittests"

echo 'ROOT := $(strip ${ROOT})/..' > ${build_mk}
echo "DIRNAME := unittests" >> ${build_mk}
echo "" >> ${build_mk}
echo "STATIC_MODULES += ${module}" >> ${build_mk}
echo "" >> ${build_mk}
echo "DIR_MODULE := ${module}" >> ${build_mk}
echo "include \$(strip \${BLDFILES})/header.mk" >> ${build_mk}
echo "" >> ${build_mk}
echo "unittests_MODULES += ${module}" >> ${build_mk}
echo "" >> ${build_mk}
echo "" >> ${build_mk}
echo "include \$(strip \${BLDFILES})/footer.mk" >> ${build_mk}
echo "" >> ${build_mk}
echo "# vim: set noexpandtab :" >> ${build_mk}
echo "" >> ${build_mk}
