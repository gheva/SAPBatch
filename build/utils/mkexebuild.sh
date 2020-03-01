#!/bin/bash

exe_name=${1}
dir=${2}

build_mk="${dir}/${exe_name}.mk"
src_name=${dir}/${exe_name}.cpp

echo "EXECUTABLES += ${exe_name}" > ${build_mk}
echo "" >> ${build_mk}
echo "DIR_EXES += ${exe_name}" >> ${build_mk}
echo "" >> ${build_mk}
echo "${exe_name}_OBJS += ${exe_name}" >> ${build_mk}
echo "" >> ${build_mk}
echo "# vim: set noexpandtab :" >> ${build_mk}
echo "" >> ${build_mk}

echo "" > ${src_name}
echo "int main(int argc, char** argv)" >> ${src_name}
echo "{" >> ${src_name}
echo "}" >> ${src_name}
echo "" >> ${src_name}
echo "/* vim: set cindent sw=2 expandtab : */" >> ${src_name}
echo "" >> ${src_name}

echo "Add the line:"
echo "\$(call ADD_EXE, ${exe_name}.mk)"
echo " to the file ${dir}/build.mk"
