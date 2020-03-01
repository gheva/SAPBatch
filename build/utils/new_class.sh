#!/bin/bash

# Creates a new class with corresponding test suite
# ../../../mk/utils/new_class.sh Parser compiler .
 
classname=${1}
namespace=${2}
path=${3}
tools_path=`dirname $0`

mkclass=${tools_path}/mkclass.sh
mktestsuite=${tools_path}/mktestsuite.sh

${mkclass} ${classname} ${namespace} ${path}
${mktestsuite} ${classname}Tests ${path}/unittests

