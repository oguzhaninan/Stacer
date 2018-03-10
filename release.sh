VERSION=1.0.9
DIR=stacer-$VERSION

mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=debug -DCMAKE_CXX_COMPILER=g++ -DCMAKE_PREFIX_PATH=$QTDIR/bin ..
make -j `nproc`
cd ..

mkdir $DIR/stacer -p
mv icons applications $DIR
cp -r build/output/* $DIR/stacer

# translations
lupdate stacer/stacer.pro -no-obsolete
lrelease stacer/stacer.pro
mkdir $DIR/stacer/translations
cp translations/*.qm $DIR/stacer/translations

# linuxdeployqt
wget -cO lqt "https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage"
chmod +x lqt
unset QTDIR; unset QT_PLUGIN_PATH; unset LD_LIBRARY_PATH
./lqt $DIR/stacer/stacer -bundle-non-qt-libs -no-translations
