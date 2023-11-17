#!/bin/bash

#usage
#	make_nes.sh name cfg map bin_output_dir src_dir extra_ca_options extra_ld_options source_files 

CC65_PATH=""
CA65=ca65
LD65=ld65




PROJECT_NAME=$1
CFG_PATH="$2"
MAP_PATH="$3"
OUT_PATH=$4
SRC_PATH=$5
EXTRA_CA_OPTIONS=$6
EXTRA_LD_OPTIONS=$7
SOURCE_FILES=$8
OBJECTS_PATH=$SRC_PATH
OUT_FILE="$OUT_PATH/$PROJECT_NAME.nes"

GoodByeWorld()
{
	rm -f $OBJECTS
	exit $1
}

for fn in $SOURCE_FILES
do
	OBJECT="$OBJECTS_PATH/$fn.o"
	echo $CA65 "$SRC_PATH/$fn" -I "$SRC_PATH" $EXTRA_CA_OPTIONS -o $OBJECT
	$CA65 "$SRC_PATH/$fn" -I "$SRC_PATH" $EXTRA_CA_OPTIONS -o $OBJECT

	if [ ! -e $OBJECT ]; then
		echo "ERROR: Could not build $OBJECT"
		GoodByeWorld 1
	fi
	OBJECTS="$OBJECTS $OBJECT"
done

$LD65 -o $OUT_FILE -C $CFG_PATH -m $MAP_PATH $EXTRA_LD_OPTIONS $OBJECTS
if [ ! -e $OUT_FILE ]; then
	echo "ERROR: Could not build $OUT_FILE"
	GoodByeWorld 1
fi 

echo "Build Operations Complete."
GoodByeWorld 0
