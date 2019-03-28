#!/bin/sh
################################################################################
# Copyright (C) 2015 Allwinner.
#
# Check the stack size of object file by calling checkstack.pl.
#
# Change Log:
# 2015-12-23, create the first versio. duanmintao@allwinnertech.com
################################################################################

# $1: the target to be checked
#	all: check the vmlinux and all the *.ko
#	file: scan the file in kernel, and check it
#	directory: check all *.o in the given path
# $2: the objdump tool(default: arm-linux-gnueabi-objdump)

usage()
{
	echo
	echo "You should input as follow:"
	echo "\t $0 [all/file name/directory name] (xxx-objdump)"
}

if [ $# -lt 1 ] || [ $# -gt 2 ]; then
	usage
	exit 1
fi

TARGET=$1
OBJDUMP=$2

TMP=`$OBJDUMP -v 2>&1 | grep "objdump" -c`
if [ $TMP -eq 0 ]; then
	OBJDUMP=`find .. -name arm-linux-gnueabi-objdump | grep objdump | head -1`
	echo Use the default tool: $OBJDUMP
fi

if [ $TARGET = "all" ]; then
	echo Check the vmlinux and all the *.ko:
	$OBJDUMP -d vmlinux $(find . -name "*.ko") | ./scripts/checkstack.pl arm
elif [ -d $TARGET ]; then
	echo Check the directory: $TARGET
	$OBJDUMP -d $(find $TARGET -name "*.o") | ./scripts/checkstack.pl arm
else
	TMP=`find . -name "$TARGET" | grep "$TARGET" -c`
	if [ $TMP -eq 0 ]; then
		echo "$TARGET doesn't exist!"
		usage
		exit 3
	fi

	TMP=`find . -name "$TARGET" | grep "$TARGET" | head -1`
	echo Check the file: $TMP
	$OBJDUMP -d $TMP | perl ./scripts/checkstack.pl arm
fi
