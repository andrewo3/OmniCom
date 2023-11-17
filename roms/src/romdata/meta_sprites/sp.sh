#!/bin/bash
# spritesheet_to_gif.sh COLUMNS ROWS IMAGEFILE [FRAMESDELAY]
# Uses imagemagick to divide the image in columns and rows, and builds a gif of it
 
GDELAY=10
COLUMNS=$1
ROWS=$2
FN=$3
IW=`identify -format "%w" $FN | tr -d '\r\n'`
IH=`identify -format "%h" $FN | tr -d '\r\n'`
 
if [[ -n "$4" ]]; then
    GDELAY=$4
fi
 
SPRW=$(( IW/COLUMNS ))
SPRH=$(( IH/ROWS ))
 
IMCMD="convert +matte +repage -dispose none "
 
for i in $( seq 0 1 $(( ROWS-1 )) )
do
    for j in $( seq 0 1 $(( COLUMNS-1 )) )
    do
       IMCMD="$IMCMD $FN[$((SPRW))x$((SPRH))+$((j*SPRW))+$((i*SPRH))] "
    done
done
 
$IMCMD -set delay $GDELAY ${FN%.*}.gif