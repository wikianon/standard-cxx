#!/bin/bash

QMAKE=$(which qmake)
MAKE=$(which make)

$QMAKE -project > megasena.pro
echo "QT += widgets" >> megasena.pro
$QMAKE
$MAKE

# Now clean
rm *.pro || true
rm moc* || true
rm Makefile || true
rm .qmake.stash 2>/dev/null || true
