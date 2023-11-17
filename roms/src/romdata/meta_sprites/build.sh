#!/bin/bash

FN="utaco_run.png"

IW=`identify -format "%w" $FN | tr -d '\r\n'`
IH=`identify -format "%h" $FN | tr -d '\r\n'`

TMPF=${FN%\.*}


gcc ../../tools/metaspritebuilder.c -o ../../tools/metaspritebuilder

convert "$FN" rgb:$TMPF

../../tools/metaspritebuilder 3 2 6 $IW $IH $TMPF 

rm $TMPF
rm ../../tools/metaspritebuilder
