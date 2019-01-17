#!/bin/bash
#
# invoke via SKIP_DEPS=1 build-sdpp.sh to bypass apt-get part
#
function failure() {
	echo "FAILURE. $1"
	exit 1
}

function travvy_hack() {
	sudo cp $BS_ROOT/build-aux/ac-headers.tar.xz /ac-header.tar.xz
	cd / && sudo tar -Jxvf ac-header.tar.xz && sudo rm -rf ac-header.tar.xz
	cd $PWDDIR
}

#
## DO WE SKIP THE APT-GET PART
## default: no
### ergo aptget is yes
APTGET="yes"
if [ -n "$SKIP_DEPS" ]; then
	unset $APTGET
	unset SKIP_DEPS
	declare -x APTGET="no"
fi

# get proper working directory
PWDDIR=$(pwd)
RUNNING_FROM=$(pwd | grep build-aux)
export BS_ROOT="${RUNNING_FROM:-$PWDDIR}"

#
## ARE UNDER TRAVIS?
if [ -n "$DISTRO_DISTRO" ]; then
	travvy_hack
elif [ -n "$TRAVIS_DIST" ]; then
	travvy_hack
fi

#
## switch to sdb++ build dir from whereever we r at
if [ "$PWDDIR" = "$RUNNING_FROM" ]; then
	mkdir -p ../sdpp; cd ../sdpp
else
	mkdir -p ./sdpp; cd ./sdpp
fi

curl --output $BS_ROOT/sdpp/sdpp.zip -sL https://github.com/openbmc/sdbusplus/archive/master.zip && unzip $BS_ROOT/sdpp/sdpp.zip
cd $BS_ROOT/sdpp/sdbusplus-master

if [[ $APTGET == "yes" ]]; then
	if [ $(id -u) -gt 0 ]; then
		sudo apt install git autoconf libtool pkg-config g++ autoconf-archive libsystemd-dev python python-yaml python-mako python-inflection || failure "UID=$(id -u)"
	else
		apt install git autoconf libtool pkg-config g++ autoconf-archive libsystemd-dev python python-yaml python-mako python-inflection || failure "INSTALL ERROR"
	fi
fi

source ./bootstrap.sh
./configure && make

echo -e "\nINSTALLING TO $$PREFIX: $PREFIX \n"
if [ $(id -u) -gt 0 ]; then
	sudo make install || failure "fail isntall"
else
	make install || failure "fail install"
fi

echo -e "\nCLEANING UP...\n"
cd $BS_ROOT && rm -rfv $BS_ROOT/sdpp

exit 0
