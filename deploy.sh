#!/bin/bash

P_DIR=$(pwd)
DIST_DIR=dist
BUILD_DIR=build

#export PATH=/qt/path/gcc_64/bin:$PATH
#export QTDIR=/qt/path/gcc_64
export QT_PLUGIN_PATH=$QTDIR/plugins
export LD_LIBRARY_PATH=$QTDIR/lib

#clean
rm -rf dist

mkdir -p $DIST_DIR/$BUILD_DIR
cd $DIST_DIR/$BUILD_DIR

#build
$QTDIR/bin/qmake -spec linux-g++ "CONFIG += release" $P_DIR/Stacer.pro && make

cd $P_DIR/$DIST_DIR/$BUILD_DIR/stacer-core && make
cd $P_DIR/$DIST_DIR/$BUILD_DIR/stacer && make

mkdir $P_DIR/$DIST_DIR/$BUILD_DIR/stacer/lib

cp $P_DIR/$DIST_DIR/$BUILD_DIR/stacer-core/libstacer-core.so.1.0.0 $P_DIR/$DIST_DIR/$BUILD_DIR/stacer/lib/libstacer-core.so.1

cd $P_DIR
lrelease stacer/stacer.pro

mkdir $P_DIR/$DIST_DIR/$BUILD_DIR/stacer/translations

mv $P_DIR/translations/*.qm $P_DIR/$DIST_DIR/$BUILD_DIR/stacer/translations

rm -rf $P_DIR/$DIST_DIR/$BUILD_DIR/stacer-core

find $P_DIR/$DIST_DIR/$BUILD_DIR/stacer \( -name "moc_*" -or -name "*.o" -or -name "qrc_*" -or -name "Makefile*" -or -name "*.a" -or -name "*.h" \) -exec rm {} \;

cd $P_DIR/$DIST_DIR/$BUILD_DIR/stacer &&
cp $P_DIR/stacer/static/logo.png stacer.png &&
cp $P_DIR/stacer.desktop stacer.desktop

cd $P_DIR

if [ "$1" == "appimage" ]; then
	linuxdeployqt $DIST_DIR/$BUILD_DIR/stacer/stacer -no-translations -appimage
else 
	linuxdeployqt $DIST_DIR/$BUILD_DIR/stacer/stacer -no-translations
fi

rm $P_DIR/$DIST_DIR/$BUILD_DIR/stacer/{AppRun,.DirIcon}


