#!/bin/sh
fbgrab /tmp/background.png
env LD_LIBRARY_PATH=/mnt/onboard/.adds/qt-linux-5.15.2-kobo/lib:/lib:/usr/lib:/usr/local/lib QT_QPA_PLATFORM=kobo chroot /kobo /usr/local/bin/charger_manager.bin
fbink -g file=/tmp/background.png
