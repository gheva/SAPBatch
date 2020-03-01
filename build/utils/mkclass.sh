#!/bin/bash

# Creates the header an cpp files for a class
# Usage:
# mkclass.sh ClassName namespace path/to/directory/containing/class
# mkclass.sh REPL lisper src/experiments/interperting_lisp/
classname=$1
namespace=$2
path=$3

class_file_name=`echo ${classname} | tr 'A-Z' 'a-z'`
header_path="${path}/${class_file_name}.h"
cpp_path="${path}/${class_file_name}.cpp"

include_guard=`echo "${namespace}_${classname}_h__" | tr 'a-z' 'A-Z'`


echo "#ifndef ${include_guard}" > ${header_path}
echo "#define ${include_guard}" >> ${header_path}
echo "" >> ${header_path}
echo "namespace ${namespace}" >> ${header_path}
echo "{" >> ${header_path}
echo "" >> ${header_path}
echo "class ${classname}" >> ${header_path}
echo "{" >> ${header_path}
echo "public:" >> ${header_path}
echo "${classname}();" >> ${header_path}
echo "virtual ~${classname}();" >> ${header_path}
echo "protected:" >> ${header_path}
echo "private:" >> ${header_path}
echo "};" >> ${header_path}
echo "" >> ${header_path}
echo "} // namespace" >> ${header_path}
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
echo "${classname}::${classname}()" >> ${cpp_path}
echo "{" >> ${cpp_path}
echo "}" >> ${cpp_path}
echo "" >> ${cpp_path}
echo "${classname}::~${classname}()" >> ${cpp_path}
echo "{" >> ${cpp_path}
echo "}" >> ${cpp_path}
echo "" >> ${cpp_path}
echo "} // namespace" >> ${cpp_path}
echo "" >> ${cpp_path}
echo "/* vim: set cindent sw=2 expandtab : */" >> ${cpp_path}
echo "" >> ${cpp_path}

