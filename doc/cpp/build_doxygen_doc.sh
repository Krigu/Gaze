#!/bin/bash
#
# this script builds the doxygen documentation
# for every project that is configured in the 
# __doxygen_conf/ directory. 
#
# make sure, that all paths in the doxygen config-files
# are relative to the directory of this script!
#
# how to bild a doxy-config file:
# cd __doxygen_conf && doxygen -g PROJECT_FILENAME
#

WORKING_DIR=$(cd `dirname "${BASH_SOURCE[0]}"` && pwd)

# run doxygen for every configured project
for project in `ls $WORKING_DIR/__doxygen_conf`; do
    mkdir -p $WORKING_DIR/$project
    cd $WORKING_DIR && doxygen __doxygen_conf/$project > /dev/null
done

