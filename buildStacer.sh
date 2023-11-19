#!/bin/bash

# Implement global settings for shell options.
# This is necessary to run the 'cp' command with
# the !(build) exclusion qualifier. 
shopt -s extglob

# WARNING: Update this path to the folder that contains
# your Stacer repository, otherwise the compile
# will fail.
cd '/home/ubuntu/GitHub Repos/'

# The project files are moved into the build
# directory so that it can function as an isolated
# environment for running the build files.
# This is to ensure that the build process does not
# affect the main project files. Since build is listed
# in the .gitignore file, it will also allow you to compile 
# locally without having git view it as a change to the 
# project directories or files.
if [ -e Stacer/build ]; # If the build folder already exists
then
	rm -rf Stacer/build # remove it
fi

mkdir Stacer/build # make a fresh build folder

cd Stacer # move into the Stacer directory

cp -rf !(build) build/ # copy the entire contents of the Stacer folder
		       # into the new build folder (with the exception
		       # of the build folder itself

cd build # move into the build folder

# Execute the cmake build file
# WARNING: You muust update the path for
# -DCMAKE_PREFIX_PATH to reflect your own
# Qt 5.10.0 installation directory or the
# compile will fail.
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH=/home/ubuntu/Qt5.10.0/5.10.0/gcc_64/lib/cmake

make -j $(nproc) # Create the project executable
		 # This will be placed in
		 # ../Stacer/build/output/bin/

cd output/bin # move into the folder that contains the executable

./stacer # run the executable
