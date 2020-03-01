#!/bin/bash

# Creates the header an cpp files for a testsuite
# Usage:
# mktestsuite.sh ClassName path/to/directory/containing/class
# mktestsuite.sh REPL lisper src/experiments/interperting_lisp/

classname=$1
path=$2
namespace="unittests"

class_file_name=`echo ${classname} | tr 'A-Z' 'a-z'`
header_path="${path}/${class_file_name}.h"
cpp_path="${path}/${class_file_name}.cpp"

include_guard=`echo "${namespace}_${classname}_h__" | tr 'a-z' 'A-Z'`

echo "#ifndef ${include_guard}" > ${header_path}
echo "#define ${include_guard}" >> ${header_path}
echo "" >> ${header_path}
echo "#include \"ut/unittests.h\"" >> ${header_path}
echo "" >> ${header_path}
echo "namespace ${namespace}" >> ${header_path}
echo "{" >> ${header_path}
echo "" >> ${header_path}
echo "DECLARE_SUITE(${classname});" >> ${header_path}
echo "" >> ${header_path}
echo "} // namespace" >> ${header_path}
echo "" >> ${header_path}
echo "DECLARE_ADD_TO_RUNNER(${classname});" >> ${header_path}
echo "" >> ${header_path}
echo "#endif // ${include_guard}" >> ${header_path}
echo "" >> ${header_path}
echo "/* vim: set cindent sw=2 expandtab : */" >> ${header_path}
echo "" >> ${header_path}

echo "#include \"${class_file_name}.h\"" > ${cpp_path}
echo "" >> ${cpp_path}
echo "namespace ${namespace}" >> ${cpp_path}
echo "{" >> ${cpp_path}
echo "" >> ${cpp_path}
echo "void ${classname}::init()" >> ${cpp_path}
echo "{" >> ${cpp_path}
echo "}" >> ${cpp_path}
echo "" >> ${cpp_path}
echo "void ${classname}::destroy()" >> ${cpp_path}
echo "{" >> ${cpp_path}
echo "}" >> ${cpp_path}
echo "" >> ${cpp_path}
echo "void ${classname}::load_cases()" >> ${cpp_path}
echo "{" >> ${cpp_path}
echo "}" >> ${cpp_path}
echo "" >> ${cpp_path}
echo "} // namespace" >> ${cpp_path}
echo "" >> ${cpp_path}
echo "DEFINE_ADD_TO_RUNNER(${classname});" >> ${cpp_path}
echo "" >> ${cpp_path}
echo "/* vim: set cindent sw=2 expandtab : */" >> ${cpp_path}
echo "" >> ${cpp_path}

