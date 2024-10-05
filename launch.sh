#!/bin/bash

COMPILE="src/main.c src/glad.c src/shader.c"
FLAGS="-lglfw -lGL -lX11 -lpthread -lXrandr -ldl -lm"

DATESTAMP="$(date +"%Y.%m.%d-%H%M%S")"
EXC="exc/opengl_"$DATESTAMP
HIST=2

gcc $FLAGS -g $COMPILE -o $EXC
if [[ $1 = "-r" ]]; then
  echo "EXECUTING $EXC"
  ./$EXC
fi

# Remove old exectables
EXCNO=0
for file in "exc"/*; do
  let EXCNO++
done
IT=0
for file in "exc"/*; do
  if [[ $IT -ge $(($EXCNO - ($HIST + 1))) ]]; then
    break
  fi
  let IT++
  rm $file
done
