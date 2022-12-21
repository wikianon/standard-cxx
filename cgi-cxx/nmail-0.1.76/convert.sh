#!/bin/sh


if [ ! -f makeimage ]; then
   gcc -o makeimage makeimage.cpp
fi

cd images

for file in *.gif; do
   ../makeimage $file
   gcc -c ${file%.gif}.cpp;
done

cd icons

for file in *.gif; do
   ../../makeimage $file
   gcc -c ${file%.gif}.cpp;
done

cd ../..

