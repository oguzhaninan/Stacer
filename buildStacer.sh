#!/bin/bash
shopt -s extglob

cd '/home/jordan/GitHub Repos/'

if [ -e Stacer/build ];
then
	rm -rf Stacer/build
fi

mkdir Stacer/build

cd Stacer

cp -rf !(build) build/

cd build

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/home/jordan/Qt5.10.0/5.10.0/gcc_64/lib/cmake

make -j $(nproc)

cd output/bin

./stacer
