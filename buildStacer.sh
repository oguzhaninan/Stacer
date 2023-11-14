#!/bin/bash

cd ..

if [ -e Stacer/build ];
then
	rm -rf Stacer/build
fi

cp -r Stacer/* Stacer/build

cd Stacer/build

cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/home/jordan/Qt5.10.0/5.10.0/gcc_64/lib/cmake

make -j $(nproc)

cd output

./stacer
