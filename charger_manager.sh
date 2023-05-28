#!/bin/sh
fbgrab /tmp/background.png
env QT_QPA_PLATFORM=kobo chroot /kobo /usr/local/bin/charger_manager.bin
fbink -g file=/tmp/background.png
