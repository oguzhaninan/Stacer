#!/bin/bash

APPIMAGE_FILE=
DESKTOP_FILE=
ICON=

cd /tmp

wget $APPIMAGE_FILE
wget $DESKTOP_FILE
wget $ICON

sudo mv *.AppImage /usr/bin/stacer
sudo chmod 755 /usr/bin/stacer

sudo mv stacer.desktop /usr/share/applications/
cp /usr/share/applications/stacer.desktop ~/Desktop

sudo mv stacer.png /usr/share/icons/hicolor/128x128/apps/

# uninstall file
sudo touch /usr/bin/uninstall-stacer
sudo bash -c "echo -e '#!/bin/bash

sudo rm -f /usr/bin/stacer \\
/usr/share/applications/stacer.desktop \\
/usr/share/icons/hicolor/128x128/apps/stacer.png

rm -f /usr/bin/uninstall-stacer

echo Successfully uninstalled ' > /usr/bin/uninstall-stacer"

echo "Successfully installed"

