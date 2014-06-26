#!/bin/bash
#
# @sh.file.header
#  _________        _____ __________________        _____
#  __  ____/___________(_)______  /__  ____/______ ____(_)_______
#  _  / __  __  ___/__  / _  __  / _  / __  _  __ `/__  / __  __ \
#  / /_/ /  _  /    _  /  / /_/ /  / /_/ /  / /_/ / _  /  _  / / /
#  \____/   /_/     /_/   \_,__/   \____/   \__,_/  /_/   /_/ /_/
#
# Version: @sh.file.version
#

#
# Hadoop class path resolver.
#

#
# Check HADOOP_HOME
#

HADOOP_COMMON_HOME=

if [ "$HADOOP_HOME" == "" ]; then
    #Try get all variables from /etc/default
    HADOOP_DEFAULTS=/etc/default/hadoop

    if [ -f $HADOOP_DEFAULTS ]; then
        . $HADOOP_DEFAULTS
    fi
fi

if [ "$HADOOP_HOME" == "" ]; then
    exit
fi

#
# Setting all hadoop modules if it's not set by /etc/default/hadoop
#
if [ "$HADOOP_COMMON_HOME" == "" ]; then
    export HADOOP_COMMON_HOME=$HADOOP_HOME/share/hadoop/common
fi

if [ "$HADOOP_HDFS_HOME" == "" ]; then
    HADOOP_HDFS_HOME=$HADOOP_HOME/share/hadoop/hdfs
fi

if [ "$HADOOP_MAPRED_HOME" == "" ]; then
    HADOOP_MAPRED_HOME=$HADOOP_HOME/share/hadoop/mapreduce
fi

#
# Libraries included in classpath.
#

CP="$HADOOP_COMMON_HOME/lib/*${SEP}$HADOOP_MAPRED_HOME/lib/*${SEP}$HADOOP_MAPRED_HOME/lib/*"

files=$(ls \
$HADOOP_HDFS_HOME/hadoop-hdfs-* \
$HADOOP_COMMON_HOME/hadoop-common-* \
$HADOOP_MAPRED_HOME/hadoop-mapreduce-client-common-* \
$HADOOP_MAPRED_HOME/hadoop-mapreduce-client-core-*)

for file in $files; do
    if [ ${file: -10} != "-tests.jar" ]; then
        CP=${CP}${SEP}$file
    fi
done

GRIDGAIN_HADOOP_CLASSPATH=$CP