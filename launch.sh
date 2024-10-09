#!/bin/bash

COMPILE="src/main.c src/glad.c src/shader.c src/stb_image.c"
FLAGS="-lglfw -lGL -lX11 -lpthread -lXrandr -ldl -lm"

DATESTAMP="$(date +"%Y.%m.%d-%H%M%S")"
EXC="opengl_$DATESTAMP.exc"
echo $EXC
HIST=2

if [[ $2 = "-b" ]]; then
  gcc $FLAGS -g $COMPILE -o $EXC
fi
if [[ $1 = "-r" ]]; then
  echo "EXECUTING $EXC"
  ./$EXC
fi

# Remove old exectables
EXCNO=0
for file in *.exc; do
  let EXCNO++
done

IT=0
for file in *.exc; do
  if [[ $IT -ge $(($EXCNO - ($HIST + 1))) ]]; then
    break
  fi
  let IT++
  rm $file
done
